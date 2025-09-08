// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_Project \
typedef Project ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) Project(); }static void DestructSelf(void* Ptr) { ((Project*)(Ptr))->~Project(); }struct Z_TypeRegister_Project { \
Z_TypeRegister_Project() { \
TypeBuilder Builder{}; \
Builder.CreateType<Project>("Project"); \
Builder.SetConstructor(Project::ConstructSelf).SetDestructor(Project::DestructSelf); \
Builder.AddField("mProjectPath", &Project::mProjectPath); \
Builder.AddField("mProjectName", &Project::mProjectName); \
Builder.AddField("mPersistentId", &Project::mPersistentId); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_Project __Z_TypeRegister_Project_Instance; \

