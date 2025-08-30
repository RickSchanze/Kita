//
// Created by kita on 25-8-30.
//

#include "RenderContext.h"

#include "Core/Performance/ProfilerMark.h"
#include "RHI/CommandBuffer.h"
#include "RHI/GfxContext.h"
#include "RHI/Sync.h"

#if KITA_EDITOR
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#endif

void RenderContext::StartUp(RHISurfaceWindow* InWindow) {
  auto& Self = GetRef();
  Self.mGfxContext = GfxContext::Get();
  Self.mSurfaceWindow = InWindow;
  Self.mRenderTicker = MakeUnique<RenderTicker>(InWindow);
  Self.mCommandPool = Self.mGfxContext->CreateCommandPoolU(ERHIQueueFamilyType::Graphics);
  for (Int32 Index = 0; Index < KITA_MAX_FRAMES_IN_FLIGHT; Index++) {
    Self.mImageAvailableSemaphores[Index] = Self.mGfxContext->CreateSemaphoreU();
    Self.mRenderFinishedSemaphores[Index] = Self.mGfxContext->CreateSemaphoreU();
    Self.mInFlightFences[Index] = Self.mGfxContext->CreateFenceU();
    Self.mCommandBuffers[Index] = Self.mCommandPool->CreateCommandBuffer();
  }
}

void RenderContext::Render(double Time) {
  if (ShouldRender())
    return;
  CPU_PROFILING_SCOPE;
  Int32 FrameIndex = mFrameIndex % MAX_FRAMES_INFLIGHT;
  mInFlightFences[FrameIndex]->Wait(UINT64_MAX);
  bool NeedRecreation;
  UInt32 ImageIndex = mGfxContext->GetNextImage(mSurfaceWindow, mImageAvailableSemaphores[FrameIndex].Get(), nullptr, mNeedRecreation);
  if (mNeedRecreation) {
  }

  mInFlightFences[FrameIndex]->Reset();
  mCommandBuffers[FrameIndex]->Reset();
  mCommandBuffers[FrameIndex]->BeginRecord();

#if KITA_EDITOR
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
#endif
  bool show_demo_window = true;
  ImGui::ShowDemoWindow(&show_demo_window);

  RenderPipelineDrawParams Params = {};
  Params.DeltaSeconds = Time;
  Params.Cmd = mCommandBuffers[FrameIndex].Get();
  Params.TargetFramebuffer = mSurfaceWindow->GetImGuiFrameBuffer(ImageIndex);
  Params.Width = mSurfaceWindow->GetSize().X();
  Params.Height = mSurfaceWindow->GetSize().Y();

  ImGui::Render();
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
      .SetFence(mInFlightFences[mInFlightFences[FrameIndex]].Get());
  ExecuteWaitHandle.WaitSync();
  mGfxContext->Submit(SubmitParams);

  RHIPresentParams PresentParams = {};
  PresentParams
      .SetWaitSemaphores({mRenderFinishedSemaphores[FrameIndex].Get()}) //
      .SetSurfaceWindow(mSurfaceWindow)                                 //
      .SetImageIndex(ImageIndex);
  mNeedRecreation = mGfxContext->Present(PresentParams);
}

bool RenderContext::ShouldRender() const { return mRenderPipeline; }