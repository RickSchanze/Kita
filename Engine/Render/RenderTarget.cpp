//
// Created by kita on 25-9-21.
//

#include "RenderTarget.h"

RenderTarget::RenderTarget(UInt32 Width, UInt32 Height, ERHIFormat Format) {}

RenderTarget::~RenderTarget() {
  mFrameBuffer = nullptr;
  mView = nullptr;
  mImage = nullptr;
}