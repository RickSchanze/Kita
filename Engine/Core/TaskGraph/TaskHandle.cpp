//
// Created by kita on 25-8-17.
//

#include "TaskHandle.h"

#include "TaskGraph.h"

void TaskHandle::StartLazy() const {
  TaskGraph::GetRef().StartLazyTask(mInstance);
}

ETaskState TaskHandle::GetState(bool Lock) const {
  if (mInstance == nullptr) {
    return ETaskState::Count;
  }
  if (!TaskGraph::GetRef().IsTaskExists(mInstance)) {
    return ETaskState::Finished;
  }
  return mInstance->GetState(Lock);
}

bool TaskHandle::IsValid() const { return mInstance != nullptr; }

void TaskHandle::WaitSync() const {
  TaskGraph::GetRef().WaitTaskSync(mInstance);
}
