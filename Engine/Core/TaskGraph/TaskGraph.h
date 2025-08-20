#pragma once
#include "Core/Concurrency/ConcurrentSet.h"
#include "Core/Container/Array.h"
#include "Core/Macros.h"
#include "Core/Memory/SharedPtr.h"
#include "Core/Singleton/Singleton.h"
#include "Core/TypeDefs.h"
#include "QueuedThread.h"
#include "QueuedThreadPool.h"
#include "TaskHandle.h"
#include "TaskNode.h"

struct TaskInstance {
  mutable std::mutex Mutex;
  std::condition_variable CV;

  TaskNode* Node = nullptr;
  ETaskState State = ETaskState::Finished; // DummyTaskInstance认为是Finished状态
  Array<TaskInstance*> Subsequence;
  std::atomic<int> RemainingDependencies = 0;

  String DebugName;

  void SetState(const ETaskState NewState) {
    {
      std::lock_guard Lock(Mutex);
      State = NewState;
    }
    CV.notify_all();
  }

  ~TaskInstance() { Delete(Node); }

  void Lock() const { Mutex.lock(); }
  void Unlock() const { Mutex.unlock(); }

  [[nodiscard]] ETaskState GetState(const bool InLock = true) const {
    if (!InLock) {
      return State;
    }
    std::lock_guard Lock(Mutex);
    return State;
  }
};

struct TaskHandleListAutoLock {
  std::initializer_list<TaskHandle> TaskHandles;
  TaskHandleListAutoLock(const std::initializer_list<TaskHandle> TaskHandles) : TaskHandles(TaskHandles) {
    for (const auto& Handle : TaskHandles) {
      Handle.GetInstance()->Lock();
    }
  }
  ~TaskHandleListAutoLock() {
    for (const auto& Handle : TaskHandles) {
      Handle.GetInstance()->Unlock();
    }
  }
};

class TaskGraph : public Singleton<TaskGraph> {
public:
  static void StartUp();
  static void ShutDown();

  /// 创建一个延迟启动的任务, 需要调用TaskHandle::Start()才能进入Pending状态等待所有依赖完成
  /// @tparam T 任务类型
  /// @tparam Args 任务构造参数类型
  /// @param DebugName 任务名称
  /// @param InDeps 任务依赖
  /// @param InArgs 任务构造参数
  /// @return 任务句柄
  template <typename T, typename... Args>
    requires(Traits::IsConstructible<T, Args...> && Traits::IsBaseOf<TaskNode, T>)
  TaskHandle CreateLazyTask(const StringView DebugName, const std::initializer_list<TaskHandle> InDeps, Args&&... InArgs) {
    TaskHandleListAutoLock Lock(InDeps);
    Int32 RemainingDependencies = 0;
    for (const auto& Dep : InDeps) {
      if (Dep.GetState(false) != ETaskState::Finished) {
        RemainingDependencies++;
      }
    }
    T* NewTask = New<T>(std::forward<Args>(InArgs)...);
    TaskInstance* NewInstance = New<TaskInstance>();
    NewInstance->State = ETaskState::Lazy;
    NewInstance->RemainingDependencies = RemainingDependencies;
    NewInstance->Node = NewTask;
    NewInstance->DebugName = String(DebugName);
    for (const auto& Dep : InDeps) {
      Dep.GetInstance()->Subsequence.Add(NewInstance);
    }
    mInstances.Add(NewInstance);
    return TaskHandle{NewInstance};
  }

  /// 创建一个立即启动的任务
  /// @tparam T 任务类型
  /// @tparam Args 任务构造参数类型
  /// @param DebugName 任务名称
  /// @param InDeps 任务依赖
  /// @param InArgs 任务构造参数
  /// @return 任务句柄
  template <typename T, typename... Args>
    requires(Traits::IsConstructible<T, Args...> && Traits::IsBaseOf<TaskNode, T>)
  TaskHandle CreateTask(const StringView DebugName, std::initializer_list<TaskHandle> InDeps, Args&&... InArgs) {
    const TaskHandle Handle = CreateLazyTask<T>(DebugName, InDeps, std::forward<Args>(InArgs)...);
    Handle.StartLazy();
    return Handle;
  }

  /// 当一个任务完成时调用此函数, 用于通知依赖
  /// @param InInstance 完成的任务实例
  void NotifyTaskCompleted(TaskInstance* InInstance);

  /// 当一个任务失败时调用此函数,
  /// 此时其后继节点会递归的被Delete 并且不被执行
  void NotifyTaskFailed(TaskInstance* InInstance) const;

  [[nodiscard]] bool IsTaskExists(TaskInstance* InInstance) const;

  void StartLazyTask(TaskInstance* InInstance);

  /// 等待任务完成
  void WaitTaskSync(TaskInstance* InInstance) const;

private:
  void ScheduleTask(TaskInstance* InInstance);
  // 管理所有的TaskInstance
  ConcurrentSet<TaskInstance*> mInstances;
  QueuedThread mRenderExecutor;
  QueuedThreadPool mIOExecutor;
  /// 主线程每帧执行的任务
  ConcurrentQueue<TaskInstance*> mGameThreadTasks;
};
