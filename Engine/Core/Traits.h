#pragma once
#include "String/StringView.h"

#include <algorithm>
#include <source_location>
#include <type_traits>

namespace Traits {

namespace Pri {

template <typename T, typename... Args> struct AnyOf : std::bool_constant<(std::is_same_v<T, Args> || ...)> {};

} // namespace Pri

template <typename T, typename... Args>
concept AnyOf = Pri::AnyOf<T, Args...>::value;

template <typename T1, typename T2>
concept SameAs = std::is_same_v<T1, T2>;

template <typename T> using Pure = std::remove_cvref_t<T>;

template <typename T>
concept IsEnum = std::is_enum_v<T>;

template <typename T>
concept IsFloatNumber = std::is_floating_point_v<T>;

template <typename Parent, typename Child>
concept IsBaseOf = std::is_base_of_v<Parent, Child>;

template <typename Child, typename Parent>
concept IsDerivedFrom = std::is_base_of_v<Parent, Child>;

template <typename T>
concept IsTrivial = std::is_trivial_v<T>;

template <typename T, typename... Args>
concept IsConstructible = std::is_constructible_v<T, Args...>;

// 一些通用性概念接口
template <typename T>
concept IEquatable = requires(const T& a, const T& b) {
  { a == b } -> std::convertible_to<bool>;
};

} // namespace Traits
