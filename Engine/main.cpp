#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/TypeRegistry.h"
#include "Windows.h"

struct A {
  int b;
  String C;
};

enum class D { L, M, C, P };

int main() {
  SetConsoleOutputCP(CP_UTF8);
  Map<String, Int32> t = {{"12", 13}};
  TypeBuilder{}.CreateType<A>("A").AddField("b", &A::b).AddField("C", &A::C).Register();
  TypeBuilder{}.CreateType<D>("D").AddField("L", D::L).AddField("M", D::M).AddField("C", D::C).AddField("P", D::P).Register();
  const Type* AType = TypeOf<A>();
  const Type* PType = TypeOf<D>();
  const Type* IntAType = TypeOf<Int32>();
  SizeType s = AType->GetSize();
}
