#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "Windows.h"

struct A {
  int b = 13;
  String C = "你好";

  void WriteArchive(OutputArchive& Archive) const {
    Archive.Write("b", b);
    Archive.Write("C", C);
  }
};

enum class D { L, M, C, P };

int main() {
  SetConsoleOutputCP(CP_UTF8);
  A MyA{};
  TOMLOutputArchive Archive;
  Archive.WriteType("Type", MyA);
  Archive.WriteFile("Test.toml");
}
