//
// Created by kita on 25-8-31.
//

#include "Project.h"

#include "Core/Serialization/YAML/YAMLInputArchive.h"
#include "Core/Serialization/YAML/YAMLOutputArchive.h"
#include <filesystem>

void Project::StartUp(StringView ProjectPath) {
  auto& Self = GetRef();
  Self.mProjectPath = ProjectPath.ToString();
  EnsureProjectPathExists();
  EnsureProjectFileExists();
  EnsureLibraryExists();
  EnsureIntermediateExists();
  ReadProjectFile();
  std::filesystem::current_path(ProjectPath.GetStdStringView());
}

void Project::ShutDown() {
  YAMLOutputArchive Archive;
  auto& Self = GetRef();
  Archive.WriteType("Project", Self);
  Archive.WriteFile(Self.mProjectFilePath);
}

void Project::EnsureLibraryExists() {
  StringView Path = GetLibraryPath();
  if (!Path::IsExists(Path)) {
    if (!Path::CreateDirectory(Path)) {
      gLogger.Critical("Project", "创建项目库路径失败.");
    }
  }
}

void Project::EnsureIntermediateExists() {
  StringView Path = GetIntermediatePath();
  if (!Path::IsExists(Path)) {
    if (!Path::CreateDirectory(Path)) {
      gLogger.Critical("Project", "创建项目中间文件路径失败.");
    }
  }
}

void Project::EnsureProjectFileExists() {
  StringView Path = GetProjectFilePath();
  if (Path::IsExists(Path) && !Path::IsDirectory(Path)) {
    return;
  }
  YAMLOutputArchive Archive;
  auto& Self = GetRef();
  Archive.WriteType("Project", Self);
  Archive.WriteFile(Path);
}

void Project::EnsureProjectPathExists() {
  auto& Self = GetRef();
  if (Path::IsExists(Self.mProjectPath)) {
    if (!Path::IsDirectory(Self.mProjectPath)) {
      gLogger.Critical("Project", "Project的根路径一定要是一个文件夹.");
    }
  }
}

StringView Project::GetProjectFilePath() {
  auto& Self = GetRef();
  if (Self.mProjectFilePath.Empty()) {
    Self.mProjectFilePath = Path::Combine(Self.mProjectPath, "project.yaml");
    return Self.mProjectFilePath;
  }

  return Self.mProjectFilePath;
}

StringView Project::GetLibraryPath() {
  auto& Self = GetRef();
  if (Self.mLibraryPath.Empty()) {
    Self.mLibraryPath = Path::Combine(Self.mProjectPath, "Library");
  }
  return Self.mLibraryPath;
}

StringView Project::GetIntermediatePath() {
  auto& Self = GetRef();
  if (Self.mIntermediatePath.Empty()) {
    Self.mIntermediatePath = Path::Combine(Self.mProjectPath, "Intermediate");
  }
  return Self.mIntermediatePath;
}

void Project::SetProjectName(StringView NewName) {
  auto& Self = GetRef();
  if (NewName.Empty()) {
    ;
  } else {
    Self.mProjectName = NewName.ToString();
  }
}

void Project::ReadProjectFile() {
  StringView Path = GetProjectFilePath();
  YAMLInputArchive Archive;
  ESerializationError Error = Archive.ParseFile(Path);
  if (Error != ESerializationError::Ok) {
    gLogger.Critical("Project", "解析项目文件失败.");
  }
  Archive.ReadType("Project", GetRef());
}
