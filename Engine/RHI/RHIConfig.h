#pragma once
#include "Core/Config/IConfig.h"
#include "Core/Reflection/MetaMark.h"
#include "RHIFormat.h"

#include "RHIConfig.generated.h"

KCLASS(Category = RHI)
class RHIConfig : public IConfig {
  GENERATED_BODY(RHIConfig)

public:
  KPROPERTY()
  ERHIPresentMode PresentMode = ERHIPresentMode::VSync;
};
