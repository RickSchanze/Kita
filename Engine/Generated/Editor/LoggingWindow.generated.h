// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_LoggingWindow \
public: \
typedef ThisClass Super; \
typedef LoggingWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) LoggingWindow(); }static void DestructSelf(void* Ptr) { ((LoggingWindow*)(Ptr))->~LoggingWindow(); }struct Z_TypeRegister_LoggingWindow { \
Z_TypeRegister_LoggingWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<LoggingWindow>("LoggingWindow"); \
Builder.AddParent<EditorWindow>(); \
Builder.SetConstructor(LoggingWindow::ConstructSelf).SetDestructor(LoggingWindow::DestructSelf); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_LoggingWindow __Z_TypeRegister_LoggingWindow_Instance; \
private: \

#define GENERATED_HEADER_MenuAction_OpenLoggingWindow \
public: \
typedef ThisClass Super; \
typedef MenuAction_OpenLoggingWindow ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) MenuAction_OpenLoggingWindow(); }static void DestructSelf(void* Ptr) { ((MenuAction_OpenLoggingWindow*)(Ptr))->~MenuAction_OpenLoggingWindow(); }struct Z_TypeRegister_MenuAction_OpenLoggingWindow { \
Z_TypeRegister_MenuAction_OpenLoggingWindow() { \
TypeBuilder Builder{}; \
Builder.CreateType<MenuAction_OpenLoggingWindow>("MenuAction_OpenLoggingWindow"); \
Builder.AddParent<MenuAction>(); \
Builder.SetConstructor(MenuAction_OpenLoggingWindow::ConstructSelf).SetDestructor(MenuAction_OpenLoggingWindow::DestructSelf); \
Builder.SetTypeAttribute("MenuActionItem", "工具/日志"); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_MenuAction_OpenLoggingWindow __Z_TypeRegister_MenuAction_OpenLoggingWindow_Instance; \
private: \

