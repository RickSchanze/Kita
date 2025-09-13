// Auto-generated source file
#include "Engine/Object/Actor.h"

const Type* Actor::GetStaticType() { return TypeOf<Actor>(); }
const Type* Actor::GetType() { return TypeOf<Actor>(); }
void Actor::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("mComponents", mComponents); 
} 
void Actor::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("mComponents", mComponents); 
} 
