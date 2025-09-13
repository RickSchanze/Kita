#include "Core/Config/ConfigManager.h"

#include "RHI/GfxContext.h"
#include "RHI/RHIConfig.h"
#include "RHI/Vulkan/RHIEnums_Vulkan.h"
#include "Windows.h"
#include "World/EngineLoop.h"

int main(int ArgC, const char** ArgV) {
  SetConsoleOutputCP(CP_UTF8);
  EngineLoop::StartUpSystems(ArgV, ArgC);
  EngineLoop::Run();
  EngineLoop::ShutDownSystems();
}
