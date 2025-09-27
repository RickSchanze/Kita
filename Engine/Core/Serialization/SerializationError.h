#pragma once

class InputArchive;
class OutputArchive;
enum class ESerializationError {
  TargetInvalid,
  ParseError,
  KeyNotFound,
  KeyInvalid,
  TypeMismatch,
  Unknown,
  Ok, // 成功
};

template <typename T1, typename T2> struct KeyValuePair {
  T1 Key;
  T2 Value;
};
