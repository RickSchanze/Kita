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
    const auto Result = File::ReadAllText(Path);
    if (!Result) {
      switch (Result.Error()) {
      case EFileSystemError::FileNotFound:
      case EFileSystemError::ReadDirectory:
        return ESerializationError::TargetInvalid;
      default:
        return ESerializationError::TargetInvalid;
      }
    }
    try {
      Root = toml::parse(Result.Value().GetStdString());
      NodeStack.Push(&Root);
      return ESerializationError::Count;
    } catch (toml::parse_error& e) {
      LOG_ERROR_TAG("Serialization", "Parse toml file error: {}", e.what());
      return ESerializationError::ParseError;
    }
  }

  bool IsTargetValid() { return NodeStack.Count() > 0; }

  toml::node* CurrentNode() { return NodeStack.Top(); }
};

ESerializationError TOMLInputArchive::ParseFile(const StringView Path) { return mImpl->ParseFile(Path); }