// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_Actor \
public: \
typedef ThisClass Super; \
typedef Actor ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_Actor { \
Z_TypeRegister_Actor() { \
TypeBuilder Builder{}; \
Builder.CreateType<Actor>("Actor"); \
Builder.AddParent<Object>(); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Actor __Z_TypeRegister_Actor_Instance; \
private: \

