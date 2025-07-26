#pragma once
#include "Core/Memory/Malloc.h"
#include "Core/String/StringView.h"

struct Type {
  Type(const char* InName, const Int32 InSize, const bool InIsEnum, const SizeType InHashCode) : Name(InName), Size(InSize), HashCode(InHashCode), IsEnum(InIsEnum) {}

  const StringView Name;
  const Int32 Size{0};
  const SizeType HashCode;
  const bool IsEnum;
};

namespace Pri {
void RegisterTypeToRegistry(const Type* Type);
}

template <typename T>
const Type* CreateType(const char* Name) {
  static SizeType HashCodeHolder{};
  SizeType HashCode = reinterpret_cast<SizeType>(&HashCodeHolder);
  bool IsEnum = std::is_enum_v<T>;
  SizeType Size = sizeof(T);
  const Type* NewType = New<Type>(Name, Size, IsEnum, HashCode);
  Pri::RegisterTypeToRegistry(NewType);
  return NewType;
}
