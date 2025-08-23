#pragma once
#include "Core/Container/Array.h"
#include "Field.h"
#include "TemplateAdapter.h"
#include "Type.h"

struct Field;
struct Type;

class TypeRegistry : NonCopyable, NonMovable {
public:
  TypeRegistry();
  ~TypeRegistry();

  void RegisterType(const Type* Type);
  [[nodiscard]] const Type* GetType(const StringView& TypeName) const;
  [[nodiscard]] const Type* GetType(SizeType& HashCode) const;

private:
  Array<const Type*> mRegisteredTypes;
};

inline TypeRegistry gTypeRegistry;

inline TypeRegistry& GetTypeRegistry() { return gTypeRegistry; }

namespace Traits {
template <typename T>
concept HasFuncIsReflected = requires {
  { T::IsReflected() } -> std::convertible_to<bool>;
};

template <typename T>
concept IsReflected = HasFuncIsReflected<T> && T::IsReflected();
} // namespace Traits

template <typename T> const Type* TypeOf() {
  SizeType HashCode = GetTypeStaticHashCode<T>();
  return GetTypeRegistry().GetType(HashCode);
}

struct TypeBuilder {
  template <typename T> TypeBuilder& CreateType(const StringView Name) {
    OperatingType = ::CreateType<T>(Name);
    return *this;
  }

  /// 添加一个反射字段
  /// @warning 不做任何验证, 请保证OperatingType不为空且不会重复添加
  template <typename ClassType, typename FieldType> TypeBuilder& AddField(const StringView Name, FieldType ClassType::* FieldPtr) {
    Int32 FieldOffset = static_cast<Int32>(reinterpret_cast<SizeType>(&(static_cast<ClassType*>(nullptr)->*FieldPtr)));
    Int32 FieldSize = sizeof(FieldType);
    const Type* DeclaringType = nullptr;
    if constexpr (Traits::IsBaseOf<TemplateAdapter, FieldType>) {
      DeclaringType = TypeOf<typename FieldType::ReflectionType>();
    } else {
      DeclaringType = TypeOf<FieldType>();
    }
    const Type* OwnerType = OperatingType;
    auto* NewField = New<Field>(Name, FieldOffset, FieldSize, DeclaringType, OwnerType);
    NewField->SetArrayProxy(CreateArrayProxy<FieldType>());
    NewField->SetMapProxy(CreateMapProxy<FieldType>());
    NewField->CorrectSelf();
    LastOperatingField = NewField;
    OperatingType->RegisterField(NewField);
    return *this;
  }

  TypeBuilder& AddField(const Field* NewField) {
    OperatingType->RegisterField(NewField);
    return *this;
  }

  /// 添加一个枚举字段
  /// @warning 不做任何验证, 请保证OperatingType不为空, 为枚举类型且不会重复添加
  template <Traits::IsEnum ClassType> TypeBuilder& AddField(const StringView Name, ClassType EnumValue) {
    Int32 FieldOffset = -1;
    Int32 FieldSize = static_cast<Int32>(EnumValue);
    // TODO: 这里先暂时认定所有注册的反射Enum都是Int32类型, 之后再支持其他类型
    const Type* DeclaringType = TypeOf<Int32>();
    const Type* OwnerType = OperatingType;
    auto* NewField = New<Field>(Name, FieldOffset, FieldSize, DeclaringType, OwnerType);
    LastOperatingField = NewField;
    OperatingType->RegisterField(NewField);
    return *this;
  }

  TypeBuilder& SetFieldAttribute(const StringView Key, const StringView Value) {
    if (LastOperatingField) {
      LastOperatingField->SetAttribute(Key, Value);
    }
    return *this;
  }

  TypeBuilder& SetTypeAttribute(const StringView Key, const StringView Value) {
    if (OperatingType) {
      OperatingType->SetAttribute(Key, Value);
    }
    return *this;
  }

  void Register() const { GetTypeRegistry().RegisterType(OperatingType); }

  template <typename T> void AddParent() const {
    if constexpr (Traits::IsReflected<T>) {
      const Type* Base = TypeOf<T>();
      OperatingType->AddParent(Base);
    }
  }

  /// 当前正在构建的类型
  Type* OperatingType;
  /// 最后一个添加的字段, 可以用于为字段设置属性
  Field* LastOperatingField;
};
