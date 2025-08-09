#pragma once
#include "Core/Memory/Malloc.h"

#include <stack>

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> struct Stack {
public:
  Stack() = default;
  ~Stack() = default;

  void Push(T Value) { mStack.push(Value); }
  T Pop() {
    T Value = mStack.top();
    mStack.pop();
    return Value;
  }

  bool Empty() { return mStack.empty(); }

  bool TryPop(T& Value) {
    if (mStack.empty()) {
      return false;
    }
    Value = mStack.top();
    mStack.pop();
    return true;
  }

  SizeType Count() { return mStack.size(); }

  auto&& Top(this auto&& Self) { return Self.mStack.top(); }

private:
  std::stack<T> mStack;
};