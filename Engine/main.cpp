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
#include "Math/Vector.h"
#include "Object/Actor.h"
#include "Windows.h"
int main() {
  SetConsoleOutputCP(CP_UTF8);
  Array<Vector4f> Test = {{1, 2, 3, 4}, {5, 6, 7, 8}};
  LOG_INFO("Vector: {}", Test);
  const Type* T1 = TypeOf<Vector4f>();
  const Type* T2 = TypeOf<Vector3f>();
}
