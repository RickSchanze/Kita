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

#define GENERATED_HEADER_SubmeshInfo \
typedef SubmeshInfo ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) SubmeshInfo(); }static void DestructSelf(void* Ptr) { ((SubmeshInfo*)(Ptr))->~SubmeshInfo(); }struct Z_TypeRegister_SubmeshInfo { \
Z_TypeRegister_SubmeshInfo() { \
TypeBuilder Builder{}; \
Builder.CreateType<SubmeshInfo>("SubmeshInfo"); \
Builder.SetConstructor(SubmeshInfo::ConstructSelf).SetDestructor(SubmeshInfo::DestructSelf); \
Builder.AddField("VertexOffset", &SubmeshInfo::VertexOffset).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "顶点偏移"); \
Builder.AddField("VertexCount", &SubmeshInfo::VertexCount).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "顶点数"); \
Builder.AddField("IndexOffset", &SubmeshInfo::IndexOffset).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "索引偏移"); \
Builder.AddField("IndexCount", &SubmeshInfo::IndexCount).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "索引数"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_SubmeshInfo __Z_TypeRegister_SubmeshInfo_Instance; \

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
Builder.AddField("mSubmeshes", &Mesh::mSubmeshes).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "子Mesh"); \
Builder.AddField("mMeta", &Mesh::mMeta).SetFieldAttribute("Transient", "true").SetFieldAttribute("Label", "导入选项"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Mesh __Z_TypeRegister_Mesh_Instance; \
private: \

