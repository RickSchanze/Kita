// Auto-generated source file
#include "Object/Object.h"

const Type* Object::GetStaticType() { return TypeOf<Object>(); }
const Type* Object::GetType() { return TypeOf<Object>(); }
void Object::WriteArchive(OutputArchive& Archive) const { 
Archive.WriteType("mName", mName); 
} 
void Object::ReadArchive(InputArchive& Archive) { 
Archive.ReadType("mName", mName); 
} 
