#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Trace.h"

#include "Core/Memory/SharedPtr.h"
#include "Core/Memory/UniquePtr.h"
#include "malloc.h"

int main() {
  UniquePtr<int> a = MakeUnique<int>(12);
  LOG_INFO("Hello World! {}", *a);
  SharedPtr<int> b = MakeShared<int>(13);
  LOG_INFO("Hello World! {}", *b);
  return 0;
}
