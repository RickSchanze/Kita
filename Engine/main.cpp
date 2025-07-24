#include <iostream>

#include "Core/Logging/Logger.hpp"

int main() {
  LOG_INFO("{}", 12);
  LOG_WARN("{}", 12);
  LOG_ERROR("{}", 12);
  LOG_CRITICAL("{}", 12);
  system("pause");
  return 0;
}
