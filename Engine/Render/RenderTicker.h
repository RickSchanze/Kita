#pragma once
#include "World/ITick.h"

class RenderTicker : public ITick {
public:
  RenderTicker() : ITick(false) {}
  virtual void Tick(double DeltaSeconds) override;
};
