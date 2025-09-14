// Auto-generated source file
#include "Assets/Texture.h"

const Type* TextureMeta::GetStaticType() { return TypeOf<TextureMeta>(); }
const Type* TextureMeta::GetType() { return TypeOf<TextureMeta>(); }
void TextureMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
Archive.WriteType("EnableMipMap", EnableMipMap); 
} 
void TextureMeta::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
Archive.ReadType("EnableMipMap", EnableMipMap); 
} 
const Type* Texture::GetStaticType() { return TypeOf<Texture>(); }
const Type* Texture::GetType() { return TypeOf<Texture>(); }
void Texture::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Texture::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
const Type* Texture2D::GetStaticType() { return TypeOf<Texture2D>(); }
const Type* Texture2D::GetType() { return TypeOf<Texture2D>(); }
void Texture2D::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Texture2D::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
const Type* TextureCube::GetStaticType() { return TypeOf<TextureCube>(); }
const Type* TextureCube::GetType() { return TypeOf<TextureCube>(); }
void TextureCube::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void TextureCube::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
