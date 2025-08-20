//
// Created by kita on 25-8-17.
//

#include "TaskGraphBoard.h"

void TaskGraphBoard::ReleaseAll()  {
  for (const auto& DataPtr : mData | Ranges::Views::Values) {
    Free(DataPtr);
  }
  mTaskDataRecorder = {};
  mData = {};
}