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

void File::Create(const StringView Path, const EFileContentMode ContentMode) {
  if (ContentMode == EFileContentMode::Text) {
    std::ofstream FS(Path.Data());
  }
}

Result<UInt64, EFileSystemError> File::GetLastModifiedTimeImpl(const StringView Path) {
  try {
    const std::string_view StdPath = Path.GetStdStringView();

    // 检查文件是否存在
    if (!std::filesystem::exists(StdPath)) {
      return EFileSystemError::FileNotFound;
    }

    // 检查是否为文件（而不是目录）
    if (!std::filesystem::is_regular_file(StdPath)) {
      return EFileSystemError::ReadDirectory;
    }

    // 获取最后修改时间
    const auto LastWriteTime = std::filesystem::last_write_time(StdPath);

    // 方法1：使用 duration_cast 转换（更兼容的方式）
    const auto Sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(LastWriteTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

    // 转换为Unix时间戳
    const auto Duration = Sctp.time_since_epoch();
    const auto Seconds = std::chrono::duration_cast<std::chrono::seconds>(Duration);
    return static_cast<UInt64>(Seconds.count());
  } catch (const std::filesystem::filesystem_error& E) {
    return HandleFilesystemError(E);
  } catch (const std::exception&) {
    return EFileSystemError::Unknown;
  }
}

Result<UInt64, EFileSystemError> File::HandleFilesystemError(const std::filesystem::filesystem_error& E) {
  switch (E.code().value()) {
  case static_cast<int>(std::errc::no_such_file_or_directory):
    return EFileSystemError::FileNotFound;
  case static_cast<int>(std::errc::permission_denied):
    return EFileSystemError::PermissionDenied;
  case static_cast<int>(std::errc::invalid_argument):
    return EFileSystemError::FileNotFound;
  case static_cast<int>(std::errc::no_space_on_device):
    return EFileSystemError::DiskFull;
  case static_cast<int>(std::errc::read_only_file_system):
    return EFileSystemError::ReadOnly;
  default:
    return EFileSystemError::Unknown;
  }
}
