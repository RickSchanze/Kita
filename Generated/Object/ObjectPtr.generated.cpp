// Auto-generated source file
#include "Engine/Object/ObjectPtr.h"

const Type* ObjectPtrBase::GetStaticType() { return TypeOf<ObjectPtrBase>(); }
const Type* ObjectPtrBase::GetType() { return TypeOf<ObjectPtrBase>(); }
void ObjectPtrBase::WriteArchive(OutputArchive& Archive) const { 
Archive.WriteType("mObjectHandle", mObjectHandle); 
} 
void ObjectPtrBase::ReadArchive(InputArchive& Archive) { 
Archive.ReadType("mObjectHandle", mObjectHandle); 
} 
