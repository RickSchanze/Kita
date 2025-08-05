#pragma once
#include "Core/Serialization/Writer.h"

class INIWriter : public Writer
{
public:
  virtual void BeginObject() override;
  virtual void EndObject() override;
  virtual void BeginArray() override;
  virtual void EndArray() override;
  virtual void Write(StringView Key, StringView Value) override;
  virtual void Write(StringView Key, Int8 Value) override;
  virtual void Write(StringView Key, Int16 Value) override;
  virtual void Write(StringView Key, Int32 Value) override;
  virtual void Write(StringView Key, Int64 Value) override;
  virtual void Write(StringView Key, UInt8 Value) override;
  virtual void Write(StringView Key, UInt16 Value) override;
  virtual void Write(StringView Key, UInt32 Value) override;
  virtual void Write(StringView Key, UInt64 Value) override;
  virtual void Write(StringView Key, Float32 Value) override;
  virtual void Write(StringView Key, Float64 Value) override;
  virtual void Write(StringView Key, bool Value) override;
};
