// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_MaterialMeta \
typedef ThisStruct Super; \
typedef MaterialMeta ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MaterialMeta(); }static void DestructSelf(void* Ptr) { ((MaterialMeta*)(Ptr))->~MaterialMeta(); }struct Z_TypeRegister_MaterialMeta { \
Z_TypeRegister_MaterialMeta() { \
TypeBuilder Builder{}; \
Builder.CreateType<MaterialMeta>("MaterialMeta"); \
Builder.AddParent<AssetMeta>(); \
Builder.SetConstructor(MaterialMeta::ConstructSelf).SetDestructor(MaterialMeta::DestructSelf); \
Builder.AddField("Id", &MaterialMeta::Id); \
Builder.AddField("Path", &MaterialMeta::Path); \
Builder.AddField("ObjectHandle", &MaterialMeta::ObjectHandle); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MaterialMeta __Z_TypeRegister_MaterialMeta_Instance; \

#define GENERATED_HEADER_Material \
public: \
typedef ThisClass Super; \
typedef Material ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Material(); }static void DestructSelf(void* Ptr) { ((Material*)(Ptr))->~Material(); }struct Z_TypeRegister_Material { \
Z_TypeRegister_Material() { \
TypeBuilder Builder{}; \
Builder.CreateType<Material>("Material"); \
Builder.AddParent<Asset>(); \
Builder.SetConstructor(Material::ConstructSelf).SetDestructor(Material::DestructSelf); \
Builder.AddField("mShader", &Material::mShader); \
Builder.AddField("mTextures", &Material::mTextures); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Material __Z_TypeRegister_Material_Instance; \
void GetReferencingObject(Array<Int32>& OutArray) \
{ \
ExtractReferencingObjectHandle(mShader, OutArray); \
ExtractReferencingObjectHandle(mTextures, OutArray); \
} \
private: \

