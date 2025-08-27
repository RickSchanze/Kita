#pragma once
#include "Core/Memory/Malloc.h"
#include "Core/Ranges.h"
#include "Core/String/StringTraits.h"
#include "Core/String/ToString.h"

#include <unordered_set>

template <typename K, EMemoryLabel Label = EMemoryLabel::Default> class Set {
public:
  using key_type = K;

#if KITA_DEBUG
  using set_type = std::unordered_set<K, std::hash<K>, std::equal_to<K>, STLAllocator<K, Label>>;
#else
  using set_type = absl::flat_hash_set<K, std::hash<K>, absl::DefaultHashContainerEq<K>, STLAllocator<K, Label>>;
#endif

  Set() = default;
  Set(const Set&) = default;
  Set(Set&&) = default;
  ~Set() = default;
  Set& operator=(const Set&) = default;
  Set& operator=(Set&&) = default;

  Set(std::initializer_list<K> Init) : mData(Init) {}

  template <typename... Args> Set(Args&&... InArgs) : mData(std::forward<Args>(InArgs)...) {}

  [[nodiscard]] bool Empty() const { return mData.empty(); }
  [[nodiscard]] SizeType Count() const { return mData.size(); }

  auto begin() { return mData.begin(); }
  auto end() { return mData.end(); }
  auto begin() const { return mData.begin(); }
  auto end() const { return mData.end(); }

  auto Begin() { return mData.begin(); }
  auto End() { return mData.end(); }
  auto Begin() const { return mData.begin(); }
  auto End() const { return mData.end(); }

  bool Add(const K& Key) { return mData.insert(Key).second; }
  bool Add(K&& Key) { return mData.insert(std::move(Key)).second; }
  bool Remove(const K& Key) { return mData.erase(Key) != 0; }
  [[nodiscard]] bool Contains(const K& Key) const { return mData.contains(Key); }

  auto Find(const K& Key) const { return mData.find(Key); }

  [[nodiscard]] String ToString() const
    requires(Traits::IStringable<K>)
  {
    String Result = "{";

    for (const auto& [Index, Item] : *this | Ranges::Views::Enumerate) {
      if constexpr (Traits::IToString<K>) {
        Result += String("\"") + Item.ToString() + "\"";
      } else {
        Result += String("\"") + ::ToString(Item) + "\"";
      }
      if (Index != Count() - 1) {
        Result += ", ";
      }
    }

    Result += "}";
    return Result;
  }

private:
  set_type mData;
};

namespace Traits {

namespace Pri {

template <typename T> struct IsSet : std::false_type {};

template <typename K, EMemoryLabel Label> struct IsSet<Set<K, Label>> : std::true_type {};

} // namespace Pri

template <typename T>
concept IsSet = Pri::IsSet<T>::value;

template <typename T> struct SetTraits {};
template <typename K, EMemoryLabel Label> struct SetTraits<Set<K, Label>> {
  using KeyType = K;
  constexpr static EMemoryLabel MemoryLabel = Label;
};

} // namespace Traits