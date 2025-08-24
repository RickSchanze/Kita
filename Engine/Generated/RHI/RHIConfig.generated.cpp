// Auto-generated source file
#include "RHI/RHIConfig.h"

const Type* RHIConfig::GetStaticType() { return TypeOf<RHIConfig>(); }
const Type* RHIConfig::GetType() { return TypeOf<RHIConfig>(); }
void RHIConfig::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("PresentMode", PresentMode); 
Archive.WriteType("SurfaceWindowType", SurfaceWindowType); 
} 
void RHIConfig::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("PresentMode", PresentMode); 
Archive.ReadType("SurfaceWindowType", SurfaceWindowType); 
} 
