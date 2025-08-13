#pragma once
#include "Core/Container/Stack.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/Serialization/OutputArchive.h"
#include "Core/Serialization/SerializationError.h"

class YAMLOutputArchive : public OutputArchive
{
public:
  YAMLOutputArchive();
  virtual ~YAMLOutputArchive() override;

  enum State {
    WritingObject,
    WritingArray,
  };

  virtual void BeginObject(StringView ObjectName) override;
  virtual void EndObject() override;
  virtual void BeginArray(StringView Key) override;
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

  ESerializationError WriteFile(StringView Path);

  struct Impl;
private:
  UniquePtr<Impl> mImpl;
  Stack<State> mStateStack;
};
