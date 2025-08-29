//
// Created by kita on 25-8-23.
//

#include "ConfigManager.h"

#include "Core/Reflection/Type.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "IConfig.h"

void ConfigManager::ShutDown() {
  auto& Self = GetRef();
  Self.SaveAllDirtyConfigsM();
  Self.ClearDeleteAllConfigs();
}

void ConfigManager::SaveAllDirtyConfigsM() {
  Map<String, TOMLOutputArchive> FileArchives;
  Map<String, Array<String>> FileConfigTypes;
  for (const auto& [ConfigType, Config] : mConfigs) {
    String ConfigPath = Config->GetFilePath();
    if (!FileArchives.Contains(ConfigPath)) {
      FileArchives[ConfigPath] = {};
    }
    FileConfigTypes[ConfigPath].Add(ConfigType->GetName().ToString());
  }
  // 按类型和文件写入
  for (const auto& Config : mConfigs | Ranges::Views::Values) {
    String ConfigPath = Config->GetFilePath();
    ASSERT_MSG(FileArchives.Contains(ConfigPath), "FileArchives不包含ConfigPath:{}", ConfigPath);
    auto& Archive = FileArchives[ConfigPath];
    Archive.WriteType(Config->GetCategory(), *Config);
  }
  // 写入文件
  for (auto& [ConfigPath, Archive] : FileArchives) {
    Archive.WriteFile(ConfigPath);
  }
}

IConfig* ConfigManager::GetConfigM(const Type* InType) {
  if (mConfigs.Contains(InType)) {
    return mConfigs[InType];
  } else {
    IConfig* NewConfig = InType->CreateInstanceT<IConfig>();
    if (NewConfig == nullptr) {
      LOG_CRITICAL_TAG("Config", "ConfigType:{}未注册, 创建失败.", InType->GetName());
    }
    String FilePath = NewConfig->GetFilePath();
    String Category = NewConfig->GetCategory();
    ASSERT_MSG(!Category.Empty(), "配置文件{}必须有Category.", InType->GetName());
    TOMLInputArchive Archive{};
    ESerializationError Error = Archive.ParseFile(FilePath);
    if (Error != ESerializationError::Ok) {
      if (Error == ESerializationError::TargetInvalid) {
        LOG_WARN_TAG("Config", "{}对应的路径'{}'不存在, 将使用默认值.", InType->GetName(), FilePath);
        NewConfig->SetSaveDirty(true);
      } else {
        LOG_ERROR_TAG("Config", "{}配置文件[{}]解析失败, 将使用默认值. Error={}", InType->GetName(), FilePath, static_cast<int>(Error));
      }
    } else {
      Archive.ReadType(Category, *NewConfig);
    }
    mConfigs.Add(InType, NewConfig);
    return NewConfig;
  }
}

void ConfigManager::ClearDeleteAllConfigs() {
  for (auto& [ConfigType, Config] : mConfigs) {
    Delete(Config);
  }
  mConfigs.Clear();
}