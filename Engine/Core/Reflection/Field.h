#pragma once
#include "ArrayProxy.h"
#include "Core/Container/Map.h"
#include "Core/Container/Optional.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/String/StringView.h"
#include "MapProxy.h"

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

  [[nodiscard]] Result<const Type*, EReflectionError> GetDeclaringType() const {
    if (mMapProxy)
      return EReflectionError::IsMap;
    if (mArrayProxy)
      return EReflectionError::IsArray;
    return mDeclaringType;
  }

  [[nodiscard]] const Type* GetOwnerType() const { return mOwnerType; }

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
  void SetMapProxy(UniquePtr<MapProxy> MapProxy) { mMapProxy = std::move(MapProxy); }

  void CorrectSelf() {
    if (mArrayProxy || mMapProxy) {
      mDeclaringType = nullptr;
    }
  }

  [[nodiscard]] Result<const Type*, EReflectionError> GetArrayElementType() const {
    if (mArrayProxy) {
      return mArrayProxy->GetElementType();
    }
    return EReflectionError::NotArray;
  }

  [[nodiscard]] Result<const Type*, EReflectionError> GetMapKeyType() const {
    if (mMapProxy) {
      return mMapProxy->GetKeyType();
    }
    return EReflectionError::NotMap;
  }

  [[nodiscard]] Result<const Type*, EReflectionError> GetMapValueType() const {
    if (mMapProxy) {
      return mMapProxy->GetValueType();
    }
    return EReflectionError::NotMap;
  }

private:
  /// 声明此字段的类型, 如果mOwnerType为枚举, 则mDeclaringType为空
  /// 当ArrayProxy或者MapProxy不为空时, mDeclaringType为空
  const struct Type* mDeclaringType{nullptr};
  /// 是在哪个类里声明这个字段的?
  const struct Type* mOwnerType{nullptr};
  /// 字段名称
  StringView mName;
  /// 字段在类内的偏移, 如果是枚举字段, 则为-1
  Int32 mOffset{-1};
  /// 这个字段在类内的大小, mOwnerType为一个枚举, 那么这个字段代表枚举值
  Int32 mSize{0};
  /// 字段的属性
  Map<StringView, StringView> mAttributes;
  /// 不为空表示此字段是一个Array容器
  UniquePtr<ArrayProxy> mArrayProxy;
  /// 不为空表示此字段是一个Map容器
  UniquePtr<MapProxy> mMapProxy;
};
