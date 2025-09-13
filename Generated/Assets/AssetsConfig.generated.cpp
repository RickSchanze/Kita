// Auto-generated source file
#include "Engine/Assets/AssetsConfig.h"

const Type* AssetsConfig::GetStaticType() { return TypeOf<AssetsConfig>(); }
const Type* AssetsConfig::GetType() { return TypeOf<AssetsConfig>(); }
void AssetsConfig::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("ShaderSearchPaths", ShaderSearchPaths); 
} 
void AssetsConfig::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("ShaderSearchPaths", ShaderSearchPaths); 
} 
