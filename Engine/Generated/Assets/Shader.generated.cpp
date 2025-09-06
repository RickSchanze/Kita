// Auto-generated source file
#include "Assets/Shader.h"

const Type* ShaderMeta::GetStaticType() { return TypeOf<ShaderMeta>(); }
const Type* ShaderMeta::GetType() { return TypeOf<ShaderMeta>(); }
void ShaderMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
} 
void ShaderMeta::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
} 
const Type* ShaderCache::GetStaticType() { return TypeOf<ShaderCache>(); }
const Type* ShaderCache::GetType() { return TypeOf<ShaderCache>(); }
void ShaderCache::WriteArchive(OutputArchive& Archive) const { 
Archive.WriteType("LastTextModifiedTime", LastTextModifiedTime); 
Archive.WriteType("LastBinaryModifiedTime", LastBinaryModifiedTime); 
} 
void ShaderCache::ReadArchive(InputArchive& Archive) { 
Archive.ReadType("LastTextModifiedTime", LastTextModifiedTime); 
Archive.ReadType("LastBinaryModifiedTime", LastBinaryModifiedTime); 
} 
const Type* Shader::GetStaticType() { return TypeOf<Shader>(); }
const Type* Shader::GetType() { return TypeOf<Shader>(); }
void Shader::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Shader::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
