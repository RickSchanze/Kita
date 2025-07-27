#pragma once
#include "Core/String/StringView.h"

///
/// class A{
///   int a;
/// };
/// a的DeclaringType是A
/// a的OwnerType是A
struct Field {
  Field(const StringView Name, const Int32 Offset, const Int32 Size, struct Type* DeclaringType, struct Type* OwnerType)
      : mDeclaringType(DeclaringType), mOwnerType(OwnerType), mName(Name), mOffset(Offset), mSize(Size) {}

  const struct Type* GetDeclaringType() const { return mDeclaringType; }
  const struct Type* GetOwnerType() const { return mOwnerType; }
  StringView GetName() const { return mName; }
  Int32 GetOffset() const { return mOffset; }
  Int32 GetSize() const { return mSize; }

private:
  /// 声明此字段的类型, 如果mOwnerType为枚举, 则mDeclaringType为空
  struct Type* mDeclaringType{nullptr};
  /// 是在哪个类里声明这个字段的?
  struct Type* mOwnerType{nullptr};
  /// 字段名称
  StringView mName;
  /// 字段在类内的偏移
  Int32 mOffset{-1};
  /// 这个字段在类内的偏移, mOwnerType为一个枚举, 那么这个字段代表枚举值
  Int32 mSize{0};
};
