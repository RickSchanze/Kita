#pragma once
#include "Core/Singleton/Singleton.h"

struct InputDataPC {};

class Input : Singleton<Input> {
  friend class InputTicker;

public:
  static bool ShouldClose() { return GetRef().mShouldClose; }

private:
  bool mShouldClose = false;
};
