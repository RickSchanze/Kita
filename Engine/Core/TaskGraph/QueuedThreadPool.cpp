//
// Created by kita on 25-8-20.
//

#include "QueuedThreadPool.h"

#include "TaskGraph.h"

#include <Windows.h>

void QueuedThreadPool::StartUp(const String& Name, const SizeType ThreadCount) {
  mWorkers.Resize(ThreadCount);
  for (SizeType i = 0; i < ThreadCount; ++i) {
    mWorkers[i] = std::thread(&QueuedThreadPool::ThreadLoop, this, Name, i);
  }
}

void QueuedThreadPool::ShutDown() { Stop(); }

void QueuedThreadPool::Stop() {
  if (!mStop.exchange(true)) {
    // 推送空任务唤醒所有线程
    for (size_t i = 0; i < mWorkers.Count(); ++i) {
      mQueue.Enqueue(nullptr);
    }
    for (auto& t : mWorkers) {
      if (t.joinable()) {
        t.join();
      }
    }
    mWorkers.Clear();
  }
}

// 线程命名
static void SetCurrentThreadName(const String& name) {
#if defined(_WIN32)
  using SetThreadDescriptionFunc = HRESULT(WINAPI*)(HANDLE, PCWSTR);
  auto setThreadDescription = reinterpret_cast<SetThreadDescriptionFunc>(GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "SetThreadDescription"));
  if (setThreadDescription) {
    std::wstring wname(name.begin(), name.end());
    setThreadDescription(GetCurrentThread(), wname.c_str());
  }
#elif defined(__APPLE__)
  pthread_setname_np(name.c_str());
#else // Linux
  pthread_setname_np(pthread_self(), name.c_str());
#endif
}

void QueuedThreadPool::ThreadLoop(const String& Name, SizeType Index) {
  // 命名线程，例如 "A0", "A1"
  SetCurrentThreadName(Name + ::ToString(Index));

  while (!mStop) {
    SharedPtr<TaskInstance> Task = nullptr;
    mQueue.WaitDequeued(Task);
    if (!Task) {
      continue; // nullptr 任务用于唤醒退出
    }
    TaskNode* Node = Task->Node;
    if (Node == nullptr) {
      continue;
    }
    if (const ETaskNodeResult Result = Node->Run(); Result == ETaskNodeResult::Success) {
      TaskGraph::GetRef().NotifyTaskCompleted(Task);
    } else if (Result == ETaskNodeResult::RescheduleRequired) {
      mQueue.Enqueue(Task);
    }
  }
}
