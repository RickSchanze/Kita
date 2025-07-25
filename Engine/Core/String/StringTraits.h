#pragma once
#include "String.h"

namespace Traits {
template <typename T>
concept IStringable = requires(T A) {
  { A.ToString() } -> std::convertible_to<String>;
};
} // namespace Traits
