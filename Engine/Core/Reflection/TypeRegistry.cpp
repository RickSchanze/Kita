//
// Created by kita on 25-7-26.
//

#include "TypeRegistry.h"
#include "Type.h"

namespace Pri {
template <typename T> const Type* CreateType(const char* Name) {
  static SizeType HashCodeHolder{};
  SizeType HashCode = reinterpret_cast<SizeType>(&HashCodeHolder);
  bool IsEnum = std::is_enum_v<T>;
  SizeType Size = sizeof(T);
  const Type* NewType = New<Type>(Name, Size, IsEnum, HashCode);
  return NewType;
}
} // namespace Pri

TypeRegistry::TypeRegistry() {
  mRegisteredTypes.Reserve(1000);
  RegisterType(Pri::CreateType<Int8>("Int8"));
  RegisterType(Pri::CreateType<Int16>("Int16"));
  RegisterType(Pri::CreateType<Int32>("Int32"));
  RegisterType(Pri::CreateType<Int64>("Int64"));
  RegisterType(Pri::CreateType<UInt8>("UInt8"));
  RegisterType(Pri::CreateType<UInt16>("UInt16"));
  RegisterType(Pri::CreateType<UInt32>("UInt32"));
  RegisterType(Pri::CreateType<UInt64>("UInt64"));
  RegisterType(Pri::CreateType<bool>("bool"));
  RegisterType(Pri::CreateType<Float32>("Float32"));
  RegisterType(Pri::CreateType<Float64>("Float64"));
  RegisterType(Pri::CreateType<String>("String"));
  RegisterType(Pri::CreateType<StringView>("StringView"));
}

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
