#pragma once
#include "Core/String/StringView.h"

struct Type {
  Type(const char* InName, const Int32 InSize, const bool InIsEnum, const SizeType InHashCode) : Name(InName), Size(InSize), HashCode(InHashCode), IsEnum(InIsEnum) {}

  const StringView Name;
  const Int32 Size{0};
  const SizeType HashCode;
  const bool IsEnum;
};

const Type* CreateType() {
  // todo
  return nullptr;
}
