// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_TextureInspector \
public: \
typedef ThisClass Super; \
typedef TextureInspector ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) TextureInspector(); }static void DestructSelf(void* Ptr) { ((TextureInspector*)(Ptr))->~TextureInspector(); }struct Z_TypeRegister_TextureInspector { \
Z_TypeRegister_TextureInspector() { \
TypeBuilder Builder{}; \
Builder.CreateType<TextureInspector>("TextureInspector"); \
Builder.AddParent<Inspector>(); \
Builder.SetConstructor(TextureInspector::ConstructSelf).SetDestructor(TextureInspector::DestructSelf); \
Builder.SetTypeAttribute("Inspector", "Texture2D"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_TextureInspector __Z_TypeRegister_TextureInspector_Instance; \
private: \

