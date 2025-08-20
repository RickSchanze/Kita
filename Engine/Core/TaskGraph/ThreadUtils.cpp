//
// Created by kita on 25-8-20.
//

#include "ThreadUtils.h"

#include <sstream>
#include <thread>

UInt64 ThreadUtils::GetCurrentThreadId() {
  std::ostringstream oss;
  oss << std::this_thread::get_id();
  return std::stoull(oss.str());
}

void ThreadUtils::SetCurrentThreadMain() { mMainThreadId = ThreadUtils::GetCurrentThreadId(); }

UInt64 ThreadUtils::GetMainThreadId() { return mMainThreadId; }

bool ThreadUtils::IsCurrentMainThread() { return mMainThreadId == ThreadUtils::GetCurrentThreadId(); }
