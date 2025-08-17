#pragma once
#include "Core/Traits.h"

template <typename T> class Singleton {
public:
  static_assert(Traits::IsBaseOf<NonCopyable, T>, "Singleton must be derived from NonCopyable");
  static_assert(Traits::IsBaseOf<NonMovable, T>, "Singleton must be derived from NonMovable");

  static FORCE_INLINE T* Get() {
    static T Instance;
    return &Instance;
  }

  static FORCE_INLINE T& GetRef() { return *Get(); }
};
