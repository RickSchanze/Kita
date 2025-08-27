#pragma once

enum class ETaskState {
  Lazy,       // 等待被启动
  Pending,    // 等待依赖完成
  Scheduling, // 等待被调度
  Running,    // 正在运行
  Finished,   // 运行完成
  Count,      // 错误
};

struct TaskInstance;

struct TaskHandle {
  explicit TaskHandle(TaskInstance* Instance) : mInstance(Instance) {}
  TaskHandle() = default;

  /**
   * 启动一个Lazy任务
   */
  void StartLazy() const;

  /**
   * 获取任务状态
   * @return
   */
  [[nodiscard]] ETaskState GetState(bool Lock = true) const;

  [[nodiscard]] bool IsRunning() const { return GetState() == ETaskState::Running; }
  [[nodiscard]] bool IsFinished() const { return GetState() == ETaskState::Finished; }
  [[nodiscard]] bool IsScheduled() const { return GetState() == ETaskState::Scheduling; }
  [[nodiscard]] bool IsLazy() const { return GetState() == ETaskState::Lazy; }
  [[nodiscard]] bool IsValid() const;

  void WaitSync() const;

  [[nodiscard]] TaskInstance* GetInstance() const { return mInstance; }

private:
  TaskInstance* mInstance = nullptr;
};
