#pragma once
#include "Core/Container/Array.h"
#include "RHI/SurfaceWindow.h"

#include <vulkan/vulkan_core.h>

struct PhysicalDeviceSwapchainFeatures;

void GetVulkanGLFWSurfaceWindowExtensions(Array<const char*>& OutExtensions);

class RHISurfaceWindow_Vulkan : public RHISurfaceWindow {
public:
  explicit RHISurfaceWindow_Vulkan(int32_t Width = -1, int32_t Height = -1, bool InternalUse = false);
  virtual ~RHISurfaceWindow_Vulkan() override;

  virtual void* GetNativeSurfaceObject() override { return mSurface; }

  virtual void* GetNativeSwapchainObject() override { return mSwapchain; }

  void CreateSurface(VkInstance Instance);
  void DestroySurface(VkInstance Instance);

  virtual void CreateSwapchain() override;
  virtual void DestroySwapchain() override;

  virtual Vector2i GetSize() override;

  virtual bool ShouldClose() override;

protected:
  static VkSurfaceFormatKHR ChooseSwapchainFormat(const PhysicalDeviceSwapchainFeatures& Features);
  static VkPresentModeKHR ChoosePresentMode(const PhysicalDeviceSwapchainFeatures& Features);
  [[nodiscard]] VkExtent2D ChooseSwapchainExtent(const PhysicalDeviceSwapchainFeatures& Features) const;

private:
  VkSurfaceKHR mSurface{};
  VkSwapchainKHR mSwapchain{};
  struct GLFWwindow* mWindow{};
  uint32_t mSwapchainImageCount{};
  Vector2i mSwapchainImageSize{};
  VkFormat mSwapchainImageFormat{};
  Array<VkImage> mSwapchainImages{};
  Array<SharedPtr<class RHIImageView>> mSwapchainImageViews{};
};