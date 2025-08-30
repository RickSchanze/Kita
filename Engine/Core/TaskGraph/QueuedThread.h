#pragma once
#include "Core/Concurrency/ConcurrentQueue.h"
#include "Core/Memory/SharedPtr.h"
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
  void Enqueue(const SharedPtr<TaskInstance>& Task) { mQueue.Enqueue(Task); }

  /// 停止工作线程
  void Stop();

  UInt64 GetThreadId();

  void SetThreadName(StringView Name);

private:
  void ThreadLoop();

private:
  ConcurrentQueue<SharedPtr<TaskInstance>> mQueue;
  std::thread mThread;
  std::atomic<bool> mStop;
};