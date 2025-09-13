// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ContentBrowserWindow \
public: \
typedef ThisClass Super; \
typedef ContentBrowserWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ContentBrowserWindow(); }static void DestructSelf(void* Ptr) { ((ContentBrowserWindow*)(Ptr))->~ContentBrowserWindow(); }struct Z_TypeRegister_ContentBrowserWindow { \
Z_TypeRegister_ContentBrowserWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<ContentBrowserWindow>("ContentBrowserWindow"); \
Builder.AddParent<EditorWindow>(); \
Builder.SetConstructor(ContentBrowserWindow::ConstructSelf).SetDestructor(ContentBrowserWindow::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ContentBrowserWindow __Z_TypeRegister_ContentBrowserWindow_Instance; \
private: \

#define GENERATED_HEADER_MenuAction_OpenContentWindow \
public: \
typedef ThisClass Super; \
typedef MenuAction_OpenContentWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MenuAction_OpenContentWindow(); }static void DestructSelf(void* Ptr) { ((MenuAction_OpenContentWindow*)(Ptr))->~MenuAction_OpenContentWindow(); }struct Z_TypeRegister_MenuAction_OpenContentWindow { \
Z_TypeRegister_MenuAction_OpenContentWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<MenuAction_OpenContentWindow>("MenuAction_OpenContentWindow"); \
Builder.AddParent<MenuAction>(); \
Builder.SetConstructor(MenuAction_OpenContentWindow::ConstructSelf).SetDestructor(MenuAction_OpenContentWindow::DestructSelf); \
Builder.SetTypeAttribute("MenuActionItem", "窗口/内容浏览器"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MenuAction_OpenContentWindow __Z_TypeRegister_MenuAction_OpenContentWindow_Instance; \
private: \

