//
// Created by kita on 25-8-24.
//

#include "GfxContext_Vulkan.h"

#include "Core/Config/ConfigManager.h"
#include "../SurfaceWindow/VulkanGLFWSurfaceWindow.hpp"
#include "RHIEnums_Vulkan.h"
#include "VulkanCommandBuffer.hpp"
#include "VulkanDescriptorSet.hpp"
#include "FrameBuffer_Vulkan.h"
#include "Image_Vulkan.h"
#include "ImageView_Vulkan.h"
#include "VulkanPipeline.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanShaderModule.hpp"
#include "VulkanSync.hpp"

const char* VALIDATION_LAYER_NAME = "VK_LAYER_KHRONOS_validation";


void kita::rhi::VulkanGfxContext::OnPostGfxContextCreated(GfxContext* Context)
{
    VulkanGfxContext* Ctx = static_cast<kita::rhi::VulkanGfxContext*>(Context);
    VulkanGLFWSurfaceWindow TempWindow{-1, -1, true};
    TempWindow.CreateSurface(Ctx->mInstance);
    Ctx->SetupDebugMessenger();
    Ctx->SelectPhysicalDevice(TempWindow);
    Ctx->CreateLogicalDevice(TempWindow);
    TempWindow.DestroySurface(Ctx->mInstance);
}

kita::rhi::VulkanGfxContext::VulkanGfxContext()
{
    // 此初始化函数仅仅创建Vulkan实例 选择物理设备并创建逻辑设备, 而不关心交换链、渲染管线等
    CreateInstance();
    // 创建一个临时的SurfaceWindow用于创建Device
    Evt_PostGfxContextCreated.Add(&VulkanGfxContext::OnPostGfxContextCreated);
}

kita::rhi::VulkanGfxContext::~VulkanGfxContext()
{
    Dyn_DestroyDebugUtilsMessengerEXT(mDebugMessenger, nullptr);
    vkDestroyInstance(mInstance, nullptr);
}

std::shared_ptr<kita::rhi::Image> kita::rhi::VulkanGfxContext::CreateImage(const ImageDesc& Desc)
{
    return nullptr;
}

std::shared_ptr<kita::rhi::ImageView> kita::rhi::VulkanGfxContext::CreateImageViewS(const ImageViewDesc& Desc)
{
    return std::make_shared<VulkanImageView>(Desc);
}

std::shared_ptr<kita::rhi::Fence> kita::rhi::VulkanGfxContext::CreateFenceS()
{
    return std::make_shared<VulkanFence>();
}

std::unique_ptr<kita::rhi::Fence> kita::rhi::VulkanGfxContext::CreateFenceU()
{
    return std::make_unique<VulkanFence>();
}

std::unique_ptr<kita::rhi::Semaphore> kita::rhi::VulkanGfxContext::CreateSemaphoreU()
{
    return std::make_unique<VulkanSemaphore>();
}

std::shared_ptr<kita::rhi::ImageView> kita::rhi::VulkanGfxContext::CreateSwapchainImageView(VkImage Img, VkFormat Format)
{
    return std::make_shared<VulkanImageView>(Img, Format);
}

kita::rhi::SurfaceWindow* kita::rhi::VulkanGfxContext::CreateSurfaceWindowR(int32_t Width, int32_t Height)
{
    return new VulkanGLFWSurfaceWindow(Width, Height);
}

uint32_t kita::rhi::VulkanGfxContext::GetNextImage(SurfaceWindow* Window, Semaphore* WaitSemaphore, Fence* WaitFence, bool& NeedRecreation)
{
    uint32_t Return;
    const VkResult Result = vkAcquireNextImageKHR(mDevice, static_cast<VkSwapchainKHR>(Window->GetNativeSwapchainObject()), UINT64_MAX,
                                                  WaitSemaphore == nullptr ? VK_NULL_HANDLE : WaitSemaphore->GetNativeHandleT<VkSemaphore>(),
                                                  WaitFence == nullptr ? VK_NULL_HANDLE : WaitFence->GetNativeHandleT<VkFence>(), &Return);
    if (Result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        NeedRecreation = true;
    }
    else if (Result == VK_SUCCESS)
    {
    }
    else
    {
        LogFatalStream("Vulkan") << "无法获取下一个交换链图像!" << Log::EndLine;
    }
    return Return;
}

std::unique_ptr<kita::rhi::RenderPass> kita::rhi::VulkanGfxContext::CreateRenderPassU(const RenderPassDesc& Desc)
{
    return std::make_unique<VulkanRenderPass>(Desc);
}

std::unique_ptr<kita::rhi::FrameBuffer> kita::rhi::VulkanGfxContext::CreateFrameBufferU(const FrameBufferDesc& Desc)
{
    return std::make_unique<VulkanFrameBuffer>(Desc);
}

std::unique_ptr<kita::rhi::CommandPool> kita::rhi::VulkanGfxContext::CreateCommandPoolU(EQueueFamilyType QueueFamily)
{
    return std::make_unique<VulkanCommandPool>(QueueFamily);
}

std::unique_ptr<kita::rhi::ShaderModule> kita::rhi::VulkanGfxContext::CreateShaderModuleU(const ShaderModuleDesc& Desc)
{
    return std::make_unique<VulkanShaderModule>(Desc);
}

std::unique_ptr<kita::rhi::DescriptorSetLayout> kita::rhi::VulkanGfxContext::CreateDescriptorSetLayoutU(const DescriptorSetLayoutDesc& Desc)
{
    return std::make_unique<VulkanDescriptorSetLayout>(Desc);
}

std::unique_ptr<kita::rhi::DescriptorPool> kita::rhi::VulkanGfxContext::CreateDescriptorPoolU(const DescriptorPoolDesc& Desc)
{
    return std::make_unique<VulkanDescriptorPool>(Desc);
}

std::unique_ptr<kita::rhi::PipelineLayout> kita::rhi::VulkanGfxContext::CreatePipelineLayoutU(const PipelineLayoutDesc& Desc)
{
    return std::make_unique<VulkanPipelineLayout>(Desc);
}

std::unique_ptr<kita::rhi::Pipeline> kita::rhi::VulkanGfxContext::CreatePipeline(const GraphicsPipelineDesc& Desc)
{
    return std::make_unique<VulkanPipeline>(Desc);
}

void kita::rhi::VulkanGfxContext::Submit(const CommandBufferSubmitParams& Params)
{
    VkSubmitInfo SubmitInfo{};
    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.commandBufferCount = 1;
    VkCommandBuffer CommandBuffer = Params.CommandBuffer->GetNativeHandleT<VkCommandBuffer>();
    SubmitInfo.pCommandBuffers = &CommandBuffer;
    SubmitInfo.signalSemaphoreCount = Params.SignalSemaphores.size();
    container::inplace_vector<VkSemaphore, KITA_MAX_COMMAND_BUFFER_SUBMIT_SIGNAL_SEMAPHORES_COUNT> SignalSemaphores;
    for (const auto& Semaphore : Params.SignalSemaphores)
    {
        SignalSemaphores.push_back(Semaphore->GetNativeHandleT<VkSemaphore>());
    }
    SubmitInfo.pSignalSemaphores = SignalSemaphores.data();
    container::inplace_vector<VkSemaphore, KITA_MAX_COMMAND_BUFFER_SUBMIT_WAIT_SEMAPHORES_COUNT> WaitSemaphores;
    for (const auto& Semaphore : Params.WaitSemaphores)
    {
        WaitSemaphores.push_back(Semaphore->GetNativeHandleT<VkSemaphore>());
    }
    SubmitInfo.waitSemaphoreCount = WaitSemaphores.size();
    VkPipelineStageFlags WaitPipelineStageFlags = RHIPipelineStageToVkPipelineStage(Params.WaitPipelineStages);
    SubmitInfo.pWaitDstStageMask = &WaitPipelineStageFlags;
    VkFence WaitFence = Params.Fence == nullptr ? VK_NULL_HANDLE : Params.Fence->GetNativeHandleT<VkFence>();
    VkQueue Queue = GetQueue(Params.TargetQueueFamily);
    VkResult Result = vkQueueSubmit(Queue, 1, &SubmitInfo, WaitFence);
    if (Result != VK_SUCCESS)
    {
        LogFatalStream("Vulkan") << "提交渲染指令失败, 错误码=" << Result << Log::EndLine;
    }
}

bool kita::rhi::VulkanGfxContext::IsLayerSupported(const char* LayerName)
{
    uint32_t LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(LayerCount);
    vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

    for (const auto& layerProperties : AvailableLayers)
    {
        if (strcmp(LayerName, layerProperties.layerName) == 0)
        {
            return true;
        }
    }

    return false;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT MessageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    if (MessageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        LogErrorStream("Vulkan") << pCallbackData->pMessage << kita::Log::EndLine;
    }
    else if (MessageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        LogWarnStream("Vulkan") << pCallbackData->pMessage << kita::Log::EndLine;
    }
    else
    {
        LogInfoStream("Vulkan") << pCallbackData->pMessage << kita::Log::EndLine;
    }
    return VK_FALSE;
}

static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& Info)
{
    Info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    Info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    Info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    Info.pfnUserCallback = &DebugCallback;
}

void kita::rhi::VulkanGfxContext::CreateInstance()
{
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
    std::vector<const char*> Extensions;
    std::vector<const char*> Layers;
    auto& RHICfg = ConfigManager::GetConfig<RHIConfig>();
    switch (RHICfg.GetSurfaceWindowType())
    {
    case ESurfaceWindowType::GLFW:
        GetVulkanGLFWSurfaceWindowExtensions(Extensions);
        break;
    default:
        KITA_UNREACHABLE();
    }
    VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo{};
    if (RHICfg.GetEnableVulkanValidationLayer())
    {
        if (IsLayerSupported(VALIDATION_LAYER_NAME))
        {
            Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            Layers.push_back(VALIDATION_LAYER_NAME);
            // DebugMessenger
            PopulateDebugMessengerCreateInfo(MessengerCreateInfo);
            mEnabledValidationLayer = true;
        }
        else
        {
            LogErrorStream() << "Vulkan验证层不支持, 自动关闭验证层" << Log::EndLine;
        }
    }
    InstanceInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
    InstanceInfo.ppEnabledExtensionNames = Extensions.data();
    InstanceInfo.enabledLayerCount = static_cast<uint32_t>(Layers.size());
    InstanceInfo.ppEnabledLayerNames = Layers.data();
    InstanceInfo.pNext = &MessengerCreateInfo;
    KITA_ASSERT(vkCreateInstance(&InstanceInfo, nullptr, &mInstance) == VK_SUCCESS, "创建Vulkan实例失败");
    LogInfoStream("Vulkan") << "创建Vulkan Instance成功!" << Log::EndLine;
    LogInfoStream("Vulkan") << "开启的扩展:" << Log::EndLine;
    for (const auto& Extension : Extensions)
    {
        LogInfoStream("Vulkan") << "  " << Extension << Log::EndLine;
    }
    LogInfoStream("Vulkan") << "开启的层" << Log::EndLine;
    for (const auto& Layer : Layers)
    {
        LogInfoStream("Vulkan") << "  " << Layer << Log::EndLine;
    }
}

void kita::rhi::VulkanGfxContext::SetupDebugMessenger()
{
    if (!mEnabledValidationLayer)
        return;
    VkDebugUtilsMessengerCreateInfoEXT Info{};
    PopulateDebugMessengerCreateInfo(Info);
    KITA_ASSERT(Dyn_CreateDebugUtilsMessengerEXT(&Info, nullptr, &mDebugMessenger) == VK_SUCCESS, "Vulkan调试信息设置失败");
}

void kita::rhi::VulkanGfxContext::SelectPhysicalDevice(SurfaceWindow& TempWindow)
{
    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &DeviceCount, nullptr);

    if (DeviceCount == 0)
    {
        LogFatalStream() << "没有可用的Vulkan设备!" << Log::EndLine;
    }

    std::vector<VkPhysicalDevice> devices(DeviceCount);
    vkEnumeratePhysicalDevices(mInstance, &DeviceCount, devices.data());

    for (const auto& Device : devices)
    {
        if (IsDeviceSuitable(Device, TempWindow))
        {
            mPhysicalDevice = Device;
            break;
        }
    }

    if (mPhysicalDevice == VK_NULL_HANDLE)
    {
        LogFatalStream() << "没有可用的Vulkan设备!" << Log::EndLine;
    }
    else
    {
        FindPhysicalDeviceFeatures();
        mPhysicalDeviceSwapchainFeatures = QueryPhysicalDeviceSwapchainFeatures(mPhysicalDevice, TempWindow);
    }
}

bool kita::rhi::VulkanGfxContext::IsDeviceSuitable(VkPhysicalDevice Device, SurfaceWindow& TempWindow) const
{
    bool ExtensionsSupported = CheckDeviceExtensionSupport(Device);

    PhysicalDeviceSwapchainFeatures SwapchainFeatures = QueryPhysicalDeviceSwapchainFeatures(Device, TempWindow);
    bool SwapchainAvailable = !SwapchainFeatures.Formats.empty() && !SwapchainFeatures.PresentModes.empty();

    VkPhysicalDeviceFeatures SupportedFeatures{};
    vkGetPhysicalDeviceFeatures(Device, &SupportedFeatures);

    QueueFamilyIndices Indices = FindQueueFamilies(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()));

    return ExtensionsSupported && SupportedFeatures.samplerAnisotropy && SwapchainAvailable && Indices.IsComplete();
}

bool kita::rhi::VulkanGfxContext::CheckDeviceExtensionSupport(VkPhysicalDevice Device) const
{
    uint32_t Extension;
    vkEnumerateDeviceExtensionProperties(Device, nullptr, &Extension, nullptr);

    std::vector<VkExtensionProperties> AvailableExtensions(Extension);
    vkEnumerateDeviceExtensionProperties(Device, nullptr, &Extension, AvailableExtensions.data());

    std::set<std::string> RequiredExtensions(mDeviceExtensions.begin(), mDeviceExtensions.end());

    for (const auto& extension : AvailableExtensions)
    {
        RequiredExtensions.erase(extension.extensionName);
    }

    return RequiredExtensions.empty();
}

void kita::rhi::VulkanGfxContext::CreateLogicalDevice(SurfaceWindow& TempWindow)
{
    std::vector<VkDeviceQueueCreateInfo> QueueInfos{};
    float QueuePriority = 1.0f;
    const QueueFamilyIndices Indices = FindQueueFamilies(mPhysicalDevice, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()));
    std::set<uint32_t> UniqueQueueFamilies = {Indices.GraphicsFamily.value(), Indices.PresentFamily.value()};
    for (uint32_t QueueFamily : UniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo QueueInfo{};
        QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        QueueInfo.queueFamilyIndex = QueueFamily;
        QueueInfo.queueCount = 1;
        QueueInfo.pQueuePriorities = &QueuePriority;
        QueueInfos.push_back(QueueInfo);
    }

    VkPhysicalDeviceFeatures DeviceFeatures{};
    DeviceFeatures.samplerAnisotropy = true;

    VkDeviceCreateInfo DeviceInfo{};
    DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DeviceInfo.pQueueCreateInfos = QueueInfos.data();
    DeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueInfos.size());
    DeviceInfo.pEnabledFeatures = &DeviceFeatures;
    DeviceInfo.enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size());
    DeviceInfo.ppEnabledExtensionNames = mDeviceExtensions.data();
    std::vector<const char*> ValidationLayerNames = {VALIDATION_LAYER_NAME};
    if (mEnabledValidationLayer)
    {
        DeviceInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayerNames.size());
        DeviceInfo.ppEnabledLayerNames = ValidationLayerNames.data();
    }
    if (VkResult Code = vkCreateDevice(mPhysicalDevice, &DeviceInfo, nullptr, &mDevice); Code != VK_SUCCESS)
    {
        LogFatalStream("Vulkan") << "初始化Vulkan上下文失败, 创建Device时返回了" << Code << Log::EndLine;
    }
}

kita::rhi::QueueFamilyIndices kita::rhi::VulkanGfxContext::FindQueueFamilies(VkPhysicalDevice Device, VkSurfaceKHR Surface)
{
    QueueFamilyIndices Indices;

    uint32_t QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

    int Index = 0;
    for (const auto& queueFamily : QueueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            Indices.GraphicsFamily = Index;
        }

        VkBool32 PresentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(Device, Index, Surface, &PresentSupport);
        if (PresentSupport)
        {
            Indices.PresentFamily = Index;
        }

        if (Indices.IsComplete())
        {
            break;
        }

        Index++;
    }

    return Indices;
}

kita::rhi::QueueFamilyIndices kita::rhi::VulkanGfxContext::GetQueueFamilies(VkSurfaceKHR Surface) const
{
    return FindQueueFamilies(mPhysicalDevice, Surface);
}

uint32_t kita::rhi::VulkanGfxContext::GetQueueFamilyIndex(EQueueFamilyType Family) const
{
    switch (Family)
    {
    case EQueueFamilyType::Graphics:
        return mQueueFamilies.GraphicsFamily.value();
    case EQueueFamilyType::Compute:
        KITA_UNREACHABLE();
    case EQueueFamilyType::Transfer:
        KITA_UNREACHABLE();
    case EQueueFamilyType::Present:
        return mQueueFamilies.PresentFamily.value();
    }
    return -1;
}

VkQueue kita::rhi::VulkanGfxContext::GetQueue(EQueueFamilyType Family) const
{
    switch (Family)
    {
    case EQueueFamilyType::Graphics:
        return mGraphicsQueue;
    case EQueueFamilyType::Compute:
        break;
    case EQueueFamilyType::Transfer:
        break;
    case EQueueFamilyType::Present:
        return mPresentQueue;
    }
    return VK_NULL_HANDLE;
}

void kita::rhi::VulkanGfxContext::FindPhysicalDeviceFeatures()
{
    VkPhysicalDeviceProperties PhysicalDeviceProperties;
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &PhysicalDeviceProperties);

    VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures(mPhysicalDevice, &PhysicalDeviceFeatures);

    VkSampleCountFlags MaxSampleCount =
        PhysicalDeviceProperties.limits.framebufferColorSampleCounts & PhysicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if (MaxSampleCount & VK_SAMPLE_COUNT_64_BIT)
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_64;
    }
    else if (MaxSampleCount & VK_SAMPLE_COUNT_32_BIT)
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_32;
    }
    else if (MaxSampleCount & VK_SAMPLE_COUNT_16_BIT)
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_16;
    }
    else if (MaxSampleCount & VK_SAMPLE_COUNT_8_BIT)
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_8;
    }
    else if (MaxSampleCount & VK_SAMPLE_COUNT_4_BIT)
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_4;
    }
    else if (MaxSampleCount & VK_SAMPLE_COUNT_2_BIT)
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_2;
    }
    else
    {
        mGfxDeviceFeatures.MaxSampleCount = ESampleCount::SC_1;
    }

    mGfxDeviceFeatures.SamplerAnisotropy = PhysicalDeviceFeatures.samplerAnisotropy;

    LogInfoStream("Vulkan") << "选用设备: " << PhysicalDeviceProperties.deviceName << Log::EndLine;
}

kita::rhi::PhysicalDeviceSwapchainFeatures kita::rhi::VulkanGfxContext::QueryPhysicalDeviceSwapchainFeatures(VkPhysicalDevice Device,
                                                                                                             SurfaceWindow& TempWindow)
{
    PhysicalDeviceSwapchainFeatures Features{};
    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &SurfaceCapabilities);
    uint32_t FormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &FormatCount, nullptr);
    Features.Capabilities.MinImageExtent = {SurfaceCapabilities.minImageExtent.width, SurfaceCapabilities.minImageExtent.height};
    Features.Capabilities.MaxImageExtent = {SurfaceCapabilities.maxImageExtent.width, SurfaceCapabilities.maxImageExtent.height};
    Features.Capabilities.CurrentExtent = {SurfaceCapabilities.currentExtent.width, SurfaceCapabilities.currentExtent.height};
    Features.Capabilities.MinImageCount = SurfaceCapabilities.minImageCount;
    Features.Capabilities.MaxImageCount = SurfaceCapabilities.maxImageCount;
    Features.Capabilities.SupportedTransforms = VkSurfaceTransformToRHISurfaceTransform(SurfaceCapabilities.currentTransform);
    Features.Capabilities.CurrentTransform =
        static_cast<ESurfaceTransformBits>(VkSurfaceTransformToRHISurfaceTransform(SurfaceCapabilities.currentTransform));
    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR> PresentModes;
    if (FormatCount != 0)
    {
        Formats.resize(FormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &FormatCount, Formats.data());
        uint32_t PresentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &PresentModeCount, nullptr);
        if (PresentModeCount != 0)
        {
            PresentModes.resize(PresentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(Device, static_cast<VkSurfaceKHR>(TempWindow.GetNativeSurfaceObject()), &PresentModeCount,
                                                      PresentModes.data());
        }
    }
    for (const auto SurfaceFormatKhr : Formats)
    {
        PhysicalDeviceSwapchainFeatures::SurfaceFormat NewFormat{};
        NewFormat.Format = VkFormatToRHIFormat(SurfaceFormatKhr.format);
        NewFormat.ColorSpace = VkColorSpaceToRHIColorSpace(SurfaceFormatKhr.colorSpace);
        Features.Formats.push_back(NewFormat);
    }
    for (const auto PresentMode : PresentModes)
    {
        Features.PresentModes.push_back(VkPresentModeToRHIPresentMode(PresentMode));
    }
    return Features;
}

VkResult kita::rhi::VulkanGfxContext::Dyn_CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                                       const VkAllocationCallbacks* pAllocator,
                                                                       VkDebugUtilsMessengerEXT* pDebugMessenger) const
{
    static const auto Func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
    if (Func != nullptr)
    {
        return Func(mInstance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        LogErrorStream("Vulkan") << "未找到函数vkCreateDebugUtilsMessengerEXT" << Log::EndLine;
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void kita::rhi::VulkanGfxContext::Dyn_DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT DebugMessenger,
                                                                    const VkAllocationCallbacks* pAllocator) const
{
    static auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
    if (Func != nullptr)
    {
        Func(mInstance, DebugMessenger, pAllocator);
    }
    else
    {
        LogErrorStream("Vulkan") << "未找到函数vkDestroyDebugUtilsMessengerEXT" << Log::EndLine;
    }
}
