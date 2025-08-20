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

  [[nodiscard]] ETaskState GetState() const {
    std::lock_guard Lock(Mutex);
    return State;
  }
};

class TaskGraph : public NonCopyMovable, public Singleton<TaskGraph> {
public:
  static void StartUp();
  static void ShutDown();
  /// 创建一个延迟启动的任务, 需要调用TaskHandle::Start()才能进入Pending状态等待所有依赖完成
  /// @tparam T 任务类型
  /// @tparam Args 任务构造参数类型
  /// @param InDeps 任务依赖
  /// @param InArgs 任务构造参数
  /// @return 任务句柄
  template <typename T, typename... Args>
    requires(Traits::IsConstructible<T, Args...> && Traits::IsBaseOf<TaskNode, T>)
  TaskHandle CreateLazyTask(std::initializer_list<TaskHandle> InDeps, Args&&... InArgs);

  /// 创建一个立即启动的任务
  /// @tparam T 任务类型
  /// @tparam Args 任务构造参数类型
  /// @param InDeps 任务依赖
  /// @param InArgs 任务构造参数
  /// @return 任务句柄
  template <typename T, typename... Args>
    requires(Traits::IsConstructible<T, Args...> && Traits::IsBaseOf<TaskNode, T>)
  TaskHandle CreateTask(std::initializer_list<TaskHandle> InDeps, Args&&... InArgs) {
    const TaskHandle Handle = CreateLazyTask(InDeps, std::forward<Args>(InArgs)...);
    return Handle;
  }

  /// 当一个任务完成时调用此函数, 用于通知依赖
  /// @param InInstance 完成的任务实例
  void NotifyTaskCompleted(TaskInstance* InInstance);

  void ScheduleTask(TaskInstance* InInstance);

private:
  // 管理所有的TaskInstance
  ConcurrentSet<TaskInstance*> mInstances;
  QueuedThread mRenderExecutor;
  QueuedThreadPool mIOExecutor;
};
