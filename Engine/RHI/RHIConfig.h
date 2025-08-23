#pragma once
#include "Core/Config/IConfig.h"
#include "Core/Reflection/MetaMark.h"
#include "RHIFormat.h"
#include "SurfaceWindow.h"

#include "RHIConfig.generated.h"

KCLASS(Category = RHI)
class RHIConfig : public IConfig {
  GENERATED_BODY(RHIConfig)

public:
  KITA_CONFIG_GETTER_SETTER(ERHIPresentMode, PresentMode)
  KITA_CONFIG_GETTER_SETTER(ERHISurfaceWindowType, SurfaceWindowType)

private:
  KPROPERTY()
  ERHIPresentMode PresentMode = ERHIPresentMode::VSync;

  KPROPERTY()
  ERHISurfaceWindowType SurfaceWindowType = ERHISurfaceWindowType::GLFW;
};
