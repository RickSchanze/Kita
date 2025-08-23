// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_RHIConfig \
public: \
typedef ThisClass Super; \
typedef RHIConfig ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) RHIConfig(); }static void DestructSelf(void* Ptr) { ((RHIConfig*)(Ptr))->~RHIConfig(); }struct Z_TypeRegister_RHIConfig { \
Z_TypeRegister_RHIConfig() { \
TypeBuilder Builder{}; \
Builder.CreateType<RHIConfig>("RHIConfig"); \
Builder.AddParent<IConfig>(); \
Builder.SetConstructor(RHIConfig::ConstructSelf).SetDestructor(RHIConfig::DestructSelf); \
Builder.SetTypeAttribute("Category", "RHI"); \
Builder.AddField("PresentMode", &RHIConfig::PresentMode); \
Builder.AddField("SurfaceWindowType", &RHIConfig::SurfaceWindowType); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_RHIConfig __Z_TypeRegister_RHIConfig_Instance; \
private: \

