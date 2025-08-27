#pragma once
#include "Malloc.h"

#include <memory>

template <typename T> struct SharedPtrAllocator {
  using value_type = T;
  SharedPtrAllocator() = default;
  template <typename U> explicit SharedPtrAllocator(const SharedPtrAllocator<U>&) noexcept {}

  static T* allocate(const SizeType N) { return static_cast<T*>(Malloc(N * sizeof(T), EMemoryLabel::Default)); }

  static void deallocate(T* p, SizeType) noexcept { Free(p, EMemoryLabel::Default); }
};

template <typename T> struct SharedPtr {
  using element_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  template <typename R> friend struct SharedPtr;

  SharedPtr() = default;
  SharedPtr(std::nullptr_t) {}
  explicit SharedPtr(const std::shared_ptr<T>& InPtr) : mPtr(InPtr) {}
  SharedPtr& operator=(std::nullptr_t) {
    mPtr.reset();
    return *this;
  }
  explicit SharedPtr(T* InPtr) : mPtr(InPtr) {}
  SharedPtr(SharedPtr&& Other) noexcept : mPtr(std::move(Other.mPtr)) {}
  SharedPtr& operator=(SharedPtr&& Other) noexcept {
    mPtr = std::move(Other.mPtr);
    return *this;
  }
  SharedPtr(const SharedPtr& Other) : mPtr(Other.mPtr) {}
  SharedPtr& operator=(const SharedPtr& Other) {
    mPtr = Other.mPtr;
    return *this;
  }

  template <typename R> SharedPtr(const SharedPtr<R>& Other) : mPtr(Other.mPtr) {}

  T* Get() { return mPtr.get(); }
  const T* Get() const { return mPtr.get(); }
  T& operator*() { return *mPtr; }
  const T& operator*() const { return *mPtr; }
  T* operator->() { return mPtr.get(); }
  const T* operator->() const { return mPtr.get(); }
  T& GetRef() { return *mPtr; }
  const T& GetRef() const { return *mPtr; }

  operator bool() const { return mPtr != nullptr; }

private:
  std::shared_ptr<T> mPtr;
};

template <typename T, typename U> bool operator==(const SharedPtr<T>& Lhs, const SharedPtr<U>& Rhs) { return Lhs.Get() == Rhs.Get(); }

template <typename T> bool operator==(const SharedPtr<T>& Lhs, std::nullptr_t) { return !Lhs.Get(); }

template <typename T, typename... Args> SharedPtr<T> MakeShared(Args&&... args) {
  auto Allocator = SharedPtrAllocator<T>{};
  return SharedPtr<T>(std::allocate_shared<T>(Allocator, std::forward<Args>(args)...));
}
template <EMemoryLabel Label, typename T, typename... Args> SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(std::allocate_shared<T>(SharedPtrAllocator<T>{}, std::forward<Args>(args)...));
}
