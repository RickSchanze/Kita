#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/EnumString.h"
#include "Core/Traits.h"
#include "OutputArchive.h"
#include "SerializationError.h"

class InputArchive {
public:
  virtual ~InputArchive() = default;

  virtual ESerializationError BeginObject(StringView ScopeName) = 0;
  virtual ESerializationError EndObject() = 0;

  virtual ESerializationError BeginArray(StringView ScopeName) = 0;
  virtual ESerializationError EndArray() = 0;
  virtual SizeType GetCurrentArraySize() = 0;

  virtual ESerializationError Read(StringView Key, Int8& Value) = 0;
  virtual ESerializationError Read(StringView Key, Int16& Value) = 0;
  virtual ESerializationError Read(StringView Key, Int32& Value) = 0;
  virtual ESerializationError Read(StringView Key, Int64& Value) = 0;
  virtual ESerializationError Read(StringView Key, UInt8& Value) = 0;
  virtual ESerializationError Read(StringView Key, UInt16& Value) = 0;
  virtual ESerializationError Read(StringView Key, UInt32& Value) = 0;
  virtual ESerializationError Read(StringView Key, UInt64& Value) = 0;
  virtual ESerializationError Read(StringView Key, Float32& Value) = 0;
  virtual ESerializationError Read(StringView Key, Float64& Value) = 0;
  virtual ESerializationError Read(StringView Key, bool& Value) = 0;
  virtual ESerializationError Read(StringView Key, String& Value) = 0;

  template <typename T> void ReadType(StringView Key, T& Value);
};

namespace Traits {

template <typename T>
concept HasGlobalInputArchiveFunc = requires(T& Value, InputArchive& Ar) {
  { ReadArchive(Ar, Value) } -> SameAs<ESerializationError>;
};

template <typename T>
concept HasMemberInputArchiveFunc = requires(T& Value, InputArchive& Ar) {
  { Value.ReadArchive(Ar) } -> SameAs<ESerializationError>;
};

template <typename T>
concept HasInputArchiveFunc = HasGlobalInputArchiveFunc<T> || HasMemberInputArchiveFunc<T>;

} // namespace Traits

template <typename T> void InputArchive::ReadType(StringView Key, T& Value) {
  if constexpr (Traits::AnyOf<Traits::Pure<T>, StringView, String, Int8, Int16, Int32, Int64, UInt8, UInt16, UInt32, UInt64, Float32, Float64, bool>) {
    Read(Key, Value);
  } else if constexpr (Traits::IsArray<T>) {
    BeginArray(Key);
    if (SizeType S = GetCurrentArraySize(); S == INVALID_SIZE) {
      LOG_ERROR_TAG("Serialization", "数组具有异常大小. Key=[{}]", Key);
    } else {
      Value.Resize(S);
      for (SizeType I = 0; I < S; ++I) {
        ReadType("", Value[I]);
      }
    }
    EndArray();
  } else if constexpr (Traits::IsMap<T>) {
    static_assert(false, "TOMLOutputArchive does not support map. Use struct instead.");
  } else if constexpr (Traits::IsEnum<T>) {
    String EnumString;
    Read(Key, EnumString);
    Value = StringToEnum<T>(EnumString);
  } else {
    if constexpr (Traits::HasGlobalOutputArchiveFunc<T>) {
      BeginObject(Key);
      ReadArchive(*this, Value);
      EndObject();
    } else if constexpr (Traits::HasMemberOutputArchiveFunc<T>) {
      ESerializationError Error = BeginObject(Key);
      if (Error == ESerializationError::Ok) {
        Value.ReadArchive(*this);
      }
      EndObject();
    } else {
      static_assert(false, "TOMLOutputArchive does not support this type. Implement WriteArchive(OutputArchive& Archive) for this type.");
    }
  }
}
