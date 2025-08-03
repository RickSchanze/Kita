#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Container/Optional.h"
#include "Core/Memory/Malloc.h"
#include "Core/String/StringView.h"

struct Type {
  Type() = default;
  Type(const StringView InName, const Int32 InSize, const SizeType InHashCode) : mName(InName), mSize(InSize), mHashCode(InHashCode) {}

  [[nodiscard]] StringView GetName() const { return mName; }
  [[nodiscard]] Int32 GetSize() const {
    if (IsEnum())
      return sizeof(Int32);
    return mSize;
  }
  [[nodiscard]] SizeType GetHashCode() const noexcept { return mHashCode; }
  [[nodiscard]] bool IsEnum() const { return mSize == -1; }
  [[nodiscard]] Array<const Type*> GetBases() const { return mBases; }

  /// @brief 获取此类型的所有字段, 包括基类的字段
  /// @return
  [[nodiscard]] Array<const struct Field*> GetFields() const;

  /// @brief 获取此类型直接声明的字段
  /// @return
  [[nodiscard]] const Array<const struct Field*>& GetDeclaringFields() const { return mFields; }

  /// @brief 检查是否派生自 InBase
  /// @param InBase
  /// @note 当InBase == this时 返回true
  /// @return
  [[nodiscard]] bool IsDerivedFrom(const Type* InBase) const;

  /// @brief 检查是否是 InChild 的父类
  /// @param Child
  /// @return 当Child == this时 返回true
  [[nodiscard]] bool IsParentOf(const Type* Child) const;

  /// 此类型是否有基类
  /// @return
  [[nodiscard]] bool HasParent() const { return !mBases.Empty(); }

  /// 注册一个类成员, 不做校验直接Add
  void RegisterField(const Field* Field);

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

private:
  StringView mName;
  /// 这个类的大小, 当其Size = -1时, 此类型表示枚举
  Int32 mSize{0};
  SizeType mHashCode{};
  Array<const Type*> mBases;
  Array<const Field*> mFields;
  /// 类的元数据属性, 其键值都是StringView说明都必须是字符串字面量
  Map<StringView, StringView> mAttributes;
};

template <typename T> SizeType GetTypeStaticHashCode() {
  static Int8 HashCodeHolder{};
  return reinterpret_cast<SizeType>(&HashCodeHolder);
}

/// @brief 创建一个类型的Type 不负责生命周期, 请注册至TypeRegistry或者自己进行管理
/// @tparam T
/// @param Name
/// @return
template <typename T> Type* CreateType(StringView Name) {
  auto HashCode = GetTypeStaticHashCode<T>();
  Int32 Size = sizeof(T);
  Type* NewType = New<Type>(Name, Size, HashCode);
  return NewType;
}

template <Traits::IsEnum T> Type* CreateType(StringView Name) {
  auto HashCode = GetTypeStaticHashCode<T>();
  Int32 Size = -1;
  Type* NewType = New<Type>(Name, Size, HashCode);
  return NewType;
}
