// Auto-generated source file
#include "Assets/Mesh.h"

const Type* MeshMeta::GetStaticType() { return TypeOf<MeshMeta>(); }
const Type* MeshMeta::GetType() { return TypeOf<MeshMeta>(); }
void MeshMeta::WriteArchive(OutputArchive& Archive) const { 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
Archive.WriteType("FlipUV", FlipUV); 
Archive.WriteType("CalcTangent", CalcTangent); 
Archive.WriteType("EnsureTriangles", EnsureTriangles); 
} 
void MeshMeta::ReadArchive(InputArchive& Archive) { 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
Archive.ReadType("FlipUV", FlipUV); 
Archive.ReadType("CalcTangent", CalcTangent); 
Archive.ReadType("EnsureTriangles", EnsureTriangles); 
} 
const Type* SubmeshInfo::GetStaticType() { return TypeOf<SubmeshInfo>(); }
const Type* SubmeshInfo::GetType() { return TypeOf<SubmeshInfo>(); }
void SubmeshInfo::WriteArchive(OutputArchive& Archive) const { 
} 
void SubmeshInfo::ReadArchive(InputArchive& Archive) { 
} 
const Type* Mesh::GetStaticType() { return TypeOf<Mesh>(); }
const Type* Mesh::GetType() { return TypeOf<Mesh>(); }
void Mesh::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Mesh::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
