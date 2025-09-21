#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"

#include <iostream>
#include <string>
#include <string_view>

class String;

class StringView {
public:
  StringView() = default;
  // ReSharper disable once CppNonExplicitConvertingConstructor
  StringView(const char* Str) : mStr(Str) {} // NOLINT(*-explicit-constructor)
  explicit StringView(const std::string_view Str) : mStr(Str) {}

  // ReSharper disable once CppNonExplicitConvertingConstructor
  StringView(const String& Str); // NOLINT(*-explicit-constructor)
  StringView(const std::string& Str) : mStr(Str) {}

  StringView(const char* Str, const SizeType InSize) : mStr(Str, InSize) {}

  StringView(StringView&&) = default;
  StringView& operator=(StringView&&) = default;
  StringView(const StringView&) = default;
  StringView& operator=(const StringView&) = default;

  INLINE_NODISCARD std::string_view GetStdStringView() const { return mStr; }

  [[nodiscard]] SizeType GetHashCode() const noexcept { return std::hash<std::string_view>{}(mStr); }

  [[nodiscard]] const char* Data() const { return mStr.data(); }

  [[nodiscard]] SizeType Count() const { return mStr.size(); }

  [[nodiscard]] bool EndsWith(const StringView Suffix) const { return mStr.ends_with(Suffix.GetStdStringView()); }

  [[nodiscard]] bool Empty() const { return mStr.empty(); }

  auto begin() { return mStr.begin(); }
  auto end() { return mStr.end(); }
  [[nodiscard]] auto begin() const { return mStr.begin(); }
  [[nodiscard]] auto end() const { return mStr.end(); }

  [[nodiscard]] String ToString() const;

  // 输出流操作符 (friend函数)
  friend std::ostream& operator<<(std::ostream& OS, const StringView& StrView) {
    OS << StrView.mStr;
    return OS;
  }

  [[nodiscard]] SizeType IndexOf(char Target, SizeType Start = 0) const;
  [[nodiscard]] SizeType LastIndexOf(char Target, SizeType Start = std::string::npos) const;

  [[nodiscard]] SizeType IndexOf(StringView Target, SizeType Start = 0) const;
  [[nodiscard]] SizeType LastIndexOf(StringView Target, SizeType Start = std::string::npos) const;

  [[nodiscard]] StringView SubStr(SizeType Start, SizeType Length = std::string::npos) const;

  [[nodiscard]] StringView Trim(char Char) const;
  [[nodiscard]] StringView Trim(StringView Chars) const;

  [[nodiscard]] StringView TrimLeft(char Char) const;
  [[nodiscard]] StringView TrimLeft(StringView Chars) const;

  [[nodiscard]] StringView TrimRight(char Char) const;
  [[nodiscard]] StringView TrimRight(StringView Chars) const;

private:
  std::string_view mStr;
};

inline bool operator==(const StringView& Lhs, const StringView& Rhs) { return Lhs.GetStdStringView() == Rhs.GetStdStringView(); }
