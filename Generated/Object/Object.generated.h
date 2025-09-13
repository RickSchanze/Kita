// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_Object \
public: \
typedef Object ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Object(); }static void DestructSelf(void* Ptr) { ((Object*)(Ptr))->~Object(); }struct Z_TypeRegister_Object { \
Z_TypeRegister_Object() { \
TypeBuilder Builder{}; \
Builder.CreateType<Object>("Object"); \
Builder.SetConstructor(Object::ConstructSelf).SetDestructor(Object::DestructSelf); \
Builder.AddField("mName", &Object::mName); \
Builder.AddField("mHandle", &Object::mHandle); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Object __Z_TypeRegister_Object_Instance; \
private: \

