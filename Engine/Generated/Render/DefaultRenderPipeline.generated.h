// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_DefaultRenderPipeline \
public: \
typedef ThisClass Super; \
typedef DefaultRenderPipeline ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) DefaultRenderPipeline(); }static void DestructSelf(void* Ptr) { ((DefaultRenderPipeline*)(Ptr))->~DefaultRenderPipeline(); }struct Z_TypeRegister_DefaultRenderPipeline { \
Z_TypeRegister_DefaultRenderPipeline() { \
TypeBuilder Builder{}; \
Builder.CreateType<DefaultRenderPipeline>("DefaultRenderPipeline"); \
Builder.AddParent<RenderPipeline>(); \
Builder.SetConstructor(DefaultRenderPipeline::ConstructSelf).SetDestructor(DefaultRenderPipeline::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_DefaultRenderPipeline __Z_TypeRegister_DefaultRenderPipeline_Instance; \
private: \

