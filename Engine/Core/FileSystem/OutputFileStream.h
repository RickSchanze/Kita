#pragma once
#include "Core/String/String.h"
#include "FileSystemError.h"

#include <filesystem>
#include <fstream>
#include <ios>

class OutputFileStream {
public:
  OutputFileStream() = default;

  OutputFileStream(const String& FilePath, const std::ios::openmode Mode = std::ios::out, const bool CreateDirectory = true) { Open(FilePath, Mode, CreateDirectory); }

  EFileSystemError Open(const String& FilePath, const std::ios::openmode Mode = std::ios::out, const bool CreateDirectory = true) {
    namespace fs = std::filesystem;
    if (const fs::path Path(FilePath.GetStdString()); CreateDirectory && !Path.parent_path().empty() && !fs::exists(Path.parent_path())) {
      fs::create_directories(Path.parent_path());
    }
    mStream.open(FilePath.GetStdString(), Mode);
    if (!mStream.is_open()) {
      return EFileSystemError::Unknown;
    }
    return EFileSystemError::Ok;
  }

  void Close() { mStream.close(); }

  bool IsOpen() const { return mStream.is_open(); }

  std::ofstream& GetStream() { return mStream; }

  // 模板化的 << 运算符
  template <typename T> OutputFileStream& operator<<(T&& Value) {
    mStream.operator<<(std::forward<T>(Value));
    return *this;
  }

  // 支持操作符链（如 << std::endl）
  OutputFileStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
    mStream << manip;
    return *this;
  }

private:
  std::ofstream mStream;
};
