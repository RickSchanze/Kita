#pragma once
#include "Core/Container/Array.h"

class StringUtils {
public:
  static Array<StringView> Split(StringView Str, const char Delimiter);
};