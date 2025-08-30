#pragma once
#include "Core/Traits.h"
#include "Malloc.h"

#include <memory>

template <typename T, EMemoryLabel Label> struct UniquePtrDeleter {
  void operator()(T* Ptr) { Delete<T, Label>(Ptr); }
};

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> struct UniquePtr {
  UniquePtr(T* InPtr) : mPtr(InPtr, UniquePtrDeleter<T, Label>()) {}

  template <typename R, EMemoryLabel RLabel> friend struct UniquePtr;

  template <typename R> UniquePtr(UniquePtr<R, Label>&& Other) noexcept {
    if constexpr (Traits::IsBaseOf<T, R>) {
      R* Ptr = Other.mPtr.release();
      T* TPtr = static_cast<T*>(Ptr);
      mPtr.reset(TPtr);
    } else {
      static_assert(false, "Cannot cast between unrelated types");
    }
  }

  UniquePtr() = default;
  UniquePtr(UniquePtr&) = delete;
  UniquePtr(UniquePtr&& Other) noexcept : mPtr(std::move(Other.mPtr)) {}
  UniquePtr& operator=(UniquePtr&& Other) noexcept {
    mPtr = std::move(Other.mPtr);
    return *this;
  }
  UniquePtr& operator=(UniquePtr&) = delete;
  T* Get() { return mPtr.get(); }
  T* Get() const { return mPtr.get(); }
  T& operator*() { return *mPtr; }
  const T& operator*() const { return *mPtr; }
  T* operator->() { return mPtr.get(); }
  const T* operator->() const { return mPtr.get(); }
  T& GetRef() { return *mPtr; }
  const T& GetRef() const { return *mPtr; }
  operator bool() const { return mPtr.get() != nullptr; }

private:
  std::unique_ptr<T, UniquePtrDeleter<T, Label>> mPtr;
};

template <typename T, typename... TArgs> UniquePtr<T> MakeUnique(TArgs&&... Args) { return UniquePtr<T>(New<T>(std::forward<TArgs>(Args)...)); }

template <EMemoryLabel Label, typename T, typename... TArgs> UniquePtr<T> MakeUnique(TArgs&&... Args) { return UniquePtr<T>(New<T, Label>(std::forward<TArgs>(Args)...)); }
