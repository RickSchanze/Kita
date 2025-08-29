//
// Created by kita on 25-8-29.
//

#include "EngineLoop.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "RHI/GfxContext.h"
#include "TickManager.h"

void EngineLoop::StartUpSystemsM() {
  TaskGraph::StartUp();
  GfxContext::StartUp();
  auto& Ref = TickManager::GetRef();
  Ref.SetRenderTickInstance(New<ITick>());
  Ref.SetTickInstance(New<ITick>());
}

void EngineLoop::ShutDownSystemsM() {
  GfxContext::ShutDown();
  ConfigManager::ShutDown();
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
  }
}