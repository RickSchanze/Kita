// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_InspectorWindow \
public: \
typedef ThisClass Super; \
typedef InspectorWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) InspectorWindow(); }static void DestructSelf(void* Ptr) { ((InspectorWindow*)(Ptr))->~InspectorWindow(); }struct Z_TypeRegister_InspectorWindow { \
Z_TypeRegister_InspectorWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<InspectorWindow>("InspectorWindow"); \
Builder.AddParent<EditorWindow>(); \
Builder.SetConstructor(InspectorWindow::ConstructSelf).SetDestructor(InspectorWindow::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_InspectorWindow __Z_TypeRegister_InspectorWindow_Instance; \
private: \

#define GENERATED_HEADER_MenuAction_OpenInspectorWindow \
public: \
typedef ThisClass Super; \
typedef MenuAction_OpenInspectorWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MenuAction_OpenInspectorWindow(); }static void DestructSelf(void* Ptr) { ((MenuAction_OpenInspectorWindow*)(Ptr))->~MenuAction_OpenInspectorWindow(); }struct Z_TypeRegister_MenuAction_OpenInspectorWindow { \
Z_TypeRegister_MenuAction_OpenInspectorWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<MenuAction_OpenInspectorWindow>("MenuAction_OpenInspectorWindow"); \
Builder.AddParent<MenuAction>(); \
Builder.SetConstructor(MenuAction_OpenInspectorWindow::ConstructSelf).SetDestructor(MenuAction_OpenInspectorWindow::DestructSelf); \
Builder.SetTypeAttribute("MenuActionItem", "窗口/细节"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MenuAction_OpenInspectorWindow __Z_TypeRegister_MenuAction_OpenInspectorWindow_Instance; \
private: \

