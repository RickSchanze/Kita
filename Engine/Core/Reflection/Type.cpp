//
// Created by kita on 25-7-26.
//

#include "Type.h"
#include "Core/Assert.h"
#include "Core/Trace.h"
#include "TypeRegistry.h"
#include "Core/Logging/Logger.hpp"

static void FindFieldsRecursive(const Type* Type, Array<const struct Field*>& Fields) {
  if (!Type->HasParent()) {
    Fields.AddRange(Type->GetDeclaringFields());
  } else {
    for (const ::Type* Base : Type->GetBases()) {
      FindFieldsRecursive(Base, Fields);
    }
  }
}

Array<const struct Field*> Type::GetFields() const {
  Array<const struct Field*> Fields;
  FindFieldsRecursive(this, Fields);
  return Fields;
}

Type::~Type() {
  for (const auto Field : mFields) {
    Delete(const_cast<struct Field*>(Field));
  }
}

bool Type::IsDerivedFrom(const Type* const InBase) const {
  if (InBase == this) {
    return true;
  }
  if (mBases.Empty()) {
    return false;
  }
  return Ranges::AnyOf(mBases, [InBase](const Type* Base) { return Base->IsDerivedFrom(InBase); });
}

bool Type::IsParentOf(const Type* Child) const {
  if (Child == nullptr)
    return false;
  return Child->IsDerivedFrom(this);
}

void Type::RegisterField(const Field* Field) { mFields.Add(Field); }

void Type::SetAttribute(const StringView Key, const StringView Value) {
  DEBUG_ASSERT_MSG(!mAttributes.Contains(Key), "TypeAttribute {} already registered", Key);
  mAttributes[Key] = Value;
}

void Type::AddParent(const Type* Parent) { mBases.AddUnique(Parent); }

void Type::DestroyInstance(void* Ptr) const { GetTypeRegistry().DestroyTypeInstance(this, Ptr); }

void* Type::CreateInstance() const { return GetTypeRegistry().CreateTypeInstance(this); }
