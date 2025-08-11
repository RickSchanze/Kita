//
// Created by kita on 25-8-9.
//

#include "TOMLOutputArchive.h"

#include "Core/Assert.h"
#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Path.h"
#include "Core/Logging/Logger.hpp"
#include "toml++/toml.hpp"

struct TOMLOutputArchive::Impl {
  toml::table Root;
  Stack<toml::node*> NodeStack;

  toml::node* CurrentNode() { return NodeStack.Top(); }

  Impl() { NodeStack.Push(&Root); }
};

TOMLOutputArchive::TOMLOutputArchive() { mImpl = MakeUnique<Impl>(); }

void TOMLOutputArchive::BeginObject(const StringView ObjectName) {
  auto* Parent = mImpl->CurrentNode();
  if (const auto Table = Parent->as_table()) {
    toml::table Object;
    auto& NewObject = Table->insert_or_assign(ObjectName.Data(), std::move(Object)).first->second;
    mImpl->NodeStack.Push(&NewObject);
  } else if (const auto Array = Parent->as_array()) {
    toml::table Object;
    auto& NewObject = Array->emplace_back(std::move(Object));
    mImpl->NodeStack.Push(NewObject.as_table());
  } else {
    ASSERT_MSG(false, "Parent is not a table or array.");
  }
}

void TOMLOutputArchive::EndObject() {
  ASSERT_MSG(mImpl->NodeStack.Count() > 1, "No object to end.");
  mImpl->NodeStack.Pop();
}

void TOMLOutputArchive::BeginArray(const StringView Key) {
  const auto Parent = mImpl->CurrentNode();
  if (const auto Table = Parent->as_table()) {
    toml::array NewArray;
    auto& arr_node = Table->insert_or_assign(Key.Data(), std::move(NewArray)).first->second;
    mImpl->NodeStack.Push(arr_node.as_array());
  } else if (const auto Array = Parent->as_array()) {
    toml::array NewArray;
    auto& ArrayNode = Array->emplace_back(std::move(NewArray));
    mImpl->NodeStack.Push(ArrayNode.as_array());
  } else {
    ASSERT_MSG(false, "Parent is not a table or array.");
  }
}

void TOMLOutputArchive::EndArray() {
  ASSERT_MSG(mImpl->NodeStack.Count() > 1, "No array to end.");
  mImpl->NodeStack.Pop();
}

template <typename T> void SetValue(toml::node* CurrentNode, const StringView Key, T&& Value, const TOMLOutputArchive::ArchiveState State) {
  if (State == TOMLOutputArchive::ArrayWriting) {
    CurrentNode->as_array()->emplace_back(std::forward<T>(Value));
  } else if (State == TOMLOutputArchive::ObjectWriting) {
    CurrentNode->as_table()->insert_or_assign(Key.Data(), std::forward<T>(Value));
  }
}

void TOMLOutputArchive::Write(const StringView Key, const StringView Value) { SetValue(mImpl->CurrentNode(), Key, Value.Data(), mState); }
void TOMLOutputArchive::Write(const StringView Key, Int8 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, Int16 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, Int32 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, Int64 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, UInt8 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, UInt16 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, UInt32 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
// TOML++: 整数值必须被无损转换到int64_t
void TOMLOutputArchive::Write(const StringView Key, const UInt64 Value) { SetValue(mImpl->CurrentNode(), Key, static_cast<Int64>(Value), mState); }
void TOMLOutputArchive::Write(const StringView Key, Float32 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, Float64 Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }
void TOMLOutputArchive::Write(const StringView Key, bool Value) { SetValue(mImpl->CurrentNode(), Key, Value, mState); }

ESerializationError TOMLOutputArchive::WriteFile(const StringView Path) {
  if (!Path.EndsWith(".toml")) {
    LOG_ERROR_TAG("Serialization", "Path must end with .toml.");
    return ESerializationError::TargetInvalid;
  }
  if (Path::IsDirectory(Path)) {
    LOG_ERROR_TAG("Serialization", "Path must not be a directory.");
    return ESerializationError::TargetInvalid;
  }
  std::fstream FS(Path.Data(), std::ios::out);
  std::stringstream Stream;
  Stream << toml::toml_formatter(mImpl->Root);
  const std::string Content = Stream.str();
  File::WriteAllText(Path, Content);
  return ESerializationError::Ok;
}
