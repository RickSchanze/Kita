#pragma once
/// Vector系列: Vector4 Vector3 Vector2

#pragma once
#include "Core/Macros.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Traits.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

// =================== Vector2 ===================
template <typename T> struct Vector2 : InlinedOutput {
  glm::vec<2, T> Data;

  FORCE_INLINE constexpr Vector2(T X = {}, T Y = {}) : Data(X, Y) {}
  FORCE_INLINE constexpr Vector2(const glm::vec<2, T>& Vec) : Data(Vec) {}

  // Getter
  FORCE_INLINE constexpr T& X() { return Data.x; }
  FORCE_INLINE constexpr T& Y() { return Data.y; }
  FORCE_INLINE constexpr const T& X() const { return Data.x; }
  FORCE_INLINE constexpr const T& Y() const { return Data.y; }

  // Setter
  FORCE_INLINE constexpr Vector2& SetX(T Value) {
    Data.x = Value;
    return *this;
  }
  FORCE_INLINE constexpr Vector2& SetY(T Value) {
    Data.y = Value;
    return *this;
  }

  // Index
  FORCE_INLINE constexpr T& operator[](size_t Index) { return Data[Index]; }
  FORCE_INLINE constexpr const T& operator[](size_t Index) const { return Data[Index]; }

  // Conversion
  FORCE_INLINE constexpr operator glm::vec<2, T>&() { return Data; }
  FORCE_INLINE constexpr operator const glm::vec<2, T>&() const { return Data; }

  // Operators
  FORCE_INLINE constexpr Vector2 operator+(const Vector2& Other) const { return Data + Other.Data; }
  FORCE_INLINE constexpr Vector2 operator-(const Vector2& Other) const { return Data - Other.Data; }
  FORCE_INLINE constexpr Vector2 operator*(T Scalar) const { return Data * Scalar; }
  FORCE_INLINE constexpr Vector2 operator/(T Scalar) const { return Data / Scalar; }
  FORCE_INLINE constexpr Vector2 operator*(const Vector2& Other) const { return Data * Other.Data; }
  FORCE_INLINE constexpr Vector2 operator/(const Vector2& Other) const { return Data / Other.Data; }

  FORCE_INLINE constexpr bool operator==(const Vector2& Other) const { return Data == Other.Data; }
  FORCE_INLINE constexpr bool operator!=(const Vector2& Other) const { return Data != Other.Data; }

  // Member Functions
  FORCE_INLINE constexpr T Length() const
    requires Traits::IsFloatNumber<T>
  {
    return glm::length(Data);
  }

  FORCE_INLINE constexpr Vector2 Normalize() const
    requires Traits::IsFloatNumber<T>
  {
    return glm::normalize(Data);
  }

  [[nodiscard]] String ToString() const { return Format("[{}, {}]", X(), Y()); }

  void WriteArchive(OutputArchive& Archive) const {
    Archive.WriteType("X", X());
    Archive.WriteType("Y", Y());
  }

  void ReadArchive(InputArchive& Archive) {
    Archive.ReadType("X", X());
    Archive.ReadType("Y", Y());
  }
};

#define VECTOR2_DECL(Type, Name)                                                                                                 \
  class Z_Vector2_REFL_REGISTER_WRAPPER_##Type {                                                                                 \
    struct Z_Reflection_Vector2_Register_##Type {                                                                                \
      Z_Reflection_Vector2_Register_##Type() {                                                                                   \
        TypeBuilder Builder{};                                                                                                   \
        Builder.CreateType<Vector2<##Type>>(Name);                                                                               \
        const Field* XField = New<Field>("X", OffsetOf(&glm::vec2::x), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(XField);                                                                                                \
        const Field* YField = New<Field>("Y", OffsetOf(&glm::vec2::y), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(YField);                                                                                                \
        Builder.Register();                                                                                                      \
      }                                                                                                                          \
    };                                                                                                                           \
    static inline Z_Reflection_Vector2_Register_##Type Z_Reflection_Vector2_Register___##Type{};                                 \
  };

VECTOR2_DECL(float, "Vector2f");
typedef Vector2<float> Vector2f;
VECTOR2_DECL(double, "Vector2d");
typedef Vector2<double> Vector2d;
VECTOR2_DECL(Int32, "Vector2i");
typedef Vector2<Int32> Vector2i;
VECTOR2_DECL(UInt32, "Vector2u");
typedef Vector2<UInt32> Vector2u;
#undef VECTOR2_DECL

// =================== Vector3 ===================
template <typename T> struct Vector3 : InlinedOutput {
  glm::vec<3, T> Data;

  FORCE_INLINE constexpr Vector3(T X = 0, T Y = 0, T Z = 0) : Data(X, Y, Z) {}
  FORCE_INLINE constexpr Vector3(const glm::vec<3, T>& Vec) : Data(Vec) {}

  FORCE_INLINE constexpr T& X() { return Data.x; }
  FORCE_INLINE constexpr T& Y() { return Data.y; }
  FORCE_INLINE constexpr T& Z() { return Data.z; }
  FORCE_INLINE constexpr const T& X() const { return Data.x; }
  FORCE_INLINE constexpr const T& Y() const { return Data.y; }
  FORCE_INLINE constexpr const T& Z() const { return Data.z; }

  FORCE_INLINE constexpr Vector3& SetX(T Value) {
    Data.x = Value;
    return *this;
  }
  FORCE_INLINE constexpr Vector3& SetY(T Value) {
    Data.y = Value;
    return *this;
  }
  FORCE_INLINE constexpr Vector3& SetZ(T Value) {
    Data.z = Value;
    return *this;
  }

  FORCE_INLINE constexpr T& operator[](size_t Index) { return Data[Index]; }
  FORCE_INLINE constexpr const T& operator[](size_t Index) const { return Data[Index]; }

  FORCE_INLINE constexpr operator glm::vec<3, T>&() { return Data; }
  FORCE_INLINE constexpr operator const glm::vec<3, T>&() const { return Data; }

  FORCE_INLINE constexpr Vector3 operator+(const Vector3& Other) const { return Data + Other.Data; }
  FORCE_INLINE constexpr Vector3 operator-(const Vector3& Other) const { return Data - Other.Data; }
  FORCE_INLINE constexpr Vector3 operator*(T Scalar) const { return Data * Scalar; }
  FORCE_INLINE constexpr Vector3 operator/(T Scalar) const { return Data / Scalar; }
  FORCE_INLINE constexpr Vector3 operator*(const Vector3& Other) const { return Data * Other.Data; }
  FORCE_INLINE constexpr Vector3 operator/(const Vector3& Other) const { return Data / Other.Data; }

  FORCE_INLINE constexpr bool operator==(const Vector3& Other) const { return Data == Other.Data; }
  FORCE_INLINE constexpr bool operator!=(const Vector3& Other) const { return Data != Other.Data; }

  // Member Functions
  FORCE_INLINE constexpr T Length() const
    requires Traits::IsFloatNumber<T>
  {
    return glm::length(Data);
  }

  FORCE_INLINE constexpr Vector3 Normalize() const
    requires Traits::IsFloatNumber<T>
  {
    return glm::normalize(Data);
  }

  [[nodiscard]] String ToString() const { return Format("[{}, {}, {}]", X(), Y(), Z()); }
};

#define VECTOR3_DECL(Type, Name)                                                                                                 \
  class Z_Vector3_Refl_Register_Wrapper_##Type {                                                                                 \
    struct Z_Reflection_Vector3_Register_##Type {                                                                                \
      Z_Reflection_Vector3_Register_##Type() {                                                                                   \
        TypeBuilder Builder{};                                                                                                   \
        Builder.CreateType<Vector3<##Type>>(Name);                                                                               \
        const Field* XField = New<Field>("X", OffsetOf(&glm::vec3::x), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(XField);                                                                                                \
        const Field* YField = New<Field>("Y", OffsetOf(&glm::vec3::y), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(YField);                                                                                                \
        const Field* ZField = New<Field>("Z", OffsetOf(&glm::vec3::z), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(ZField);                                                                                                \
        Builder.Register();                                                                                                      \
      }                                                                                                                          \
    };                                                                                                                           \
    static inline Z_Reflection_Vector3_Register_##Type Z_Reflection_Vector3_Register___##Type{};                                 \
  };

VECTOR3_DECL(float, "Vector3f");
typedef Vector3<float> Vector3f;
VECTOR3_DECL(double, "Vector3d");
typedef Vector3<double> Vector3d;
#undef VECTOR3_DECL

// =================== Vector4 ===================
template <typename T> struct Vector4 : InlinedOutput {
  glm::vec<4, T> Data;

  constexpr Vector4() = default;
  FORCE_INLINE constexpr Vector4(T X, T Y, T Z, T W) : Data(X, Y, Z, W) {}
  FORCE_INLINE constexpr Vector4(const glm::vec<4, T>& Vec) : Data(Vec) {}

  FORCE_INLINE constexpr T& X() { return Data.x; }
  FORCE_INLINE constexpr T& Y() { return Data.y; }
  FORCE_INLINE constexpr T& Z() { return Data.z; }
  FORCE_INLINE constexpr T& W() { return Data.w; }
  FORCE_INLINE constexpr const T& X() const { return Data.x; }
  FORCE_INLINE constexpr const T& Y() const { return Data.y; }
  FORCE_INLINE constexpr const T& Z() const { return Data.z; }
  FORCE_INLINE constexpr const T& W() const { return Data.w; }

  FORCE_INLINE constexpr Vector4& SetX(T Value) {
    Data.x = Value;
    return *this;
  }
  FORCE_INLINE constexpr Vector4& SetY(T Value) {
    Data.y = Value;
    return *this;
  }
  FORCE_INLINE constexpr Vector4& SetZ(T Value) {
    Data.z = Value;
    return *this;
  }
  FORCE_INLINE constexpr Vector4& SetW(T Value) {
    Data.w = Value;
    return *this;
  }

  FORCE_INLINE constexpr T& operator[](size_t Index) { return Data[Index]; }
  FORCE_INLINE constexpr const T& operator[](size_t Index) const { return Data[Index]; }

  FORCE_INLINE constexpr operator glm::vec<4, T>&() { return Data; }
  FORCE_INLINE constexpr operator const glm::vec<4, T>&() const { return Data; }

  FORCE_INLINE constexpr Vector4 operator+(const Vector4& Other) const { return Data + Other.Data; }
  FORCE_INLINE constexpr Vector4 operator-(const Vector4& Other) const { return Data - Other.Data; }
  FORCE_INLINE constexpr Vector4 operator*(T Scalar) const { return Data * Scalar; }
  FORCE_INLINE constexpr Vector4 operator/(T Scalar) const { return Data / Scalar; }
  FORCE_INLINE constexpr Vector4 operator*(const Vector4& Other) const { return Data * Other.Data; }
  FORCE_INLINE constexpr Vector4 operator/(const Vector4& Other) const { return Data / Other.Data; }

  FORCE_INLINE constexpr bool operator==(const Vector4& Other) const { return Data == Other.Data; }
  FORCE_INLINE constexpr bool operator!=(const Vector4& Other) const { return Data != Other.Data; }

  // Member Functions
  FORCE_INLINE constexpr T Length() const
    requires Traits::IsFloatNumber<T>
  {
    return glm::length(Data);
  }

  FORCE_INLINE constexpr Vector4 Normalize() const
    requires Traits::IsFloatNumber<T>
  {
    return glm::normalize(Data);
  }

  [[nodiscard]] String ToString() const { return Format("[{}, {}, {}, {}]", X(), Y(), Z(), W()); }

  void WriteArchive(OutputArchive& Archive) const {
    Archive.WriteType("X", X());
    Archive.WriteType("Y", Y());
    Archive.WriteType("Z", Z());
    Archive.WriteType("W", W());
  }

  void ReadArchive(InputArchive& Archive) {
    Archive.ReadType("X", X());
    Archive.ReadType("Y", Y());
    Archive.ReadType("Z", Z());
    Archive.ReadType("W", W());
  }
};

#define VECTOR4_DECL(Type, Name)                                                                                                 \
  class Z_Vector4_REFL_REGISTER_WRAPPER_##Type {                                                                                 \
    struct Z_Reflection_Vector4_Register_##Type {                                                                                \
      Z_Reflection_Vector4_Register_##Type() {                                                                                   \
        TypeBuilder Builder{};                                                                                                   \
        Builder.CreateType<Vector4<##Type>>(Name);                                                                               \
        const Field* XField = New<Field>("X", OffsetOf(&glm::vec4::x), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(XField);                                                                                                \
        const Field* YField = New<Field>("Y", OffsetOf(&glm::vec4::y), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(YField);                                                                                                \
        const Field* ZField = New<Field>("Z", OffsetOf(&glm::vec4::z), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(ZField);                                                                                                \
        const Field* WField = New<Field>("W", OffsetOf(&glm::vec4::w), sizeof(##Type), TypeOf<##Type>(), Builder.OperatingType); \
        Builder.AddField(WField);                                                                                                \
        Builder.Register();                                                                                                      \
      }                                                                                                                          \
    };                                                                                                                           \
    static inline Z_Reflection_Vector4_Register_##Type Z_Reflection_Vector4_Register___##Type{};                                 \
  };

VECTOR4_DECL(float, "Vector4f");
typedef Vector4<float> Vector4f;
VECTOR4_DECL(double, "Vector4d");
typedef Vector4<double> Vector4d;
