#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Singleton/Singleton.h"
#include "Math/Vector.h"
#include "RenderPipeline.h"
#include "RenderTicker.h"

class RHIFence;
class RHISemaphore;
class RHICommandPool;
constexpr Int32 MAX_FRAMES_INFLIGHT = 2;

class RenderContext : public Singleton<RenderContext> {
public:
  RenderContext();
  ~RenderContext();
  static void StartUp(RHISurfaceWindow* InWindow);
  static void ShutDown();

  void Render(double Time);

  bool ShouldRender() const;

  RenderTicker* GetRenderTicker() { return mRenderTicker.Get(); }

  static void SetRenderPipeline(UniquePtr<RenderPipeline> InNewPipeline) { GetRef().mRenderPipeline = std::move(InNewPipeline); }

  /// 更新并且获知当前Window有没有进行Resize
  bool IsWindowResized();

private:
  UniquePtr<RenderTicker> mRenderTicker{};

  /// 默认的RenderPipeline是一个只绘制ImGui的RenderPipeline
  UniquePtr<RenderPipeline> mRenderPipeline{};

  RHISurfaceWindow* mSurfaceWindow = nullptr;
  UniquePtr<RHICommandPool> mCommandPool;
  UniquePtr<RHISemaphore> mImageAvailableSemaphores[MAX_FRAMES_INFLIGHT];
  UniquePtr<RHISemaphore> mRenderFinishedSemaphores[MAX_FRAMES_INFLIGHT];
  UniquePtr<RHIFence> mInFlightFences[MAX_FRAMES_INFLIGHT];
  UniquePtr<RHICommandBuffer> mCommandBuffers[MAX_FRAMES_INFLIGHT];
  Int32 mFrameIndex = 0;
  class GfxContext* mGfxContext = nullptr;
  Vector2i mWindowSize = {1, 1};
};
