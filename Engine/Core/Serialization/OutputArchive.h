#pragma once
#include "Core/String/StringView.h"

class OutputArchive {
public:
  virtual ~OutputArchive() = default;

  virtual void BeginObject(StringView ObjectName) = 0;
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
};
