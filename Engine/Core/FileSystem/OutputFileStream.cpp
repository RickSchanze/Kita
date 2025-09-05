//
// Created by kita on 25-8-23.
//

#include "OutputFileStream.h"

EFileSystemError OutputFileStream::Open(const String& FilePath, const std::ios::openmode Mode, const bool CreateDirectory) {
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

EFileSystemError OutputFileStream::WriteLine() {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }
  mStream << std::endl;
  if (mStream.fail()) {
    return EFileSystemError::WriteError;
  }
  return EFileSystemError::Ok;
}

EFileSystemError OutputFileStream::WriteBytes(const char* data, std::streamsize size) {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }
  mStream.write(data, size);
  if (mStream.fail()) {
    return EFileSystemError::WriteError;
  }
  return EFileSystemError::Ok;
}

EFileSystemError OutputFileStream::Flush() {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }
  mStream.flush();
  if (mStream.fail()) {
    return EFileSystemError::WriteError;
  }
  return EFileSystemError::Ok;
}

EFileSystemError OutputFileStream::SetPosition(const std::streampos& pos) {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }
  mStream.seekp(pos);
  if (mStream.fail()) {
    return EFileSystemError::Unknown;
  }
  return EFileSystemError::Ok;
}