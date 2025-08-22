// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ObjectPtrBase \
typedef ObjectPtrBase ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_ObjectPtrBase { \
Z_TypeRegister_ObjectPtrBase() { \
TypeBuilder Builder{}; \
Builder.CreateType<ObjectPtrBase>("ObjectPtrBase"); \
Builder.AddField("mObjectHandle", &ObjectPtrBase::mObjectHandle); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ObjectPtrBase __Z_TypeRegister_ObjectPtrBase_Instance; \

