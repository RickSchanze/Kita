#pragma once
#include "Core/FileSystem/Path.h"
#include "Core/Reflection/MetaMark.h"
#include "Core/String/String.h"

#include "Project.generated.h"

KSTRUCT()
struct Project {
  GENERATED_BODY(Project)
public:
  static void StartUp(StringView ProjectPath);
  static void ShutDown();
  static void UpdatePersistentId(Int32 NewId);
  static Int32 GetPersistentId() { return GetRef().mPersistentId; }

  /// 确保ProjectPath/Library存在 不存在则Crash
  static void EnsureLibraryExists();

  /// 确保ProjectPath/Intermediate存在 不存在则Crash
  static void EnsureIntermediateExists();

  /// 确保ProjectPath/Project.yaml存在 不存在则Crash
  static void EnsureProjectFileExists();

  /// 确保ProjectPath本身存在
  static void EnsureProjectPathExists();

  /// 获取对应路径 如果路径字符串不存在则缓存
  static StringView GetProjectFilePath();
  static StringView GetLibraryPath();
  static StringView GetIntermediatePath();

  static void SetProjectName(StringView NewName);
  static StringView GetProjectName() { return GetRef().mProjectName; }

  static StringView GetProjectPath() { return GetRef().mProjectPath; }

  static Project& GetRef() {
    static Project Project;
    return Project;
  }

  static Project* Get() { return &GetRef(); }

private:
  static void ReadProjectFile();

  KPROPERTY()
  String mProjectPath;

  KPROPERTY()
  String mProjectName = "UnnamedProject";

  /// 记录当前持久化ID用到哪了
  KPROPERTY()
  Int32 mPersistentId = 0;

  /// 缓存路径
  String mProjectFilePath;
  String mLibraryPath;
  String mIntermediatePath;
};
