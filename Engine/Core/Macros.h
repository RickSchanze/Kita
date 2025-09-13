#pragma once

#if DEBUG_MODE
#define KITA_DEBUG 1
#else
#define KITA_DEBUG 0
#endif

#if KITA_DEBUG
#define FORCE_INLINE inline

// 定义宏，使枚举类支持位运算
#define ENABLE_BITMASK_OPERATORS(Enum)                                                                                                                                               \
  inline constexpr Enum operator|(Enum lhs, Enum rhs) { return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(lhs) | static_cast<std::underlying_type_t<Enum>>(rhs)); } \
  inline constexpr Enum operator&(Enum lhs, Enum rhs) { return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(lhs) & static_cast<std::underlying_type_t<Enum>>(rhs)); } \
  inline constexpr Enum operator^(Enum lhs, Enum rhs) { return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(lhs) ^ static_cast<std::underlying_type_t<Enum>>(rhs)); } \
  inline constexpr Enum operator~(Enum e) { return static_cast<Enum>(~static_cast<std::underlying_type_t<Enum>>(e)); }                                                               \
  inline constexpr Enum& operator|=(Enum& lhs, Enum rhs) { return lhs = lhs | rhs; }                                                                                                 \
  inline constexpr Enum& operator&=(Enum& lhs, Enum rhs) { return lhs = lhs & rhs; }                                                                                                 \
  inline constexpr Enum& operator^=(Enum& lhs, Enum rhs) { return lhs = lhs ^ rhs; }
#include <type_traits>

template <typename T>
  requires std::is_enum_v<T>
bool True(T Flag) {
  return std::underlying_type_t<T>(Flag) != 0;
}

template <typename T>
  requires std::is_enum_v<T>
bool False(T Flag) {
  return std::underlying_type_t<T>(Flag) == 0;
}

#ifdef _MSC_VER
#define DEBUG_BREAK()         \
  if (CheckDebuggerPresent()) \
  __debugbreak()
#endif
#else
#define FORCE_INLINE __forceinline
#define DEBUG_BREAK()
#endif

#define INLINE_NODISCARD [[nodiscard]] FORCE_INLINE
#define LIKELY(x) (x) [[likely]]
#define UNLIKELY(x) (x) [[unlikely]]

#ifdef KITA_EDITOR
#define KITA_EDITOR 1
#endif

bool CheckDebuggerPresent();