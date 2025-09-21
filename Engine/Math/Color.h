#pragma once
#include "Vector.h"

#define COLOR_ORDER_ABGR

struct Color : InlinedOutput {
  constexpr Color(const float R = 0, const float G = 0, const float B = 0, const float A = 1) : Data(R, G, B, A) {}

  constexpr float& R() { return Data.X(); }
  [[nodiscard]] constexpr const float& R() const { return Data.X(); }
  constexpr float& G() { return Data.Y(); }
  [[nodiscard]] constexpr const float& G() const { return Data.Y(); }
  constexpr float& B() { return Data.Z(); }
  [[nodiscard]] constexpr const float& B() const { return Data.Z(); }
  constexpr float& A() { return Data.W(); }
  [[nodiscard]] constexpr const float& A() const { return Data.W(); }

  void WriteArchive(OutputArchive& Archive) const;
  void ReadArchive(InputArchive& Archive);

  [[nodiscard]] constexpr UInt32 ToUInt32() const {
    auto Clamp = [](float v) constexpr -> UInt32 {
      v = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); // clamp
      // round + cast
      return static_cast<UInt32>(static_cast<int>(v * 255.0f + 0.5f));
    };

    const UInt32 r = Clamp(R());
    const UInt32 g = Clamp(G());
    const UInt32 b = Clamp(B());
    const UInt32 a = Clamp(A());

#ifdef COLOR_ORDER_RGBA
    return (r << 24) | (g << 16) | (b << 8) | (a);
#elifdef COLOR_ORDER_ABGR
    return (a << 24) | (b << 16) | (g << 8) | (r);
#endif
  }

  Vector4f Data;

  [[nodiscard]] String ToString() const { return Data.ToString(); }

  static constexpr Color HighlightText() { return Color(1.0f, 0.843f, 0); }
  static constexpr Color HighlightBackground() { return Color(0.26f, 0.59f, 0.98f); }
  static constexpr Color Mul(Color Left, Color Right, const bool Clamped) {
    if (Clamped) {
      return Color(std::min(Left.R() * Right.R(), 1.0f), std::min(Left.G() * Right.G(), 1.0f), std::min(Left.B() * Right.B(), 1.0f), std::min(Left.A() * Right.A(), 1.0f));
    } else {
      return Color(Left.R() * Right.R(), Left.G() * Right.G(), Left.B() * Right.B(), Left.A() * Right.A());
    }
  }
};

inline Color operator*(const Color Left, const Color Right) {
  return Color::Mul(Left, Right, true);
}

struct Z_Reflection_Color_Register {
  Z_Reflection_Color_Register() {
    TypeBuilder Builder{};
    Builder.CreateType<Color>("Color");
    Builder.AddField(New<Field>("R", 0, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.AddField(New<Field>("G", 4, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.AddField(New<Field>("B", 8, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.AddField(New<Field>("A", 12, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.Register();
  }
};
inline Z_Reflection_Color_Register Z_Reflection_Color_Register___Instance{};
