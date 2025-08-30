#pragma once
#include "World/ITick.h"

class RHISurfaceWindow;

class InputTicker : public ITick {
public:
  explicit InputTicker(RHISurfaceWindow* InWindow) : mWindow(InWindow) {}
  virtual void Tick(double DeltaSeconds) override;

private:
  void UpdateInputData() const;

  RHISurfaceWindow* mWindow = nullptr;
};
