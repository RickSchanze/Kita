#pragma once

enum class EFileSystemError {
  FileNotFound,
  DirectoryNotFound,
  ReadDirectory, // 表示从一个目录读取文件
  FileNotOpen,
  WriteError,
  EndOfFile,
  ReadError,
  PermissionDenied,
  DiskFull,
  ReadOnly,
  Unknown,
  Ok,
};
