//
// Created by kita on 25-8-29.
//

#include "EngineLoop.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Input/Input.h"
#include "Input/InputTicker.h"
#include "RHI/GfxContext.h"
#include "Render/RenderContext.h"
#include "Render/RenderTicker.h"
#include "TickManager.h"

void EngineLoop::StartUpSystemsM() {
  // TaskGraph
  TaskGraph::StartUp();
  // GfxContext
  GfxContext::StartUp();

  // Swapchain
  mSurfaceWindow = GfxContext::GetRef().CreateSurfaceWindowR(1920, 1080);
  mSurfaceWindow->CreateSwapchain();

  // RenderContext
  RenderContext::StartUp(mSurfaceWindow);

  // Input
  mInputTicker = New<InputTicker>(mSurfaceWindow);

  // 设置Ticker
  auto& Ref = TickManager::GetRef();
  Ref.SetRenderTickInstance(RenderContext::GetRef().GetRenderTicker());
  Ref.SetTickInstance(mInputTicker);
  mEngineData = {};
}

void EngineLoop::ShutDownSystemsM() {
  // 清理Ticker
  auto& TickManagerRef = TickManager::GetRef();
  TickManagerRef.SetTickInstance(nullptr);
  TickManagerRef.SetRenderTickInstance(nullptr);

  // Input
  mInputTicker = {};

  // RenderContext
  RenderContext::ShutDown();

  // Swapchain
  mSurfaceWindow->DestroySwapchain();
  Delete(mSurfaceWindow);
  mSurfaceWindow = nullptr;

  // GfxContext
  GfxContext::ShutDown();

  // ConfigManager
  ConfigManager::ShutDown();

  // TaskGraph
  TaskGraph::ShutDown();
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
    if (Input::ShouldClose()) {
      break;
    }
  }
}