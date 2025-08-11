#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Serialization/InputArchive.h"

class TOMLInputArchive final : public InputArchive
{
public:
  TOMLInputArchive();
  ~TOMLInputArchive() override;

  enum State {
    ReadingObject,
    ReadingArray,
  };

  virtual ESerializationError BeginObject(StringView ScopeName) override;
  virtual ESerializationError EndObject() override;
  virtual ESerializationError BeginArray(StringView ScopeName) override;
  virtual ESerializationError EndArray() override;
  virtual ESerializationError Read(StringView Key, Int8& Value) override;
  virtual ESerializationError Read(StringView Key, Int16& Value) override;
  virtual ESerializationError Read(StringView Key, Int32& Value) override;
  virtual ESerializationError Read(StringView Key, Int64& Value) override;
  virtual ESerializationError Read(StringView Key, UInt8& Value) override;
  virtual ESerializationError Read(StringView Key, UInt16& Value) override;
  virtual ESerializationError Read(StringView Key, UInt32& Value) override;
  virtual ESerializationError Read(StringView Key, UInt64& Value) override;
  virtual ESerializationError Read(StringView Key, Float32& Value) override;
  virtual ESerializationError Read(StringView Key, Float64& Value) override;
  virtual ESerializationError Read(StringView Key, bool& Value) override;
  virtual ESerializationError Read(StringView Key, String& Value) override;

  ESerializationError ParseFile(StringView Path);
  struct Impl;
private:

  UniquePtr<Impl> mImpl;
  State mState = ReadingObject;
};
