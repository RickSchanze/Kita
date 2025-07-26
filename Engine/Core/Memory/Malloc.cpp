//
// Created by Tencent_GO on 25-7-25.
//

#include "Malloc.h"
#include "mimalloc.h"

void* Malloc(const Size InSize, const EMemoryLabel Label) {
  if (Label == EMemoryLabel::Default) {
    return mi_malloc(InSize);
  }
  return nullptr;
}

void Free(void* Ptr, const EMemoryLabel Label) {
  if (Label == EMemoryLabel::Default) {
    mi_free(Ptr);
  }
}

void* Pri::NewN(const Size Count, const Size TypeSize) { return mi_new_n(Count, TypeSize); }
