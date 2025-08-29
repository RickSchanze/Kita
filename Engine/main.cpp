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
#include "RHI/GfxContext.h"
#include "RHI/RHIConfig.h"
#include "RHI/RHIEnums.h"
#include "RHI/Vulkan/RHIEnums_Vulkan.h"
#include "Windows.h"
#include "World/EngineLoop.h"

int main() {
  SetConsoleOutputCP(CP_UTF8);
  EngineLoop::StartUpSystems();
  EngineLoop::Run();
  EngineLoop::ShutDownSystems();
}
