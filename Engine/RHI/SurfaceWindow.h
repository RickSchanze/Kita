#pragma once
#define KITA_MAX_FRAMES_IN_FLIGHT 2
#include "Core/Container/Array.h"
#include "Core/Reflection/MetaMark.h"
#include "Math/Vector.h"

KENUM()
enum class ERHISurfaceWindowType {
  GLFW,
  Count,
};

extern void GetVulkanGLFWSurfaceWindowExtensions(Array<const char*>& OutExtensions);

class RHISurfaceWindow {
public:
  virtual ~RHISurfaceWindow() = default;

  virtual void* GetNativeSurfaceObject() = 0;
  virtual void* GetNativeSwapchainObject() = 0;
  virtual void CreateSwapchain() = 0;
  virtual void DestroySwapchain() = 0;
  virtual Vector2i GetSize() = 0;
  virtual bool ShouldClose() = 0;
};