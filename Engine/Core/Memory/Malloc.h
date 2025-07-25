#pragma once
#include "Core/TypeDefs.h"

enum class EMemoryLabel {
  Default,
  Count,
};

void* Malloc(Size InSize, EMemoryLabel Label);
void Free(void* Ptr, EMemoryLabel Label);

template <typename T, typename... Args> T* New(Args&&... InArgs) { return new (Malloc(sizeof(T), EMemoryLabel::Default)) T(std::forward<Args...>(InArgs...)); }
template <EMemoryLabel Label, typename T, typename... Args> T* New(Args&&... InArgs) { return new (Malloc(sizeof(T), Label)) T(std::forward<Args...>(InArgs...)); }

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> void Delete(T* Ptr) {
  Ptr->~T();
  Free(Ptr, Label);
}
