#pragma once
#include <functional>

template <typename Func> class Function {
public:
  template <typename... Args> auto operator()(Args&&... args) { return mFunc(std::forward<Args>(args)...); }
  template <typename OtherFunc> Function(OtherFunc&& InFunc) : mFunc(std::forward<OtherFunc>(InFunc)) {}
  Function() = default;
  Function(const Function&) = default;
  Function(Function&&) = default;
  Function& operator=(const Function&) = default;
  Function& operator=(Function&&) = default;
  ~Function() = default;

  [[nodiscard]] bool HoldFunc() const { return mFunc != nullptr; }
  void Reset() { mFunc = nullptr; }
  operator bool() const { return mFunc != nullptr; }

private:
  std::function<Func> mFunc;
};