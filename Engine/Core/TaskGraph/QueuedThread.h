#pragma once
#include "Core/Concurrency/ConcurrentQueue.h"
#include "Core/String/StringView.h"
#include "Core/TypeDefs.h"

#include <atomic>
#include <thread>

struct TaskInstance;
class QueuedThread : NonCopyMovable {
public:
  QueuedThread() : mStop(false) {}

  ~QueuedThread() { Stop(); }

  void StartUp(StringView Name);
  void ShutDown();

  /// 添加任务（不阻塞）
  void Enqueue(TaskInstance* task) { mQueue.Enqueue(task); }

  /// 停止工作线程
  void Stop() {
    mStop = true;

    // 推入一个空任务，确保 WaitDequeued 被唤醒
    mQueue.Enqueue(nullptr);

    if (mThread.joinable()) {
      mThread.join();
    }
  }

  UInt64 GetThreadId();

  void SetThreadName(StringView Name);

private:
  void ThreadLoop();

private:
  ConcurrentQueue<TaskInstance*> mQueue;
  std::thread mThread;
  std::atomic<bool> mStop;
};