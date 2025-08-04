#pragma once
#include "ArrayProxy.h"
#include "Core/Container/Map.h"
#include "Core/Container/Optional.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/String/StringView.h"

///
/// class A{
///   int a;
/// };
/// a的DeclaringType是A
/// a的OwnerType是A
struct Field {
  Field(const StringView Name, const Int32 Offset, const Int32 Size, const struct Type* DeclaringType, const struct Type* OwnerType)
      : mDeclaringType(DeclaringType), mOwnerType(OwnerType), mName(Name), mOffset(Offset), mSize(Size) {}

  ~Field();

  [[nodiscard]] const struct Type* GetDeclaringType() const { return mDeclaringType; }
  [[nodiscard]] const struct Type* GetOwnerType() const { return mOwnerType; }
  [[nodiscard]] StringView GetName() const { return mName; }
  [[nodiscard]] Int32 GetOffset() const { return mOffset; }
  [[nodiscard]] Int32 GetSize() const { return mSize; }

  /// 注册一个属性
  void RegisterAttribute(const String& Key, const String& Value) { mAttributes.Add(Key, Value); }
  /// 是否注册了特定键的属性
  [[nodiscard]] bool IsAttributeRegistered(const StringView Key) const { return mAttributes.Contains(Key); }
  /// 寻找特定键的属性 找不到返回End()
  [[nodiscard]] auto FindAttribute(const StringView Key) const { return mAttributes.Find(Key); }
  /// 获得特定键的属性
  [[nodiscard]] Optional<StringView> GetAttribute(const StringView Key) const {
    if (const auto It = FindAttribute(Key); It != mAttributes.End()) {
      return MakeOptional(It->second);
    }
    return NullOpt;
  }

  /// 设置此类型的属性, Key不应该重复
  void SetAttribute(StringView Key, StringView Value);

  void* GetInstanceAddress(void* Instance) const;

  /// 这个字段是不是一个枚举类型的字段
  [[nodiscard]] bool IsEnumField() const;

  void SetArrayProxy(UniquePtr<ArrayProxy> ArrayProxy) { mArrayProxy = std::move(ArrayProxy); }

private:
  /// 声明此字段的类型, 如果mOwnerType为枚举, 则mDeclaringType为空
  const struct Type* mDeclaringType{nullptr};
  /// 是在哪个类里声明这个字段的?
  const struct Type* mOwnerType{nullptr};
  /// 字段名称
  StringView mName;
  /// 字段在类内的偏移, 如果是枚举字段, 则为-1
  Int32 mOffset{-1};
  /// 这个字段在类内的偏移, mOwnerType为一个枚举, 那么这个字段代表枚举值
  Int32 mSize{0};
  /// 字段的属性
  Map<StringView, StringView> mAttributes;
  /// 字段的ArrayProxy 不为空表示此字段是类型为mDeclaringType的关联容器
  UniquePtr<ArrayProxy> mArrayProxy;
};
