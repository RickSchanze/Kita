#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Vector.h"

struct Color {
  float& R() { return Data.X(); }
  const float& R() const { return Data.X(); }
  float& G() { return Data.Y(); }
  const float& G() const { return Data.Y(); }
  float& B() { return Data.Z(); }
  const float& B() const { return Data.Z(); }
  float& A() { return Data.W(); }
  const float& A() const { return Data.W(); }

  Vector4f Data;

  [[nodiscard]] String ToString() const { return Data.ToString(); }
};

struct Z_Reflection_Color_Register {
  Z_Reflection_Color_Register() {
    TypeBuilder Builder;
    Builder.CreateType<Color>("Color");
    Builder.AddField(New<Field>("R", 0, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.AddField(New<Field>("G", 4, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.AddField(New<Field>("B", 8, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.AddField(New<Field>("A", 12, sizeof(float), TypeOf<float>(), Builder.OperatingType));
    Builder.Register();
  }
};
static Z_Reflection_Color_Register Z_Reflection_Color_Register___Instance;
