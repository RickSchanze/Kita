// Auto-generated source file
#include "Object/ObjectPtr.h"

const Type* ObjectPtrBase::GetStaticType() { return TypeOf<ObjectPtrBase>(); }
const Type* ObjectPtrBase::GetType() { return TypeOf<ObjectPtrBase>(); }
void ObjectPtrBase::WriteArchive(OutputArchive& Archive) const { 
Archive.WriteType("ObjectHandle", ObjectHandle); 
} 
void ObjectPtrBase::ReadArchive(InputArchive& Archive) { 
Archive.ReadType("ObjectHandle", ObjectHandle); 
} 
