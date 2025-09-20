//
// Created by kita on 25-8-31.
//

#include "ReflUtils.h"

#include "TypeRegistry.h"

Array<const Type*> ReflUtils::GetTypes(Function<bool(const Type*)> Predicate) {
  Array<const Type*> Result;
  for (auto Type : GetTypeRegistry().GetAllTypes()) {
    if (Predicate(Type)) {
      Result.Add(Type);
    }
  }
  return Result;
}

const Type* ReflUtils::GetType(Function<bool(const Type*)> Predicate) {
  for (auto Type : GetTypeRegistry().GetAllTypes()) {
    if (Predicate(Type)) {
      return Type;
    }
  }
  return nullptr;
}

const Type* ReflUtils::GetType(StringView TypeName) {
  return GetType([TypeName](const Type* InType) { return InType->GetName() == TypeName; });
}