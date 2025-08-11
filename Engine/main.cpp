#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "Windows.h"

struct A {
  int b;
  String C;
};

enum class D { L, M, C, P };

int main() {
  SetConsoleOutputCP(CP_UTF8);

}
