// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_Component \
public: \
typedef ThisClass Super; \
typedef Component ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_Component { \
Z_TypeRegister_Component() { \
TypeBuilder Builder{}; \
Builder.CreateType<Component>("Component"); \
Builder.AddParent<Object>(); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Component __Z_TypeRegister_Component_Instance; \
private: \

