// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_Texture2DMeta \
typedef ThisStruct Super; \
typedef Texture2DMeta ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Texture2DMeta(); }static void DestructSelf(void* Ptr) { ((Texture2DMeta*)(Ptr))->~Texture2DMeta(); }struct Z_TypeRegister_Texture2DMeta { \
Z_TypeRegister_Texture2DMeta() { \
TypeBuilder Builder{}; \
Builder.CreateType<Texture2DMeta>("Texture2DMeta"); \
Builder.AddParent<AssetMeta>(); \
Builder.SetConstructor(Texture2DMeta::ConstructSelf).SetDestructor(Texture2DMeta::DestructSelf); \
Builder.AddField("Id", &Texture2DMeta::Id); \
Builder.AddField("Path", &Texture2DMeta::Path); \
Builder.AddField("ObjectHandle", &Texture2DMeta::ObjectHandle); \
Builder.AddField("EnableMipMap", &Texture2DMeta::EnableMipMap); \
Builder.AddField("Format", &Texture2DMeta::Format); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Texture2DMeta __Z_TypeRegister_Texture2DMeta_Instance; \

#define GENERATED_HEADER_TextureCubeMeta \
typedef ThisStruct Super; \
typedef TextureCubeMeta ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) TextureCubeMeta(); }static void DestructSelf(void* Ptr) { ((TextureCubeMeta*)(Ptr))->~TextureCubeMeta(); }struct Z_TypeRegister_TextureCubeMeta { \
Z_TypeRegister_TextureCubeMeta() { \
TypeBuilder Builder{}; \
Builder.CreateType<TextureCubeMeta>("TextureCubeMeta"); \
Builder.AddParent<AssetMeta>(); \
Builder.SetConstructor(TextureCubeMeta::ConstructSelf).SetDestructor(TextureCubeMeta::DestructSelf); \
Builder.AddField("Id", &TextureCubeMeta::Id); \
Builder.AddField("Path", &TextureCubeMeta::Path); \
Builder.AddField("ObjectHandle", &TextureCubeMeta::ObjectHandle); \
Builder.AddField("EnableMipMap", &TextureCubeMeta::EnableMipMap); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_TextureCubeMeta __Z_TypeRegister_TextureCubeMeta_Instance; \

#define GENERATED_HEADER_Texture \
public: \
typedef ThisClass Super; \
typedef Texture ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_Texture { \
Z_TypeRegister_Texture() { \
TypeBuilder Builder{}; \
Builder.CreateType<Texture>("Texture"); \
Builder.AddParent<Asset>(); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Texture __Z_TypeRegister_Texture_Instance; \
private: \

#define GENERATED_HEADER_Texture2D \
public: \
typedef ThisClass Super; \
typedef Texture2D ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Texture2D(); }static void DestructSelf(void* Ptr) { ((Texture2D*)(Ptr))->~Texture2D(); }struct Z_TypeRegister_Texture2D { \
Z_TypeRegister_Texture2D() { \
TypeBuilder Builder{}; \
Builder.CreateType<Texture2D>("Texture2D"); \
Builder.AddParent<Texture>(); \
Builder.SetConstructor(Texture2D::ConstructSelf).SetDestructor(Texture2D::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Texture2D __Z_TypeRegister_Texture2D_Instance; \
private: \

#define GENERATED_HEADER_TextureCube \
public: \
typedef ThisClass Super; \
typedef TextureCube ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) TextureCube(); }static void DestructSelf(void* Ptr) { ((TextureCube*)(Ptr))->~TextureCube(); }struct Z_TypeRegister_TextureCube { \
Z_TypeRegister_TextureCube() { \
TypeBuilder Builder{}; \
Builder.CreateType<TextureCube>("TextureCube"); \
Builder.AddParent<Texture>(); \
Builder.SetConstructor(TextureCube::ConstructSelf).SetDestructor(TextureCube::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_TextureCube __Z_TypeRegister_TextureCube_Instance; \
private: \

