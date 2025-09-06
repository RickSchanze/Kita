// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ShaderMeta \
typedef ThisStruct Super; \
typedef ShaderMeta ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ShaderMeta(); }static void DestructSelf(void* Ptr) { ((ShaderMeta*)(Ptr))->~ShaderMeta(); }struct Z_TypeRegister_ShaderMeta { \
Z_TypeRegister_ShaderMeta() { \
TypeBuilder Builder{}; \
Builder.CreateType<ShaderMeta>("ShaderMeta"); \
Builder.AddParent<AssetMeta>(); \
Builder.SetConstructor(ShaderMeta::ConstructSelf).SetDestructor(ShaderMeta::DestructSelf); \
Builder.AddField("Id", &ShaderMeta::Id); \
Builder.AddField("Path", &ShaderMeta::Path); \
Builder.AddField("ObjectHandle", &ShaderMeta::ObjectHandle); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ShaderMeta __Z_TypeRegister_ShaderMeta_Instance; \

#define GENERATED_HEADER_ShaderCache \
typedef ShaderCache ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ShaderCache(); }static void DestructSelf(void* Ptr) { ((ShaderCache*)(Ptr))->~ShaderCache(); }struct Z_TypeRegister_ShaderCache { \
Z_TypeRegister_ShaderCache() { \
TypeBuilder Builder{}; \
Builder.CreateType<ShaderCache>("ShaderCache"); \
Builder.SetConstructor(ShaderCache::ConstructSelf).SetDestructor(ShaderCache::DestructSelf); \
Builder.AddField("LastTextModifiedTime", &ShaderCache::LastTextModifiedTime); \
Builder.AddField("LastBinaryModifiedTime", &ShaderCache::LastBinaryModifiedTime); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ShaderCache __Z_TypeRegister_ShaderCache_Instance; \

#define GENERATED_HEADER_Shader \
public: \
typedef ThisClass Super; \
typedef Shader ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Shader(); }static void DestructSelf(void* Ptr) { ((Shader*)(Ptr))->~Shader(); }struct Z_TypeRegister_Shader { \
Z_TypeRegister_Shader() { \
TypeBuilder Builder{}; \
Builder.CreateType<Shader>("Shader"); \
Builder.AddParent<Asset>(); \
Builder.SetConstructor(Shader::ConstructSelf).SetDestructor(Shader::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Shader __Z_TypeRegister_Shader_Instance; \
private: \

