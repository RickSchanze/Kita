//
// Created by kita on 25-8-17.
//

#include "TaskGraph.h"

void TaskGraph::StartUp() {
  auto& Self = GetRef();
  Self.mRenderExecutor.StartUp("Render");
  Self.mIOExecutor.StartUp("IO");
}

void TaskGraph::ShutDown() {
  auto& Self = GetRef();
  Self.mRenderExecutor.ShutDown();
  Self.mIOExecutor.ShutDown();
}

void TaskGraph::NotifyTaskCompleted(TaskInstance* InInstance) {
  ASSERT_MSG(InInstance != nullptr, "TaskGraph::NotifyTaskCompleted called with nullptr");
  ASSERT_MSG(mInstances.Contains(InInstance), "TaskGraph::NotifyTaskCompleted called with invalid instance(instance not be managed). Name=\"{}\", Ptr={:p}", InInstance->DebugName, InInstance);
  for (auto& DependentInstance : InInstance->Subsequence) {
    if (DependentInstance == nullptr) {
      continue;
    }
    if (const int Remaining = --DependentInstance->RemainingDependencies; Remaining == 0) {
      // 依赖此时应该要么Lazy 要么Pending
      if (DependentInstance->GetState() == ETaskState::Lazy) {
        // Lazy则不管它
      } else if (DependentInstance->GetState() == ETaskState::Pending) {
        // Pending则加入其DesiredThread进行执行
        ScheduleTask(InInstance);
      } else {
        LOG_ERROR_TAG("TaskGraph", "任务\"{}\"状态错误, ptr={:p}.", InInstance->DebugName, InInstance);
      }
    }
  }
  Delete(InInstance);
  mInstances.Remove(InInstance);
}

void TaskGraph::ScheduleTask(TaskInstance* InInstance) {}