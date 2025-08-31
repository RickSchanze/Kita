#pragma once
#include "Core/Memory/Malloc.h"
#include "Core/Traits.h"
#include "Core/TypeDefs.h"

#include <deque>
#include <initializer_list>
#include <ranges>

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> class Deque {
public:
  using SizeType = size_t;

  Deque() = default;
  explicit Deque(SizeType InSize) : mData(InSize) {}
  Deque(std::initializer_list<T> InList) : mData(InList) {}
  Deque(const Deque&) = default;
  Deque(Deque&&) = default;
  Deque& operator=(const Deque&) = default;
  Deque& operator=(Deque&&) = default;

  // ✅ C++23 from_range 构造
  template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, T>
  Deque(std::from_range_t, R&& InRange) : mData(std::ranges::begin(InRange), std::ranges::end(InRange)) {}
  // 移除头部元素
  void RemoveFront() {
    if (!mData.empty()) {
      mData.pop_front();
    }
  }

  // 移除尾部元素
  void RemoveBack() {
    if (!mData.empty()) {
      mData.pop_back();
    }
  }
  //----------------------------------------------------
  // 遍历接口
  auto begin() { return mData.begin(); }
  auto end() { return mData.end(); }
  auto begin() const { return mData.begin(); }
  auto end() const { return mData.end(); }

  //----------------------------------------------------
  [[nodiscard]] SizeType Count() const { return mData.size(); }
  void Resize(SizeType NewSize) { mData.resize(NewSize); }
  void Clear() { mData.clear(); }
  [[nodiscard]] bool Empty() const { return mData.empty(); }

  //----------------------------------------------------
  // 索引与搜索
  SizeType IndexOf(const T& InValue) const {
    static_assert(Traits::IEquatable<T>, "T must be equatable when use IndexOf");
    for (SizeType i = 0; i < mData.size(); ++i)
      if (mData[i] == InValue)
        return i;
    return INVALID_INDEX;
  }

  SizeType IndexOf(auto&& InPredicate) const {
    for (SizeType i = 0; i < mData.size(); ++i)
      if (InPredicate(mData[i]))
        return i;
    return INVALID_INDEX;
  }

  //----------------------------------------------------
  // 删除
  void Remove(const T& InValue) {
    static_assert(Traits::IEquatable<T>, "T must be equatable when use Remove");
    for (SizeType i = 0; i < mData.size(); ++i) {
      if (mData[i] == InValue) {
        mData.erase(mData.begin() + i);
        return;
      }
    }
  }

  //----------------------------------------------------
  // 访问
  auto& Last() { return mData.back(); }
  const auto& Last() const { return mData.back(); }
  auto& First() { return mData.front(); }
  const auto& First() const { return mData.front(); }

  auto&& operator[](this auto&& self, SizeType index) { return self.mData[index]; }

  //----------------------------------------------------
  // 尾部操作
  template <typename U> void Add(U&& InValue) { mData.push_back(std::forward<U>(InValue)); }
  template <typename... Args> auto Emplace(Args&&... args) { return mData.emplace_back(std::forward<Args>(args)...); }

  // 头部操作
  template <typename U> void AddFront(U&& InValue) { mData.push_front(std::forward<U>(InValue)); }
  template <typename... Args> auto EmplaceFront(Args&&... args) { return mData.emplace_front(std::forward<Args>(args)...); }

  //----------------------------------------------------
  void AddUnique(const T& Value) {
    if (IndexOf(Value) == INVALID_INDEX)
      Add(Value);
  }

  template <typename Cont> void AddRange(Cont&& Container) { mData.insert(mData.end(), Container.begin(), Container.end()); }
  template <typename Cont> void AddRangeFront(Cont&& Container) { mData.insert(mData.begin(), Container.begin(), Container.end()); }

private:
  std::deque<T, STLAllocator<T, Label>> mData;
};