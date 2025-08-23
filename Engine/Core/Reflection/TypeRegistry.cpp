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

void TypeRegistry::RegisterCtorAndDtor(const Type* Type, Constructor Ctor, Destructor Dtor) {
  if (Type) {
    if (!mConstructors.Contains(Type) && !mDestructors.Contains(Type)) {
      mConstructors.Add(Type, Ctor);
      mDestructors.Add(Type, Dtor);
    }
  }
}

Constructor TypeRegistry::GetConstructor(const Type* Type) const {
  if (Type) {
    if (mConstructors.Contains(Type))
      return mConstructors.At(Type);
  }
  return nullptr;
}

Destructor TypeRegistry::GetDestructor(const Type* Type) const {
  if (Type) {
    if (mDestructors.Contains(Type))
      return mDestructors.At(Type);
  }
  return nullptr;
}

void* TypeRegistry::CreateTypeInstance(const Type* InType) const {
  if (InType) {
    if (!mConstructors.Contains(InType)) {
      return nullptr;
    }
    Constructor Ctor = mConstructors.At(InType);
    void* TypeInstance = Malloc(InType->GetSize());
    Ctor(TypeInstance);
    return TypeInstance;
  }
  return nullptr;
}

void TypeRegistry::DestroyTypeInstance(const Type* InType, void* Instance) const {
  if (InType && Instance) {
    if (!mDestructors.Contains(InType)) {
      return;
    }
    const Destructor Dtor = mDestructors.At(InType);
    Dtor(Instance);
    Free(Instance);
  }
}
