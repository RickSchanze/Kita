#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Byte.h"
#include "Core/Container/Result.h"
#include "Core/String/String.h"
#include "FileSystemError.h"

#include <filesystem>
#include <fstream>
#include <iosfwd>

class InputFileStream {
public:
  InputFileStream() = default;

  explicit InputFileStream(const String& FilePath, const std::ios::openmode Mode = std::ios::in) { Open(FilePath, Mode); }

  EFileSystemError Open(const String& FilePath, std::ios::openmode Mode = std::ios::in);

  void Close() {
    if (mStream.is_open()) {
      mStream.close();
    }
  }

  [[nodiscard]] bool IsOpen() const { return mStream.is_open(); }

  std::ifstream& GetStream() { return mStream; }

  // 模板化的 >> 运算符
  template <typename T> InputFileStream& operator>>(T&& Value) {
    mStream.operator>>(std::forward<T>(Value));
    return *this;
  }

  // Read方法 - 读取单个值
  template <typename T> EFileSystemError Read(T& data) {
    if (!IsOpen()) {
      return EFileSystemError::FileNotOpen;
    }
    mStream >> data;
    if (mStream.fail()) {
      if (mStream.eof()) {
        return EFileSystemError::EndOfFile;
      }
      return EFileSystemError::ReadError;
    }
    return EFileSystemError::Ok;
  }

  // Read方法重载 - 读取多个值
  template <typename T, typename... Args> EFileSystemError Read(T& First, Args&... InArgs) {
    auto Result = Read(First);
    if (Result != EFileSystemError::Ok) {
      return Result;
    }
    return Read(InArgs...);
  }

  // ReadLine方法 - 读取一行
  Result<String, EFileSystemError> ReadLine();

  // ReadAllLines方法 - 读取所有行
  Result<Array<String>, EFileSystemError> ReadAllLines();

  // ReadAllText方法 - 读取整个文件内容
  [[nodiscard]] Result<String, EFileSystemError> ReadAllText() const;

  // 读取原始字节数据
  EFileSystemError ReadBytes(Byte* Buffer, UInt64 Size, UInt64& OutBytesRead);

  // 检查是否到达文件末尾
  [[nodiscard]] bool IsEOF() const { return mStream.eof(); }

  // 获取当前读取位置
  [[nodiscard]] std::streampos GetPosition() {
    if (!IsOpen()) {
      return -1;
    }
    return mStream.tellg();
  }

  // 设置读取位置
  EFileSystemError SetPosition(const std::streampos& Pos);

  // 获取文件大小
  Result<UInt64, EFileSystemError> GetFileSize();

private:
  std::ifstream mStream;
};