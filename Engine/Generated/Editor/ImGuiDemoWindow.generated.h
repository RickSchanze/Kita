// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ImGuiDemoWindow \
public: \
typedef ThisClass Super; \
typedef ImGuiDemoWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ImGuiDemoWindow(); }static void DestructSelf(void* Ptr) { ((ImGuiDemoWindow*)(Ptr))->~ImGuiDemoWindow(); }struct Z_TypeRegister_ImGuiDemoWindow { \
Z_TypeRegister_ImGuiDemoWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<ImGuiDemoWindow>("ImGuiDemoWindow"); \
Builder.AddParent<EditorWindow>(); \
Builder.SetConstructor(ImGuiDemoWindow::ConstructSelf).SetDestructor(ImGuiDemoWindow::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ImGuiDemoWindow __Z_TypeRegister_ImGuiDemoWindow_Instance; \
private: \

#define GENERATED_HEADER_MenuAction_ShowImGuiDemo \
public: \
typedef ThisClass Super; \
typedef MenuAction_ShowImGuiDemo ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MenuAction_ShowImGuiDemo(); }static void DestructSelf(void* Ptr) { ((MenuAction_ShowImGuiDemo*)(Ptr))->~MenuAction_ShowImGuiDemo(); }struct Z_TypeRegister_MenuAction_ShowImGuiDemo { \
Z_TypeRegister_MenuAction_ShowImGuiDemo() { \
TypeBuilder Builder{}; \
Builder.CreateType<MenuAction_ShowImGuiDemo>("MenuAction_ShowImGuiDemo"); \
Builder.AddParent<MenuAction>(); \
Builder.SetConstructor(MenuAction_ShowImGuiDemo::ConstructSelf).SetDestructor(MenuAction_ShowImGuiDemo::DestructSelf); \
Builder.SetTypeAttribute("MenuActionItem", "工具/ImGui Demo"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MenuAction_ShowImGuiDemo __Z_TypeRegister_MenuAction_ShowImGuiDemo_Instance; \
private: \

