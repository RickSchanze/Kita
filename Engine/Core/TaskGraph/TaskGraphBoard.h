#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Container/Optional.h"
#include "Core/Singleton/Singleton.h"
#include "Core/TypeDefs.h"
#include "TaskGraph.h"

struct TaskGraphBoardDataId {
  Int32 Handle;

  [[nodiscard]] SizeType GetHashCode() const { return std::hash<Int32>()(Handle); }
};

/**
 * TaskGraphBoard是一个所有任务共用的用于存储数据的类
 * TaskNode可以主动从中取出或者存放数据
 */
class TaskGraphBoard : public Singleton<TaskGraphBoard> {
public:
  template <typename T>
    requires Traits::IsTrivial<T>
  bool Put(const TaskGraphBoardDataId DataId, const T& Data) {
    if (mData.Contains(DataId)) {
      LOG_ERROR_TAG("TaskGraph", "重复ID的数据设置.");
      return false;
    }
    void* Ptr = Malloc(sizeof(T));
    Memcpy(Ptr, &Data, sizeof(T));
    mData.Add(DataId, Ptr);
    return true;
  }

  /// 只允许Get消费一次, 一旦此Id被Get, 就会从Board中移除
  template <typename T> Optional<T> Get(const TaskGraphBoardDataId Id) {
    if (mData.Contains(Id)) {
      T Data = *static_cast<T*>(mData[Id]);
      Free(mData[Id]);
      mData.Remove(Id);
      return Optional<T>(Data);
    }
    return NullOpt;
  }

  /// 只允许Get消费一次, 一旦此Id被Get, 就会从Board中移除
  template <typename T> bool TryGet(const TaskGraphBoardDataId Id, T& OutData) {
    if (mData.Contains(Id)) {
      OutData = *static_cast<T*>(mData[Id]);
      Free(mData[Id]);
      mData.Remove(Id);
      return true;
    }
    return false;
  }

  void ReleaseAll();

private:
  /// 一个ID对应的数据存放的地方
  Map<TaskGraphBoardDataId, void*> mData;
};
