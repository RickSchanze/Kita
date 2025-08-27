#pragma once
#include "Core/Config/IConfig.h"
#include "Core/Reflection/MetaMark.h"
#include "RHIEnums.h"
#include "SurfaceWindow.h"

#include "RHIConfig.generated.h"

KCLASS(Category = RHI)
class RHIConfig : public IConfig {
  GENERATED_BODY(RHIConfig)

public:
  KITA_CONFIG_GETTER_SETTER(ERHIPresentMode, PresentMode)
  KITA_CONFIG_GETTER_SETTER(ERHISurfaceWindowType, SurfaceWindowType)
  KITA_CONFIG_GETTER_SETTER(bool, EnableVulkanValidationLayer)
  KITA_CONFIG_GETTER_SETTER(ERHIFormat, DesiredSwapchainFormat)
  KITA_CONFIG_GETTER_SETTER(ERHIColorSpace, DesiredSwapchainColorSpace)
  KITA_CONFIG_GETTER_SETTER(Vector2i, DefaultWindowSize)

private:
  KPROPERTY()
  ERHIPresentMode PresentMode = ERHIPresentMode::VSync;

  KPROPERTY()
  ERHISurfaceWindowType SurfaceWindowType = ERHISurfaceWindowType::GLFW;

  KPROPERTY()
  bool EnableVulkanValidationLayer = true;

  KPROPERTY()
  ERHIFormat DesiredSwapchainFormat = ERHIFormat::B8G8R8A8_SRGB;

  KPROPERTY()
  ERHIColorSpace DesiredSwapchainColorSpace = ERHIColorSpace::sRGB;

  KPROPERTY()
  Vector2i DefaultWindowSize = {1920, 1080};
};
