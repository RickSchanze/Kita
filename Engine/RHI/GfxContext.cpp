//
// Created by kita on 25-8-23.
//

#include "GfxContext.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "RHIConfig.h"

#include "GLFW/glfw3.h"

void GfxContext::StartUp() {}

void GfxContext::ShutDown() {}

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
