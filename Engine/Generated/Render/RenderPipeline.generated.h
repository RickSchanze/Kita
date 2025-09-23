// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_RenderPipeline \
public: \
typedef RenderPipeline ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
struct Z_TypeRegister_RenderPipeline { \
Z_TypeRegister_RenderPipeline() { \
TypeBuilder Builder{}; \
Builder.CreateType<RenderPipeline>("RenderPipeline"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_RenderPipeline __Z_TypeRegister_RenderPipeline_Instance; \
private: \

