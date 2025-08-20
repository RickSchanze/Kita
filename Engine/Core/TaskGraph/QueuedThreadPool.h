#pragma once
#include "Core/Concurrency/ConcurrentQueue.h"
#include "Core/Container/Array.h"

struct TaskInstance;
class QueuedThreadPool : public NonCopyMovable {
public:
  explicit QueuedThreadPool() : mStop(false) {}
  ~QueuedThreadPool() = default;

  void StartUp(const String& Name, SizeType ThreadCount);
  void ShutDown();

  // 添加任务
  void Enqueue(TaskInstance* task) {
    mQueue.Enqueue(task);
  }

  void Stop();

private:
  void ThreadLoop(const String& Name, SizeType index);

private:
  ConcurrentQueue<TaskInstance*> mQueue;
  Array<std::thread> mWorkers;
  std::atomic<bool> mStop;
};
