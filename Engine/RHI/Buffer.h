#pragma once
#include "Core/TypeDefs.h"
#include "IRHIResource.h"
#include "RHIEnums.h"

struct RHIBufferDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Size, 0);                                                                 // Size in bytes necessary
  RHI_DEFINE_BUILDER_FIELD(ERHIBufferUsage, Usage, ERHIBufferUsage::None);                                   // Usage flags necessary
  RHI_DEFINE_BUILDER_FIELD(ERHIBufferMemoryProperty, MemoryProperty, ERHIBufferMemoryProperty::DeviceLocal); // Memory property flags 默认是DeviceLocal 只有CPU可以访问

#if KITA_DEBUG_NAME
  RHI_DEFINE_BUILDER_FIELD(String, DebugName, "")
#endif
};

class RHIBuffer : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Buffer; }

  [[nodiscard]] UInt32 GetSize() const { return mDesc.Size; }
  [[nodiscard]] ERHIBufferUsage GetUsage() const { return mDesc.Usage; }
  [[nodiscard]] ERHIBufferMemoryProperty GetMemoryProperty() const { return mDesc.MemoryProperty; }

  virtual void* Map(UInt64 Size, UInt64 Offset) = 0;
  virtual void Unmap(void* MappedPtr) = 0;

protected:
  RHIBufferDesc mDesc;
};
