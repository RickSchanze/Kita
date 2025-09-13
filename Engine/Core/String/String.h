#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"
#include "StringView.h"

#include <string>
#include <utility>
#include <iostream>

class StringView;

class String {
public:
  String() = default;
  FORCE_INLINE explicit String(const StringView& Str);
  String(const char* Str) : mStr(Str) {}
  explicit String(const char* Str, const SizeType InSize) : mStr(Str, InSize) {}
  String(std::string Str) : mStr(std::move(Str)) {}
  INLINE_NODISCARD const std::string& GetStdString() const { return mStr; }

  String& operator+=(const String& Str) {
    mStr += Str.mStr;
    return *this;
  }

  [[nodiscard]] SizeType GetHashCode() const noexcept { return std::hash<std::string>{}(mStr); }
  [[nodiscard]] bool Empty() const noexcept { return mStr.empty(); }

  [[nodiscard]] auto begin() const { return mStr.begin(); }
  [[nodiscard]] auto end() const { return mStr.end(); }
  auto begin() { return mStr.begin(); }
  auto end() { return mStr.end(); }

  [[nodiscard]] const char* Data() const { return mStr.c_str(); }

  [[nodiscard]] bool EndsWith(const StringView& Suffix) const;

  // 输出流操作符 (friend函数)
  friend std::ostream& operator<<(std::ostream& OS, const String& Str) {
    OS << Str.mStr;
    return OS;
  }

  // 输入流操作符 (friend函数)
  friend std::istream& operator>>(std::istream& IS, String& Str);

  // 替换所有出现的字符
  [[nodiscard]] String Replace(char OldChar, char NewChar) const;

  // 替换所有出现的子字符串
  [[nodiscard]] String Replace(const StringView& OldStr, const StringView& NewStr) const;

  [[nodiscard]] StringView Trim(char Ch) const;
  [[nodiscard]] StringView Trim(StringView Chars) const;

  [[nodiscard]] StringView TrimLeft(char Ch) const;
  [[nodiscard]] StringView TrimLeft(StringView Chars) const;

  [[nodiscard]] StringView TrimRight(char Ch) const;
  [[nodiscard]] StringView TrimRight(StringView Chars) const;

  [[nodiscard]] SizeType Count() const { return mStr.size(); }

private:
  std::string mStr;
};

FORCE_INLINE String operator+(const String& Str1, const String& Str2) { return {Str1.GetStdString() + Str2.GetStdString()}; }
String operator+(const String& Str1, const StringView& Str2);
String operator+(const String& Str1, const char* Str2);
