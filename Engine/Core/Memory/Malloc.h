#pragma once
#include "Core/Macros.h"
#include "Core/TypeDefs.h"

enum class EMemoryLabel {
  Default,
  Count,
};

void* Malloc(SizeType InSize, EMemoryLabel Label = EMemoryLabel::Default);
void Free(void* Ptr, EMemoryLabel Label = EMemoryLabel::Default);
FORCE_INLINE void* Memcpy(void* Dest, const void* Source, const SizeType Size) { return memcpy(Dest, Source, Size); }

template <typename T, typename... Args> T* New(Args&&... InArgs) {
  T* Ptr = static_cast<T*>(Malloc(sizeof(T), EMemoryLabel::Default));
  return new (Ptr) T(std::forward<Args>(InArgs)...);
}

template <EMemoryLabel Label, typename T, typename... Args> T* New(Args&&... InArgs) {
  T* Ptr = static_cast<T*>(Malloc(sizeof(T), Label));
  return new (Ptr) T(std::forward<Args>(InArgs)...);
}

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> void Delete(T* Ptr) {
  Ptr->~T();
  Free(Ptr, Label);
}

namespace Pri {
void* NewN(SizeType Count, SizeType TypeSize);
}

template <typename T, EMemoryLabel Label> struct STLAllocator {
  typedef T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type& reference;
  typedef value_type const& const_reference;
  typedef value_type* pointer;
  typedef value_type const* const_pointer;
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;
  template <class U, class... Args> void construct(U* p, Args&&... args) { ::new (p) U(std::forward<Args>(args)...); }
  template <class U> static void destroy(U* p) noexcept { p->~U(); }
  [[nodiscard]] static SizeType max_size() noexcept { return (PTRDIFF_MAX / sizeof(value_type)); }
  static pointer address(reference x) { return &x; }
  static const_pointer address(const_reference x) { return &x; }

  template <class U> struct rebind {
    typedef STLAllocator<U, Label> other;
  };

  STLAllocator() noexcept = default;
  STLAllocator(const STLAllocator&) noexcept = default;
  template <class U> explicit STLAllocator(const STLAllocator<U, Label>&) noexcept {}
  STLAllocator select_on_container_copy_construction() const { return *this; }
  static void deallocate(T* p, size_type) { Free(p, Label); }
  [[nodiscard]] static pointer allocate(const size_type count, const void* = nullptr) { return static_cast<pointer>(Pri::NewN(count, sizeof(value_type))); }
};
