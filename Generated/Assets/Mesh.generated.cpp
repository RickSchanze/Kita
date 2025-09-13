// Auto-generated source file
#include "Engine/Assets/Mesh.h"

const Type* MeshMeta::GetStaticType() { return TypeOf<MeshMeta>(); }
const Type* MeshMeta::GetType() { return TypeOf<MeshMeta>(); }
void MeshMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
Archive.WriteType("FlipUV", FlipUV); 
Archive.WriteType("CalcTangent", CalcTangent); 
Archive.WriteType("EnsureTriangles", EnsureTriangles); 
} 
void MeshMeta::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
Archive.ReadType("FlipUV", FlipUV); 
Archive.ReadType("CalcTangent", CalcTangent); 
Archive.ReadType("EnsureTriangles", EnsureTriangles); 
} 
const Type* SubMeshInfo::GetStaticType() { return TypeOf<SubMeshInfo>(); }
const Type* SubMeshInfo::GetType() { return TypeOf<SubMeshInfo>(); }
void SubMeshInfo::WriteArchive(OutputArchive& Archive) const { 
} 
void SubMeshInfo::ReadArchive(InputArchive& Archive) { 
} 
const Type* Mesh::GetStaticType() { return TypeOf<Mesh>(); }
const Type* Mesh::GetType() { return TypeOf<Mesh>(); }
void Mesh::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Mesh::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
