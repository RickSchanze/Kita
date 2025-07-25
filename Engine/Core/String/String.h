#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"

#include <string>
#include <utility>

class StringView;

class String {
public:
  FORCE_INLINE explicit String(const StringView& Str);
  explicit String(const char* Str) : mStr(Str) {}
  explicit String(const char* Str, const Size InSize) : mStr(Str, InSize) {}
  String(std::string Str) : mStr(std::move(Str)) {}
  INLINE_NODISCARD const std::string& GetStdString() const { return mStr; }

private:
  std::string mStr;
};
