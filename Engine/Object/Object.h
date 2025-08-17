#pragma once

#include "Core/String/String.h"

#include "Object.generated.h"

KCLASS()
class Object {
  GENERATED_BODY(Object)
public:
  Object() = default;
  virtual ~Object() = default;

protected:
  KPROPERTY()
  String mName;
};
