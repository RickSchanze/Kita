// Auto-generated source file
#include "Assets/Texture.h"

const Type* Texture2DMeta::GetStaticType() { return TypeOf<Texture2DMeta>(); }
const Type* Texture2DMeta::GetType() { return TypeOf<Texture2DMeta>(); }
void Texture2DMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
Archive.WriteType("EnableMipMap", EnableMipMap); 
Archive.WriteType("Format", Format); 
} 
void Texture2DMeta::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
Archive.ReadType("EnableMipMap", EnableMipMap); 
Archive.ReadType("Format", Format); 
} 
const Type* TextureCubeMeta::GetStaticType() { return TypeOf<TextureCubeMeta>(); }
const Type* TextureCubeMeta::GetType() { return TypeOf<TextureCubeMeta>(); }
void TextureCubeMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
Archive.WriteType("EnableMipMap", EnableMipMap); 
} 
void TextureCubeMeta::ReadArchive(InputArchive& Archive) { 
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
