#pragma once
#include "Core/Reflection/MetaMark.h"

class [[ALabel("Test")]] M {
  [[AReflected]]
  Array<int> m;
};
