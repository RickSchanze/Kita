//
// Created by kita on 25-7-26.
//

#include "TypeRegistry.h"
#include "Type.h"

TypeRegistry::TypeRegistry() { mRegisteredTypes.Reserve(1000); }

TypeRegistry::~TypeRegistry() {
  for (const auto Element : mRegisteredTypes) {
    Delete(Element);
  }
}

void TypeRegistry::RegisterType(const Type* Type) { mRegisteredTypes.AddUnique(Type); }

const Type* TypeRegistry::GetType(const StringView& TypeName) const {
  const SizeType Index = mRegisteredTypes.IndexOf([TypeName](const Type* Type) { return Type->Name == TypeName; });
  if (Index == INVALID_INDEX)
    return nullptr;
  return mRegisteredTypes[Index];
}

const Type* TypeRegistry::GetType(SizeType& HashCode) const {
  const SizeType Index = mRegisteredTypes.IndexOf([HashCode](const Type* Type) { return Type->HashCode == HashCode; });
  if (Index == INVALID_INDEX)
    return nullptr;
  return mRegisteredTypes[Index];
}
