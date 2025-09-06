#pragma once
#include "Core/Container/Result.h"
#include "Core/String/String.h"
#include "FileSystemError.h"

#include <filesystem>

enum class EFileContentMode {
  Text,
};

class File {
public:
  static Result<String, EFileSystemError> ReadAllText(StringView Path);
  static EFileSystemError WriteAllText(StringView Path, StringView Text);
  static void Create(StringView Path, EFileContentMode ContentMode);

  // 获取文件最后修改时间（Unix时间戳，秒）
  static Result<UInt64, EFileSystemError> GetLastModifiedTime(const StringView Path) { return GetLastModifiedTimeImpl(Path); }

private:
  // 内部实现函数，支持不同的时间精度
  static Result<UInt64, EFileSystemError> GetLastModifiedTimeImpl(StringView Path);

  // 统一的文件系统错误处理
  static Result<UInt64, EFileSystemError> HandleFilesystemError(const std::filesystem::filesystem_error& E);
};
