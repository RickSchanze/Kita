#pragma once
#include "Core/TypeDefs.h"
#include "IRHIResource.h"
class RHIFence : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Fence; }

  virtual void Wait(UInt64 Timeout) = 0;
  virtual void Reset() = 0;
};

class RHISemaphore : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Semaphore; }
};