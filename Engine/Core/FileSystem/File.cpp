//
// Created by kita on 25-8-9.
//

#include "File.h"

#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"
#include "Path.h"

#include <fstream>

Result<String, EFileSystemError> File::ReadAllText(StringView Path) {
  if (!Path::IsExists(Path)) {
    return EFileSystemError::FileNotFound;
  }
  if (Path::IsDirectory(Path)) {
    return EFileSystemError::ReadDirectory;
  }
  std::string Content;
  std::ifstream FS(Path.Data(), std::ios::in);
  ASSERT_MSG(FS.is_open(), "Failed to open file {}.", Path);
  FS.seekg(0, std::ios::end);
  Content.resize(FS.tellg());
  FS.seekg(0, std::ios::beg);
  FS.read(Content.data(), Content.size());
  FS.close();
  return String(Content);
}

EFileSystemError File::WriteAllText(StringView Path, const StringView Text) {
  if (Path::IsDirectory(Path)) {
    return EFileSystemError::ReadDirectory;
  }
  std::ofstream FS(Path.Data());
  ASSERT_MSG(FS.is_open(), "Failed to open file {}.", Path);
  FS << Text;
  FS.close();
  return EFileSystemError::Ok;
}

void File::Create(StringView Path, EFileContentMode ContentMode) {
  if (ContentMode == EFileContentMode::Text) {
    std::ofstream FS(Path.Data());
  }
}
