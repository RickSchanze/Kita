#include "Core/Config/ConfigManager.h"
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/EnumString.h"
#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "Core/Serialization/YAML/YAMLInputArchive.h"
#include "Core/Serialization/YAML/YAMLOutputArchive.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/ThreadUtils.h"
#include "Math/Vector.h"
#include "Object/Actor.h"
#include "RHI/RHIConfig.h"
#include "RHI/RHIEnums.h"
#include "RHI/Vulkan/RHIEnums_Vulkan.h"
#include "Windows.h"

void StartUpSystems() { TaskGraph::StartUp(); }

void ShutDownSystems() {

  ConfigManager::ShutDown();
  TaskGraph::ShutDown();
}
int main() {
  SetConsoleOutputCP(CP_UTF8);
  StartUpSystems();

  RHIConfig* Cfg = ConfigManager::GetConfig<RHIConfig>();
  RHIConfig* Cfg1 = ConfigManager::GetConfig<RHIConfig>();

  RHIFormatToVkFormat(ERHIFormat::A2B10G10R10_UNorm);

  ShutDownSystems();
}
