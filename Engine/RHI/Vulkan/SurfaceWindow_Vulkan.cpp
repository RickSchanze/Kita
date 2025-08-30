//
// Created by kita on 25-8-27.
//

#include "SurfaceWindow_Vulkan.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "GfxContext_Vulkan.h"
#include "RHI/FrameBuffer.h"
#include "RHI/GfxContext.h"
#include "RHI/RHIConfig.h"
#include "RHIEnums_Vulkan.h"

#include <GLFW/glfw3.h>

#if KITA_EDITOR
#include "imgui_impl_glfw.h"
#endif

void GetVulkanGLFWSurfaceWindowExtensions(Array<const char*>& OutExtensions) {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  for (uint32_t i = 0; i < glfwExtensionCount; i++) {
    OutExtensions.Add(glfwExtensions[i]);
  }
}

RHISurfaceWindow_Vulkan::RHISurfaceWindow_Vulkan(int32_t Width, int32_t Height, bool HideWindow, bool CreateImGuiContext) {
  CPU_PROFILING_SCOPE;
  mInitImGui = CreateImGuiContext;
  const RHIConfig& Cfg = ConfigManager::GetConfigRef<RHIConfig>();
  Width = Width <= 0 ? Cfg.GetDefaultWindowSize().X() : Width;
  Height = Height <= 0 ? Cfg.GetDefaultWindowSize().Y() : Height;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  if (HideWindow) {
    glfwWindowHint(GLFW_VISIBLE, false);
  } else {
    glfwWindowHint(GLFW_VISIBLE, true);
  }
  mWindow = glfwCreateWindow(Width, Height, "KitaEngine", nullptr, nullptr);
  if (HideWindow) // 内部使用则直接返回
    return;
  // 非内部使用则自动创建Surface与Swapchain
  const auto& Ctx = GetVulkanGfxContexRef();
  CreateSurface(Ctx.GetInstance());
  if (CreateImGuiContext) {
    StartUpImGui();
  }
  // TODO: 交换链图像先写死
}

RHISurfaceWindow_Vulkan::~RHISurfaceWindow_Vulkan() {
  const auto& Ctx = GetVulkanGfxContexRef();
  DestroySurface(Ctx.GetInstance());
  if (mInitImGui) {
    ImGui_ImplGlfw_Shutdown();
  }
  glfwDestroyWindow(mWindow);
  mWindow = nullptr;
}

void RHISurfaceWindow_Vulkan::CreateSurface(VkInstance Instance) {
  if (Instance == VK_NULL_HANDLE) {
    Instance = GetVulkanGfxContexRef().GetInstance();
  }

  glfwCreateWindowSurface(Instance, mWindow, nullptr, &mSurface);
}

void RHISurfaceWindow_Vulkan::DestroySurface(VkInstance Instance) {
  if (Instance == VK_NULL_HANDLE) {
    Instance = GetVulkanGfxContexRef().GetInstance();
  }
  vkDestroySurfaceKHR(Instance, mSurface, nullptr);
  mSurface = VK_NULL_HANDLE;
}

void RHISurfaceWindow_Vulkan::CreateSwapchain() {
  const auto& Ctx = GetVulkanGfxContexRef();
  const PhysicalDeviceSwapchainFeatures& Features = Ctx.GetPhysicalDeviceSwapchainFeatures(*this);
  const VkSurfaceFormatKHR SurfaceFormat = ChooseSwapchainFormat(Features);
  const VkPresentModeKHR PresentMode = ChoosePresentMode(Features);
  const VkExtent2D Extent = ChooseSwapchainExtent(Features);
  // TODO: 交换链图像数量选择 现在先写死为2
  mSwapchainImageCount = 2;
  // if (Features.Capabilities.MaxImageCount > 0 && mSwapchainImageCount > Features.Capabilities.MaxImageCount) {
  //   mSwapchainImageCount = Features.Capabilities.MaxImageCount;
  // }

  VkSwapchainCreateInfoKHR CreateInfo{};
  CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  CreateInfo.surface = mSurface;

  CreateInfo.minImageCount = mSwapchainImageCount;
  CreateInfo.imageFormat = SurfaceFormat.format;
  CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
  CreateInfo.imageExtent = Extent;
  CreateInfo.imageArrayLayers = 1;
  CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  const QueueFamilyIndices Indices = GetVulkanGfxContexRef().GetQueueFamilies(mSurface);
  const uint32_t QueueFamilyIndices[] = {*Indices.GraphicsFamily, *Indices.PresentFamily};

  if (Indices.GraphicsFamily != Indices.PresentFamily) {
    CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    CreateInfo.queueFamilyIndexCount = 2;
    CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
  } else {
    CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  CreateInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(RHISurfaceTransformToVkSurfaceTransform(Features.Capabilities.CurrentTransform));
  CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  CreateInfo.presentMode = PresentMode;
  CreateInfo.clipped = VK_TRUE;

  if (const VkResult CreateResult = vkCreateSwapchainKHR(Ctx.GetDevice(), &CreateInfo, nullptr, &mSwapchain); CreateResult != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "创建交换链失败, 错误码={}", CreateResult);
    mSwapchain = nullptr;
  }

  vkGetSwapchainImagesKHR(Ctx.GetDevice(), mSwapchain, &mSwapchainImageCount, nullptr);
  mSwapchainImages.Resize(mSwapchainImageCount);
  vkGetSwapchainImagesKHR(Ctx.GetDevice(), mSwapchain, &mSwapchainImageCount, mSwapchainImages.Data());

  mSwapchainImageFormat = SurfaceFormat.format;
  mSwapchainImageSize = {static_cast<Int32>(Extent.width), static_cast<Int32>(Extent.height)};
  mSwapchainImageViews.Resize(mSwapchainImageCount);
  for (size_t i = 0; i < mSwapchainImageCount; i++) {
    mSwapchainImageViews[i] = GetVulkanGfxContexRef().CreateSwapchainImageView(mSwapchainImages[i], mSwapchainImageFormat);
  }
#if KITA_EDITOR
  RHIFrameBufferDesc FrameBufferDesc{};
  FrameBufferDesc.SetWidth(Extent.width).SetHeight(Extent.height).SetLayers(1).SetRenderPass(GfxContext::GetRef().GetImGuiRenderPass());
  for (size_t i = 0; i < mSwapchainImageCount; i++) {
    FrameBufferDesc.SetAttachments({mSwapchainImageViews[i].Get()});
    mImGuiFrameBuffers.Add(GfxContext::GetRef().CreateFrameBufferU(FrameBufferDesc));
  }
#endif
}

void RHISurfaceWindow_Vulkan::DestroySwapchain() {
  const auto& Ctx = GetVulkanGfxContexRef();
#if KITA_EDITOR
  mImGuiFrameBuffers.Clear();
#endif
  mSwapchainImageViews.Clear();
  vkDestroySwapchainKHR(Ctx.GetDevice(), mSwapchain, nullptr);
  mSwapchain = nullptr;
  mSwapchainImageCount = 0;
  mSwapchainImageFormat = VK_FORMAT_MAX_ENUM;
  mSwapchainImages.Clear();
  mSwapchainImageSize = {};
}

Vector2i RHISurfaceWindow_Vulkan::GetSize() {
  Vector2i Result;
  glfwGetWindowSize(mWindow, &Result.X(), &Result.Y());
  return Result;
}

bool RHISurfaceWindow_Vulkan::ShouldClose() { return glfwWindowShouldClose(mWindow); }

void RHISurfaceWindow_Vulkan::TickInput() {
  glfwPollEvents();
  ImGui_ImplGlfw_NewFrame();
}

VkSurfaceFormatKHR RHISurfaceWindow_Vulkan::ChooseSwapchainFormat(const PhysicalDeviceSwapchainFeatures& Features) {
  const auto& Cfg = ConfigManager::GetConfigRef<RHIConfig>();
  VkSurfaceFormatKHR Result{};
  const ERHIFormat DesiredFormat = Cfg.GetDesiredSwapchainFormat();
  const ERHIColorSpace DesiredColorSpace = Cfg.GetDesiredSwapchainColorSpace();
  for (const auto& Format : Features.Formats) {
    if (Format.Format == DesiredFormat && Format.ColorSpace == DesiredColorSpace) {
      Result.format = RHIFormatToVkFormat(DesiredFormat);
      Result.colorSpace = RHIColorSpaceToVkColorSpace(DesiredColorSpace);
      return Result;
    }
  }
  LOG_ERROR_TAG("RHI.Vulkan", "创建窗口表面时期望格式不支持, 回滚至VK_FORMAT_B8G8R8A8SRGB以及VK_COLOR_SPACE_SRGB_NONLINEAR_KHR");
  for (const auto& Format : Features.Formats) {
    if (Format.Format == ERHIFormat::B8G8R8A8_SRGB && Format.ColorSpace == ERHIColorSpace::sRGB) {
      Result.format = RHIFormatToVkFormat(ERHIFormat::B8G8R8A8_SRGB);
      Result.colorSpace = RHIColorSpaceToVkColorSpace(ERHIColorSpace::sRGB);
      return Result;
    }
  }
  LOG_ERROR_TAG("RHI.Vulkan", "没有找到此窗口表明支持的格式");
  return {};
}

VkPresentModeKHR RHISurfaceWindow_Vulkan::ChoosePresentMode(const PhysicalDeviceSwapchainFeatures& Features) {
  const RHIConfig* Cfg = ConfigManager::GetConfig<RHIConfig>();
  for (const auto& Mode : Features.PresentModes) {
    if (Mode == Cfg->GetPresentMode()) {
      return RHIPresentModeToVkPresentMode(Mode);
    }
  }
  LOG_ERROR_TAG("RHI.Vulkan", "创建窗口表面时期望模式不支持, 回滚至VK_PRESENT_MODE_FIFO_KHR");
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RHISurfaceWindow_Vulkan::ChooseSwapchainExtent(const PhysicalDeviceSwapchainFeatures& Features) const {
  if (Features.Capabilities.CurrentExtent.X() != std::numeric_limits<uint32_t>::max()) {
    return {static_cast<UInt32>(Features.Capabilities.CurrentExtent.X()), static_cast<UInt32>(Features.Capabilities.CurrentExtent.Y())};
  }
  int Width, Height;
  glfwGetFramebufferSize(mWindow, &Width, &Height);

  VkExtent2D ActualExtent = {static_cast<uint32_t>(Width), static_cast<uint32_t>(Height)};

  ActualExtent.width = std::clamp(ActualExtent.width, static_cast<UInt32>(Features.Capabilities.MinImageExtent.X()), static_cast<UInt32>(Features.Capabilities.MaxImageExtent.X()));
  ActualExtent.height = std::clamp(ActualExtent.height, static_cast<UInt32>(Features.Capabilities.MinImageExtent.Y()), static_cast<UInt32>(Features.Capabilities.MaxImageExtent.Y()));

  return ActualExtent;
}

#if KITA_EDITOR

void RHISurfaceWindow_Vulkan::StartUpImGui() { ImGui_ImplGlfw_InitForVulkan(mWindow, true); }

void RHISurfaceWindow_Vulkan::ShutDownImGui() { ImGui_ImplGlfw_Shutdown(); }

#endif
