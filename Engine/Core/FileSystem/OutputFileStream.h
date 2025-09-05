#pragma once
#include "Core/String/String.h"
#include "FileSystemError.h"

#include <filesystem>
#include <fstream>
#include <iosfwd>
class OutputFileStream {
public:
  OutputFileStream() = default;

  OutputFileStream(const String& FilePath, const std::ios::openmode Mode = std::ios::out, const bool CreateDirectory = true) { Open(FilePath, Mode, CreateDirectory); }

  EFileSystemError Open(const String& FilePath, std::ios::openmode Mode = std::ios::out, bool CreateDirectory = true);

  void Close() {
    if (mStream.is_open()) {
      mStream.close();
    }
  }

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

  // Write方法 - 写入数据但不换行
  template <typename T> EFileSystemError Write(const T& data) {
    if (!IsOpen()) {
      return EFileSystemError::FileNotOpen;
    }
    mStream << data;
    if (mStream.fail()) {
      return EFileSystemError::WriteError;
    }
    return EFileSystemError::Ok;
  }

  // Write方法重载 - 写入多个参数
  template <typename T, typename... Args> EFileSystemError Write(const T& first, const Args&... args) {
    if (!IsOpen()) {
      return EFileSystemError::FileNotOpen;
    }
    mStream << first;
    if (mStream.fail()) {
      return EFileSystemError::WriteError;
    }
    return Write(args...);
  }

  // WriteLine方法 - 写入数据并换行
  template <typename T> EFileSystemError WriteLine(const T& data) {
    if (!IsOpen()) {
      return EFileSystemError::FileNotOpen;
    }
    mStream << data << std::endl;
    if (mStream.fail()) {
      return EFileSystemError::WriteError;
    }
    return EFileSystemError::Ok;
  }

  // WriteLine方法重载 - 写入多个参数并换行
  template <typename T, typename... Args> EFileSystemError WriteLine(const T& first, const Args&... args) {
    if (!IsOpen()) {
      return EFileSystemError::FileNotOpen;
    }
    mStream << first;
    if (mStream.fail()) {
      return EFileSystemError::WriteError;
    }
    auto result = Write(args...);
    if (result != EFileSystemError::Ok) {
      return result;
    }
    mStream << std::endl;
    if (mStream.fail()) {
      return EFileSystemError::WriteError;
    }
    return EFileSystemError::Ok;
  }

  // WriteLine方法 - 仅换行
  EFileSystemError WriteLine();

  // 写入原始数据
  EFileSystemError WriteBytes(const char* data, std::streamsize size);

  // 刷新缓冲区
  EFileSystemError Flush();

  // 获取当前写入位置
  std::streampos GetPosition() {
    if (!IsOpen()) {
      return -1;
    }
    return mStream.tellp();
  }

  // 设置写入位置
  EFileSystemError SetPosition(const std::streampos& pos);

private:
  std::ofstream mStream;
};