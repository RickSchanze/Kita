//
// Created by kita on 25-8-30.
//

#include "RenderContext.h"

#include "Core/Performance/ProfilerMark.h"
#include "RHI/CommandBuffer.h"
#include "RHI/GfxContext.h"
#include "RHI/Sync.h"
#include "Render/RenderTicker.h"

#if KITA_EDITOR
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#endif

RenderContext::~RenderContext() = default;
void RenderContext::StartUp(RHISurfaceWindow* InWindow) {
  auto& Self = GetRef();
  Self.mGfxContext = GfxContext::Get();
  Self.mSurfaceWindow = InWindow;
  Self.mRenderTicker = MakeUnique<RenderTicker>();
  Self.mCommandPool = Self.mGfxContext->CreateCommandPoolU(ERHIQueueFamilyType::Graphics, true);
  for (Int32 Index = 0; Index < KITA_MAX_FRAMES_IN_FLIGHT; Index++) {
    Self.mImageAvailableSemaphores[Index] = Self.mGfxContext->CreateSemaphoreU();
    Self.mRenderFinishedSemaphores[Index] = Self.mGfxContext->CreateSemaphoreU();
    Self.mInFlightFences[Index] = Self.mGfxContext->CreateFenceU();
    Self.mCommandBuffers[Index] = Self.mCommandPool->CreateCommandBuffer();
  }

  // TODO: 可配置RenderPipeline
  Self.mRenderPipeline = MakeUnique<RenderPipeline>();
}

void RenderContext::ShutDown() {
  GfxContext::GetRef().WaitDeviceIdle();
  auto& Self = GetRef();
  Self.mRenderPipeline = nullptr;
  for (Int32 Index = 0; Index < KITA_MAX_FRAMES_IN_FLIGHT; Index++) {
    Self.mImageAvailableSemaphores[Index] = nullptr;
    Self.mRenderFinishedSemaphores[Index] = nullptr;
    Self.mInFlightFences[Index] = nullptr;
    Self.mCommandBuffers[Index] = nullptr;
  }
  Self.mCommandPool = nullptr;
  Self.mRenderTicker = nullptr;
  Self.mGfxContext = nullptr;
  Self.mSurfaceWindow = nullptr;
}

void RenderContext::Render(double Time) {
  CPU_PROFILING_SCOPE;
  if (IsWindowResized()) {
    if (mWindowSize.X() == 0 || mWindowSize.Y() == 0) {
      return;
    }
    CPU_NAMED_PROFILING_SCOPE("SwapchainRecreate");
    GfxContext::GetRef().WaitDeviceIdle();
    mSurfaceWindow->DestroySwapchain();
    mSurfaceWindow->CreateSwapchain();
  }
  if (!ShouldRender())
    return;

  Int32 FrameIndex;
  {
    CPU_NAMED_PROFILING_SCOPE("WaitFence");
    FrameIndex = mFrameIndex % MAX_FRAMES_INFLIGHT;
    mInFlightFences[FrameIndex]->Wait(UINT64_MAX);
  }
  UInt32 ImageIndex;
  {
    CPU_NAMED_PROFILING_SCOPE("GetNextImage");
    bool ResizeNeeded;
    ImageIndex = mGfxContext->GetNextImage(mSurfaceWindow, mImageAvailableSemaphores[FrameIndex].Get(), nullptr, ResizeNeeded);
  }
  mInFlightFences[FrameIndex]->Reset();
  mCommandBuffers[FrameIndex]->Reset();
  mCommandBuffers[FrameIndex]->BeginRecord();
  // 一开始记录就开始Execute一次 避免与ImGui冲突
  mCommandBuffers[FrameIndex]->Execute("");

  RenderPipelineDrawParams Params = {};
  Params.DeltaSeconds = Time;
  Params.Cmd = mCommandBuffers[FrameIndex].Get();
  Params.TargetFramebuffer = mSurfaceWindow->GetImGuiFrameBuffer(ImageIndex);
  Params.Width = mSurfaceWindow->GetSize().X();
  Params.Height = mSurfaceWindow->GetSize().Y();
  mRenderPipeline->Draw(Params);

  mCommandBuffers[FrameIndex]->EndRecord();
  // TODO: 提交也应该异步
  TaskHandle ExecuteWaitHandle = mCommandBuffers[FrameIndex]->Execute("");

  RHICommandBufferSubmitParams SubmitParams = {};
  SubmitParams
      .SetCommandBuffer(mCommandBuffers[FrameIndex].Get())              //
      .SetWaitSemaphores({mImageAvailableSemaphores[FrameIndex].Get()}) //
      .SetWaitPipelineStages(PSFB_ColorAttachmentOutput)
      .SetSignalSemaphores({mRenderFinishedSemaphores[FrameIndex].Get()})
      .SetTargetQueueFamily(ERHIQueueFamilyType::Graphics) //
      .SetFence(mInFlightFences[FrameIndex].Get());
  ExecuteWaitHandle.WaitSync();
  mGfxContext->Submit(SubmitParams);

  {
    CPU_NAMED_PROFILING_SCOPE("Present");
    RHIPresentParams PresentParams = {};
    PresentParams
        .SetWaitSemaphores({mRenderFinishedSemaphores[FrameIndex].Get()}) //
        .SetSurfaceWindow(mSurfaceWindow)                                 //
        .SetImageIndex(ImageIndex);
    mGfxContext->Present(PresentParams);
  }
}

bool RenderContext::ShouldRender() const { return mRenderPipeline && mWindowSize.X() != 0 && mWindowSize.Y() != 0; }

bool RenderContext::IsWindowResized() {
  Vector2i NewSize = mSurfaceWindow->GetSize();
  if (NewSize != mWindowSize) {
    mWindowSize = NewSize;
    return true;
  }
  return false;
}