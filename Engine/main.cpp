#include "Core/Container/Array.h"
#include "Core/Event/Callback.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Memory/UniquePtr.h"

Callback<void, int> A;

struct TestB {
  static String ToString() {
    return "B";
  }
};

int main() {
  UniquePtr<int> a = MakeUnique<int>(1);
  Array Test{1,2,3};
  auto& m = Test[1];
  m = 12;
  LOG_INFO_TAG("Test", "{}", Test);
}
