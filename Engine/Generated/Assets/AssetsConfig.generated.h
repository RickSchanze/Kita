// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_AssetsConfig \
public: \
typedef ThisClass Super; \
typedef AssetsConfig ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) AssetsConfig(); }static void DestructSelf(void* Ptr) { ((AssetsConfig*)(Ptr))->~AssetsConfig(); }struct Z_TypeRegister_AssetsConfig { \
Z_TypeRegister_AssetsConfig() { \
TypeBuilder Builder{}; \
Builder.CreateType<AssetsConfig>("AssetsConfig"); \
Builder.AddParent<IConfig>(); \
Builder.SetConstructor(AssetsConfig::ConstructSelf).SetDestructor(AssetsConfig::DestructSelf); \
Builder.SetTypeAttribute("Category", "Assets"); \
Builder.AddField("ShaderSearchPaths", &AssetsConfig::ShaderSearchPaths); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_AssetsConfig __Z_TypeRegister_AssetsConfig_Instance; \
private: \

