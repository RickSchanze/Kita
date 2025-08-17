// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_Object \
public: \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_Object { \
Z_TypeRegister_Object() { \
TypeBuilder Builder{}; \
Builder.CreateType<Object>("Object"); \
Builder.AddField("mName", &Object::mName); \
} \
}; \
static inline Z_TypeRegister_{class_info.name} __Z_TypeRegister_{class_info.name}_Instance; \
private: \

