#pragma once
#include "Core/Container/Array.h"
#include "RHI/SurfaceWindow.h"

#include <vulkan/vulkan_core.h>

struct PhysicalDeviceSwapchainFeatures;

void GetVulkanGLFWSurfaceWindowExtensions(Array<const char*>& OutExtensions);

class RHISurfaceWindow_Vulkan : public RHISurfaceWindow {
public:
  explicit RHISurfaceWindow_Vulkan(Int32 Width = -1, Int32 Height = -1, bool HideWindow = false, bool CreateImGuiContext = true);
  virtual ~RHISurfaceWindow_Vulkan() override;

  virtual void* GetNativeSurfaceObject() override { return mSurface; }

  virtual void* GetNativeSwapchainObject() override { return mSwapchain; }

  void CreateSurface(VkInstance Instance);
  void DestroySurface(VkInstance Instance);

  virtual void CreateSwapchain() override;
  virtual void DestroySwapchain() override;

  virtual Vector2i GetSize() override;

  virtual bool ShouldClose() override;

  virtual void TickInput() override;

  static VkSurfaceFormatKHR ChooseSwapchainFormat(const PhysicalDeviceSwapchainFeatures& Features);
  static VkPresentModeKHR ChoosePresentMode(const PhysicalDeviceSwapchainFeatures& Features);
  [[nodiscard]] VkExtent2D ChooseSwapchainExtent(const PhysicalDeviceSwapchainFeatures& Features) const;

#if KITA_EDITOR
  void StartUpImGui();
  void ShutDownImGui();
#endif

private:
  VkSurfaceKHR mSurface{};
  VkSwapchainKHR mSwapchain{};
  struct GLFWwindow* mWindow{};
  UInt32 mSwapchainImageCount{};
  Vector2i mSwapchainImageSize{};
  VkFormat mSwapchainImageFormat{};
  Array<VkImage> mSwapchainImages{};
  Array<SharedPtr<class RHIImageView>> mSwapchainImageViews{};
#if KITA_EDITOR
  bool mInitImGui = true;

#endif
};