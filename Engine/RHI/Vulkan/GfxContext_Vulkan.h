#pragma once
#include "RHI/GfxContext.h"
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
  virtual SharedPtr<RHIImageView> CreateImageViewS(const ImageViewDesc& Desc) override;
  virtual SharedPtr<RHIFence> CreateFenceS() override;
  virtual UniquePtr<RHIFence> CreateFenceU() override;
  virtual UniquePtr<RHISemaphore> CreateSemaphoreU() override;
  SharedPtr<RHIImageView> CreateSwapchainImageView(VkImage Img, VkFormat Format);
  virtual RHISurfaceWindow* CreateSurfaceWindowR(int32_t Width, int32_t Height) override;
  virtual UInt32 GetNextImage(RHISurfaceWindow* Window, RHISemaphore* WaitSemaphore, RHIFence* WaitFence, bool& NeedRecreation) override;
  virtual UniquePtr<RHIRenderPass> CreateRenderPassU(const RHIRenderPassDesc&) override;
  virtual UniquePtr<RHIFrameBuffer> CreateFrameBufferU(const RHIFrameBufferDesc& Desc) override;
  virtual UniquePtr<RHICommandPool> CreateCommandPoolU(ERHIQueueFamilyType QueueFamily) override;
  virtual UniquePtr<RHIShaderModule> CreateShaderModuleU(const RHIShaderModuleDesc& Desc) override;
  virtual UniquePtr<RHIDescriptorSetLayout> CreateDescriptorSetLayoutU(const RHIDescriptorSetLayoutDesc& Desc) override;
  virtual UniquePtr<RHIDescriptorPool> CreateDescriptorPoolU(const RHIDescriptorPoolDesc& Desc) override;
  virtual UniquePtr<RHIPipelineLayout> CreatePipelineLayoutU(const RHIPipelineLayoutDesc& Desc) override;
  virtual UniquePtr<RHIPipeline> CreatePipeline(const RHIGraphicsPipelineDesc& Desc) override;

  virtual void Submit(const RHICommandBufferSubmitParams& Params) override;

  VkDevice GetDevice() const { return mDevice; }

  VkPhysicalDevice GetPhysicalDevice() const { return mPhysicalDevice; }

  VkInstance GetInstance() const { return mInstance; }

  static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice Device, VkSurfaceKHR Surface);
  QueueFamilyIndices GetQueueFamilies(VkSurfaceKHR Surface) const;

  UInt32 GetQueueFamilyIndex(ERHIQueueFamilyType Family) const;
  VkQueue GetQueue(ERHIQueueFamilyType Family) const;

private:
  static bool IsLayerSupported(const char* LayerName);
  static void OnPostGfxContextCreated(GfxContext* Context);
  void CreateInstance();
  void SetupDebugMessenger();
  void SelectPhysicalDevice(RHISurfaceWindow& TempWindow);
  bool IsDeviceSuitable(VkPhysicalDevice Device, RHISurfaceWindow& TempWindow) const;
  bool CheckDeviceExtensionSupport(VkPhysicalDevice Device) const;
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

  QueueFamilyIndices mQueueFamilies;

  bool mEnabledValidationLayer = false;
  const std::vector<const char*> mDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};