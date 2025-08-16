//
// Created by kita on 25-8-5.
//

#include "YAMLInputArchive.h"

#include "Core/Container/Result.h"
#include "Core/FileSystem/Path.h"

#include "yaml-cpp/yaml.h"

struct YAMLInputArchive::Impl {
  Stack<YAML::Node> NodeStack;
  SizeType CurrentArrayIndex = INVALID_INDEX;

  bool IsTargetValid() { return !NodeStack.Empty(); }

  YAML::Node GetCurrentArrayElement() {
    if (NodeStack.Count() == 0 || CurrentArrayIndex == INVALID_INDEX)
      return {};
    auto& Array = NodeStack.Top();
    if (!Array.IsDefined() || !Array.IsSequence())
      return {};

    const size_t Index = CurrentArrayIndex;
    if (Index >= Array.size())
      return {};

    return Array[Index];
  }
};

YAMLInputArchive::YAMLInputArchive() { mImpl = MakeUnique<Impl>(); }

YAMLInputArchive::~YAMLInputArchive() = default;

ESerializationError YAMLInputArchive::BeginObject(StringView ScopeName) {
  if (mImpl->NodeStack.Empty()) {
    return ESerializationError::TargetInvalid;
  }
  YAML::Node& Current = mImpl->NodeStack.Top();
  if (!Current.IsMap()) {
    LOG_ERROR_TAG("Serialization", "试图从Array读取一个键. Key=\"{}\"", ScopeName);
    return ESerializationError::TypeMismatch;
  }
  const YAML::Node Child = Current[ScopeName.Data()];
  if (!Child.IsDefined()) {
    LOG_ERROR_TAG("Serialization", "找不到键. Key=\"{}\"", ScopeName);
    return ESerializationError::KeyNotFound;
  }
  if (!Child.IsMap()) {
    LOG_ERROR_TAG("Serialization", "键对应的值不是一个Object. Key=\"{}\"", ScopeName);
    return ESerializationError::TypeMismatch;
  }
  mImpl->NodeStack.Push(Child);
  mStateStack.Push(ReadingObject);
  return ESerializationError::Ok;
}

ESerializationError YAMLInputArchive::EndObject() {
  if (mImpl->NodeStack.Empty()) {
    return ESerializationError::TargetInvalid;
  }
  if (mStateStack.Top() != ReadingObject) {
    LOG_ERROR_TAG("Serialization", "试图以EndObject结束一个Array.");
    return ESerializationError::TypeMismatch;
  }
  mImpl->NodeStack.Pop();
  mStateStack.Pop();
  return ESerializationError::Ok;
}

ESerializationError YAMLInputArchive::BeginArray(StringView ScopeName) {
  if (mImpl->NodeStack.Empty()) {
    return ESerializationError::TargetInvalid;
  }
  YAML::Node& Current = mImpl->NodeStack.Top();
  if (!Current.IsMap()) {
    LOG_ERROR_TAG("Serialization", "试图从Array读取一个键. Key=\"{}\"", ScopeName);
    return ESerializationError::TypeMismatch;
  }
  const YAML::Node Child = Current[ScopeName.Data()];
  if (!Child.IsDefined()) {
    LOG_ERROR_TAG("Serialization", "找不到键. Key=\"{}\"", ScopeName);
    return ESerializationError::KeyNotFound;
  }
  if (!Child.IsSequence()) {
    LOG_ERROR_TAG("Serialization", "键对应的值不是一个Array. Key=\"{}\"", ScopeName);
    return ESerializationError::TypeMismatch;
  }
  mImpl->NodeStack.Push(Child);
  mStateStack.Push(ReadingArray);
  mImpl->CurrentArrayIndex = 0;
  return ESerializationError::Ok;
}

ESerializationError YAMLInputArchive::EndArray() {
  if (mImpl->NodeStack.Empty()) {
    return ESerializationError::TargetInvalid;
  }
  if (mStateStack.Top() != ReadingArray) {
    LOG_ERROR_TAG("Serialization", "试图以EndArray结束一个Object.");
    return ESerializationError::TypeMismatch;
  }
  mImpl->NodeStack.Pop();
  mStateStack.Pop();
  mImpl->CurrentArrayIndex = INVALID_INDEX;
  return ESerializationError::Ok;
}

SizeType YAMLInputArchive::GetCurrentArraySize() {
  if (mImpl->NodeStack.Empty()) {
    return INVALID_SIZE;
  }
  const YAML::Node& Current = mImpl->NodeStack.Top();
  if (!Current.IsSequence()) {
    return INVALID_SIZE;
  }
  return Current.size();
}

template <typename T> static ESerializationError ReadValueImpl(YAMLInputArchive::Impl* impl, StringView Key, T& Value, bool IsReadingArray) {
  if (!impl->IsTargetValid())
    return ESerializationError::TargetInvalid;
  if (IsReadingArray) {
    if (!Key.Empty()) {
      LOG_WARN_TAG("Serialization", "读取数组时Key必须为空.");
      return ESerializationError::KeyInvalid;
    }
    const YAML::Node Current = impl->GetCurrentArrayElement();
    if (!Current.IsDefined()) {
      LOG_ERROR_TAG("Serialization", "读取数组元素失败.");
      return ESerializationError::Unknown;
    }
    Value = Current.as<T>();
    impl->CurrentArrayIndex++;
  } else {
    const auto Node = impl->NodeStack.Top()[Key.Data()];
    if (!Node.IsDefined()) {
      LOG_ERROR_TAG("Serialization", "找不到键. Key=\"{}\"", Key);
      return ESerializationError::KeyInvalid;
    }
    Value = Node.as<T>();
  }
  return ESerializationError::Ok;
}

ESerializationError YAMLInputArchive::Read(StringView Key, Int8& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, Int16& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, Int32& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, Int64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, UInt8& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, UInt16& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, UInt32& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, UInt64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, Float32& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, Float64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, bool& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }
ESerializationError YAMLInputArchive::Read(StringView Key, String& Value) {
  std::string StrValue;
  const ESerializationError Error = ReadValueImpl(mImpl.Get(), Key, StrValue, mStateStack.Top() == ReadingArray);
  if (Error == ESerializationError::Ok) {
    Value = StrValue;
  }
  return Error;
}

static Result<YAML::Node, ESerializationError> ParseYAMLFile(StringView Filename) {
  try {
    YAML::Node Root = YAML::LoadFile(Filename.Data());
    return Root;
  } catch (const std::exception& e) {
    LOG_ERROR_TAG("Serialization", "解析YAML文件{}失败: {}", Filename, e.what());
    return ESerializationError::ParseError;
  }
}

ESerializationError YAMLInputArchive::ParseFile(StringView Filename) {
  if (!Filename.EndsWith(".yaml")) {
    LOG_ERROR_TAG("Serialization", "文件后缀名必须为yaml");
    return ESerializationError::TargetInvalid;
  }
  if (!Path::IsExists(Filename)) {
    LOG_ERROR_TAG("Serialization", "YAML文件{}不存在", Filename);
    return ESerializationError::TargetInvalid;
  }
  if (Path::IsDirectory(Filename)) {
    LOG_ERROR_TAG("Serialization", "YAML路径{}为目录", Filename);
    return ESerializationError::TargetInvalid;
  }
  auto Result = ParseYAMLFile(Filename);
  if (Result.HasError()) {
    return Result.Error();
  }
  mImpl->NodeStack.Push(Result.Value());
  mStateStack.Push(ReadingObject);
  return ESerializationError::Ok;
}