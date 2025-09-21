#pragma once
#include "Core/Memory/UniquePtr.h"
#include "RHI/RHIEnums.h"

class RHIFrameBuffer;
class RHIImageView;
class RHIImage;
class RenderTarget {
public:
  RenderTarget(UInt32 Width, UInt32 Height, ERHIFormat Format = ERHIFormat::R8G8B8A8_UNorm);
  ~RenderTarget();

private:
  UniquePtr<RHIImage> mImage;
  UniquePtr<RHIImageView> mView;
  UniquePtr<RHIFrameBuffer> mFrameBuffer;
};
