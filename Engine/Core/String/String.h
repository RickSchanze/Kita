#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"

#include <string>

class StringView;

class String {
public:
  KITA_FORCE_INLINE explicit String(const StringView& Str);
  explicit String(const char* Str) : mStr(Str) {}
  explicit String(const char* Str, const Size InSize) : mStr(Str, InSize) {}
  KITA_INLINE_NODISCARD const std::string& GetStdString() const { return mStr; }

private:
  std::string mStr;
};
