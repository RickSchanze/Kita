// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_IConfig \
public: \
typedef IConfig ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_IConfig { \
Z_TypeRegister_IConfig() { \
TypeBuilder Builder{}; \
Builder.CreateType<IConfig>("IConfig"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_IConfig __Z_TypeRegister_IConfig_Instance; \
private: \

