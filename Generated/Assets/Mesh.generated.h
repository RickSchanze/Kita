// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_MeshMeta \
typedef ThisStruct Super; \
typedef MeshMeta ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MeshMeta(); }static void DestructSelf(void* Ptr) { ((MeshMeta*)(Ptr))->~MeshMeta(); }struct Z_TypeRegister_MeshMeta { \
Z_TypeRegister_MeshMeta() { \
TypeBuilder Builder{}; \
Builder.CreateType<MeshMeta>("MeshMeta"); \
Builder.AddParent<AssetMeta>(); \
Builder.SetConstructor(MeshMeta::ConstructSelf).SetDestructor(MeshMeta::DestructSelf); \
Builder.AddField("Id", &MeshMeta::Id); \
Builder.AddField("Path", &MeshMeta::Path); \
Builder.AddField("ObjectHandle", &MeshMeta::ObjectHandle); \
Builder.AddField("FlipUV", &MeshMeta::FlipUV); \
Builder.AddField("CalcTangent", &MeshMeta::CalcTangent); \
Builder.AddField("EnsureTriangles", &MeshMeta::EnsureTriangles); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MeshMeta __Z_TypeRegister_MeshMeta_Instance; \

#define GENERATED_HEADER_SubMeshInfo \
typedef SubMeshInfo ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) SubMeshInfo(); }static void DestructSelf(void* Ptr) { ((SubMeshInfo*)(Ptr))->~SubMeshInfo(); }struct Z_TypeRegister_SubMeshInfo { \
Z_TypeRegister_SubMeshInfo() { \
TypeBuilder Builder{}; \
Builder.CreateType<SubMeshInfo>("SubMeshInfo"); \
Builder.SetConstructor(SubMeshInfo::ConstructSelf).SetDestructor(SubMeshInfo::DestructSelf); \
Builder.AddField("VertexOffset", &SubMeshInfo::VertexOffset).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "顶点偏移"); \
Builder.AddField("VertexCount", &SubMeshInfo::VertexCount).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "顶点数"); \
Builder.AddField("IndexOffset", &SubMeshInfo::IndexOffset).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "索引偏移"); \
Builder.AddField("IndexCount", &SubMeshInfo::IndexCount).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "索引数"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_SubMeshInfo __Z_TypeRegister_SubMeshInfo_Instance; \

#define GENERATED_HEADER_Mesh \
public: \
typedef ThisClass Super; \
typedef Mesh ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Mesh(); }static void DestructSelf(void* Ptr) { ((Mesh*)(Ptr))->~Mesh(); }struct Z_TypeRegister_Mesh { \
Z_TypeRegister_Mesh() { \
TypeBuilder Builder{}; \
Builder.CreateType<Mesh>("Mesh"); \
Builder.AddParent<Asset>(); \
Builder.SetConstructor(Mesh::ConstructSelf).SetDestructor(Mesh::DestructSelf); \
Builder.AddField("mVertexCount", &Mesh::mVertexCount).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "总顶点数"); \
Builder.AddField("mIndexCount", &Mesh::mIndexCount).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "总索引数"); \
Builder.AddField("mSubMeshes", &Mesh::mSubMeshes).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "子Mesh"); \
Builder.AddField("mMeta", &Mesh::mMeta).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "导入选项"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Mesh __Z_TypeRegister_Mesh_Instance; \
private: \

