// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_PersistentObject \
public: \
typedef ThisClass Super; \
typedef PersistentObject ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_PersistentObject { \
Z_TypeRegister_PersistentObject() { \
TypeBuilder Builder{}; \
Builder.CreateType<PersistentObject>("PersistentObject"); \
Builder.AddParent<Object>(); \
} \
}; \
static inline Z_TypeRegister_PersistentObject __Z_TypeRegister_PersistentObject_Instance; \
private: \

