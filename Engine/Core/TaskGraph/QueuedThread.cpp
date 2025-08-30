//
// Created by kita on 25-8-20.
//

#include "QueuedThread.h"

#include "TaskGraph.h"

#include <Windows.h>

void QueuedThread::StartUp(const StringView Name) {
  mThread = std::thread(&QueuedThread::ThreadLoop, this);
  SetThreadName(Name);
}

void QueuedThread::ShutDown() { Stop(); }

void QueuedThread::Stop() {
  if (!mStop.exchange(true)) {
    mStop = true;

    // 推入一个空任务，确保 WaitDequeued 被唤醒
    mQueue.Enqueue(nullptr);

    if (mThread.joinable()) {
      mThread.join();
    }
  }
}

UInt64 QueuedThread::GetThreadId() { return reinterpret_cast<UInt64>(mThread.native_handle()); }

void QueuedThread::SetThreadName(StringView Name) {
#if defined(_WIN32)
  // Windows 10 1607+
  using SetThreadDescriptionFunc = HRESULT(WINAPI*)(HANDLE, PCWSTR);
  if (const auto SetThreadDescription = reinterpret_cast<SetThreadDescriptionFunc>(GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "SetThreadDescription"))) {
    const std::wstring WName(Name.begin(), Name.end());
    const auto Id = static_cast<HANDLE>(mThread.native_handle());
    SetThreadDescription(Id, WName.c_str());
  }
#elif defined(__APPLE__)
  pthread_setname_np(name.c_str());
#else // Linux
  pthread_setname_np(pthread_self(), name.c_str());
#endif
}

void QueuedThread::ThreadLoop() {
  while (!mStop) {
    SharedPtr<TaskInstance> Task = nullptr;
    mQueue.WaitDequeued(Task); // 会阻塞直到有任务
    if (!Task) {
      // nullptr 用于唤醒并退出
      continue;
    }
    if (TaskNode* Node = Task->Node; Node != nullptr) {
      if (const ETaskNodeResult Result = Node->Run(); Result == ETaskNodeResult::Success) {
        TaskGraph::GetRef().NotifyTaskCompleted(Task);
      } else if (Result == ETaskNodeResult::RescheduleRequired) {
        mQueue.Enqueue(Task);
      } else if (Result == ETaskNodeResult::Failed) {
        TaskGraph::GetRef().NotifyTaskFailed(Task);
      }
    }
  }
}