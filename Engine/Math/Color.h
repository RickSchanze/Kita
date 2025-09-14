#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Vector.h"

struct Color : InlinedOutput {
  Color(const float R = 0, const float G = 0, const float B = 0, const float A = 1) : Data(R, G, B, A) {}

  float& R() { return Data.X(); }
  [[nodiscard]] const float& R() const { return Data.X(); }
  float& G() { return Data.Y(); }
  [[nodiscard]] const float& G() const { return Data.Y(); }
  float& B() { return Data.Z(); }
  [[nodiscard]] const float& B() const { return Data.Z(); }
  float& A() { return Data.W(); }
  [[nodiscard]] const float& A() const { return Data.W(); }

  void WriteArchive(OutputArchive& Archive) const;
  void ReadArchive(InputArchive& Archive);

  [[nodiscard]] UInt32 ToUInt32() const {
    auto Clamp = [](const float v) -> UInt32 { return static_cast<UInt32>(std::round(std::clamp(v, 0.0f, 1.0f) * 255.0f)); };

    const UInt32 r = Clamp(R());
    const UInt32 g = Clamp(G());
    const UInt32 b = Clamp(B());
    const UInt32 a = Clamp(A());

    return (r << 24) | (g << 16) | (b << 8) | (a);
  }

  Vector4f Data;

  [[nodiscard]] String ToString() const { return Data.ToString(); }
};

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
