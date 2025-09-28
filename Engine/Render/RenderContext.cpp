//
// Created by kita on 25-8-30.
//

#include "RenderContext.h"

#include "Core/Performance/ProfilerMark.h"
#include "Editor/EditorWindowManager.h"
#include "Editor/MenuActionManager.h"
#include "RHI/CommandBuffer.h"
#include "RHI/GfxContext.h"
#include "RHI/Sync.h"
#include "Render/RenderTicker.h"
#include "RenderPassManager.h"

#if KITA_EDITOR
#include "imgui_impl_vulkan.h"
#endif

RenderContext::RenderContext() = default;
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
#if KITA_DEBUG
    String DebugName = Format("FrameFence_{}", Index);
#else
    StringView DebugName = "";
#endif
    Self.mInFlightFences[Index] = Self.mGfxContext->CreateFenceU(DebugName);
    Self.mCommandBuffers[Index] = Self.mCommandPool->CreateCommandBuffer();
  }
  RenderPassManager::StartUp();
}

void RenderContext::ShutDown() {
  GfxContext::GetRef().WaitDeviceIdle();
  RenderPassManager::ShutDown();
  auto& Self = GetRef();
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

static void StaticDrawImGui() {
  CPU_PROFILING_SCOPE;
  ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* Viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(Viewport->WorkPos);
  ImGui::SetNextWindowSize(Viewport->WorkSize);
  ImGui::SetNextWindowViewport(Viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  WindowFlags |= ImGuiWindowFlags_NoBackground;
  static ImGuiDockNodeFlags DockSpaceFlags = ImGuiDockNodeFlags_None;
  bool pOpen;
  ImGui::Begin("DockSpace Demo", &pOpen, WindowFlags);
  ImGuiID DockSpaceId = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(DockSpaceId, ImVec2(0.0f, 0.0f), DockSpaceFlags);
  ImGui::PopStyleVar(2);
  MenuActionManager::Render();
  ImGui::End();
}

static TaskHandle RecordImGuiCommands(const RenderPipelineDrawParams& Params) {
  CPU_PROFILING_SCOPE;
  return GfxContext::GetRef().DrawImGui(Params.Cmd, Params.TargetFramebuffer, Params.Width, Params.Height);
}

static void DrawImGui(const RenderPipelineDrawParams& Params) {
  ImGui_ImplVulkan_NewFrame();
  ImGui::NewFrame();
  StaticDrawImGui();
  EditorWindowManager::Render();
  ImGui::Render();
  RecordImGuiCommands(Params);
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
  Params.TargetFramebuffer = mSurfaceWindow->GetImGuiFrameBuffer(static_cast<Int32>(ImageIndex));
  Params.Width = mSurfaceWindow->GetSize().X();
  Params.Height = mSurfaceWindow->GetSize().Y();
  OnRenderPipelineRender.Invoke(Params);
  DrawImGui(Params);

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

bool RenderContext::ShouldRender() const { return mWindowSize.X() != 0 && mWindowSize.Y() != 0; }

bool RenderContext::IsWindowResized() {
  Vector2i NewSize = mSurfaceWindow->GetSize();
  if (NewSize != mWindowSize) {
    mWindowSize = NewSize;
    return true;
  }
  return false;
}