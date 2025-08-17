//
// Created by kita on 25-8-17.
//

#include "TaskGraphBoard.h"
bool TaskGraphBoard::ReleaseTask(const TaskHandleId TaskId) {
  if (mTaskDataRecorder.Contains(TaskId)) {
    for (const auto& DataId : mTaskDataRecorder[TaskId]) {
      ASSERT_MSG(mData.Contains(DataId), "数据ID不存在");
      mData.Remove(DataId);
    }
    mTaskDataRecorder.Remove(TaskId);
    return true;
  }
  return false;
}

void TaskGraphBoard::ReleaseAll()  {
  for (const auto& DataPtr : mData | Ranges::Views::Values) {
    Free(DataPtr);
  }
  mTaskDataRecorder = {};
  mData = {};
}