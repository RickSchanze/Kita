//
// Created by kita on 25-8-30.
//

#include "RenderTicker.h"

#include "RenderContext.h"

void RenderTicker::Tick(double DeltaSeconds) { RenderContext::GetRef().Render(DeltaSeconds); }