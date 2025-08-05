#pragma once
#include "Core/String/String.h"

class Reader {
public:
  virtual ~Reader() = default;

  virtual void BeginObject() = 0;
  virtual void EndObject() = 0;
  virtual void BeginArray() = 0;
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
