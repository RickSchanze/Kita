#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Span.h"
#include "Core/Memory/Malloc.h"
#include "Core/String/StringView.h"

struct Type {
  Type(const char* InName, const Int32 InSize, const bool InIsEnum, const SizeType InHashCode) : mName(InName), mSize(InSize), mHashCode(InHashCode), mIsEnum(InIsEnum) {}

  StringView GetName() const { return mName; }
  Int32 GetSize() const { return mSize; }
  SizeType GetHashCode() const noexcept { return mHashCode; }
  bool IsEnum() const { return mIsEnum; }
  Array<const Type*> GetBases() const { return mBases; }

  /// @brief 获取此类型的所有字段, 包括基类的字段
  /// @return
  Array<const struct Field*> GetFields() const;

  /// @brief 获取此类型直接声明的字段
  /// @return
  const Array<const struct Field*>& GetDeclaringFields() const { return mFields; }

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

private:
  StringView mName;
  Int32 mSize{0};
  SizeType mHashCode;
  Array<const Type*> mBases;
  Array<const struct Field*> mFields;
  bool mIsEnum;
};

namespace Pri {
void RegisterTypeToRegistry(const Type* Type);
}

template <typename T> const Type* CreateType(const char* Name) {
  static SizeType HashCodeHolder{};
  SizeType HashCode = reinterpret_cast<SizeType>(&HashCodeHolder);
  bool IsEnum = std::is_enum_v<T>;
  SizeType Size = sizeof(T);
  const Type* NewType = New<Type>(Name, Size, IsEnum, HashCode);
  Pri::RegisterTypeToRegistry(NewType);
  return NewType;
}
