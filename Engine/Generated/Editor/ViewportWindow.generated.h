// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ViewportWindow \
public: \
typedef ThisClass Super; \
typedef ViewportWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ViewportWindow(); }static void DestructSelf(void* Ptr) { ((ViewportWindow*)(Ptr))->~ViewportWindow(); }struct Z_TypeRegister_ViewportWindow { \
Z_TypeRegister_ViewportWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<ViewportWindow>("ViewportWindow"); \
Builder.AddParent<EditorWindow>(); \
Builder.SetConstructor(ViewportWindow::ConstructSelf).SetDestructor(ViewportWindow::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ViewportWindow __Z_TypeRegister_ViewportWindow_Instance; \
private: \

#define GENERATED_HEADER_MenuAction_OpenViewportWindow \
public: \
typedef ThisClass Super; \
typedef MenuAction_OpenViewportWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MenuAction_OpenViewportWindow(); }static void DestructSelf(void* Ptr) { ((MenuAction_OpenViewportWindow*)(Ptr))->~MenuAction_OpenViewportWindow(); }struct Z_TypeRegister_MenuAction_OpenViewportWindow { \
Z_TypeRegister_MenuAction_OpenViewportWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<MenuAction_OpenViewportWindow>("MenuAction_OpenViewportWindow"); \
Builder.AddParent<MenuAction>(); \
Builder.SetConstructor(MenuAction_OpenViewportWindow::ConstructSelf).SetDestructor(MenuAction_OpenViewportWindow::DestructSelf); \
Builder.SetTypeAttribute("MenuActionItem", "窗口/视口"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MenuAction_OpenViewportWindow __Z_TypeRegister_MenuAction_OpenViewportWindow_Instance; \
private: \

