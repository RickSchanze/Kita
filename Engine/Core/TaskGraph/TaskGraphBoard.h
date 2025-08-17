#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Container/Optional.h"
#include "Core/Singleton/Singleton.h"
#include "Core/TypeDefs.h"
#include "TaskGraph.h"
#include "TaskHandle.h"

struct TaskGraphBoardDataId {
  Int32 Handle;

  [[nodiscard]] SizeType GetHashCode() const { return std::hash<Int32>()(Handle); }
};

/**
 * TaskGraphBoard是一个所有任务共用的用于存储数据的类
 * TaskNode可以主动从中取出或者存放数据
 */
class TaskGraphBoard : public NonMovable, public NonCopyable, public Singleton<TaskGraphBoard> {
public:
  template <typename T>
    requires Traits::IsTrivial<T>
  bool Put(const TaskHandleId TaskId, const TaskGraphBoardDataId DataId, const T& Data) {
    if (mData.Contains(DataId)) {
      LOG_ERROR_TAG("TaskGraph", "重复ID的数据设置.");
      return false;
    }
    void* Ptr = Malloc(sizeof(T));
    Memcpy(Ptr, &Data, sizeof(T));
    mData.Add(DataId, Ptr);
    mTaskDataRecorder[TaskId].Add(DataId);
    return true;
  }

  template <typename T> Optional<T> Get(const TaskGraphBoardDataId Id) {
    if (mData.Contains(Id)) {
      return Optional<T>(*static_cast<T*>(mData[Id]));
    }
    return NullOpt;
  }

  template <typename T> bool TryGet(const TaskGraphBoardDataId Id, T& OutData) {
    if (mData.Contains(Id)) {
      OutData = *static_cast<T*>(mData[Id]);
      return true;
    }
    return false;
  }

  /// 将TaskId对应的所有数据进行释放
  /// @param TaskId 任务ID
  /// @return
  bool ReleaseTask(const TaskHandleId TaskId);

  void ReleaseAll();

private:
  /// 一个ID对应的数据存放的地方
  Map<TaskGraphBoardDataId, void*> mData;
  /// 一个任务ID对应该任务往此Board放了那些任务ID的数据
  Map<TaskHandleId, Array<TaskGraphBoardDataId>> mTaskDataRecorder;
};
