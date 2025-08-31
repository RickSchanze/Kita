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