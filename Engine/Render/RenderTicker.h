#pragma once
#include "World/ITick.h"

class RenderTicker : public ITick {
public:
  virtual void Tick(double DeltaSeconds) override;
};
