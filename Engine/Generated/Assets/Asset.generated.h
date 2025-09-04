// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
void Z_Reflection_Register_Func_Enum_EAssetType();struct Z_Reflection_Register_Enum_EAssetType { Z_Reflection_Register_Enum_EAssetType() {Z_Reflection_Register_Func_Enum_EAssetType(); }};
inline Z_Reflection_Register_Enum_EAssetType Z_Reflection_Register_Enum_EAssetType___Instance{}; 
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

