//
// Created by kita on 25-8-5.
//

#include "YAMLOutputArchive.h"

#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Path.h"
#include "yaml-cpp/yaml.h"

struct YAMLOutputArchive::Impl {
  Impl() { Emitter << YAML::BeginMap; }

  ~Impl() { Emitter << YAML::EndMap; }

  YAML::Emitter Emitter;
};

YAMLOutputArchive::YAMLOutputArchive() {
  mImpl = MakeUnique<Impl>();
  mStateStack.Push(State::WritingObject);
}

YAMLOutputArchive::~YAMLOutputArchive() = default;

void YAMLOutputArchive::BeginObject(StringView ObjectName) {
  mImpl->Emitter << YAML::Key << ObjectName.Data() << YAML::Value << YAML::BeginMap;
  mStateStack.Push(State::WritingObject);
}
void YAMLOutputArchive::EndObject() {
  mImpl->Emitter << YAML::EndMap;
  mStateStack.Pop();
}

void YAMLOutputArchive::BeginArray(StringView Key) {
  mImpl->Emitter << YAML::Key << Key.Data() << YAML::Value << YAML::BeginSeq;
  mStateStack.Push(State::WritingArray);
}
void YAMLOutputArchive::EndArray() {
  mImpl->Emitter << YAML::EndSeq;
  mStateStack.Pop();
}

template <typename T> static void Write(StringView Key, T Value, YAMLOutputArchive::State State, YAMLOutputArchive::Impl* Impl) {
  if (State == YAMLOutputArchive::WritingArray) {
    if (!Key.Empty()) {
      LOG_WARN_TAG("Serialization", "数组元素不能有Key, 将被忽略.");
    }
    Impl->Emitter << Value;
  } else {
    Impl->Emitter << YAML::Key << Key.Data() << YAML::Value << Value;
  }
}

void YAMLOutputArchive::Write(StringView Key, StringView Value) { ::Write(Key, Value.Data(), mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, Int8 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, Int16 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, Int32 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, Int64 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, UInt8 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, UInt16 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, UInt32 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, UInt64 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, Float32 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, Float64 Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

void YAMLOutputArchive::Write(StringView Key, bool Value) { ::Write(Key, Value, mStateStack.Top(), mImpl.Get()); }

ESerializationError YAMLOutputArchive::WriteFile(StringView Path) {
  if (Path::IsDirectory(Path)) {
    LOG_ERROR_TAG("Serialization", "写入文件路径不能是目录.");
    return ESerializationError::TargetInvalid;
  }
  File::WriteAllText(Path, mImpl->Emitter.c_str());
  return ESerializationError::Ok;
}
