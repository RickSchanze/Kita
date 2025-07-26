#pragma once
#include "Core/Traits.h"
#include "String.h"

namespace Traits {
template <typename T>
concept IStringable = requires(const T& A) {
  { A.ToString() } -> std::convertible_to<String>;
};

template <typename T>
concept ISelfStringable = AnyOf<T, UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64, Float32, Float64, bool>;

} // namespace Traits
