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
  SizeType CurrentArrayIndex = INVALID_INDEX;

  Impl() = default;
  ESerializationError ParseFile(const StringView Path) {
    if (Path::IsDirectory(Path)) {
      gLogger.Error("Serialization", "Path must not be a directory.");
      return ESerializationError::TargetInvalid;
    }
    if (!Path::IsExists(Path)) {
      gLogger.Error("Serialization", "Path must exists.");
      return ESerializationError::TargetInvalid;
    }
    try {
      std::ifstream FS(Path.Data());
      Root = toml::parse(FS);
      NodeStack.Push(&Root);
      return ESerializationError::Ok;
    } catch (toml::parse_error& e) {
      gLogger.Error("Serialization", "Parse toml file error: {}", e.what());
      return ESerializationError::ParseError;
    }
  }

  bool IsTargetValid() { return NodeStack.Count() > 0; }

  toml::node* CurrentNode() { return NodeStack.Top(); }

  static toml::node* GetKeyNode(toml::node* Parent, const StringView Key) {
    if (!Parent)
      return nullptr;
    if (Parent->is_table()) {
      auto* Table = Parent->as_table();
      const auto It = Table->find(Key.GetStdStringView());
      if (It != Table->end()) {
        return &It->second;
      }
    }
    return nullptr;
  }

  toml::node* GetCurrentArrayElement() {
    if (NodeStack.Count() == 0 || CurrentArrayIndex == INVALID_INDEX)
      return nullptr;
    auto* Array = NodeStack.Top()->as_array();
    if (!Array)
      return nullptr;

    const size_t Index = CurrentArrayIndex;
    if (Index >= Array->size())
      return nullptr;

    return Array->get(Index);
  }
};

ESerializationError TOMLInputArchive::ParseFile(const StringView Path) {
  const ESerializationError Error = mImpl->ParseFile(Path);
  if (Error == ESerializationError::Ok) {
    mStateStack.Push(ReadingObject);
  }
  return Error;
}

TOMLInputArchive::TOMLInputArchive() { mImpl = MakeUnique<Impl>(); }
TOMLInputArchive::~TOMLInputArchive() = default;
ESerializationError TOMLInputArchive::BeginObject(const StringView ScopeName) {
  if (!mImpl->IsTargetValid())
    return ESerializationError::TargetInvalid;

  toml::node* Current = mImpl->CurrentNode();
  toml::node* Child = TOMLInputArchive::Impl::GetKeyNode(Current, ScopeName);
  if (!Child) {
    gLogger.Warn("Serialization", "BeginObject: KeyNotFound. Key=\"{}\"", ScopeName);
    return ESerializationError::KeyNotFound;
  }
  if (!Child->is_table()) {
    gLogger.Error("Serialization", "BeginObject: TypeMismatch. Key \"{}\" is not a table.", ScopeName);
    return ESerializationError::TypeMismatch;
  }

  mImpl->NodeStack.Push(Child);
  mStateStack.Push(ReadingObject);
  return ESerializationError::Ok;
}

ESerializationError TOMLInputArchive::EndObject() {
  if (!mImpl->IsTargetValid() || mStateStack.Top() != ReadingObject) {
    gLogger.Error("Serialization", "非法的EndObject");
    return ESerializationError::TargetInvalid;
  }

  mImpl->NodeStack.Pop();
  mStateStack.Pop();
  return ESerializationError::Ok;
}

ESerializationError TOMLInputArchive::BeginArray(StringView ScopeName) {
  if (!mImpl->IsTargetValid())
    return ESerializationError::TargetInvalid;

  toml::node* current = mImpl->CurrentNode();
  toml::node* child = TOMLInputArchive::Impl::GetKeyNode(current, ScopeName);
  if (!child) {
    gLogger.Error("Serialization", "BeginArray: KeyNotFound. Key=\"{}\"", ScopeName);
    return ESerializationError::KeyNotFound;
  }
  if (!child->is_array()) {
    gLogger.Error("Serialization", "BeginArray: TypeMismatch. Key=\"{}\", not a array.", ScopeName);
    return ESerializationError::TypeMismatch;
  }

  mImpl->NodeStack.Push(child);
  mStateStack.Push(ReadingArray);
  mImpl->CurrentArrayIndex++;
  return ESerializationError::Ok;
}

ESerializationError TOMLInputArchive::EndArray() {
  if (!mImpl->IsTargetValid() || mStateStack.Top() != ReadingArray) {
    gLogger.Error("Serialization", "非法的EndArray");
    return ESerializationError::TargetInvalid;
  }

  mImpl->NodeStack.Pop();
  mImpl->CurrentArrayIndex = INVALID_INDEX;
  mStateStack.Pop();
  return ESerializationError::Ok;
}

template <typename T> static ESerializationError ReadValueImpl(TOMLInputArchive::Impl* impl, StringView Key, T& Value, bool IsReadingArray) {
  if (!impl->IsTargetValid())
    return ESerializationError::TargetInvalid;
  toml::node* Child = nullptr;
  if (IsReadingArray) {
    if (!Key.Empty()) {
      gLogger.Error("Serialization", "读取数组时Key必须为空");
      return ESerializationError::KeyInvalid;
    }
    Child = impl->GetCurrentArrayElement();
  } else {
    Child = TOMLInputArchive::Impl::GetKeyNode(impl->CurrentNode(), Key);
  }

  if (!Child) {
    gLogger.Warn("Serialization", "Read: KeyNotFound. Key=[{}]", Key);
    return ESerializationError::TypeMismatch;
  }
  if (!Child->is_value()) {
    gLogger.Error("Serialization", "Read: TypeMismatch. Key=[{}], not a value.", Key);
    return ESerializationError::TypeMismatch;
  }

  auto val = Child->value<T>();
  if (!val)
    return ESerializationError::TypeMismatch;

  Value = *val;

  if (IsReadingArray) {
    impl->CurrentArrayIndex++;
  }
  return ESerializationError::Ok;
}

// 实例化各类 Read
ESerializationError TOMLInputArchive::Read(StringView Key, Int8& Value) {
  int64_t tmp;
  auto err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (err == ESerializationError::Ok)
    Value = static_cast<Int8>(tmp);
  return err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, Int16& Value) {
  int64_t tmp;
  auto err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (err == ESerializationError::Ok)
    Value = static_cast<Int16>(tmp);
  return err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, Int32& Value) {
  int64_t tmp;
  auto err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (err == ESerializationError::Ok)
    Value = static_cast<Int32>(tmp);
  return err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, Int64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }

ESerializationError TOMLInputArchive::Read(StringView Key, UInt8& Value) {
  int64_t tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (Err == ESerializationError::Ok)
    Value = static_cast<UInt8>(tmp);
  return Err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, UInt16& Value) {
  int64_t tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (Err == ESerializationError::Ok)
    Value = static_cast<UInt16>(tmp);
  return Err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, UInt32& Value) {
  int64_t tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (Err == ESerializationError::Ok)
    Value = static_cast<UInt32>(tmp);
  return Err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, UInt64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }

ESerializationError TOMLInputArchive::Read(StringView Key, Float32& Value) {
  double tmp;
  auto Err = ReadValueImpl(mImpl.Get(), Key, tmp, mStateStack.Top() == ReadingArray);
  if (Err == ESerializationError::Ok)
    Value = static_cast<Float32>(tmp);
  return Err;
}

ESerializationError TOMLInputArchive::Read(StringView Key, Float64& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }

ESerializationError TOMLInputArchive::Read(StringView Key, bool& Value) { return ReadValueImpl(mImpl.Get(), Key, Value, mStateStack.Top() == ReadingArray); }

ESerializationError TOMLInputArchive::Read(StringView Key, String& Value) {
  if (!mImpl->IsTargetValid())
    return ESerializationError::TargetInvalid;
  toml::node* Child = nullptr;
  if (mStateStack.Top() == ReadingArray) {
    if (!Key.Empty()) {
      gLogger.Error("Serialization", "读取数组时Key必须为空");
      return ESerializationError::KeyInvalid;
    }
    Child = mImpl->GetCurrentArrayElement();
  } else {
    Child = TOMLInputArchive::Impl::GetKeyNode(mImpl->CurrentNode(), Key);
  }
  if (!Child)
    return ESerializationError::KeyNotFound;
  if (!Child->is_value())
    return ESerializationError::TypeMismatch;

  auto Val = Child->value<std::string>();
  if (!Val)
    return ESerializationError::TypeMismatch;

  Value = String(*Val);

  if (mStateStack.Top() == ReadingArray) {
    mImpl->CurrentArrayIndex++;
  }
  return ESerializationError::Ok;
}

SizeType TOMLInputArchive::GetCurrentArraySize() {
  auto* Array = mImpl->CurrentNode()->as_array();
  if (Array == nullptr) {
    return INVALID_SIZE;
  }
  return Array->size();
}