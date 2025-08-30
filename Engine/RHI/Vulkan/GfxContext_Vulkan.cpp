//
// Created by kita on 25-8-24.
//

#include "GfxContext_Vulkan.h"

#include "CommandBuffer_Vulkan.h"
#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/TaskNode.h"
#include "DescriptorSet_Vulkan.h"
#include "FrameBuffer_Vulkan.h"
#include "ImageView_Vulkan.h"
#include "Image_Vulkan.h"
#include "Pipeline_Vulkan.h"
#include "RHI/RHIConfig.h"
#include "RHIEnums_Vulkan.h"
#include "RenderPass_Vulkan.h"
#include "ShaderModule_Vulkan.h"
#include "SurfaceWindow_Vulkan.h"
#include "Sync_Vulkan.h"

#if KITA_EDITOR
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#endif

auto VALIDATION_LAYER_NAME = "VK_LAYER_KHRONOS_validation";

void GfxContext_Vulkan::OnPostGfxContextCreated(GfxContext* Context) {
  const auto Ctx = static_cast<GfxContext_Vulkan*>(Context);
  RHISurfaceWindow_Vulkan TempWindow{-1, -1, true, false};
  TempWindow.CreateSurface(Ctx->mInstance);
  Ctx->SetupDebugMessenger();
  Ctx->SelectPhysicalDevice(TempWindow);
  Ctx->CreateLogicalDevice(TempWindow);
  TempWindow.DestroySurface(Ctx->mInstance);
#if KITA_EDITOR
  Ctx->StartUpImGui();
#endif
}

GfxContext_Vulkan::GfxContext_Vulkan() {
  // 此初始化函数仅仅创建Vulkan实例 选择物理设备并创建逻辑设备, 而不关心交换链、渲染管线等
  CreateInstance();
  // 创建一个临时的SurfaceWindow用于创建Device
  Evt_PostGfxContextCreated.Add(&GfxContext_Vulkan::OnPostGfxContextCreated);
}

GfxContext_Vulkan::~GfxContext_Vulkan() {
  ShutDownImGui();
  Dyn_DestroyDebugUtilsMessengerEXT(mDebugMessenger, nullptr);
  vkDestroyInstance(mInstance, nullptr);
}

SharedPtr<RHIImage> GfxContext_Vulkan::CreateImage(const RHIImageDesc& Desc) { return nullptr; }

SharedPtr<RHIImageView> GfxContext_Vulkan::CreateImageViewS(const RHIImageViewDesc& Desc) { return MakeShared<RHIImageView_Vulkan>(Desc); }

SharedPtr<RHIFence> GfxContext_Vulkan::CreateFenceS() { return MakeShared<RHIFence_Vulkan>(); }

UniquePtr<RHIFence> GfxContext_Vulkan::CreateFenceU() { return MakeUnique<RHIFence_Vulkan>(); }

UniquePtr<RHISemaphore> GfxContext_Vulkan::CreateSemaphoreU() { return MakeUnique<RHISemaphore_Vulkan>(); }

SharedPtr<RHIImageView> GfxContext_Vulkan::CreateSwapchainImageView(VkImage Img, VkFormat Format) { return MakeShared<RHIImageView_Vulkan>(Img, Format); }

RHISurfaceWindow* GfxContext_Vulkan::CreateSurfaceWindowR(const Int32 Width, const Int32 Height) { return new RHISurfaceWindow_Vulkan(Width, Height); }

UInt32 GfxContext_Vulkan::GetNextImage(RHISurfaceWindow* Window, RHISemaphore* WaitSemaphore, RHIFence* WaitFence, bool& NeedRecreation) {
  UInt32 Return;
  const VkResult Result = vkAcquireNextImageKHR(mDevice, static_cast<VkSwapchainKHR>(Window->GetNativeSwapchainObject()), UINT64_MAX,
                                                WaitSemaphore == nullptr ? VK_NULL_HANDLE : WaitSemaphore->GetNativeHandleT<VkSemaphore>(),
                                                WaitFence == nullptr ? VK_NULL_HANDLE : WaitFence->GetNativeHandleT<VkFence>(), &Return);
  if (Result == VK_ERROR_OUT_OF_DATE_KHR) {
    NeedRecreation = true;
  } else if (Result == VK_SUCCESS) {
  } else {
    LOG_CRITICAL_TAG("RHI.Vulkan", "无法获取下一个交换链图像!Code={}", Result);
  }
  return Return;
}

UniquePtr<RHIRenderPass> GfxContext_Vulkan::CreateRenderPassU(const RHIRenderPassDesc& Desc) { return MakeUnique<RHIRenderPass_Vulkan>(Desc); }

UniquePtr<RHIFrameBuffer> GfxContext_Vulkan::CreateFrameBufferU(const RHIFrameBufferDesc& Desc) { return MakeUnique<RHIFrameBuffer_Vulkan>(Desc); }

UniquePtr<RHICommandPool> GfxContext_Vulkan::CreateCommandPoolU(ERHIQueueFamilyType QueueFamily) { return MakeUnique<RHICommandPool_Vulkan>(QueueFamily); }

UniquePtr<RHIShaderModule> GfxContext_Vulkan::CreateShaderModuleU(const RHIShaderModuleDesc& Desc) { return MakeUnique<RHIShaderModule_Vulkan>(Desc); }

UniquePtr<RHIDescriptorSetLayout> GfxContext_Vulkan::CreateDescriptorSetLayoutU(const RHIDescriptorSetLayoutDesc& Desc) { return MakeUnique<RHIDescriptorSetLayout_Vulkan>(Desc); }

UniquePtr<RHIDescriptorPool> GfxContext_Vulkan::CreateDescriptorPoolU(const RHIDescriptorPoolDesc& Desc) { return MakeUnique<RHIDescriptorPool_Vulkan>(Desc); }

UniquePtr<RHIPipelineLayout> GfxContext_Vulkan::CreatePipelineLayoutU(const RHIPipelineLayoutDesc& Desc) { return MakeUnique<RHIPipelineLayout_Vulkan>(Desc); }

UniquePtr<RHIPipeline> GfxContext_Vulkan::CreatePipeline(const RHIGraphicsPipelineDesc& Desc) { return MakeUnique<RHIPipeline_Vulkan>(Desc); }

bool GfxContext_Vulkan::Present(const RHIPresentParams& Params) { VkPresentInfoKHR PresentInfo{}; }

class ImGuiDrawTask : public TaskNode {
public:
  VkCommandBuffer Cmd;
  VkFramebuffer FrameBuffer;
  VkRenderPass RenderPass;
  UInt32 Width;
  UInt32 Height;

  explicit ImGuiDrawTask(VkCommandBuffer Cmd, VkFramebuffer FrameBuffer, VkRenderPass RenderPass, UInt32 Width, UInt32 Height)
      : Cmd(Cmd), FrameBuffer(FrameBuffer), RenderPass(RenderPass), Width(Width), Height(Height) {}

  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }
  virtual ETaskNodeResult Run() override {
    VkRenderPassBeginInfo RenderPassBeginInfo{};
    RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    RenderPassBeginInfo.renderPass = RenderPass;
    RenderPassBeginInfo.framebuffer = FrameBuffer;
    RenderPassBeginInfo.renderArea.offset = {0, 0};
    RenderPassBeginInfo.renderArea.extent = {Width, Height};
    VkClearValue ClearValue{};
    RenderPassBeginInfo.clearValueCount = 1;
    RenderPassBeginInfo.pClearValues = &ClearValue;
    vkCmdBeginRenderPass(Cmd, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Cmd);
    vkCmdEndRenderPass(Cmd);
    return ETaskNodeResult::Success;
  }
};

void GfxContext_Vulkan::DrawImGui(RHICommandBuffer* Buffer, RHIFrameBuffer* FrameBuffer, UInt32 Width, UInt32 Height) {
  TaskGraph::CreateTask<ImGuiDrawTask>("", {}, Buffer->GetNativeHandleT<VkCommandBuffer>(), FrameBuffer->GetNativeHandleT<VkFramebuffer>(), mImGuiRenderPass, Width, Height);
}

void GfxContext_Vulkan::Submit(const RHICommandBufferSubmitParams& Params) {
  VkSubmitInfo SubmitInfo{};
  SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  SubmitInfo.commandBufferCount = 1;
  const auto CommandBuffer = Params.CommandBuffer->GetNativeHandleT<VkCommandBuffer>();
  SubmitInfo.pCommandBuffers = &CommandBuffer;
  SubmitInfo.signalSemaphoreCount = Params.SignalSemaphores.Count();
  Array<VkSemaphore> SignalSemaphores;
  for (const auto& Semaphore : Params.SignalSemaphores) {
    SignalSemaphores.Add(Semaphore->GetNativeHandleT<VkSemaphore>());
  }
  SubmitInfo.pSignalSemaphores = SignalSemaphores.Data();
  Array<VkSemaphore> WaitSemaphores;
  for (const auto& Semaphore : Params.WaitSemaphores) {
    WaitSemaphores.Add(Semaphore->GetNativeHandleT<VkSemaphore>());
  }
  SubmitInfo.waitSemaphoreCount = WaitSemaphores.Count();
  const VkPipelineStageFlags WaitPipelineStageFlags = RHIPipelineStageToVkPipelineStage(Params.WaitPipelineStages);
  SubmitInfo.pWaitDstStageMask = &WaitPipelineStageFlags;
  const VkFence WaitFence = Params.Fence == nullptr ? VK_NULL_HANDLE : Params.Fence->GetNativeHandleT<VkFence>();
  const VkQueue Queue = GetQueue(Params.TargetQueueFamily);
  VkResult Result = vkQueueSubmit(Queue, 1, &SubmitInfo, WaitFence);
  if (Result != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "提交渲染指令失败!Code={}", Result);
  }
}

bool GfxContext_Vulkan::IsLayerSupported(const char* LayerName) {
  UInt32 LayerCount;
  vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

  Array<VkLayerProperties> AvailableLayers(LayerCount);
  vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.Data());

  for (const auto& layerProperties : AvailableLayers) {
    if (strcmp(LayerName, layerProperties.layerName) == 0) {
      return true;
    }
  }

  return false;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  if (MessageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    LOG_ERROR_TAG("RHI.Vulkan.Validation", "{}", pCallbackData->pMessage);
  } else if (MessageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    LOG_WARN_TAG("RHI.Vulkan.Validation", "{}", pCallbackData->pMessage);
  } else {
    LOG_INFO_TAG("RHI.Vulkan.Validation", "{}", pCallbackData->pMessage);
  }
  return VK_FALSE;
}

static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& Info) {
  Info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  Info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  Info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  Info.pfnUserCallback = &DebugCallback;
}

void GfxContext_Vulkan::CreateInstance() {
  CPU_PROFILING_SCOPE;
  VkApplicationInfo AppInfo{};
  AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  AppInfo.pApplicationName = "KitaEngine";
  AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  AppInfo.pEngineName = "KitaEngine";
  AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  AppInfo.apiVersion = VK_API_VERSION_1_4;

  VkInstanceCreateInfo InstanceInfo{};
  InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  InstanceInfo.pApplicationInfo = &AppInfo;
  Array<const char*> Extensions;
  Array<const char*> Layers;
  const auto& RHICfg = ConfigManager::GetConfigRef<RHIConfig>();
  switch (RHICfg.GetSurfaceWindowType()) {
  case ERHISurfaceWindowType::GLFW:
    GetVulkanGLFWSurfaceWindowExtensions(Extensions);
    break;
  default:
    std::unreachable();
  }
  VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo{};
  if (RHICfg.GetEnableVulkanValidationLayer()) {
    if (IsLayerSupported(VALIDATION_LAYER_NAME)) {
      Extensions.Add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      Layers.Add(VALIDATION_LAYER_NAME);
      // DebugMessenger
      PopulateDebugMessengerCreateInfo(MessengerCreateInfo);
      mEnabledValidationLayer = true;
    } else {
      LOG_WARN_TAG("RHI.Vulkan", "Vulkan验证层不支持!将自动关闭.");
    }
  }
  InstanceInfo.enabledExtensionCount = static_cast<UInt32>(Extensions.Count());
  InstanceInfo.ppEnabledExtensionNames = Extensions.Data();
  InstanceInfo.enabledLayerCount = static_cast<UInt32>(Layers.Count());
  InstanceInfo.ppEnabledLayerNames = Layers.Data();
  InstanceInfo.pNext = &MessengerCreateInfo;
  ASSERT_MSG(vkCreateInstance(&InstanceInfo, nullptr, &mInstance) == VK_SUCCESS, "创建Vulkan实例失败");
  LOG_INFO_TAG("RHI.Vulkan", "创建Vulkan Instance成功!");
  LOG_INFO_TAG("RHI.Vulkan", "开启的扩展:");
  for (const auto& Extension : Extensions) {
    LOG_INFO_TAG("RHI.Vulkan", "  {}", Extension);
  }
  LOG_INFO_TAG("RHI.Vulkan", "开启的Layer:");
  for (const auto& Layer : Layers) {
    LOG_INFO_TAG("RHI.Vulkan", "  {}", Layer);
  }
}

void GfxContext_Vulkan::SetupDebugMessenger() {
  if (!mEnabledValidationLayer)
    return;
  VkDebugUtilsMessengerCreateInfoEXT Info{};
  PopulateDebugMessengerCreateInfo(Info);
  ASSERT_MSG(Dyn_CreateDebugUtilsMessengerEXT(&Info, nullptr, &mDebugMessenger) == VK_SUCCESS, "Vulkan调试信息设置失败");
}

void GfxContext_Vulkan::SelectPhysicalDevice(RHISurfaceWindow& TempWindow) {
  UInt32 DeviceCount = 0;
  vkEnumeratePhysicalDevices(mInstance, &DeviceCount, nullptr);

  if (DeviceCount == 0) {
    LOG_CRITICAL_TAG("RHI.Vulkan", "没有可用的Vulkan设备!");
  }

  Array<VkPhysicalDevice> devices(DeviceCount);
  vkEnumeratePhysicalDevices(mInstance, &DeviceCount, devices.Data());

  for (const auto& Device : devices) {
    if (IsDeviceSuitable(Device, TempWindow, mQueueFamilies)) {
      mPhysicalDevice = Device;
      break;
    }
  }

  if (mPhysicalDevice == VK_NULL_HANDLE) {
    LOG_CRITICAL_TAG("RHI.Vulkan", "没有可用的Vulkan设备!");
  } else {
    FindPhysicalDeviceFeatures();
    mPhysicalDeviceSwapchainFeatures = QueryPhysicalDeviceSwapchainFeatures(mPhysicalDevice, TempWindow);
  }
}

bool GfxContext_Vulkan::IsDeviceSuitable(VkPhysicalDevice Device, RHISurfaceWindow& TempWindow, QueueFamilyIndices& OutFamilyIndicies) const {
  bool ExtensionsSupported = CheckDeviceExtensionSupport(Device);

  PhysicalDeviceSwapchainFeatures SwapchainFeatures = QueryPhysicalDeviceSwapchainFeatures(Device, TempWindow);
  bool SwapchainAvailable = !SwapchainFeatures.Formats.Empty() && !SwapchainFeatures.PresentModes.Empty();

  VkPhysicalDeviceFeatures SupportedFeatures{};
  vkGetPhysicalDeviceFeatures(Device, &SupportedFeatures);

  OutFamilyIndicies = FindQueueFamilies(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()));

  return ExtensionsSupported && SupportedFeatures.samplerAnisotropy && SwapchainAvailable && OutFamilyIndicies.IsComplete();
}

bool GfxContext_Vulkan::CheckDeviceExtensionSupport(const VkPhysicalDevice Device) const {
  UInt32 Extension;
  vkEnumerateDeviceExtensionProperties(Device, nullptr, &Extension, nullptr);

  Array<VkExtensionProperties> AvailableExtensions(Extension);
  vkEnumerateDeviceExtensionProperties(Device, nullptr, &Extension, AvailableExtensions.Data());

  Set<std::string> RequiredExtensions(mDeviceExtensions.begin(), mDeviceExtensions.end());

  for (const auto& extension : AvailableExtensions) {
    RequiredExtensions.Remove(extension.extensionName);
  }

  return RequiredExtensions.Empty();
}

void GfxContext_Vulkan::CreateLogicalDevice(RHISurfaceWindow& TempWindow) {
  Array<VkDeviceQueueCreateInfo> QueueInfos{};
  float QueuePriority = 1.0f;
  Set<UInt32> UniqueQueueFamilies = {*mQueueFamilies.GraphicsFamily, *mQueueFamilies.PresentFamily};
  for (UInt32 QueueFamily : UniqueQueueFamilies) {
    VkDeviceQueueCreateInfo QueueInfo{};
    QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    QueueInfo.queueFamilyIndex = QueueFamily;
    QueueInfo.queueCount = 1;
    QueueInfo.pQueuePriorities = &QueuePriority;
    QueueInfos.Add(QueueInfo);
  }

  VkPhysicalDeviceFeatures DeviceFeatures{};
  DeviceFeatures.samplerAnisotropy = true;

  VkDeviceCreateInfo DeviceInfo{};
  DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  DeviceInfo.pQueueCreateInfos = QueueInfos.Data();
  DeviceInfo.queueCreateInfoCount = static_cast<UInt32>(QueueInfos.Count());
  DeviceInfo.pEnabledFeatures = &DeviceFeatures;
  DeviceInfo.enabledExtensionCount = static_cast<UInt32>(mDeviceExtensions.Count());
  DeviceInfo.ppEnabledExtensionNames = mDeviceExtensions.Data();
  Array ValidationLayerNames = {VALIDATION_LAYER_NAME};
  if (mEnabledValidationLayer) {
    DeviceInfo.enabledLayerCount = static_cast<UInt32>(ValidationLayerNames.Count());
    DeviceInfo.ppEnabledLayerNames = ValidationLayerNames.Data();
  }
  if (VkResult Code = vkCreateDevice(mPhysicalDevice, &DeviceInfo, nullptr, &mDevice); Code != VK_SUCCESS) {
    LOG_CRITICAL_TAG("RHI.Vulkan", "初始化Vulkan上下文失败, Code={}", Code);
  }
  vkGetDeviceQueue(mDevice, *mQueueFamilies.GraphicsFamily, 0, &mGraphicsQueue);
  vkGetDeviceQueue(mDevice, *mQueueFamilies.PresentFamily, 0, &mPresentQueue);
}

QueueFamilyIndices GfxContext_Vulkan::FindQueueFamilies(const VkPhysicalDevice Device, const VkSurfaceKHR Surface) {
  QueueFamilyIndices Indices;

  UInt32 QueueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

  Array<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.Data());

  int Index = 0;
  for (const auto& queueFamily : QueueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      Indices.GraphicsFamily = Index;
    }

    VkBool32 PresentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(Device, Index, Surface, &PresentSupport);
    if (PresentSupport) {
      Indices.PresentFamily = Index;
    }

    if (Indices.IsComplete()) {
      break;
    }

    Index++;
  }

  return Indices;
}

QueueFamilyIndices GfxContext_Vulkan::GetQueueFamilies(const VkSurfaceKHR Surface) const { return mQueueFamilies; }

UInt32 GfxContext_Vulkan::GetQueueFamilyIndex(const ERHIQueueFamilyType Family) const {
  switch (Family) {
  case ERHIQueueFamilyType::Graphics:
    return *mQueueFamilies.GraphicsFamily;
  case ERHIQueueFamilyType::Compute:
    std::unreachable();
  case ERHIQueueFamilyType::Transfer:
    std::unreachable();
  case ERHIQueueFamilyType::Present:
    return *mQueueFamilies.PresentFamily;
  }
  return -1;
}

VkQueue GfxContext_Vulkan::GetQueue(const ERHIQueueFamilyType Family) const {
  switch (Family) {
  case ERHIQueueFamilyType::Graphics:
    return mGraphicsQueue;
  case ERHIQueueFamilyType::Compute:
    break;
  case ERHIQueueFamilyType::Transfer:
    break;
  case ERHIQueueFamilyType::Present:
    return mPresentQueue;
  }
  return VK_NULL_HANDLE;
}

#if KITA_EDITOR

void GfxContext_Vulkan::StartUpImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& IO = ImGui::GetIO();
  IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui_ImplVulkan_InitInfo InitInfo{};
  InitInfo.Instance = mInstance;
  InitInfo.PhysicalDevice = mPhysicalDevice;
  InitInfo.Device = mDevice;
  InitInfo.QueueFamily = GetQueueFamilyIndex(ERHIQueueFamilyType::Graphics);
  InitInfo.Queue = mGraphicsQueue;
  InitInfo.PipelineCache = nullptr;
  CreateImGuiRenderPass();
  InitInfo.RenderPass = mImGuiRenderPass;
  CreateImGuiDescriptorPool();
  InitInfo.DescriptorPool = mImGuiDescriptorPool;
  InitInfo.Subpass = 0;
  // TODO: 交换链图像选择, 现在先写死为2
  InitInfo.ImageCount = 2;
  InitInfo.MinImageCount = 2;
  InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  ImGui_ImplVulkan_Init(&InitInfo);
}

void GfxContext_Vulkan::ShutDownImGui() {
  ImGui_ImplVulkan_Shutdown();
  ImGui::DestroyContext();
}

void GfxContext_Vulkan::CreateImGuiRenderPass() {
  // 颜色附件描述
  VkAttachmentDescription ColorAttachment = {};
  ColorAttachment.format = RHISurfaceWindow_Vulkan::ChooseSwapchainFormat(mPhysicalDeviceSwapchainFeatures).format;
  ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // 如果你想保留之前绘制的内容
  ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference ColorAttachmentRef = {};
  ColorAttachmentRef.attachment = 0;
  ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // 子通道（只有一个颜色附件）
  VkSubpassDescription Subpass = {};
  Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  Subpass.colorAttachmentCount = 1;
  Subpass.pColorAttachments = &ColorAttachmentRef;

  // 子通道依赖
  VkSubpassDependency Dependency = {};
  Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  Dependency.dstSubpass = 0;
  Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  Dependency.srcAccessMask = 0; // 这里可以放 COLOR_ATTACHMENT_WRITE_BIT，如果前面有写操作
  Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  // 创建 RenderPass
  VkRenderPassCreateInfo RenderPassInfo = {};
  RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  RenderPassInfo.attachmentCount = 1;
  RenderPassInfo.pAttachments = &ColorAttachment;
  RenderPassInfo.subpassCount = 1;
  RenderPassInfo.pSubpasses = &Subpass;
  RenderPassInfo.dependencyCount = 1;
  RenderPassInfo.pDependencies = &Dependency;
  VkRenderPass MyRenderPass;
  if (auto Result = vkCreateRenderPass(mDevice, &RenderPassInfo, nullptr, &MyRenderPass) != VK_SUCCESS) {
    LOG_CRITICAL_TAG("RHI.Vulkan.Imgui", "创建ImGui RenderPass失败, 错误码={}", Result);
  }
  mImGuiRenderPass = MakeUnique<RHIRenderPass_Vulkan>(MyRenderPass);
}

void GfxContext_Vulkan::CreateImGuiDescriptorPool() {
  // 列出 ImGui 可能会用到的所有 descriptor 类型
  VkDescriptorPoolSize PoolSizes[] = //
      {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
       {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
       {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
       {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
       {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
       {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
       {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
       {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
       {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
       {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
       {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

  VkDescriptorPoolCreateInfo PoolInfo{};
  PoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  PoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT; // ImGui 需要能够释放单个 set
  PoolInfo.maxSets = 1000 * IM_ARRAYSIZE(PoolSizes);                  // 最大 descriptor set 数量
  PoolInfo.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(PoolSizes));
  PoolInfo.pPoolSizes = PoolSizes;

  if (auto Result = vkCreateDescriptorPool(mDevice, &PoolInfo, nullptr, &mImGuiDescriptorPool) != VK_SUCCESS) {
    LOG_CRITICAL_TAG("RHI.Vulkan.ImGui", "创建ImGui Descriptor Pool失败, 错误码={}", Result);
  }
}

#endif

void GfxContext_Vulkan::FindPhysicalDeviceFeatures() {
  VkPhysicalDeviceProperties PhysicalDeviceProperties;
  vkGetPhysicalDeviceProperties(mPhysicalDevice, &PhysicalDeviceProperties);

  VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
  vkGetPhysicalDeviceFeatures(mPhysicalDevice, &PhysicalDeviceFeatures);

  const VkSampleCountFlags MaxSampleCount = PhysicalDeviceProperties.limits.framebufferColorSampleCounts & PhysicalDeviceProperties.limits.framebufferDepthSampleCounts;
  if (MaxSampleCount & VK_SAMPLE_COUNT_64_BIT) {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_64;
  } else if (MaxSampleCount & VK_SAMPLE_COUNT_32_BIT) {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_32;
  } else if (MaxSampleCount & VK_SAMPLE_COUNT_16_BIT) {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_16;
  } else if (MaxSampleCount & VK_SAMPLE_COUNT_8_BIT) {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_8;
  } else if (MaxSampleCount & VK_SAMPLE_COUNT_4_BIT) {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_4;
  } else if (MaxSampleCount & VK_SAMPLE_COUNT_2_BIT) {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_2;
  } else {
    mGfxDeviceFeatures.MaxSampleCount = ERHISampleCount::SC_1;
  }

  mGfxDeviceFeatures.SamplerAnisotropy = PhysicalDeviceFeatures.samplerAnisotropy;
  LOG_INFO_TAG("RHI.Vulkan", "选用设备{}", PhysicalDeviceProperties.deviceName);
}

PhysicalDeviceSwapchainFeatures GfxContext_Vulkan::QueryPhysicalDeviceSwapchainFeatures(const VkPhysicalDevice Device, RHISurfaceWindow& TempWindow) {
  PhysicalDeviceSwapchainFeatures Features{};
  VkSurfaceCapabilitiesKHR SurfaceCapabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &SurfaceCapabilities);
  UInt32 FormatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &FormatCount, nullptr);
  Features.Capabilities.MinImageExtent = {static_cast<int>(SurfaceCapabilities.minImageExtent.width), static_cast<int>(SurfaceCapabilities.minImageExtent.height)};
  Features.Capabilities.MaxImageExtent = {static_cast<int>(SurfaceCapabilities.maxImageExtent.width), static_cast<int>(SurfaceCapabilities.maxImageExtent.height)};
  Features.Capabilities.CurrentExtent = {static_cast<int>(SurfaceCapabilities.currentExtent.width), static_cast<int>(SurfaceCapabilities.currentExtent.height)};
  Features.Capabilities.MinImageCount = SurfaceCapabilities.minImageCount;
  Features.Capabilities.MaxImageCount = SurfaceCapabilities.maxImageCount;
  Features.Capabilities.SupportedTransforms = VkSurfaceTransformToRHISurfaceTransform(SurfaceCapabilities.currentTransform);
  Features.Capabilities.CurrentTransform = static_cast<ERHISurfaceTransformBits>(VkSurfaceTransformToRHISurfaceTransform(SurfaceCapabilities.currentTransform));
  Array<VkSurfaceFormatKHR> Formats;
  Array<VkPresentModeKHR> PresentModes;
  if (FormatCount != 0) {
    Formats.Resize(FormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &FormatCount, Formats.Data());
    UInt32 PresentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &PresentModeCount, nullptr);
    if (PresentModeCount != 0) {
      PresentModes.Resize(PresentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &PresentModeCount, PresentModes.Data());
    }
  }
  for (const auto SurfaceFormatKhr : Formats) {
    PhysicalDeviceSwapchainFeatures::SurfaceFormat NewFormat{};
    NewFormat.Format = VkFormatToRHIFormat(SurfaceFormatKhr.format);
    NewFormat.ColorSpace = VkColorSpaceToRHIColorSpace(SurfaceFormatKhr.colorSpace);
    Features.Formats.Add(NewFormat);
  }
  for (const auto PresentMode : PresentModes) {
    Features.PresentModes.Add(VkPresentModeToRHIPresentMode(PresentMode));
  }
  return Features;
}

VkResult GfxContext_Vulkan::Dyn_CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                                             VkDebugUtilsMessengerEXT* pDebugMessenger) const {
  static const auto Func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
  if (Func != nullptr) {
    return Func(mInstance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    LOG_WARN_TAG("RHI.Vulkan", "未找到函数vkCreateDebugUtilsMessengerEXT");
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void GfxContext_Vulkan::Dyn_DestroyDebugUtilsMessengerEXT(const VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* pAllocator) const {
  static auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
  if (Func != nullptr) {
    Func(mInstance, DebugMessenger, pAllocator);
  } else {
    LOG_WARN_TAG("RHI.Vulkan", "未找到函数vkDestroyDebugUtilsMessengerEXT");
  }
}
