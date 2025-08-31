#pragma once
#include "Core/Event/Callback.h"
#include "Math/Vector.h"
#include "RHIEnums.h"
#include "SurfaceWindow.h"

#include "Core/Assert.h"
#include "Core/TaskGraph/TaskHandle.h"
#include "GfxContext.generated.h"

class RHICommandBuffer;
class RHISurfaceWindow;
class RHIPipelineLayout;
class RHIPipeline;
class RHIImageView;
class RHIDescriptorPool;
class RHIDescriptorSetLayout;
class RHIShaderModule;
class RHIFrameBuffer;
class RHIRenderPass;
class RHICommandPool;
class RHISemaphore;
class RHIFence;
struct RHIImageDesc;
class RHIImage;

struct GfxDeviceFeatures {
  ERHISampleCount MaxSampleCount = ERHISampleCount::SC_1;

  bool SamplerAnisotropy = false; // 各向异性
};

KENUM()
enum class ERHIBackend {
  Vulkan,
  Count,
};

struct PhysicalDeviceSwapchainFeatures {
  struct SurfaceFormat {
    ERHIFormat Format;
    ERHIColorSpace ColorSpace;
  };

  struct MyCapabilities {
    Vector2i CurrentExtent;
    Vector2i MinImageExtent;
    Vector2i MaxImageExtent;
    UInt32 MaxImageCount;
    UInt32 MinImageCount;
    ERHISurfaceTransform SupportedTransforms;
    ERHISurfaceTransformBits CurrentTransform;
  };

  Array<SurfaceFormat> Formats;
  Array<ERHIPresentMode> PresentModes;
  MyCapabilities Capabilities;
};

struct RHIPresentParams {
  RHI_DEFINE_BUILDER_FIELD_PTR(RHISurfaceWindow*, SurfaceWindow, {}); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(Array<RHISemaphore*>, WaitSemaphores, {}); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, ImageIndex, 0);                    // NECESSARY
};

class GfxContext {
public:
  static void StartUp();
  static void ShutDown();

  static GfxContext* Get() {
    ASSERT_MSG(mContext != nullptr, "GfxContext::Get() called before StartUp()");
    return mContext;
  }

  static GfxContext& GetRef() {
    ASSERT_MSG(mContext != nullptr, "GfxContext::Get() called before StartUp()");
    return *mContext;
  }

  virtual ~GfxContext() = default;
  [[nodiscard]] virtual ERHIBackend GetBackend() const = 0;

  virtual SharedPtr<RHIImage> CreateImage(const RHIImageDesc& Desc) = 0;

  [[nodiscard]] const GfxDeviceFeatures& GetGfxDeviceFeatures() const { return mGfxDeviceFeatures; }

  [[nodiscard]] virtual PhysicalDeviceSwapchainFeatures GetPhysicalDeviceSwapchainFeatures(RHISurfaceWindow& Window) const = 0;

  virtual SharedPtr<RHIImageView> CreateImageViewS(const struct RHIImageViewDesc& Desc) = 0;
  virtual UniquePtr<RHIFence> CreateFenceU() = 0;
  virtual SharedPtr<RHIFence> CreateFenceS() = 0;
  virtual UniquePtr<RHISemaphore> CreateSemaphoreU() = 0;
  /// 创建一个CommandPool
  /// @param QueueFamily 队列家族
  /// @param AllowReset 是否允许进行Reset
  virtual UniquePtr<RHICommandPool> CreateCommandPoolU(ERHIQueueFamilyType QueueFamily, bool AllowReset) = 0;
  virtual UniquePtr<RHIRenderPass> CreateRenderPassU(const struct RHIRenderPassDesc&) = 0;
  virtual UniquePtr<RHIFrameBuffer> CreateFrameBufferU(const struct RHIFrameBufferDesc& Desc) = 0;
  virtual UniquePtr<RHIShaderModule> CreateShaderModuleU(const struct RHIShaderModuleDesc& Desc) = 0;
  virtual UniquePtr<RHIDescriptorSetLayout> CreateDescriptorSetLayoutU(const struct RHIDescriptorSetLayoutDesc& Desc) = 0;
  virtual UniquePtr<RHIDescriptorPool> CreateDescriptorPoolU(const struct RHIDescriptorPoolDesc& Desc) = 0;
  virtual UniquePtr<RHIPipelineLayout> CreatePipelineLayoutU(const struct RHIPipelineLayoutDesc& Desc) = 0;
  virtual UniquePtr<RHIPipeline> CreatePipeline(const struct RHIGraphicsPipelineDesc& Desc) = 0;
  virtual void WaitDeviceIdle() = 0;

  /**
   * 创建一个SurfaceWindow用于渲染
   * @param Width <=0时选择配置文件中的默认值
   * @param Height <=0时选择配置文件中的默认值
   * @return 只应该调用一次用来创建一个全局的SurfaceWindow
   */
  virtual RHISurfaceWindow* CreateSurfaceWindowR(Int32 Width, Int32 Height) = 0;
  /**
   * 获取下一个可用图像的索引
   * @param Window
   * @param WaitSemaphore
   * @param WaitFence
   * @param NeedRecreation 交换链是否需要重建
   * @return
   */
  virtual UInt32 GetNextImage(RHISurfaceWindow* Window, RHISemaphore* WaitSemaphore, RHIFence* WaitFence, bool& NeedRecreation) = 0;

  virtual void Submit(const struct RHICommandBufferSubmitParams& Params) = 0;

  /// 呈现 返回是否需要重建交换链
  virtual bool Present(const RHIPresentParams& Params) = 0;

#if KITA_EDITOR
  virtual TaskHandle DrawImGui(RHICommandBuffer* Buffer, RHIFrameBuffer* FrameBuffer, UInt32 Width, UInt32 Height) = 0;
  RHIRenderPass* GetImGuiRenderPass() const { return mImGuiRenderPass.Get(); }

protected:
  UniquePtr<RHIRenderPass> mImGuiRenderPass = {};
#endif

protected:
  GfxDeviceFeatures mGfxDeviceFeatures{};

  static inline GfxContext* mContext = nullptr;
};

inline Callback<void> Evt_PreGfxContextCreated;
inline Callback<void, GfxContext* /* GfxContext */> Evt_PostGfxContextCreated;
inline Callback<void, GfxContext* /* GfxContext */> Evt_PreGfxContextDestroyed;
inline Callback<void> Evt_PostGfxContextDestroyed;

struct WindowBackendSupport {
  static inline ERHISurfaceWindowType SurfaceWindowBackendType = ERHISurfaceWindowType::Count;

  static void InitializeWindowBackend();
  static void CleanWindowBackend();
  // 根据不同的窗口后端Tick输入数据
  static void TickFrameInputData();
  static void ShouldWindowClose(RHISurfaceWindow* MyWindow, bool& OutResult);
};
