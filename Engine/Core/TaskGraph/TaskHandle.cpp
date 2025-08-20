//
// Created by kita on 25-8-17.
//

#include "TaskHandle.h"

#include "TaskGraph.h"

void TaskHandle::Start() {}

ETaskState TaskHandle::GetState() const {
  if (mInstance == nullptr) {
    return ETaskState::Count;
  }
  return mInstance->GetState();
}