// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ShaderInspector \
public: \
typedef ThisClass Super; \
typedef ShaderInspector ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ShaderInspector(); }static void DestructSelf(void* Ptr) { ((ShaderInspector*)(Ptr))->~ShaderInspector(); }struct Z_TypeRegister_ShaderInspector { \
Z_TypeRegister_ShaderInspector() { \
TypeBuilder Builder{}; \
Builder.CreateType<ShaderInspector>("ShaderInspector"); \
Builder.AddParent<Inspector>(); \
Builder.SetConstructor(ShaderInspector::ConstructSelf).SetDestructor(ShaderInspector::DestructSelf); \
Builder.SetTypeAttribute("Inspector", "Shader"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ShaderInspector __Z_TypeRegister_ShaderInspector_Instance; \
private: \

