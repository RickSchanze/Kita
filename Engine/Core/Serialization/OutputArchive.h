#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Reflection/EnumString.h"
#include "Core/String/StringView.h"
#include "Core/Traits.h"

enum class EOutputArchiveFlag {
  None = 0,
  Inline = 1 << 0, // for toml yaml
};
ENABLE_BITMASK_OPERATORS(EOutputArchiveFlag)

// 继承此结构体, 使结构体Inline
struct InlinedOutput {};

class OutputArchive {
public:
  virtual ~OutputArchive() = default;

  virtual void BeginObject(StringView ObjectName, EOutputArchiveFlag Flag) = 0;
  virtual void EndObject() = 0;
  virtual void BeginArray(StringView Key) = 0;
  virtual void EndArray() = 0;
  virtual void Write(StringView Key, StringView Value) = 0;
  virtual void Write(StringView Key, Int8 Value) = 0;
  virtual void Write(StringView Key, Int16 Value) = 0;
  virtual void Write(StringView Key, Int32 Value) = 0;
  virtual void Write(StringView Key, Int64 Value) = 0;
  virtual void Write(StringView Key, UInt8 Value) = 0;
  virtual void Write(StringView Key, UInt16 Value) = 0;
  virtual void Write(StringView Key, UInt32 Value) = 0;
  virtual void Write(StringView Key, UInt64 Value) = 0;
  virtual void Write(StringView Key, Float32 Value) = 0;
  virtual void Write(StringView Key, Float64 Value) = 0;
  virtual void Write(StringView Key, bool Value) = 0;

  template <typename T> void WriteType(StringView Key, const T& Value);
};

namespace Traits {

template <typename T>
concept HasGlobalOutputArchiveFunc = requires(T& Value, OutputArchive& Ar) {
  { WriteArchive(Ar, Value) };
};

template <typename T>
concept HasMemberOutputArchiveFunc = requires(T& Value, OutputArchive& Ar) {
  { Value.WriteArchive(Ar) };
};

template <typename T>
concept HasOutputArchiveFunc = HasGlobalOutputArchiveFunc<T> || HasMemberOutputArchiveFunc<T>;
} // namespace Traits

template <typename T> void OutputArchive::WriteType(StringView Key, const T& Value) {
  if constexpr (Traits::AnyOf<Traits::Pure<T>, StringView, String, Int8, Int16, Int32, Int64, UInt8, UInt16, UInt32, UInt64, Float32, Float64, bool>) {
    Write(Key, Value);
  } else if constexpr (Traits::IsArray<T>) {
    BeginArray(Key);
    for (const auto& Item : Value) {
      WriteType("", Item);
    }
    EndArray();
  } else if constexpr (Traits::IsMap<T>) {
    static_assert(false, "TOMLOutputArchive does not support map. Use struct instead.");
  } else if constexpr (Traits::IsEnum<T>) {
    Write(Key, EnumToString(Value));
  } else {
    if constexpr (Traits::HasGlobalOutputArchiveFunc<T>) {
      auto Flag = EOutputArchiveFlag::None;
      if constexpr (Traits::IsBaseOf<InlinedOutput, T>) {
        Flag |= EOutputArchiveFlag::Inline;
      }
      BeginObject(Key, Flag);
      WriteArchive(*this, Value);
      EndObject();
    } else if constexpr (Traits::HasMemberOutputArchiveFunc<T>) {
      auto Flag = EOutputArchiveFlag::None;
      if constexpr (Traits::IsBaseOf<InlinedOutput, T>) {
        Flag |= EOutputArchiveFlag::Inline;
      }
      BeginObject(Key, Flag);
      Value.WriteArchive(*this);
      EndObject();
    } else {
      static_assert(false, "TOMLOutputArchive does not support this type. Implement WriteArchive(OutputArchive& Archive) for this type.");
    }
  }
}