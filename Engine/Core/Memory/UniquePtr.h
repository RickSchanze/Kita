#pragma once
#include "Malloc.h"

#include <memory>

template <typename T, EMemoryLabel Label> struct UniquePtrDeleter {
  void operator()(T* Ptr) { Free(Ptr, Label); }
};

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> struct UniquePtr {
  explicit UniquePtr(T* InPtr) : mPtr(InPtr, UniquePtrDeleter<T, Label>()) {}
  UniquePtr() = default;
  UniquePtr(UniquePtr&) = delete;
  UniquePtr(UniquePtr&& Other) noexcept : mPtr(std::move(Other.mPtr)) {}
  UniquePtr& operator=(UniquePtr&& Other) noexcept {
    mPtr = std::move(Other.mPtr);
    return *this;
  }
  UniquePtr& operator=(UniquePtr&) = delete;
  T* Get() { return mPtr.get(); }
  const T* Get() const { return mPtr.get(); }
  T& operator*() { return *mPtr; }
  const T& operator*() const { return *mPtr; }
  T* operator->() { return mPtr.get(); }
  const T* operator->() const { return mPtr.get(); }
  T& GetRef() { return *mPtr; }
  const T& GetRef() const { return *mPtr; }

private:
  std::unique_ptr<T, UniquePtrDeleter<T, Label>> mPtr;
};

template <typename T, typename... TArgs> UniquePtr<T> MakeUnique(TArgs&&... Args) { return UniquePtr<T>(New<T>(std::forward<TArgs>(Args)...)); }

template <EMemoryLabel Label, typename T, typename... TArgs> UniquePtr<T> MakeUnique(TArgs&&... Args) { return UniquePtr<T>(New<T, Label>(std::forward<TArgs>(Args)...)); }
