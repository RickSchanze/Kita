#pragma once

#include "Container/Array.h"

#include <algorithm>
#include <ranges>

namespace Ranges {

constexpr auto AnyOf = std::ranges::any_of;

namespace Views {

constexpr auto Enumerate = std::ranges::views::enumerate;
constexpr auto Values = std::ranges::views::values;
constexpr auto Transform = std::ranges::views::transform;

} // namespace Views

// 完全泛型的别名包装，支持 C++23 std::ranges::to 的所有重载
template <typename Container, typename... Args> constexpr auto To(Args&&... args) { return std::ranges::to<Container>(std::forward<Args>(args)...); }

// 如果需要支持 CTAD（像 Array 这种可以通过 deduction guide 推导类型）
template <template <typename...> class Container, typename... Args> constexpr auto To(Args&&... args) { return std::ranges::to<Container>(std::forward<Args>(args)...); }

} // namespace Ranges
