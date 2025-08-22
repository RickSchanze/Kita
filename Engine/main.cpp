#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "Core/Serialization/YAML/YAMLInputArchive.h"
#include "Core/Serialization/YAML/YAMLOutputArchive.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/ThreadUtils.h"
#include "Object/Actor.h"
#include "Windows.h"

int main() {
  SetConsoleOutputCP(CP_UTF8);
  Actor A{};
  const Type* t = TypeOf<Actor>();
  const Type* t1 = TypeOf<Actor>();

}
