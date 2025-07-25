#pragma once
#include "Core/TypeDefs.h"

enum class EMemoryLabel {
  Default,
  Count,
};

void* Malloc(Size InSize, EMemoryLabel Label);
void Free(void* Ptr, EMemoryLabel Label);
