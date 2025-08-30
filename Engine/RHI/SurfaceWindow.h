#pragma once
#define KITA_MAX_FRAMES_IN_FLIGHT 2
#include "Core/Container/Array.h"
#include "Core/Reflection/MetaMark.h"
#include "Math/Vector.h"

#include "SurfaceWindow.generated.h"

KENUM()
enum class ERHISurfaceWindowType {
  GLFW,
  Count,
};

class RHIFrameBuffer;
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
  virtual void TickInput() = 0;

#if KITA_EDITOR
  RHIFrameBuffer* GetImGuiFrameBuffer(Int32 Index) { return mImGuiFrameBuffers[Index].Get(); }

protected:
  Array<UniquePtr<RHIFrameBuffer>> mImGuiFrameBuffers;
#endif
};
