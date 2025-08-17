#pragma once
#include "NamedThreads.h"

enum class ETaskNodeResult {
  Success,
  RescheduleRequired,
  Failed,
};

class TaskNode {
public:
  virtual ~TaskNode() = default;

  virtual ETaskNodeResult Run() = 0;

  [[nodiscard]] virtual ENamedThread GetDesiredThread() const { return ENamedThread::Game; }
};
