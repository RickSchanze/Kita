//
// Created by kita on 25-8-30.
//
#include "SurfaceWindow.h"

#include "Core/Config/ConfigManager.h"
#include "RHIConfig.h"
#include "Vulkan/SurfaceWindow_Vulkan.h"

RHISurfaceWindow* CreateSurfaceWindow() {
  auto& Config = ConfigManager::GetConfigRef<RHIConfig>();
  switch (Config.GetSurfaceWindowType()) {
  case ERHISurfaceWindowType::GLFW:
    return New<RHISurfaceWindow_Vulkan>(Config.GetDefaultWindowSize().X(), Config.GetDefaultWindowSize().Y(), false);
  case ERHISurfaceWindowType::Count:
    return nullptr;
  }
  return nullptr;
}
