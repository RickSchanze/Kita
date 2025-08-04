#pragma once
#include "Core/Assert.h" // for assert
#include "Core/Logging/Logger.hpp"
#include "Core/Memory/Malloc.h"
#include "Core/Ranges.h"
#include "Core/String/String.h"
#include "Core/String/StringTraits.h"
#include "Core/String/ToString.h"

#include <vector>

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> class Array {
public:
  Array() = default;
  explicit Array(SizeType InSize) : mData(InSize) {}
  Array(std::initializer_list<T> InList) : mData(InList) {}
  Array(const Array&) = default;
  Array(Array&&) = default;
  Array& operator=(const Array&) = default;
  Array& operator=(Array&&) = default;

  auto begin() { return mData.begin(); }
  auto end() { return mData.end(); }
  auto begin() const { return mData.begin(); }
  auto end() const { return mData.end(); }

  [[nodiscard]] SizeType Count() const { return mData.size(); }
  void Resize(SizeType NewSize) { mData.resize(NewSize); }
  void Reserve(SizeType NewCapacity) { mData.reserve(NewCapacity); }
  void Clear() { mData.clear(); }

  SizeType IndexOf(const T& InValue) const {
    static_assert(Traits::IEquatable<T>, "T must be equatable when use IndexOf(const T&)");
    for (SizeType Index = 0; Index < mData.size(); Index++) {
      if (mData[Index] == InValue) {
        return Index;
      }
    }
    return INVALID_INDEX;
  }

  SizeType IndexOf(auto&& InPredicate) const {
    for (SizeType Index = 0; Index < mData.size(); Index++) {
      if (InPredicate(mData[Index])) {
        return Index;
      }
    }
    return INVALID_INDEX;
  }

  // 交换然后删除 erase-remove
  void Remove(const T& InValue) {
    static_assert(Traits::IEquatable<T>, "T must be equatable when use IndexOf(const T&)");
    for (SizeType Index = 0; Index < mData.size(); Index++) {
      if (mData[Index] == InValue) {
        mData.erase(std::remove(mData.begin() + Index, mData.begin() + Index + 1), mData.end());
      }
    }
  }

  auto&& operator[](this auto&& Self, SizeType Index) {
    DEBUG_ASSERT_MSG(Index < Self.Count(), "Index out of range");
    return Self.mData[Index];
  }

  template <typename U> void Add(U&& InValue) { mData.push_back(std::forward<U>(InValue)); }
  template <typename... Args> auto Emplace(Args&&... InArgs) { return mData.emplace_back(std::forward<Args>(InArgs)...); }

  [[nodiscard]] String ToString() const
    requires(Traits::IStringable<T>)
  {
    static_assert(!Traits::SameAs<T, bool>, "bool array is not allowed");
    String Result = "[";
    for (const auto& [Index, Item] : *this | Ranges::Views::Enumerate) {
      if constexpr (Traits::ISelfStringable<T>) {
        Result += (::ToString(Item) + (Index != Count() - 1 ? ", " : ""));
      } else {
        Result += (Item.ToString() + (Index != Count() - 1 ? ", " : ""));
      }
    }
    Result += "]";
    return Result;
  }

  void AddUnique(const T& Value) {
    if (IndexOf(Value) == INVALID_INDEX) {
      Add(Value);
    }
  }

  /// @brief 向当前容器末尾插入 Container 中的所有元素
  template <typename Cont> void AddRange(Cont&& Container) { mData.insert_range(end(), Container); }

  /// @brief 向当前容器的指定位置插入 Container 中的所有元素
  template <typename Iter, typename Cont> void AddRange(Iter&& Where, Cont&& Container) { mData.insert_range(Where, Container); }

  /// @brief 向当前容器的指定位置插入迭代器范围的元素
  template <typename Iter1, typename Iter2> void AddRange(Iter1&& Where, Iter2&& Begin, Iter2&& End) { mData.insert_range(Where, Begin, End); }

  /// @brief 检查容器是否为空
  /// @return true 为空 false 不为空
  [[nodiscard]] bool Empty() const { return mData.empty(); }

private:
  std::vector<T, STLAllocator<T, Label>> mData;
};

namespace Traits {
namespace Pri {
template <typename T> struct IsArray : std::false_type {};

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> struct IsArray<Array<T, Label>> : std::true_type {};
} // namespace Pri

template <typename T>
concept IsArray = Pri::IsArray<T>::value;

} // namespace Traits
