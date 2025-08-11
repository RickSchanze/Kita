#pragma once
#include "Core/Container/Result.h"
#include "Core/String/String.h"
#include "FileSystemError.h"

class File
{
public:
  static Result<String, EFileSystemError> ReadAllText(StringView Path);
  static EFileSystemError WriteAllText(StringView Path, StringView Text);
};
