//
// Created by kita on 25-8-29.
//

#include "ITick.h"

#include "TickManager.h"

ITick::ITick(bool RegisterSelf) {
  if (RegisterSelf) {
    TickManager::RegisterTickInstance(this);
  }
}

ITick::~ITick() { TickManager::UnRegisterTickInstance(this); }

#if KITA_DEBUG
void ITick::SetTickDebugName(const StringView Name) { mDebugTickName = Name.ToString(); }
#endif