#pragma once
#include "Core/Memory/UniquePtr.h"
#include "RHI/RHIEnums.h"

class RHIRenderPass;
struct RHIImageDesc;
class RHIFrameBuffer;
class RHIImageView;
class RHIImage;

class RenderTarget {
public:
  explicit RenderTarget(const RHIImageDesc& ImageDesc);
  ~RenderTarget();

  /// 重新设定尺寸 方法是销毁重建
  /// 会等待GPU Idle
  void Recreate(const RHIImageDesc& ImageDesc);

  void Resize(UInt32 NewWidth, UInt32 NewHeight);

  /// 如果Image没有 那么需要设定Width和Height
  /// @param RenderPass
  bool SetRenderPass(RHIRenderPass* RenderPass);

  [[nodiscard]] UInt32 GetWidth() const;
  [[nodiscard]] UInt32 GetHeight() const;

private:
  UniquePtr<RHIImage> mImage;
  UniquePtr<RHIImageView> mView;
  UniquePtr<RHIFrameBuffer> mFrameBuffer;
};
