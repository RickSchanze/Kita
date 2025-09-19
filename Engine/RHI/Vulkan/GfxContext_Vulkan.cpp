//
// Created by kita on 25-8-24.
//

#include "GfxContext_Vulkan.h"

#include <utility>

#include "Buffer_Vulkan.h"
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
#include "RHI/ImGuiConfig.h"
#include "RHI/RHIConfig.h"
#include "RHIEnums_Vulkan.h"
#include "RenderPass_Vulkan.h"
#include "ShaderModule_Vulkan.h"
#include "SurfaceWindow_Vulkan.h"
#include "Sync_Vulkan.h"

#if KITA_EDITOR
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_internal.h"
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
  vkDestroyDevice(mDevice, nullptr);
  vkDestroyInstance(mInstance, nullptr);
}

SharedPtr<RHIImage> GfxContext_Vulkan::CreateImageS(const RHIImageDesc& Desc) { return nullptr; }

UniquePtr<RHIImage> GfxContext_Vulkan::CreateImageU(const RHIImageDesc& Desc) { return nullptr; }

SharedPtr<RHIImageView> GfxContext_Vulkan::CreateImageViewS(const RHIImageViewDesc& Desc) { return MakeShared<RHIImageView_Vulkan>(Desc); }
UniquePtr<RHIImageView> GfxContext_Vulkan::CreateImageViewU(const struct RHIImageViewDesc& Desc) { return MakeUnique<RHIImageView_Vulkan>(Desc); }

SharedPtr<RHIFence> GfxContext_Vulkan::CreateFenceS(StringView DebugName) { return MakeShared<RHIFence_Vulkan>(DebugName); }

UniquePtr<RHIFence> GfxContext_Vulkan::CreateFenceU(StringView DebugName) { return MakeUnique<RHIFence_Vulkan>(DebugName); }

UniquePtr<RHISemaphore> GfxContext_Vulkan::CreateSemaphoreU() { return MakeUnique<RHISemaphore_Vulkan>(); }

SharedPtr<RHIImageView> GfxContext_Vulkan::CreateSwapchainImageView(VkImage Img, VkFormat Format) { return MakeShared<RHIImageView_Vulkan>(Img, Format); }

RHISurfaceWindow* GfxContext_Vulkan::CreateSurfaceWindowR(const Int32 Width, const Int32 Height) { return New<RHISurfaceWindow_Vulkan>(Width, Height); }

UInt32 GfxContext_Vulkan::GetNextImage(RHISurfaceWindow* Window, RHISemaphore* WaitSemaphore, RHIFence* WaitFence, bool& NeedRecreation) {
  UInt32 Return;
  const VkResult Result = vkAcquireNextImageKHR(mDevice, static_cast<VkSwapchainKHR>(Window->GetNativeSwapchainObject()), UINT64_MAX,
                                                WaitSemaphore == nullptr ? VK_NULL_HANDLE : WaitSemaphore->GetNativeHandleT<VkSemaphore>(),
                                                WaitFence == nullptr ? VK_NULL_HANDLE : WaitFence->GetNativeHandleT<VkFence>(), &Return);
  if (Result == VK_ERROR_OUT_OF_DATE_KHR) {
    NeedRecreation = true;
  } else if (Result == VK_SUCCESS) {
  } else {
    gLogger.Critical("RHI", "无法获取下一个交换链图像!Code={}", Result);
  }
  return Return;
}

UniquePtr<RHIRenderPass> GfxContext_Vulkan::CreateRenderPassU(const RHIRenderPassDesc& Desc) { return MakeUnique<RHIRenderPass_Vulkan>(Desc); }

UniquePtr<RHIFrameBuffer> GfxContext_Vulkan::CreateFrameBufferU(const RHIFrameBufferDesc& Desc) { return MakeUnique<RHIFrameBuffer_Vulkan>(Desc); }

UniquePtr<RHICommandPool> GfxContext_Vulkan::CreateCommandPoolU(ERHIQueueFamilyType QueueFamily, bool AllowReset) { return MakeUnique<RHICommandPool_Vulkan>(QueueFamily, AllowReset); }

UniquePtr<RHIShaderModule> GfxContext_Vulkan::CreateShaderModuleU(const RHIShaderModuleDesc& Desc) { return MakeUnique<RHIShaderModule_Vulkan>(Desc); }

UniquePtr<RHIDescriptorSetLayout> GfxContext_Vulkan::CreateDescriptorSetLayoutU(const RHIDescriptorSetLayoutDesc& Desc) { return MakeUnique<RHIDescriptorSetLayout_Vulkan>(Desc); }

UniquePtr<RHIDescriptorPool> GfxContext_Vulkan::CreateDescriptorPoolU(const RHIDescriptorPoolDesc& Desc) { return MakeUnique<RHIDescriptorPool_Vulkan>(Desc); }

UniquePtr<RHIPipelineLayout> GfxContext_Vulkan::CreatePipelineLayoutU(const RHIPipelineLayoutDesc& Desc) { return MakeUnique<RHIPipelineLayout_Vulkan>(Desc); }

UniquePtr<RHIPipeline> GfxContext_Vulkan::CreatePipelineU(const RHIGraphicsPipelineDesc& Desc) { return MakeUnique<RHIPipeline_Vulkan>(Desc); }

UniquePtr<RHIBuffer> GfxContext_Vulkan::CreateBufferU(const struct RHIBufferDesc& Desc) { return MakeUnique<RHIBuffer_Vulkan>(Desc); }

SharedPtr<RHIBuffer> GfxContext_Vulkan::CreateBufferS(const struct RHIBufferDesc& Desc) { return MakeShared<RHIBuffer_Vulkan>(Desc); }

bool GfxContext_Vulkan::Present(const RHIPresentParams& Params) {
  VkPresentInfoKHR PresentInfo{};
  PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  PresentInfo.pImageIndices = &Params.ImageIndex;
  PresentInfo.swapchainCount = 1;
  const auto Swapchain = static_cast<VkSwapchainKHR>(Params.SurfaceWindow->GetNativeSwapchainObject());
  PresentInfo.pSwapchains = &Swapchain;
  PresentInfo.swapchainCount = 1;
  Array<VkSemaphore> WaitSemaphores;
  for (auto& Semaphore : Params.WaitSemaphores) {
    WaitSemaphores.Add(static_cast<VkSemaphore>(Semaphore->GetNativeHandle()));
  }
  PresentInfo.pWaitSemaphores = WaitSemaphores.Data();
  PresentInfo.waitSemaphoreCount = WaitSemaphores.Count();
  const VkResult Result = vkQueuePresentKHR(mGraphicsQueue, &PresentInfo);
  if (Result == VK_ERROR_OUT_OF_DATE_KHR || Result == VK_SUBOPTIMAL_KHR) {
    return false;
  }
  if (Result != VK_SUCCESS) {
    gLogger.Critical("RHI", "呈现(Present)图像失败! Code={}", Result);
  }
  return true;
}

void GfxContext_Vulkan::WaitDeviceIdle() { vkDeviceWaitIdle(mDevice); }

VkDeviceMemory GfxContext_Vulkan::AllocateMemory(VkBuffer For, VkMemoryPropertyFlags MemoryFlags) const {
  if (For == VK_NULL_HANDLE)
    return VK_NULL_HANDLE;
  VkMemoryRequirements MemReqs;
  vkGetBufferMemoryRequirements(mDevice, For, &MemReqs);
  VkMemoryAllocateInfo AllocInfo{};
  AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  AllocInfo.allocationSize = MemReqs.size;
  AllocInfo.memoryTypeIndex = FindMemoryType(MemReqs.memoryTypeBits, MemoryFlags);
  VkDeviceMemory Memory = VK_NULL_HANDLE;
  if (auto Result = vkAllocateMemory(mDevice, &AllocInfo, nullptr, &Memory) != VK_SUCCESS) {
    gLogger.Error("RHI", "为Buffer '{:p}' 分配内存失败! Code={}", Ptr(For), Result);
    return VK_NULL_HANDLE;
  }
  return Memory;
}

VkDeviceMemory GfxContext_Vulkan::AllocateMemory(VkImage For, VkMemoryPropertyFlags MemoryFlags) const {
  if (For == VK_NULL_HANDLE)
    return VK_NULL_HANDLE;
  VkMemoryRequirements MemReqs;
  vkGetImageMemoryRequirements(mDevice, For, &MemReqs);
  VkMemoryAllocateInfo AllocInfo{};
  AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  AllocInfo.allocationSize = MemReqs.size;
  AllocInfo.memoryTypeIndex = FindMemoryType(MemReqs.memoryTypeBits, MemoryFlags);
  VkDeviceMemory Memory = VK_NULL_HANDLE;
  if (auto Result = vkAllocateMemory(mDevice, &AllocInfo, nullptr, &Memory) != VK_SUCCESS) {
    gLogger.Error("RHI", "为Image '{:p}' 分配内存失败! Code={}", Ptr(For), Result);
    return VK_NULL_HANDLE;
  }
  return Memory;
}

void GfxContext_Vulkan::DeallocateMemory(VkDeviceMemory Memory) const { vkFreeMemory(mDevice, Memory, nullptr); }

UInt32 GfxContext_Vulkan::FindMemoryType(UInt32 TypeFilter, VkMemoryPropertyFlags Properties) const {
  VkPhysicalDeviceMemoryProperties MemoryProperties;
  vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &MemoryProperties);
  for (UInt32 i = 0; i < MemoryProperties.memoryTypeCount; i++) {
    if ((TypeFilter & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties) {
      return i;
    }
  }
  return UINT32_MAX;
}

PhysicalDeviceSwapchainFeatures GfxContext_Vulkan::GetPhysicalDeviceSwapchainFeatures(RHISurfaceWindow& Window) const { return QueryPhysicalDeviceSwapchainFeatures(mPhysicalDevice, Window); }

/// ============================ IMGUI 多线程版本的Helper =================
struct ImDrawDataSnapshotEntry {
  ImDrawList* SrcCopy = NULL; // Drawlist owned by main context
  ImDrawList* OurCopy = NULL; // Our copy
  double LastUsedTime = 0.0;
};

struct ImDrawDataSnapshot {
  // Members
  ImDrawData DrawData;
  ImPool<ImDrawDataSnapshotEntry> Cache;
  float MemoryCompactTimer = 20.0f; // Discard unused data after 20 seconds

  // Functions
  ~ImDrawDataSnapshot() { Clear(); }
  void Clear();
  void SnapUsingSwap(ImDrawData* src, double current_time); // Efficient snapshot by swapping data, meaning "src_list" is unusable.
  // void                          SnapUsingCopy(ImDrawData* src, double current_time); // Deep-copy snapshop

  // Internals
  ImGuiID GetDrawListID(ImDrawList* src_list) { return ImHashData(&src_list, sizeof(src_list)); } // Hash pointer
  ImDrawDataSnapshotEntry* GetOrAddEntry(ImDrawList* src_list) { return Cache.GetOrAddByKey(GetDrawListID(src_list)); }
};

void ImDrawDataSnapshot::Clear() {
  for (int n = 0; n < Cache.GetMapSize(); n++)
    if (ImDrawDataSnapshotEntry* entry = Cache.TryGetMapData(n))
      IM_DELETE(entry->OurCopy);
  Cache.Clear();
  DrawData.Clear();
}

void ImDrawDataSnapshot::SnapUsingSwap(ImDrawData* src, double current_time) {
  ImDrawData* dst = &DrawData;
  IM_ASSERT(src != dst && src->Valid);

  // Copy all fields except CmdLists[]
  ImVector<ImDrawList*> backup_draw_list;
  backup_draw_list.swap(src->CmdLists);
  IM_ASSERT(src->CmdLists.Data == NULL);
  *dst = *src;
  backup_draw_list.swap(src->CmdLists);

  // Swap and mark as used
  for (ImDrawList* src_list : src->CmdLists) {
    ImDrawDataSnapshotEntry* entry = GetOrAddEntry(src_list);
    if (entry->OurCopy == NULL) {
      entry->SrcCopy = src_list;
      entry->OurCopy = IM_NEW(ImDrawList)(src_list->_Data);
    }
    IM_ASSERT(entry->SrcCopy == src_list);
    entry->SrcCopy->CmdBuffer.swap(entry->OurCopy->CmdBuffer); // Cheap swap
    entry->SrcCopy->IdxBuffer.swap(entry->OurCopy->IdxBuffer);
    entry->SrcCopy->VtxBuffer.swap(entry->OurCopy->VtxBuffer);
    entry->SrcCopy->CmdBuffer.reserve(entry->OurCopy->CmdBuffer.Capacity); // Preserve bigger size to avoid reallocs for two consecutive frames
    entry->SrcCopy->IdxBuffer.reserve(entry->OurCopy->IdxBuffer.Capacity);
    entry->SrcCopy->VtxBuffer.reserve(entry->OurCopy->VtxBuffer.Capacity);
    entry->LastUsedTime = current_time;
    dst->CmdLists.push_back(entry->OurCopy);
  }

  // Cleanup unused data
  const double gc_threshold = current_time - MemoryCompactTimer;
  for (int n = 0; n < Cache.GetMapSize(); n++)
    if (ImDrawDataSnapshotEntry* entry = Cache.TryGetMapData(n)) {
      if (entry->LastUsedTime > gc_threshold)
        continue;
      IM_DELETE(entry->OurCopy);
      Cache.Remove(GetDrawListID(entry->SrcCopy), entry);
    }
};

class ImGuiDrawTask : public TaskNode {
public:
  VkCommandBuffer Cmd;
  VkFramebuffer FrameBuffer;
  VkRenderPass RenderPass;
  UInt32 Width;
  UInt32 Height;
  UniquePtr<ImDrawDataSnapshot> DrawData;

  explicit ImGuiDrawTask(VkCommandBuffer Cmd, VkFramebuffer FrameBuffer, VkRenderPass RenderPass, UInt32 Width, UInt32 Height, UniquePtr<ImDrawDataSnapshot>&& Snapshot)
      : Cmd(Cmd), FrameBuffer(FrameBuffer), RenderPass(RenderPass), Width(Width), Height(Height), DrawData(std::move(Snapshot)) {}

  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }
  virtual ETaskNodeResult Run() override {
    CPU_PROFILING_SCOPE;
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
    ImGui_ImplVulkan_RenderDrawData(&DrawData->DrawData, Cmd);
    vkCmdEndRenderPass(Cmd);
    return ETaskNodeResult::Success;
  }
};

TaskHandle GfxContext_Vulkan::DrawImGui(RHICommandBuffer* Buffer, RHIFrameBuffer* FrameBuffer, UInt32 Width, UInt32 Height) {
  UniquePtr<ImDrawDataSnapshot> Snapshot = MakeUnique<ImDrawDataSnapshot>();
  ImDrawData* DrawData = ImGui::GetDrawData();
  Snapshot->SnapUsingSwap(DrawData, ImGui::GetTime());
  return TaskGraph::CreateTask<ImGuiDrawTask>( //
      "", {}, Buffer->GetNativeHandleT<VkCommandBuffer>(), FrameBuffer->GetNativeHandleT<VkFramebuffer>(), mImGuiRenderPass->GetNativeHandleT<VkRenderPass>(), Width, Height, std::move(Snapshot));
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
  SubmitInfo.pWaitSemaphores = WaitSemaphores.Data();
  const VkPipelineStageFlags WaitPipelineStageFlags = RHIPipelineStageToVkPipelineStage(Params.WaitPipelineStages);
  SubmitInfo.pWaitDstStageMask = &WaitPipelineStageFlags;
  const VkFence WaitFence = Params.Fence == nullptr ? VK_NULL_HANDLE : Params.Fence->GetNativeHandleT<VkFence>();
  const VkQueue Queue = GetQueue(Params.TargetQueueFamily);
  VkResult Result = vkQueueSubmit(Queue, 1, &SubmitInfo, WaitFence);
  if (Result != VK_SUCCESS) {
    gLogger.Error("RHI", "提交渲染指令失败!Code={}", Result);
  }
}

struct Task_SubmitAsync : TaskNode {
public:
  RHICommandBufferSubmitParams Params;
  explicit Task_SubmitAsync(RHICommandBufferSubmitParams InParams) : Params(std::move(InParams)) {}

  [[nodiscard]] virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }

  virtual ETaskNodeResult Run() override {
    GetVulkanGfxContexRef().Submit(Params);
    return ETaskNodeResult::Success;
  }
};

TaskHandle GfxContext_Vulkan::SubmitAsync(const struct RHICommandBufferSubmitParams& Params, const Array<TaskHandle>& Dependencies) {
  return TaskGraph::CreateTask<Task_SubmitAsync>("", Dependencies, Params);
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
    gLogger.Error("RHI.Validation", "{}", pCallbackData->pMessage);
  } else if (MessageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    gLogger.Warn("RHI.Validation", "{}", pCallbackData->pMessage);
  } else {
    gLogger.Info("RHI.Validation", "{}", pCallbackData->pMessage);
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
      gLogger.Warn("RHI", "Vulkan验证层不支持!将自动关闭.");
    }
  }
  InstanceInfo.enabledExtensionCount = static_cast<UInt32>(Extensions.Count());
  InstanceInfo.ppEnabledExtensionNames = Extensions.Data();
  InstanceInfo.enabledLayerCount = static_cast<UInt32>(Layers.Count());
  InstanceInfo.ppEnabledLayerNames = Layers.Data();
  InstanceInfo.pNext = &MessengerCreateInfo;
  ASSERT_MSG(vkCreateInstance(&InstanceInfo, nullptr, &mInstance) == VK_SUCCESS, "创建Vulkan实例失败");
  gLogger.Info("RHI", "创建Vulkan Instance成功!");
  gLogger.Info("RHI", "开启的扩展:");
  for (const auto& Extension : Extensions) {
    gLogger.Info("RHI", "  {}", Extension);
  }
  gLogger.Info("RHI", "开启的Layer:");
  for (const auto& Layer : Layers) {
    gLogger.Info("RHI", "  {}", Layer);
  }
}

void GfxContext_Vulkan::SetupDebugMessenger() {
  if (!mEnabledValidationLayer)
    return;
  VkDebugUtilsMessengerCreateInfoEXT Info{};
  PopulateDebugMessengerCreateInfo(Info);
  ASSERT_MSG(Dyn_CreateDebugUtilsMessengerEXT(&Info, nullptr, &mDebugMessenger) == VK_SUCCESS, "Vulkan调试信息设置失败");
  Dyn_GetSetObjectDebugNameFuncPtr();
}

void GfxContext_Vulkan::SelectPhysicalDevice(RHISurfaceWindow& TempWindow) {
  UInt32 DeviceCount = 0;
  vkEnumeratePhysicalDevices(mInstance, &DeviceCount, nullptr);

  if (DeviceCount == 0) {
    gLogger.Critical("RHI", "没有可用的Vulkan设备!");
  }

  Array<VkPhysicalDevice> devices(DeviceCount);
  vkEnumeratePhysicalDevices(mInstance, &DeviceCount, devices.Data());

  for (const auto& Device : devices) {
    if (IsDeviceSuitable(Device, TempWindow, mQueueFamilies, mSwapchainFormat)) {
      mPhysicalDevice = Device;
      break;
    }
  }

  if (mPhysicalDevice == VK_NULL_HANDLE) {
    gLogger.Critical("RHI", "没有可用的Vulkan设备!");
  } else {
    FindPhysicalDeviceFeatures();
  }
}

bool GfxContext_Vulkan::IsDeviceSuitable(VkPhysicalDevice Device, RHISurfaceWindow& TempWindow, QueueFamilyIndices& OutFamilyIndices, VkSurfaceFormatKHR& OutFormat) const {
  bool ExtensionsSupported = CheckDeviceExtensionSupport(Device);

  PhysicalDeviceSwapchainFeatures SwapchainFeatures = QueryPhysicalDeviceSwapchainFeatures(Device, TempWindow);
  bool SwapchainAvailable = !SwapchainFeatures.Formats.Empty() && !SwapchainFeatures.PresentModes.Empty();

  VkPhysicalDeviceFeatures SupportedFeatures{};
  vkGetPhysicalDeviceFeatures(Device, &SupportedFeatures);

  OutFamilyIndices = FindQueueFamilies(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()));
  OutFormat = RHISurfaceWindow_Vulkan::ChooseSwapchainFormat(SwapchainFeatures);

  return ExtensionsSupported && SupportedFeatures.samplerAnisotropy && SwapchainAvailable && OutFamilyIndices.IsComplete();
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
    gLogger.Critical("RHI", "初始化Vulkan上下文失败, Code={}", Code);
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
    // TODO: 当前Transfer和图形混用
    return *mQueueFamilies.GraphicsFamily;
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

void GfxContext_Vulkan::SetDebugName(StringView DebugName, VkObjectType ObjectType, UInt64 Object) const {
  if (mSetDebugUtilsObjectNameEXT) {
    VkDebugUtilsObjectNameInfoEXT NameInfo{};
    NameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    NameInfo.objectType = ObjectType;
    NameInfo.objectHandle = Object;
    NameInfo.pObjectName = DebugName.Data();
    mSetDebugUtilsObjectNameEXT(mDevice, &NameInfo);
  }
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
  InitInfo.RenderPass = mImGuiRenderPass->GetNativeHandleT<VkRenderPass>();
  CreateImGuiDescriptorPool();
  InitInfo.DescriptorPool = mImGuiDescriptorPool;
  InitInfo.Subpass = 0;
  // TODO: 交换链图像选择, 现在先写死为2
  InitInfo.ImageCount = 2;
  InitInfo.MinImageCount = 2;
  InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  ImGui_ImplVulkan_Init(&InitInfo);
  ConfigManager::GetConfigRef<ImGuiConfig>().ApplyImGuiSettings();
}

void GfxContext_Vulkan::ShutDownImGui() {
  ImGui_ImplVulkan_Shutdown();
  mImGuiRenderPass = nullptr;
  vkDestroyDescriptorPool(mDevice, mImGuiDescriptorPool, nullptr);
  ImGui::DestroyContext();
}

void GfxContext_Vulkan::CreateImGuiRenderPass() {
  // 颜色附件描述
  VkAttachmentDescription ColorAttachment = {};
  ColorAttachment.format = mSwapchainFormat.format;
  ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // 如果你想保留之前绘制的内容
  ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
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
    gLogger.Critical("RHI.Imgui", "创建ImGui RenderPass失败, 错误码={}", Result);
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
    gLogger.Critical("RHI.ImGui", "创建ImGui Descriptor Pool失败, 错误码={}", Result);
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
  gLogger.Info("RHI", "选用设备{}", PhysicalDeviceProperties.deviceName);
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
    gLogger.Warn("RHI", "未找到函数vkCreateDebugUtilsMessengerEXT");
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void GfxContext_Vulkan::Dyn_DestroyDebugUtilsMessengerEXT(const VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* pAllocator) const {
  static auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
  if (Func != nullptr) {
    Func(mInstance, DebugMessenger, pAllocator);
  } else {
    gLogger.Warn("RHI", "未找到函数vkDestroyDebugUtilsMessengerEXT");
  }
}

void GfxContext_Vulkan::Dyn_GetSetObjectDebugNameFuncPtr() {
  mSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(mInstance, "vkSetDebugUtilsObjectNameEXT"));
}
