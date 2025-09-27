// Auto-generated source file
#include "Assets/Material.h"

const Type* MaterialMeta::GetStaticType() { return TypeOf<MaterialMeta>(); }
const Type* MaterialMeta::GetType() { return TypeOf<MaterialMeta>(); }
void MaterialMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
} 
void MaterialMeta::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
} 
const Type* Material::GetStaticType() { return TypeOf<Material>(); }
const Type* Material::GetType() { return TypeOf<Material>(); }
void Material::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("mShader", mShader); 
Archive.WriteType("mTextures", mTextures); 
} 
void Material::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("mShader", mShader); 
Archive.ReadType("mTextures", mTextures); 
} 
