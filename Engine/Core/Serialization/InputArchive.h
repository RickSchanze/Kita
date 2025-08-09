#pragma once
#include "Core/Traits.h"
#include "SerializationError.h"

class InputArchive {
public:
  virtual ~InputArchive() = default;

  virtual void BeginObject(StringView ScopeName) = 0;
  virtual void EndObject() = 0;
  virtual void BeginArray(StringView ScopeName) = 0;
  virtual void EndArray() = 0;

  virtual void Read(StringView Key, Int8& Value) = 0;
  virtual void Read(StringView Key, Int16& Value) = 0;
  virtual void Read(StringView Key, Int32& Value) = 0;
  virtual void Read(StringView Key, Int64& Value) = 0;
  virtual void Read(StringView Key, UInt8& Value) = 0;
  virtual void Read(StringView Key, UInt16& Value) = 0;
  virtual void Read(StringView Key, UInt32& Value) = 0;
  virtual void Read(StringView Key, UInt64& Value) = 0;
  virtual void Read(StringView Key, Float32& Value) = 0;
  virtual void Read(StringView Key, Float64& Value) = 0;
  virtual void Read(StringView Key, bool& Value) = 0;
  virtual void Read(StringView Key, String& Value) = 0;
};

namespace Traits {

namespace Pri {

template <typename T>
concept HasGlobalInputArchiveFunc = requires(T& Value, InputArchive& Ar) {
  { ReadArchive(Ar, Value) } -> SameAs<ESerializationError>;
};

template <typename T>
concept HasMemberInputArchiveFunc = requires(T& Value, InputArchive& Ar) {
  { Value.ReadArchive(Ar) } -> SameAs<ESerializationError>;
};

} // namespace Pri

template <typename T>
concept HasInputArchiveFunc = Pri::HasGlobalInputArchiveFunc<T> || Pri::HasMemberInputArchiveFunc<T>;

} // namespace Traits
