// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_MeshInspector \
public: \
typedef ThisClass Super; \
typedef MeshInspector ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MeshInspector(); }static void DestructSelf(void* Ptr) { ((MeshInspector*)(Ptr))->~MeshInspector(); }struct Z_TypeRegister_MeshInspector { \
Z_TypeRegister_MeshInspector() { \
TypeBuilder Builder{}; \
Builder.CreateType<MeshInspector>("MeshInspector"); \
Builder.AddParent<Inspector>(); \
Builder.SetConstructor(MeshInspector::ConstructSelf).SetDestructor(MeshInspector::DestructSelf); \
Builder.SetTypeAttribute("Inspector", "Mesh"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MeshInspector __Z_TypeRegister_MeshInspector_Instance; \
private: \

