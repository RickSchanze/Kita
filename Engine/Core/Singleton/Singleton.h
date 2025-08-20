#pragma once
#include "Core/Traits.h"

template <typename T> class Singleton : public NonCopyMovable {
public:
  static FORCE_INLINE T* Get() {
    static T Instance;
    return &Instance;
  }

  static FORCE_INLINE T& GetRef() { return *Get(); }
};
