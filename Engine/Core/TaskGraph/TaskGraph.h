#pragma once
#include "Core/Singleton/Singleton.h"
#include "Core/TypeDefs.h"
#include "TaskHandle.h"
#include "TaskNode.h"

class TaskGraph : public NonCopyable, public NonMovable, public Singleton<TaskGraph> {
public:
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

  /// 同步等待一个Task完成
  /// @param InHandle
  void WaitTaskCompletedSync(TaskHandle InHandle);

  /// 查询一个Task的状态
  /// @param InHandle
  /// @return
  ETaskState QueryTaskState(TaskHandle InHandle);
};
