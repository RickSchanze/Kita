#pragma once
#include <cstdint>
#include <utility>

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
