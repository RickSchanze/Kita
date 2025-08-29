//
// Created by kita on 25-8-29.
//

#include "ITick.h"

#include "TickManager.h"

ITick::ITick() { TickManager::RegisterTickInstance(this); }

ITick::~ITick() { TickManager::UnRegisterTickInstance(this); }

void ITick::SetTickDebugName(const StringView Name) { mDebugTickName = Name.ToString(); }