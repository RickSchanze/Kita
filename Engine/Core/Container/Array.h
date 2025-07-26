#pragma once
#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Memory/Malloc.h"
#include "Core/Ranges.h"
#include "Core/String/String.h"
#include "Core/String/StringTraits.h"
#include "Core/Trace.h"

#include <vector>

template <typename T, EMemoryLabel Label = EMemoryLabel::Default> class Array {
public:
  Array() = default;
  explicit Array(Size InSize) : mData(InSize) {}
  Array(std::initializer_list<T> InList) : mData(InList) {}
  Array(const Array&) = default;
  Array(Array&&) = default;
  Array& operator=(const Array&) = default;
  Array& operator=(Array&&) = default;

  auto begin() { return mData.begin(); }
  auto end() { return mData.end(); }
  auto begin() const { return mData.begin(); }
  auto end() const { return mData.end(); }

  [[nodiscard]] Size Count() const { return mData.size(); }
  void Resize(Size NewSize) { mData.resize(NewSize); }
  void Reserve(Size NewCapacity) { mData.reserve(NewCapacity); }
  void Clear() { mData.clear(); }

  Size IndexOf(const T& InValue) {
    static_assert(Traits::IEquatable<T>, "T must be equatable when use IndexOf(const T&)");
    for (Size Index = 0; Index < mData.size(); Index++) {
      if (mData[Index] == InValue) {
        return Index;
      }
    }
    return INVALID_INDEX;
  }

  // 交换然后删除 erace-remove
  void Remove(const T& InValue) {
    static_assert(Traits::IEquatable<T>, "T must be equatable when use IndexOf(const T&)");
    for (Size Index = 0; Index < mData.size(); Index++) {
      if (mData[Index] == InValue) {
        mData.erase(std::remove(mData.begin() + Index, mData.begin() + Index + 1), mData.end());
      }
    }
  }

  auto&& operator[](this auto&& Self, Size Index) {
    DEBUG_ASSERT_MSG(Index < Self.Count(), "Index out of range");
    return Self.mData[Index];
  }

  template <typename U> void Add(U&& InValue) { mData.push_back(std::forward<U>(InValue)); }
  template <typename... Args> auto Emplace(Args&&... InArgs) { return mData.emplace_back(std::forward<Args>(InArgs)...); }

  [[nodiscard]] String ToString() const {
    static_assert(Traits::IStringable<T> || Traits::ISelfStringable<T>, "T must be stringable");
    static_assert(!Traits::SameAs<T, bool>, "bool array is not allowed");
    String Result = "[";
    for (const auto& [Index, Item] : *this | Ranges::Views::Enumerate) {
      if constexpr (Traits::ISelfStringable<T>) {
        Result += (std::to_string(Item) + (Index != Count() - 1 ? ", " : ""));
      } else {
        Result += (Item.ToString() + (Index != Count() - 1 ? ", " : ""));
      }
    }
    Result += "]";
    return Result;
  }

private:
  std::vector<T, STLAllocator<T, Label>> mData;
};
