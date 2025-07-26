#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"
#include "StringView.h"

#include <string>
#include <utility>

class StringView;

class String {
public:
  FORCE_INLINE explicit String(const StringView& Str);
  String(const char* Str) : mStr(Str) {}
  explicit String(const char* Str, const Size InSize) : mStr(Str, InSize) {}
  String(std::string Str) : mStr(std::move(Str)) {}
  INLINE_NODISCARD const std::string& GetStdString() const { return mStr; }

  String& operator+=(const String& Str) {
    mStr += Str.mStr;
    return *this;
  }

private:
  std::string mStr;
};

FORCE_INLINE String operator+(const String& Str1, const String& Str2) { return {Str1.GetStdString() + Str2.GetStdString()}; }
String operator+(const String& Str1, const StringView& Str2);
FORCE_INLINE String operator+(const String& Str1, const char* Str2) { return {Str1.GetStdString() + StringView(Str2)}; }
