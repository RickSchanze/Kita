#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"
#include "String.h"

#include <string_view>

class String;

class StringView {
public:
  // ReSharper disable once CppNonExplicitConvertingConstructor
  StringView(const char* Str) : mStr(Str) {} // NOLINT(*-explicit-constructor)
  explicit StringView(const std::string_view Str) : mStr(Str) {}

  // ReSharper disable once CppNonExplicitConvertingConstructor
  StringView(const String& Str); // NOLINT(*-explicit-constructor)

  StringView(const char* Str, const SizeType InSize) : mStr(Str, InSize) {}

  StringView(StringView&&) = default;
  StringView& operator=(StringView&&) = default;
  StringView(const StringView&) = default;
  StringView& operator=(const StringView&) = default;

  INLINE_NODISCARD std::string_view GetStdStringView() const { return mStr; }

  [[nodiscard]] SizeType GetHashCode() const noexcept { return std::hash<std::string_view>{}(mStr); }

private:
  std::string_view mStr;
};

inline bool operator==(const StringView& Lhs, const StringView& Rhs) { return Lhs.GetStdStringView() == Rhs.GetStdStringView(); }
