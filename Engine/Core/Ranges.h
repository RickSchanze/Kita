#pragma once

#include <algorithm>
#include <ranges>

namespace Ranges {

constexpr auto AnyOf = std::ranges::any_of;

namespace Views {

constexpr auto Enumerate = std::ranges::views::enumerate;

}

} // namespace Ranges
