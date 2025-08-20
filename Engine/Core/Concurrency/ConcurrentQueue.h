#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class ConcurrentQueue {
public:
  /// 入队
  void Enqueue(const T& value) {
    {
      std::lock_guard<std::mutex> lock(Mutex);
      Queue.push(value);
    }
    Cv.notify_one();
  }

  void Enqueue(T&& value) {
    {
      std::lock_guard<std::mutex> lock(Mutex);
      Queue.push(std::move(value));
    }
    Cv.notify_one();
  }

  /// 阻塞出队（如果队列为空会一直等待）
  T Dequeue() {
    std::unique_lock<std::mutex> lock(Mutex);
    Cv.wait(lock, [this] { return !Queue.empty(); });
    T value = std::move(Queue.front());
    Queue.pop();
    return value;
  }

  /// 尝试出队（非阻塞）
  bool TryDequeue(T& out) {
    std::lock_guard<std::mutex> lock(Mutex);
    if (Queue.empty())
      return false;
    out = std::move(Queue.front());
    Queue.pop();
    return true;
  }

  /// 阻塞直到取到一个元素，并返回 true
  void WaitDequeued(T& out) {
    std::unique_lock<std::mutex> lock(Mutex);
    Cv.wait(lock, [this] { return !Queue.empty(); });
    out = std::move(Queue.front());
    Queue.pop();
  }

  /// 获取当前元素数量（线程安全）
  size_t Count() const {
    std::lock_guard<std::mutex> lock(Mutex);
    return Queue.size();
  }

private:
  mutable std::mutex Mutex;
  std::condition_variable Cv;
  std::queue<T> Queue;
};