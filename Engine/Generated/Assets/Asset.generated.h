// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
void Z_Reflection_Register_Func_Enum_EAssetType();struct Z_Reflection_Register_Enum_EAssetType { Z_Reflection_Register_Enum_EAssetType() {Z_Reflection_Register_Func_Enum_EAssetType(); }};
inline Z_Reflection_Register_Enum_EAssetType Z_Reflection_Register_Enum_EAssetType___Instance{}; 
#define GENERATED_HEADER_AssetMeta \
typedef AssetMeta ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) AssetMeta(); }static void DestructSelf(void* Ptr) { ((AssetMeta*)(Ptr))->~AssetMeta(); }struct Z_TypeRegister_AssetMeta { \
Z_TypeRegister_AssetMeta() { \
TypeBuilder Builder{}; \
Builder.CreateType<AssetMeta>("AssetMeta"); \
Builder.SetConstructor(AssetMeta::ConstructSelf).SetDestructor(AssetMeta::DestructSelf); \
Builder.AddField("Path", &AssetMeta::Path); \
Builder.AddField("ObjectId", &AssetMeta::ObjectId); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_AssetMeta __Z_TypeRegister_AssetMeta_Instance; \

#define GENERATED_HEADER_Asset \
public: \
typedef ThisClass Super; \
typedef Asset ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_Asset { \
Z_TypeRegister_Asset() { \
TypeBuilder Builder{}; \
Builder.CreateType<Asset>("Asset"); \
Builder.AddParent<PersistentObject>(); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Asset __Z_TypeRegister_Asset_Instance; \
private: \

