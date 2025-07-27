//
// Created by kita on 25-7-26.
//

#include "TypeRegistry.h"
#include "Type.h"

TypeRegistry::TypeRegistry() {
  mRegisteredTypes.Reserve(1000);
  RegisterType(CreateType<Int8>("Int8"));
  RegisterType(CreateType<Int16>("Int16"));
  RegisterType(CreateType<Int32>("Int32"));
  RegisterType(CreateType<Int64>("Int64"));
  RegisterType(CreateType<UInt8>("UInt8"));
  RegisterType(CreateType<UInt16>("UInt16"));
  RegisterType(CreateType<UInt32>("UInt32"));
  RegisterType(CreateType<UInt64>("UInt64"));
  RegisterType(CreateType<bool>("bool"));
  RegisterType(CreateType<Float32>("Float32"));
  RegisterType(CreateType<Float64>("Float64"));
  RegisterType(CreateType<String>("String"));
  RegisterType(CreateType<StringView>("StringView"));
}

TypeRegistry::~TypeRegistry() {
  for (const auto Element : mRegisteredTypes) {
    Delete(const_cast<Type*>(Element));
  }
}

void TypeRegistry::RegisterType(const Type* Type) { mRegisteredTypes.AddUnique(Type); }

const Type* TypeRegistry::GetType(const StringView& TypeName) const {
  const SizeType Index = mRegisteredTypes.IndexOf([TypeName](const Type* Type) { return Type->GetName() == TypeName; });
  if (Index == INVALID_INDEX)
    return nullptr;
  return mRegisteredTypes[Index];
}

const Type* TypeRegistry::GetType(SizeType& HashCode) const {
  const SizeType Index = mRegisteredTypes.IndexOf([HashCode](const Type* Type) { return Type->GetHashCode() == HashCode; });
  if (Index == INVALID_INDEX)
    return nullptr;
  return mRegisteredTypes[Index];
}
