#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "Core/Serialization/YAML/YAMLInputArchive.h"
#include "Core/Serialization/YAML/YAMLOutputArchive.h"
#include "Windows.h"

struct A {
  int b = 13;
  String C = "你好";
  Array<int> D = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  void WriteArchive(OutputArchive& Archive) const {
    Archive.WriteType("b", b);
    Archive.WriteType("C", C);
    Archive.WriteType("D", D);
  }

  void ReadArchive(InputArchive& Archive) {
    Archive.ReadType("b", b);
    Archive.ReadType("C", C);
    Archive.ReadType("D", D);
  }
};

enum class D { L, M, C, P };

int main() {
  SetConsoleOutputCP(CP_UTF8);
  A MyA{};
  TOMLOutputArchive Archive;
  Archive.WriteType("Type", MyA);
  Archive.WriteFile("Test.toml");

  TOMLInputArchive Archive2;
  Archive2.ParseFile("Test.toml");
  A MyA2;
  MyA2.b = 0;
  MyA2.C = "Test";
  MyA2.D = {};
  Archive2.ReadType("Type", MyA2);
  LOG_INFO("{}", MyA2.C);
}

KCLASS(Attr1 = "Value1", Attr2 = Value2) // 这里有没有双引号都可以
class MyClass {

  KPROPERTY(Attr1 = "Value1", Attr2 = Value2)
  int MyInt = 0;

  KPROPERTY()
  String MyInt1;

  KPROPERTY(Transient) // Transient没有值 默认为"true"
  String MyInt2;

  KFUNCTION() // 和Field一样, 需要提取参数信息
  void Test(int a, int b);
};
