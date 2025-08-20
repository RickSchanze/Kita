#pragma once
#include "Core/TypeDefs.h"

#include <chrono>
#include <thread>

class ThreadUtils 
{
public:
  static UInt64 GetCurrentThreadId();
  static void SetCurrentThreadMain();
  static UInt64 GetMainThreadId();
  static bool IsCurrentMainThread();

  template <typename Rep, typename Period>
  static void WaitFor(std::chrono::duration<Rep, Period> InTimeout) {
    std::this_thread::sleep_for(InTimeout);
  }

private:
  static inline UInt64 mMainThreadId = 0;
};
