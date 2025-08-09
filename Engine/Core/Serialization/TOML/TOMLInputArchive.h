#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Serialization/InputArchive.h"

class TOMLInputArchive final : public InputArchive
{
public:
  virtual void BeginObject(StringView ScopeName) override;
  virtual void EndObject() override;
  virtual void BeginArray(StringView ScopeName) override;
  virtual void EndArray() override;
  virtual void Read(StringView Key, Int8& Value) override;
  virtual void Read(StringView Key, Int16& Value) override;
  virtual void Read(StringView Key, Int32& Value) override;
  virtual void Read(StringView Key, Int64& Value) override;
  virtual void Read(StringView Key, UInt8& Value) override;
  virtual void Read(StringView Key, UInt16& Value) override;
  virtual void Read(StringView Key, UInt32& Value) override;
  virtual void Read(StringView Key, UInt64& Value) override;
  virtual void Read(StringView Key, Float32& Value) override;
  virtual void Read(StringView Key, Float64& Value) override;
  virtual void Read(StringView Key, bool& Value) override;
  virtual void Read(StringView Key, String& Value) override;

  ESerializationError ParseFile(StringView Path);

private:
  struct Impl;
  UniquePtr<Impl> mImpl;
};
