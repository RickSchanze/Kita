#pragma once

#include <algorithm>
#include <ranges>

namespace Ranges {

constexpr auto AnyOf = std::ranges::any_of;

namespace Views {

constexpr auto Enumerate = std::ranges::views::enumerate;
constexpr auto Values = std::ranges::views::values;

} // namespace Views

} // namespace Ranges
