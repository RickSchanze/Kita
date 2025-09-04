#pragma once
#include <cstdint>
#include <utility>
#include <mutex>

using Int8 = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;
using Float32 = float;
using Float64 = double;
using SizeType = std::size_t;

constexpr SizeType INVALID_INDEX = static_cast<SizeType>(-1);
constexpr SizeType INVALID_SIZE = static_cast<SizeType>(-1);

struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

struct NonMovable {
  NonMovable() = default;
  NonMovable(NonMovable&&) = delete;
  NonMovable& operator=(NonMovable&&) = delete;
};

struct NonCopyMovable : NonCopyable, NonMovable {};

template <typename T>
concept IGetHashCode = requires(T Value) {
  { Value.GetHashCode() } -> std::same_as<SizeType>;
};

template <typename T>
concept IStdHash = requires(T Value) {
  { std::hash<T>{}(Value) } -> std::same_as<SizeType>;
};

template <typename T>
concept IHashable = IGetHashCode<T> || IStdHash<T>;

template <IHashable T> SizeType GetHashCode(const T& Value) {
  if constexpr (IGetHashCode<T>) {
    return Value.GetHashCode();
  } else {
    return std::hash<T>{}(Value);
  }
}

template <IGetHashCode T> struct std::hash<T> {
  SizeType operator()(const T& Value) const noexcept { return Value.GetHashCode(); }
};

template <typename ClassType, typename MemberType> constexpr SizeType OffsetOf(MemberType ClassType::* member) {
  return reinterpret_cast<size_t>(&(reinterpret_cast<ClassType const volatile*>(0)->*member));
}

template <typename ...Args>
using AutoLock = std::scoped_lock<Args...>;
