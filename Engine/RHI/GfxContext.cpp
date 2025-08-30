//
// Created by kita on 25-8-23.
//

#include "GfxContext.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "RHIConfig.h"

#include "GLFW/glfw3.h"
#include "Vulkan/GfxContext_Vulkan.h"

void GfxContext::StartUp() {
  Evt_PreGfxContextCreated.Invoke();
  auto& Config = ConfigManager::GetConfigRef<RHIConfig>();
  switch (Config.GetSurfaceWindowType()) {
  case ERHISurfaceWindowType::GLFW:
    glfwInit();
    break;
  default:
    std::unreachable();
  }
  switch (Config.GetGraphicsBackend()) {
  case ERHIBackend::Vulkan:
    mContext = New<GfxContext_Vulkan>();
    break;
  default:
    LOG_CRITICAL_TAG("RHI", "未知的渲染后端: {}.", EnumToString(Config.GetGraphicsBackend()));
  }
  Evt_PostGfxContextCreated.Invoke(mContext);
}

void GfxContext::ShutDown() {
  mContext->WaitDeviceIdle();
  Evt_PreGfxContextDestroyed.Invoke(mContext);
  Delete(mContext);
  auto& Config = ConfigManager::GetConfigRef<RHIConfig>();
  switch (Config.GetSurfaceWindowType()) {
  case ERHISurfaceWindowType::GLFW:
    glfwTerminate();
    break;
  default:
    std::unreachable();
  }
  Evt_PostGfxContextDestroyed.Invoke();
}

void WindowBackendSupport::InitializeWindowBackend() {
  const RHIConfig* Cfg = ConfigManager::GetConfig<RHIConfig>();
  if (const ERHISurfaceWindowType SurfaceWindow = Cfg->GetSurfaceWindowType(); SurfaceWindow == ERHISurfaceWindowType::GLFW) {
    glfwInit();
    SurfaceWindowBackendType = ERHISurfaceWindowType::GLFW;
  } else {
    LOG_CRITICAL_TAG("RHI", "未知的窗口后端: {}.", EnumToString(SurfaceWindow));
  }
}

void WindowBackendSupport::CleanWindowBackend() {
  if (SurfaceWindowBackendType == ERHISurfaceWindowType::GLFW) {
    glfwTerminate();
  } else {
    LOG_CRITICAL_TAG("RHI", "未知的窗口后端: {}.", EnumToString(SurfaceWindowBackendType));
  }
}

void WindowBackendSupport::TickFrameInputData() {
  CPU_PROFILING_SCOPE;
  if (SurfaceWindowBackendType == ERHISurfaceWindowType::GLFW) {
    glfwPollEvents();
  } else {
    LOG_CRITICAL_TAG("RHI", "未知的窗口后端: {}.", EnumToString(SurfaceWindowBackendType));
  }
}

void WindowBackendSupport::ShouldWindowClose(RHISurfaceWindow* MyWindow, bool& OutResult) {
  CPU_PROFILING_SCOPE;
  if (SurfaceWindowBackendType == ERHISurfaceWindowType::GLFW) {
    OutResult = MyWindow->ShouldClose();
  } else {
    LOG_CRITICAL_TAG("RHI", "未知的窗口后端: {}.", EnumToString(SurfaceWindowBackendType));
  }
}
