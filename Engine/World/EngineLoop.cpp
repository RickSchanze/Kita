//
// Created by kita on 25-8-29.
//

#include "EngineLoop.h"

#include "Assets/AssetsManager.h"
#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Editor/EditorWindowManager.h"
#include "Editor/MenuActionManager.h"
#include "Input/Input.h"
#include "Input/InputTicker.h"
#include "Object/ObjectTable.h"
#include "Project/Project.h"
#include "RHI/GfxContext.h"
#include "RHI/RHIConfig.h"
#include "Render/RenderContext.h"
#include "Render/RenderTicker.h"
#include "TickManager.h"

#include "argparse/argparse.hpp"

#define PROJECT_ARG "--Project"

static void StartUpProject(const char** ArgV, int ArgC) {
  argparse::ArgumentParser Program("KitaEngine");
  Program.add_argument(PROJECT_ARG).help("要打开的项目的路径").required();
  try {
    Program.parse_args(ArgC, ArgV);
  } catch (const std::exception& Err) {
    const std::string Help = Program.help().str();
    gLogger.Critical("Engine", "初始化失败: {}", Err.what());
  }

  {
    const std::string ProjectPath = Program.get<std::string>(PROJECT_ARG);
    Project::StartUp(ProjectPath);
  }
}

void EngineLoop::ShutDownSystems(const bool Tips) {
  GetRef().ShutDownSystemsM();
  if (Tips) {
    system("pause");
  }
}

void EngineLoop::StartUpSystemsM(const char** ArgV, int ArgC) {
  // Project
  StartUpProject(ArgV, ArgC);
  // TaskGraph
  TaskGraph::StartUp();
  // GfxContext
  GfxContext::StartUp();
  ObjectTable::StartUp();
  // Swapchain
  RHIConfig& Config = ConfigManager::GetConfigRef<RHIConfig>();
  mSurfaceWindow = GfxContext::GetRef().CreateSurfaceWindowR(Config.GetDefaultWindowSize().X(), Config.GetDefaultWindowSize().Y());
  mSurfaceWindow->CreateSwapchain();

  // RenderContext
  RenderContext::StartUp(mSurfaceWindow);

  // Input
  mInputTicker = New<InputTicker>(mSurfaceWindow);

  // 设置Ticker
  auto& Ref = TickManager::GetRef();
  Ref.SetRenderTickInstance(RenderContext::GetRef().GetRenderTicker());
  Ref.SetTickInstance(mInputTicker);

  // MenuAction
  MenuActionManager::StartUp();

  // EditorWindowManager
  EditorWindowManager::StartUp();

  AssetsManager::StartUp();
}

void EngineLoop::ShutDownSystemsM() {
  AssetsManager::ShutDown();
  // EditorWindowManager
  EditorWindowManager::ShutDown();
  // MenuAction
  MenuActionManager::ShutDown();
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

  ObjectTable::ShutDown();
  // GfxContext
  GfxContext::ShutDown();

  // ConfigManager
  ConfigManager::ShutDown();

  // TaskGraph
  TaskGraph::ShutDown();

  // Project
  Project::ShutDown();
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