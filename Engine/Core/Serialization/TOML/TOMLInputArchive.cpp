//
// Created by kita on 25-8-9.
//

#include "TOMLInputArchive.h"

#include "Core/Container/Stack.h"
#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Path.h"
#include "Core/Logging/Logger.hpp"
#include "toml++/toml.hpp"

struct TOMLInputArchive::Impl {
  toml::table Root;
  Stack<toml::node*> NodeStack;

  Impl() = default;
  ESerializationError ParseFile(const StringView Path) {
    if (Path::IsDirectory(Path)) {
      LOG_ERROR_TAG("Serialization", "Path must not be a directory.");
      return ESerializationError::TargetInvalid;
    }
    if (!Path::IsExists(Path)) {
      LOG_ERROR_TAG("Serialization", "Path must exists.");
      return ESerializationError::TargetInvalid;
    }
    try {
      std::ifstream FS(Path.Data());
      Root = toml::parse(FS);
      NodeStack.Push(&Root);
      return ESerializationError::Ok;
    } catch (toml::parse_error& e) {
      LOG_ERROR_TAG("Serialization", "Parse toml file error: {}", e.what());
      return ESerializationError::ParseError;
    }
  }

  bool IsTargetValid() { return NodeStack.Count() > 0; }

  toml::node* CurrentNode() { return NodeStack.Top(); }

  static toml::node* GetKeyNode(toml::node* parent, const StringView key) {
    if (!parent)
      return nullptr;
    if (parent->is_table()) {
      auto* tbl = parent->as_table();
      const auto it = tbl->find(key.GetStdStringView());
      if (it != tbl->end()) {
        return &it->second;
      }
    }
    return nullptr;
  }
};

ESerializationError TOMLInputArchive::ParseFile(const StringView Path) { return mImpl->ParseFile(Path); }

TOMLInputArchive::TOMLInputArchive() { mImpl = MakeUnique<Impl>(); }
TOMLInputArchive::~TOMLInputArchive() = default;
ESerializationError TOMLInputArchive::BeginObject(const StringView ScopeName) {
  if (!mImpl->IsTargetValid())
    return ESerializationError::TargetInvalid;

  toml::node* Current = mImpl->CurrentNode();
  toml::node* Child = TOMLInputArchive::Impl::GetKeyNode(Current, ScopeName);
  if (!Child)
    return ESerializationError::KeyNotFound;
  if (!Child->is_table())
    return ESerializationError::TypeMismatch;

  mImpl->NodeStack.Push(Child);
  mState = ReadingObject;
  return ESerializationError::Ok;
}

ESerializationError TOMLInputArchive::EndObject() {
  if (!mImpl->IsTargetValid() || mState != ReadingObject) {
    LOG_ERROR_TAG("Serialization", "非法的EndObject");
    return ESerializationError::TargetInvalid;
  }

  mImpl->NodeStack.Pop();
  return ESerializationError::Ok;
}

ESerializationError TOMLInputArchive::BeginArray(StringView ScopeName) {
  if (!mImpl->IsTargetValid())
    return ESerializationError::TargetInvalid;

  toml::node* current = mImpl->CurrentNode();
  toml::node* child = TOMLInputArchive::Impl::GetKeyNode(current, ScopeName);
  if (!child) {
    LOG_ERROR_TAG("Serialization", "BeginArray: KeyNotFound. Key={}", ScopeName);
    return ESerializationError::KeyNotFound;
  }
  if (!child->is_table()) {
    LOG_ERROR_TAG("Serialization", "BeginArray: TypeMismatch. Key={}, not a table.", ScopeName);
    return ESerializationError::TypeMismatch;
  }

  mImpl->NodeStack.Push(child);
  mState = ReadingArray;
  return ESerializationError::Ok;
}

ESerializationError TOMLInputArchive::EndArray() {
  if (!mImpl->IsTargetValid() || mState != ReadingArray) {
    LOG_ERROR_TAG("Serialization", "非法的EndArray");
    return ESerializationError::TargetInvalid;
  }

  mImpl->NodeStack.Pop();
  return ESerializationError::Ok;
}

template <typename T> static ESerializationError ReadValueImpl(TOMLInputArchive::Impl* impl, StringView Key, T& Value) {
  if (!impl->IsTargetValid())
    return ESerializationError::TargetInvalid;

  toml::node* Current = impl->CurrentNode();
  const toml::node* Child = TOMLInputArchive::Impl::GetKeyNode(Current, Key);
  if (!Child) {
    LOG_ERROR_TAG("Serialization", "Read: KeyNotFound. Key={}", Key);
    return ESerializationError::TypeMismatch;
  }
  if (!Child->is_value()) {
    LOG_ERROR_TAG("Serialization", "Read: TypeMismatch. Key={}, not a value.", Key);
    return ESerializationError::TypeMismatch;
  }

  auto val = Child->value<T>();
  if (!val)
    return ESerializationError::TypeMismatch;

  Value = *val;
  return ESerializationError::Ok;
}

// 实例化各类 Read
ESerializationError TOMLInputArchive::Read(StringView Key, Int8& Value) {
  int64_t tmp;
  auto err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (err == ESerializationError::Ok)
    Value = static_cast<Int8>(tmp);
  return err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, Int16& Value) {
  int64_t tmp;
  auto err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (err == ESerializationError::Ok)
    Value = static_cast<Int16>(tmp);
  return err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, Int32& Value) {
  int64_t tmp;
  auto err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (err == ESerializationError::Ok)
    Value = static_cast<Int32>(tmp);
  return err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, Int64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value); }
ESerializationError TOMLInputArchive::Read(StringView Key, UInt8& Value) {
  int64_t tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (Err == ESerializationError::Ok)
    Value = static_cast<UInt8>(tmp);
  return Err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, UInt16& Value) {
  int64_t tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (Err == ESerializationError::Ok)
    Value = static_cast<UInt16>(tmp);
  return Err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, UInt32& Value) {
  int64_t tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (Err == ESerializationError::Ok)
    Value = static_cast<UInt32>(tmp);
  return Err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, UInt64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value); }
ESerializationError TOMLInputArchive::Read(StringView Key, Float32& Value) {
  double tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp);
  if (Err == ESerializationError::Ok)
    Value = static_cast<Float32>(tmp);
  return Err;
}
ESerializationError TOMLInputArchive::Read(StringView Key, Float64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value); }
ESerializationError TOMLInputArchive::Read(StringView Key, bool& Value) { return ReadValueImpl(mImpl.Get(), Key, Value); }
ESerializationError TOMLInputArchive::Read(StringView Key, String& Value) {
  if (!mImpl->IsTargetValid())
    return ESerializationError::TargetInvalid;

  toml::node* Current = mImpl->CurrentNode();
  const toml::node* Child = TOMLInputArchive::Impl::GetKeyNode(Current, Key);
  if (!Child)
    return ESerializationError::KeyNotFound;
  if (!Child->is_value())
    return ESerializationError::TypeMismatch;

  auto Val = Child->value<std::string>();
  if (!Val)
    return ESerializationError::TypeMismatch;

  Value = String(*Val);
  return ESerializationError::Ok;
}