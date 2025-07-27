#pragma once
#include <optional>

template <typename T> class Optional {
public:
  Optional() = default;
  Optional(std::nullopt_t) : mData(std::nullopt) {}
  Optional(const T& InValue) : mData(InValue) {}
  Optional(T&& InValue) : mData(std::move(InValue)) {}
  Optional(const Optional&) = default;
  Optional(Optional&&) = default;
  Optional& operator=(const Optional&) = default;
  Optional& operator=(Optional&&) = default;

  [[nodiscard]] bool HasValue() const { return mData.has_value(); }
  [[nodiscard]] T& Value() { return mData.value(); }
  [[nodiscard]] const T& Value() const { return mData.value(); }
  [[nodiscard]] T ValueOr(T DefaultValue) const { return mData.value_or(DefaultValue); }

  T& operator*() { return *mData; }
  const T& operator*() const { return *mData; }
  T* operator->() { return mData.operator->(); }
  const T* operator->() const { return mData.operator->(); }

private:
  std::optional<T> mData;
};

constexpr auto NullOpt = std::nullopt;

template <typename T> [[nodiscard]] Optional<Traits::Pure<T>> MakeOptional(T&& InValue) { return Optional<Traits::Pure<T>>(std::forward<T>(InValue)); }
