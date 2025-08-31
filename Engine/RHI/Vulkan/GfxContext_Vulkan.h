#pragma once
#include "Core/TaskGraph/TaskHandle.h"
#include "RHI/GfxContext.h"
#include "RHI/ImageView.h"
#include "vulkan/vulkan.h"

struct QueueFamilyIndices {
  Optional<UInt32> GraphicsFamily;
  Optional<UInt32> PresentFamily;

  [[nodiscard]] bool IsComplete() const { return GraphicsFamily && PresentFamily; }
};

class GfxContext_Vulkan final : public GfxContext {
public:
  GfxContext_Vulkan();
  virtual ~GfxContext_Vulkan() override;

  [[nodiscard]] virtual ERHIBackend GetBackend() const override { return ERHIBackend::Vulkan; }

  virtual SharedPtr<RHIImage> CreateImage(const RHIImageDesc& Desc) override;
  virtual SharedPtr<RHIImageView> CreateImageViewS(const RHIImageViewDesc& Desc) override;
  virtual SharedPtr<RHIFence> CreateFenceS() override;
  virtual UniquePtr<RHIFence> CreateFenceU() override;
  virtual UniquePtr<RHISemaphore> CreateSemaphoreU() override;
  SharedPtr<RHIImageView> CreateSwapchainImageView(VkImage Img, VkFormat Format);
  virtual RHISurfaceWindow* CreateSurfaceWindowR(Int32 Width, Int32 Height) override;
  virtual UInt32 GetNextImage(RHISurfaceWindow* Window, RHISemaphore* WaitSemaphore, RHIFence* WaitFence, bool& NeedRecreation) override;
  virtual UniquePtr<RHIRenderPass> CreateRenderPassU(const RHIRenderPassDesc&) override;
  virtual UniquePtr<RHIFrameBuffer> CreateFrameBufferU(const RHIFrameBufferDesc& Desc) override;

  virtual UniquePtr<RHICommandPool> CreateCommandPoolU(ERHIQueueFamilyType QueueFamily, bool AllowReset) override;
  virtual UniquePtr<RHIShaderModule> CreateShaderModuleU(const RHIShaderModuleDesc& Desc) override;
  virtual UniquePtr<RHIDescriptorSetLayout> CreateDescriptorSetLayoutU(const RHIDescriptorSetLayoutDesc& Desc) override;
  virtual UniquePtr<RHIDescriptorPool> CreateDescriptorPoolU(const RHIDescriptorPoolDesc& Desc) override;
  virtual UniquePtr<RHIPipelineLayout> CreatePipelineLayoutU(const RHIPipelineLayoutDesc& Desc) override;
  virtual UniquePtr<RHIPipeline> CreatePipeline(const RHIGraphicsPipelineDesc& Desc) override;
  virtual bool Present(const RHIPresentParams& Params) override;
  virtual void WaitDeviceIdle() override;

  virtual PhysicalDeviceSwapchainFeatures GetPhysicalDeviceSwapchainFeatures(RHISurfaceWindow& Window) const override;

#if KITA_EDITOR
  virtual TaskHandle DrawImGui(RHICommandBuffer* Buffer, RHIFrameBuffer* FrameBuffer, UInt32 Width, UInt32 Height) override;
  void StartUpImGui();
  void ShutDownImGui();
  void CreateImGuiRenderPass();
  void CreateImGuiDescriptorPool();

  VkDescriptorPool mImGuiDescriptorPool = nullptr;
#endif

  virtual void Submit(const RHICommandBufferSubmitParams& Params) override;

  [[nodiscard]] VkDevice GetDevice() const { return mDevice; }

  [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return mPhysicalDevice; }

  [[nodiscard]] VkInstance GetInstance() const { return mInstance; }

  static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice Device, VkSurfaceKHR Surface);
  QueueFamilyIndices GetQueueFamilies(VkSurfaceKHR Surface) const;

  [[nodiscard]] UInt32 GetQueueFamilyIndex(ERHIQueueFamilyType Family) const;
  [[nodiscard]] VkQueue GetQueue(ERHIQueueFamilyType Family) const;

private:
  static bool IsLayerSupported(const char* LayerName);
  static void OnPostGfxContextCreated(GfxContext* Context);
  void CreateInstance();
  void SetupDebugMessenger();
  void SelectPhysicalDevice(RHISurfaceWindow& TempWindow);
  [[nodiscard]] bool IsDeviceSuitable(VkPhysicalDevice Device, RHISurfaceWindow& TempWindow, QueueFamilyIndices& OutFamilyIndices, VkSurfaceFormatKHR& OutFormat) const;
  [[nodiscard]] bool CheckDeviceExtensionSupport(VkPhysicalDevice Device) const;
  void CreateLogicalDevice(RHISurfaceWindow& TempWindow);

  void FindPhysicalDeviceFeatures();
  static PhysicalDeviceSwapchainFeatures QueryPhysicalDeviceSwapchainFeatures(VkPhysicalDevice Device, RHISurfaceWindow& TempWindow);

  VkResult Dyn_CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;

  void Dyn_DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* pAllocator) const;

  VkInstance mInstance = nullptr;
  VkDebugUtilsMessengerEXT mDebugMessenger{};
  VkPhysicalDevice mPhysicalDevice = nullptr;
  VkDevice mDevice = nullptr;
  VkQueue mGraphicsQueue = nullptr;
  VkQueue mPresentQueue = nullptr;
  /// 用于
  // PhysicalDeviceSwapchainFeatures mPhysicalDeviceSwapchainFeatures = {};

  VkSurfaceFormatKHR mSwapchainFormat{};
  QueueFamilyIndices mQueueFamilies;

  bool mEnabledValidationLayer = false;
  const Array<const char*> mDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

inline GfxContext_Vulkan& GetVulkanGfxContexRef() { return *static_cast<GfxContext_Vulkan*>(GfxContext::Get()); }
inline GfxContext_Vulkan* GetVulkanGfxContex() { return static_cast<GfxContext_Vulkan*>(GfxContext::Get()); }
