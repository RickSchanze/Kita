//
// Created by kita on 25-8-29.
//

#include "EngineLoop.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Input/InputTicker.h"
#include "RHI/GfxContext.h"
#include "Render/RenderContext.h"
#include "Render/RenderTicker.h"
#include "TickManager.h"

void EngineLoop::StartUpSystemsM() {
  TaskGraph::StartUp();
  GfxContext::StartUp();

  mSurfaceWindow = GfxContext::GetRef().CreateSurfaceWindowR(1920, 1080);
  mSurfaceWindow->CreateSwapchain();
  RenderContext::StartUp(mSurfaceWindow);

  auto& Ref = TickManager::GetRef();
  mInputTicker = New<InputTicker>(mSurfaceWindow);
  Ref.SetRenderTickInstance(RenderContext::GetRef().GetRenderTicker());
  Ref.SetTickInstance(mInputTicker);
  mEngineData = {};
}

void EngineLoop::ShutDownSystemsM() {
  RenderContext::ShutDown();
  auto& TickManagerRef = TickManager::GetRef();
  TickManagerRef.SetTickInstance(nullptr);

  GfxContext::ShutDown();
  ConfigManager::ShutDown();
  TaskGraph::ShutDown();
  mEngineData = {};
}

static auto gLastFrameTime = std::chrono::high_resolution_clock::now();

void EngineLoop::RunM() {
  while (true) {
    auto now = std::chrono::high_resolution_clock::now();
    mEngineData.DeltaSeconds = std::chrono::duration<double>(now - gLastFrameTime).count();
    gLastFrameTime = now;
    mEngineData.FrameIndex++;

    TickManager::TickAllObjects(mEngineData.DeltaSeconds);

    MARK_PROFILER_FRAME;
  }
}