//
// Created by kita on 25-9-6.
//

#include "InputFileStream.h"

EFileSystemError InputFileStream::Open(const String& FilePath, const std::ios::openmode Mode) {
  namespace fs = std::filesystem;
  if (!fs::exists(FilePath.GetStdString())) {
    return EFileSystemError::FileNotFound;
  }

  mStream.open(FilePath.GetStdString(), Mode);
  if (!mStream.is_open()) {
    return EFileSystemError::Unknown;
  }
  return EFileSystemError::Ok;
}

Result<String, EFileSystemError> InputFileStream::ReadLine() {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }

  if (std::string Line; std::getline(mStream, Line)) {
    return String(Line);
  }

  if (mStream.eof()) {
    return EFileSystemError::EndOfFile;
  }
  return EFileSystemError::ReadError;
}

Result<Array<String>, EFileSystemError> InputFileStream::ReadAllLines() {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }
  Array<String> Lines;

  while (true) {
    if (auto Line = ReadLine()) {
      Lines.Add(*Line);
    } else {
      if (Line.Error() == EFileSystemError::EndOfFile) {
        return Lines;
      }
      return Line.Error();
    }
  }
}

Result<String, EFileSystemError> InputFileStream::ReadAllText() const {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }

  std::ostringstream Buffer;
  Buffer << mStream.rdbuf();

  if (mStream.bad()) {
    return EFileSystemError::ReadError;
  }
  return String(Buffer.str());
}

EFileSystemError InputFileStream::ReadBytes(Byte* Buffer, const UInt64 Size, UInt64& OutBytesRead) {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }

  mStream.read(reinterpret_cast<char*>(Buffer), Size);
  OutBytesRead = mStream.gcount();

  if (mStream.bad()) {
    return EFileSystemError::ReadError;
  }

  if (mStream.eof() && OutBytesRead == 0) {
    return EFileSystemError::EndOfFile;
  }

  return EFileSystemError::Ok;
}

EFileSystemError InputFileStream::SetPosition(const std::streampos& Pos) {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }
  mStream.seekg(Pos);
  if (mStream.fail()) {
    return EFileSystemError::Unknown;
  }
  return EFileSystemError::Ok;
}

Result<UInt64, EFileSystemError> InputFileStream::GetFileSize() {
  if (!IsOpen()) {
    return EFileSystemError::FileNotOpen;
  }

  const auto CurrentPos = mStream.tellg();
  mStream.seekg(0, std::ios::end);
  const UInt64 Size = mStream.tellg();
  mStream.seekg(CurrentPos);

  if (mStream.fail()) {
    return EFileSystemError::Unknown;
  }
  return Size;
}