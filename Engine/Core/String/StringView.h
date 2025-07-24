#pragma once

#include "Core/Macros.h"
#include "Core/TypeDefs.h"

#include <string_view>

class String;

class StringView {
public:
  // ReSharper disable once CppNonExplicitConvertingConstructor
  StringView(const char* Str) : mStr(Str) {} // NOLINT(*-explicit-constructor)

  // ReSharper disable once CppNonExplicitConvertingConstructor
  KITA_FORCE_INLINE StringView(const String& Str); // NOLINT(*-explicit-constructor)

  StringView(const char* Str, const Size InSize) : mStr(Str, InSize) {}

  StringView(const StringView&&) = default;
  StringView& operator=(const StringView&&) = default;
  StringView(const StringView&) = default;
  StringView& operator=(const StringView&) = default;

  KITA_INLINE_NODISCARD std::string_view GetStdStringView() const { return mStr; }

private:
  std::string_view mStr;
};
