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
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MeshMeta __Z_TypeRegister_MeshMeta_Instance; \

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
Builder.AddField("mVertexCount", &Mesh::mVertexCount).SetFieldAttribute("Transient", "true"); \
Builder.AddField("mIndexCount", &Mesh::mIndexCount).SetFieldAttribute("Transient", "true"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Mesh __Z_TypeRegister_Mesh_Instance; \
private: \

