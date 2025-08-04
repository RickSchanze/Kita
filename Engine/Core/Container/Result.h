#pragma once

#include <expected>

enum class EReflectionError;
template <typename T, typename E> struct Result {
  Result(const T& value) : mValue(std::move(value)) {}
  Result(const E& error) : mValue(std::unexpected(std::move(error))) {}

  E Error() const { return mValue.error(); }
  bool HasError() const { return mValue.has_error(); }
  operator bool() const { return !HasError(); }
  const T& Value() const { return mValue.value(); }
  T& Value() { return mValue.value(); }

  template <typename F> auto Then(this auto&& Self, F&& Func) { return Self.mValue.and_then(std::forward<F>(Func)); }
  template <typename F> auto Else(this auto&& Self, F&& Func) { return Self.mValue.or_else(std::forward<F>(Func)); }
  template <typename F> auto MapError(this auto&& Self, F&& Func) { return Self.mValue.transform_error(std::forward<F>(Func)); }

private:
  std::expected<T, E> mValue;
};

struct Void {};

template <typename E> struct Result<void, E> {
  Result() : mValue() {}
  Result(const E& error) : mValue(std::unexpected(std::move(error))) {}

  E Error() const { return mValue.error(); }
  bool HasError() const { return mValue.has_error(); }
  operator bool() const { return !HasError(); }
  const T& Value() const { return mValue.value(); }
  T& Value() { return mValue.value(); }

  template <typename F> auto Then(this auto&& Self, F&& Func) { return Self.mValue.and_then(std::forward<F>(Func)); }
  template <typename F> auto Else(this auto&& Self, F&& Func) { return Self.mValue.or_else(std::forward<F>(Func)); }
  template <typename F> auto MapError(this auto&& Self, F&& Func) { return Self.mValue.transform_error(std::forward<F>(Func)); }

private:
  std::expected<void, E> mValue;
};
