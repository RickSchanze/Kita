#pragma once
#include "Core/Container/Map.h"
#include "Core/Container/Set.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Singleton/Singleton.h"

class IConfig;
struct Type;
class ConfigManager : Singleton<ConfigManager> {
public:
  static void StartUp() {
    // do nothing
  }

  static void ShutDown();

  void SaveAllDirtyConfigsM();
  static void SaveAllDirtyConfigs() { GetRef().SaveAllDirtyConfigsM(); }

  IConfig* GetConfigM(const Type* InType);
  static IConfig* GetConfig(const Type* InType) { return GetRef().GetConfigM(InType); }
  template <typename T> static T* GetConfig() { return static_cast<T*>(GetConfig(TypeOf<T>())); }

private:
  void ClearDeleteAllConfigs();

  Map<const Type*, IConfig*> mConfigs;
  Set<String> mLoadedConfigFiles;
};
