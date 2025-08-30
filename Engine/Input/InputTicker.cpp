//
// Created by kita on 25-8-30.
//

#include "InputTicker.h"

#include "Core/Performance/ProfilerMark.h"
#include "Input.h"
#include "RHI/SurfaceWindow.h"

void InputTicker::Tick(double DeltaSeconds) {
  CPU_PROFILING_SCOPE;
  mWindow->TickInput();
  UpdateInputData();
}

void InputTicker::UpdateInputData() const {
  auto& InputRef = Input::GetRef();
  InputRef.mShouldClose = mWindow->ShouldClose();
}