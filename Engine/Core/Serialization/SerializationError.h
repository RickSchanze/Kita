#pragma once

enum class ESerializationError {
  TargetInvalid,
  ParseError,
  KeyNotFound,
  KeyInvalid,
  TypeMismatch,
  Unknown,
  Ok, // 成功
};
