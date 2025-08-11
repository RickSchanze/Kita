#pragma once

enum class ESerializationError {
  TargetInvalid,
  ParseError,
  KeyNotFound,
  TypeMismatch,
  Unknown,
  Ok, // 成功
};
