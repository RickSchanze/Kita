#pragma once
#include "Core/Traits.h"
#include "StringTraits.h"

template <typename T>
  requires Traits::IStringable<T>
String ToString(const T& Value) {
  if constexpr (Traits::IToString<T>) {
    return Value.ToString();
  } else {
    if constexpr (Traits::SameAs<Traits::Pure<T>, String>)
      return Value;
    else if constexpr (Traits::SameAs<Traits::Pure<T>, StringView>)
      return String(Value);
    else
      return String(std::to_string(Value));
  }
}
