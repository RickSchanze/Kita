// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_MenuAction \
public: \
typedef ThisClass Super; \
typedef MenuAction ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_MenuAction { \
Z_TypeRegister_MenuAction() { \
TypeBuilder Builder{}; \
Builder.CreateType<MenuAction>("MenuAction"); \
Builder.AddParent<Object>(); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MenuAction __Z_TypeRegister_MenuAction_Instance; \
private: \

