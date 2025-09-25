#pragma once
#include "Core/Macros.h"
#include "Core/Memory/Malloc.h"
#include "Core/Ranges.h"
#include "Core/String/StringTraits.h"
#include "Core/String/ToString.h"

#include <functional>

#if not KITA_DEBUG
#include "absl/container/flat_hash_set.h"
#endif

#include <mutex>
#include <unordered_set>

template <typename K, EMemoryLabel Label = EMemoryLabel::Default> class ConcurrentSet {
public:
  using key_type = K;

#if KITA_DEBUG
  using set_type = std::unordered_set<K, std::hash<K>, std::equal_to<K>, STLAllocator<K, Label>>;
#else
  using set_type = absl::flat_hash_set<K, std::hash<K>, absl::DefaultHashContainerEq<K>, STLAllocator<K, Label>>;
#endif

  ConcurrentSet() = default;
  ConcurrentSet(const ConcurrentSet&) = delete;
  ConcurrentSet& operator=(const ConcurrentSet&) = delete;

  [[nodiscard]] bool Empty() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.empty();
  }

  [[nodiscard]] SizeType Count() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.size();
  }

  bool Add(const K& Key) {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.insert(Key).second;
  }

  bool Add(K&& Key) {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.insert(std::move(Key)).second;
  }

  bool Remove(const K& Key) {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.erase(Key) != 0;
  }

  [[nodiscard]] bool Contains(const K& Key) const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.contains(Key);
  }

  auto Find(const K& Key) const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.find(Key);
  }

  [[nodiscard]] String ToString() const
    requires(Traits::IStringable<K>)
  {
    std::lock_guard<std::mutex> lock(mMutex);
    String Result = "{";

    SizeType Index = 0;
    for (const auto& Item : mData) {
      if constexpr (Traits::IToString<K>) {
        Result += String("\"") + Item.ToString() + "\"";
      } else {
        Result += String("\"") + ::ToString(Item) + "\"";
      }
      if (Index != mData.size() - 1) {
        Result += ", ";
      }
      ++Index;
    }

    Result += "}";
    return Result;
  }

  void ForEach(const std::function<void(const K&)>& InFunc) const {
    std::lock_guard Lock(mMutex);
    for (const auto& Item : mData) {
      InFunc(Item);
    }
  }

private:
  set_type mData;
  mutable std::mutex mMutex;
};