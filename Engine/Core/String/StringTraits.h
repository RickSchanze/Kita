#pragma once
#include "Core/Traits.h"
#include "String.h"

namespace Traits {

template <typename T>
concept IToString = requires(T& A) {
  { A.ToString() } -> std::convertible_to<String>;
};

template <typename T>
concept ISelfStringable = AnyOf<T, UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64, Float32, Float64, bool, String, StringView>;

template <typename T>
concept IStringable = IToString<T> || ISelfStringable<T>;
} // namespace Traits
