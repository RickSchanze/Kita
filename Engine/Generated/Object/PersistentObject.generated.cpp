// Auto-generated source file
#include "Object/PersistentObject.h"

const Type* PersistentObject::GetStaticType() { return TypeOf<PersistentObject>(); }
const Type* PersistentObject::GetType() { return TypeOf<PersistentObject>(); }
void PersistentObject::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("mPath", mPath); 
} 
void PersistentObject::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("mPath", mPath); 
} 
