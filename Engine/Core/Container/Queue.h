#pragma once

#include "Core/Memory/Malloc.h"

#include <queue>

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> struct Queue {
  Queue() = default;
  ~Queue() = default;

  // 入队
  void Enqueue(T Value) { mQueue.push(Value); }

  // 出队（不检查为空，直接取值然后移除）
  T Dequeue() {
    T Value = mQueue.front();
    mQueue.pop();
    return Value;
  }

  bool Empty() { return mQueue.empty(); }

  bool TryDequeue(T& Value) {
    if (mQueue.empty()) {
      return false;
    }
    Value = mQueue.front();
    mQueue.pop();
    return true;
  }

  SizeType Count() { return mQueue.size(); }

  // 获取队首元素
  auto&& Front(this auto&& Self) { return Self.mQueue.front(); }

  // 获取队尾元素
  auto&& Back(this auto&& Self) { return Self.mQueue.back(); }

private:
  std::queue<T> mQueue;
};
