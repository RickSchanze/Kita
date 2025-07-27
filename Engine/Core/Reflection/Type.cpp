//
// Created by kita on 25-7-26.
//

#include "Type.h"
#include "TypeRegistry.h"

void Pri::RegisterTypeToRegistry(const Type* Type) { GetTypeRegistry().RegisterType(Type); }

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

bool Type::IsDerivedFrom(const Type* const InBase) const {
  if (InBase == this) {
    return true;
  }
  if (mBases.Empty()) {
    return false;
  }
  for (const Type* Base : mBases) {
    if (Base->IsDerivedFrom(InBase)) {
      return true;
    }
  }
  return false;
}

bool Type::IsParentOf(const Type* Child) const {
  if (Child == nullptr)
    return false;
  return Child->IsDerivedFrom(this);
}
