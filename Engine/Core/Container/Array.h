#pragma once
#include "Core/Memory/Malloc.h"
#include "Core/Ranges.h"
#include "Core/String/String.h"
#include "Core/String/StringTraits.h"

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
  Size Size() const { return mData.size(); }

  template <typename U> void Add(U&& InValue) { mData.push_back(std::forward<U>(InValue)); }
  template <typename... Args> auto Emplace(Args&&... InArgs) { return mData.emplace_back(std::forward<Args>(InArgs)...); }

  String ToString() const {
    static_assert(Traits::IStringable<T> || Traits::ISelfStringable<T>, "T must be stringable");
    static_assert(!Traits::SameAs<T, bool>, "bool array is not allowed");
    String Result = "[";
    for (const auto& [Index, Item] : *this | Ranges::Views::Enumerate) {
      if constexpr (Traits::ISelfStringable<T>) {
        Result += (std::to_string(Item) + (Index != Size() - 1 ? ", " : ""));
      } else {
        Result += (Item.ToString() + (Index != Size() - 1 ? ", " : ""));
      }
    }
    Result += "]";
    return Result;
  }

private:
  std::vector<T, STLAllocator<T, Label>> mData;
};
