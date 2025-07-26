#pragma once
#include <type_traits>

namespace Traits {

namespace Pri {

template <typename T, typename... Args> struct AnyOf : std::bool_constant<(std::is_same_v<T, Args> || ...)> {};

} // namespace Pri

template <typename T, typename... Args>
concept AnyOf = Pri::AnyOf<T, Args...>::value;

template <typename T1, typename T2>
concept SameAs = std::is_same_v<T1, T2>;

// 一些通用性概念接口
template <typename T>
concept IEquatable = requires(const T& a, const T& b) {
  { a == b } -> std::convertible_to<bool>;
};

} // namespace Traits
