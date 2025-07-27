#pragma once

#include "Core/Memory/Malloc.h"
#include "Core/Ranges.h"
#include "Core/String/String.h"
#include "Core/String/StringTraits.h"
#include "Core/String/ToString.h"
#include "absl/container/flat_hash_map.h"
#include "absl/hash/hash.h"

template <typename K, typename V, EMemoryLabel Label = EMemoryLabel::Default>
class Map {
public:
  using key_type = K;
  using value_type = V;
#if KITA_DEBUG
  using map_type = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, STLAllocator<std::pair<const K, V>, Label>>;
#else
  using map_type = absl::flat_hash_map<K, V, std::hash<K>, absl::DefaultHashContainerEq<K>, STLAllocator<std::pair<const K, V>, Label>>;
#endif
  Map() = default;
  Map(const Map&) = default;
  Map(Map&&) = default;
  ~Map() = default;
  Map& operator=(const Map&) = default;
  Map& operator=(Map&&) = default;

  Map(const std::initializer_list<std::pair<const K, V>>& Init) : mData(Init) {}

  [[nodiscard]] bool Empty() const { return mData.empty(); }
  [[nodiscard]] SizeType Count() const { return mData.size(); }
  auto begin() { return mData.begin(); }
  auto end() { return mData.end(); }
  auto begin() const { return mData.begin(); }
  auto end() const { return mData.end(); }

  /// 符合项目命名约定的转发
  auto Begin() { return mData.begin(); }
  auto End() { return mData.end(); }
  auto Begin() const { return mData.begin(); }
  auto End() const { return mData.end(); }

  auto&& operator[](this auto&& Self, const K& Key) { return Self.mData[Key]; }

  void Add(const K& Key, const V& Value) { mData.insert({Key, Value}); }
  bool TryAdd(const K& Key, const V& Value) { return mData.try_emplace({Key, Value}); }
  bool Remove(const K& Key) { return mData.erase(Key) != 0; }
  [[nodiscard]] bool Contains(const K& Key) const { return mData.contains(Key); }

  auto Find(const K& Key) const { return mData.find(Key); }

  [[nodiscard]] String ToString() const
    requires(Traits::IStringable<K> && Traits::IStringable<V>)
  {
    String Result = "{";

    for (const auto& [Index, Item] : *this | Ranges::Views::Enumerate) {
      if constexpr (Traits::IToString<K>) {
        Result += String("\"") + Item.first.ToString() + "\": \"";
      } else {
        Result += String("\"") + ::ToString(Item.first) + "\": \"";
      }

      if constexpr (Traits::IToString<V>) {
        Result += Item.second.ToString() + "\"" + (Index == Count() - 1 ? "" : ", ");
      } else {
        Result += ::ToString(Item.second) + "\"" + (Index == Count() - 1 ? "" : ", ");
      }
    }

    Result += "}";
    return Result;
  }

private:
  map_type mData;
};
