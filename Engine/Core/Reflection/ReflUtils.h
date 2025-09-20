#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Function.h"
#include "Type.h"

struct Type;
class ReflUtils {
public:
  static Array<const Type*> GetTypes(Function<bool(const Type*)> Predicate);
  static const Type* GetType(Function<bool(const Type*)> Predicate);
  static const Type* GetType(StringView TypeName);
};
