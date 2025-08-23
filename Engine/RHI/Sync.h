#pragma once
#include "Core/TypeDefs.h"
#include "IRHIResource.h"
class Fence : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Fence; }

  virtual void Wait(UInt64 Timeout) = 0;
};

class Semaphore : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Semaphore; }
};