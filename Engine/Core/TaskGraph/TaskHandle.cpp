//
// Created by kita on 25-8-17.
//

#include "TaskHandle.h"

#include "TaskGraph.h"

void TaskHandle::StartLazy() { TaskGraph::GetRef().StartLazyTask(mInstance); }

ETaskState TaskHandle::GetState(bool Lock) const {
  if (mInstance == nullptr) {
    return ETaskState::Count;
  }
  if UNLIKELY (!TaskGraph::GetRef().IsTaskExists(mInstance)) {
    return ETaskState::Finished;
  }
  return mInstance->GetState(Lock);
}

bool TaskHandle::IsValid() const { return mInstance != nullptr; }

void TaskHandle::WaitSync() { TaskGraph::GetRef().WaitTaskSync(mInstance); }
