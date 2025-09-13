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
static void ConstructSelf(void* Ptr) { new (Ptr) Actor(); }static void DestructSelf(void* Ptr) { ((Actor*)(Ptr))->~Actor(); }struct Z_TypeRegister_Actor { \
Z_TypeRegister_Actor() { \
TypeBuilder Builder{}; \
Builder.CreateType<Actor>("Actor"); \
Builder.AddParent<Object>(); \
Builder.SetConstructor(Actor::ConstructSelf).SetDestructor(Actor::DestructSelf); \
Builder.AddField("mComponents", &Actor::mComponents); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Actor __Z_TypeRegister_Actor_Instance; \
void GetReferencingObject(Array<Int32>& OutArray) \
{ \
ExtractReferencingObjectHandle(mComponents, OutArray); \
} \
private: \

