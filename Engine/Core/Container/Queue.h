#pragma once
#include "Core/Memory/Malloc.h"
#include "Core/TypeDefs.h"

#include <queue>
#include <type_traits>
#include <utility>

template <typename T, EMemoryLabel Label = EMemoryLabel::Default>
struct Queue {
  Queue() = default;
  ~Queue() = default;

  // 只有当 T 可拷贝时才允许 Queue 拷贝
  Queue(const Queue&) requires std::is_copy_constructible_v<T> = default;
  Queue& operator=(const Queue&) requires std::is_copy_assignable_v<T> = default;

  // 只有当 T 不可拷贝时才显式删除拷贝
  Queue(const Queue&) requires (!std::is_copy_constructible_v<T>) = delete;
  Queue& operator=(const Queue&) requires (!std::is_copy_assignable_v<T>) = delete;

  // 移动构造和移动赋值总是允许
  Queue(Queue&&) noexcept = default;
  Queue& operator=(Queue&&) noexcept = default;

  // 入队（完美转发，支持移动和拷贝）
  template <typename U>
  void Enqueue(U&& value) {
    mQueue.push(std::forward<U>(value));
  }

  // 出队（移动返回，兼容不可拷贝类型）
  T Dequeue() {
    T value = std::move(mQueue.front());
    mQueue.pop();
    return value;
  }

  bool Empty() const noexcept { return mQueue.empty(); }

  // 出队到外部变量（移动赋值）
  bool TryDequeue(T& value) {
    if (mQueue.empty()) {
      return false;
    }
    value = std::move(mQueue.front());
    mQueue.pop();
    return true;
  }

  SizeType Count() const noexcept { return mQueue.size(); }

  auto&& Front(this auto&& self) { return self.mQueue.front(); }
  auto&& Back(this auto&& self) { return self.mQueue.back(); }

private:
  std::queue<T> mQueue;
};