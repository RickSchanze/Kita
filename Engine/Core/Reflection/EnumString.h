#pragma once
#include "Core/Traits.h"
#include "Type.h"
#include "TypeRegistry.h"

/// 这里使用反射查找Enum的String和Value
/// 难有性能损失，如果不想要这个性能损失那么
/// template <>
/// String EnumToString<MyEnum>(MyEnum Value);
/// template <>
/// MyEnum StringToEnum(StringView Value);
/// 自己重载这俩函数

template <typename T>
  requires Traits::IsEnum<T>
String EnumToString(T Value) {
  for (const Type* EnumType = TypeOf<T>(); const Field* Field : EnumType->GetFields()) {
    if (Field->GetSize() == static_cast<Int32>(Value)) {
      return String(Field->GetName());
    }
  }
  return "";
}

template <typename T>
  requires Traits::IsEnum<T>
T StringToEnum(const StringView Value) {
  for (const Type* EnumType = TypeOf<T>(); const Field* Field : EnumType->GetFields()) {
    if (Field->GetName() == Value) {
      return static_cast<T>(Field->GetSize());
    }
  }
  return static_cast<T>(-1);
}
