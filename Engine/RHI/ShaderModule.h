#pragma once
#include "IRHIResource.h"
#include "RHIEnums.h"

struct RHIShaderModuleDesc {
  RHI_DEFINE_BUILDER_FIELD(SizeType, CodeSize, 0)            // NECESSARY
  RHI_DEFINE_BUILDER_FIELD_PTR(const UInt32*, Code, nullptr) // NECESSARY
};

class RHIShaderModule : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::ShaderModule; }
};
