// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
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

