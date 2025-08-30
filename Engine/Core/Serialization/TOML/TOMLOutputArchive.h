#pragma once

#include "Core/Container/Stack.h"
#include "Core/Serialization/OutputArchive.h"
#include "Core/Serialization/SerializationError.h"

class TOMLOutputArchive : public OutputArchive {
public:
  enum ArchiveState {
    WritingArray,
    WritingObject,
  };
  TOMLOutputArchive();
  virtual ~TOMLOutputArchive() override;

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
  // 可能会有精度损失
  // TOML++: 整数值必须被无损转换到int64_t
  virtual void Write(StringView Key, UInt64 Value) override;
  virtual void Write(StringView Key, Float32 Value) override;
  virtual void Write(StringView Key, Float64 Value) override;
  virtual void Write(StringView Key, bool Value) override;
  struct Impl;

  ESerializationError WriteFile(StringView Path);

private:
  std::unique_ptr<Impl> mImpl;
  Stack<ArchiveState> mStateStack;
};
