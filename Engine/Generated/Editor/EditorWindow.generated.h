// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_EditorWindow \
public: \
typedef ThisClass Super; \
typedef EditorWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) EditorWindow(); }static void DestructSelf(void* Ptr) { ((EditorWindow*)(Ptr))->~EditorWindow(); }struct Z_TypeRegister_EditorWindow { \
Z_TypeRegister_EditorWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<EditorWindow>("EditorWindow"); \
Builder.AddParent<Object>(); \
Builder.SetConstructor(EditorWindow::ConstructSelf).SetDestructor(EditorWindow::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_EditorWindow __Z_TypeRegister_EditorWindow_Instance; \
private: \

