//
// Created by kita on 25-8-17.
//

#include "TaskGraph.h"

#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"
#include "ThreadUtils.h"

ETaskState TaskInstance::GetState(const bool InLock) const {
  if (!InLock) {
    return State;
  }
  std::lock_guard Lock(Mutex);
  return State;
}

void TaskGraph::StartUp() {
  ThreadUtils::SetCurrentThreadMain();
  gLogger.Info("TaskGraph", "启动Game线程于{}.", ThreadUtils::GetCurrentThreadId());
  auto& Self = GetRef();
  Self.mRenderExecutor.StartUp("Render");
  gLogger.Info("TaskGraph", "启动Render线程于{}.", Self.mRenderExecutor.GetThreadId());
  SizeType IOPoolThreadCount = 4;
  Self.mIOExecutor.StartUp("IO", 4);
  gLogger.Info("TaskGraph", "启动IO线程池于{}个线程.", IOPoolThreadCount);
}

void TaskGraph::ShutDown() {
  auto& Self = GetRef();
  Self.mRenderExecutor.ShutDown();
  gLogger.Info("TaskGraph", "Render线程已关闭.");
  Self.mIOExecutor.ShutDown();
  gLogger.Info("TaskGraph", "IO线程池已关闭.");
}

void TaskGraph::NotifyTaskCompleted(SharedPtr<TaskInstance>& InInstance) {
  ASSERT_MSG(InInstance != nullptr, "TaskGraph::NotifyTaskCompleted called with nullptr");
  ASSERT_MSG(mInstances.Contains(InInstance), "TaskGraph::NotifyTaskCompleted called with invalid instance(instance not be managed). Name=\"{}\", Ptr={}", InInstance->DebugName, Ptr(InInstance));
  Array<SharedPtr<TaskInstance>> SubsequenceCopy;
  {
    AutoLock Lock(InInstance->Mutex);
    SubsequenceCopy = InInstance->Subsequence;
  }
  for (auto& DependentInstance : SubsequenceCopy) {
    if (DependentInstance == nullptr) {
      continue;
    }
    if (const int Remaining = --DependentInstance->RemainingDependencies; Remaining == 0) {
      // 依赖此时应该要么Lazy 要么Pending
      if (DependentInstance->GetState() == ETaskState::Lazy) {
        // Lazy则不管它 因为前面已经减少其依赖counter了
      } else if (DependentInstance->GetState() == ETaskState::Pending) {
        // Pending则加入其DesiredThread进行执行
        ScheduleTask(DependentInstance);
      } else {
        gLogger.Error("TaskGraph", "任务\"{}\"状态错误, ptr={}.", InInstance->DebugName, Ptr(InInstance));
      }
    }
  }
  InInstance->SetState(ETaskState::Finished);
  mInstances.Remove(InInstance);
}

void TaskGraph::NotifyTaskFailed(SharedPtr<TaskInstance>& InInstance) const {
  ASSERT_MSG(InInstance != nullptr, "TaskGraph::NotifyTaskCompleted called with nullptr");
  ASSERT_MSG(mInstances.Contains(InInstance), "TaskGraph::NotifyTaskCompleted called with invalid instance(instance not be managed). Name=\"{}\", Ptr={}", InInstance->DebugName, Ptr(InInstance));
  // TODO: 失败的情况有点复杂，需要去掉当前图中所有与InInstance直接或间接相关的任务, 以后再写
  std::unreachable();
}

bool TaskGraph::IsTaskExists(const SharedPtr<TaskInstance>& InInstance) const { return mInstances.Contains(InInstance); }

struct TaskInstanceLock {
  const SharedPtr<TaskInstance> Instance;
  explicit TaskInstanceLock(const SharedPtr<TaskInstance>& InInstance) : Instance(InInstance) { Instance->Lock(); }
  ~TaskInstanceLock() { Instance->Unlock(); }
};

void TaskGraph::StartLazyTask(SharedPtr<TaskInstance>& InInstance) {
  if (InInstance == nullptr) {
    return;
  }
  if UNLIKELY (!IsTaskExists(InInstance)) {
    gLogger.Error("TaskGraph", "任务\"{}\"({})不存在", InInstance->DebugName, Ptr(InInstance));
    return;
  }
  if (InInstance->GetState() != ETaskState::Lazy) {
    gLogger.Error("TaskGraph", "任务\"{}\"({})状态错误(不为Lazy), 不能启动", InInstance->DebugName, Ptr(InInstance));
    return;
  }
  TaskInstanceLock Lock(InInstance);
  InInstance->State = ETaskState::Pending;
  if (InInstance->RemainingDependencies == 0) {
    ScheduleTask(InInstance);
  }
}

void TaskGraph::WaitTaskSync(SharedPtr<TaskInstance>& InInstance) const {
  if UNLIKELY (!InInstance) {
    return;
  }
  if (!IsTaskExists(InInstance)) {
    // Task已完成
    return;
  }
  std::unique_lock Lock(InInstance->Mutex);
  InInstance->CV.wait(Lock, [InInstance] { return InInstance->GetState(false) == ETaskState::Finished; });
}

void TaskGraph::ScheduleTask(const SharedPtr<TaskInstance>& InInstance) {
  switch (InInstance->Node->GetDesiredThread()) {
  case ENamedThread::Render:
    mRenderExecutor.Enqueue(InInstance);
    break;
  case ENamedThread::IO:
    mIOExecutor.Enqueue(InInstance);
    break;
  case ENamedThread::Game:
    mGameThreadTasks.Enqueue(InInstance);
    break;
  }
}