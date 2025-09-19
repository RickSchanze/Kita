//
// Created by kita on 25-9-14.
//

#include "Texture.h"

#include "RHI/Image.h"
#include "RHI/ImageView.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Path.h"
#include "Core/Performance/ProfilerMark.h"
#include "RHI/Buffer.h"
#include "RHI/CommandBuffer.h"
#include "RHI/GfxCommandHelper.h"
#include "RHI/GfxContext.h"
#include "stb_image.h"

Texture::Texture() {}

Texture::~Texture() {}

UInt32 Texture::GetWidth() const {
  if (!mImage) {
    gLogger.Error("Texture", "Image is null");
    return 0;
  }
  return mImage->GetDesc().Width;
}

UInt32 Texture::GetHeight() const {
  if (!mImage) {
    gLogger.Error("Texture", "Image is null");
    return 0;
  }
  return mImage->GetDesc().Height;
}

ERHIFormat Texture::GetFormat() const {
  if (!mImage) {
    gLogger.Error("Texture", "Image is null");
    return ERHIFormat::Count;
  }
  return mImage->GetDesc().Format;
}

Texture2D::Texture2D() {}

Texture2D::~Texture2D() {}

void Texture2D::ApplyMeta(const AssetMeta& Meta) {
  const auto TextureMeta = static_cast<const Texture2DMeta&>(Meta);
  mMeta = TextureMeta;
}

void Texture2D::Load() {
  if (mPath.Empty() || Path::IsExists(mPath) || Path::IsDirectory(mPath) || !((mPath.EndsWith(".png") || mPath.EndsWith(".jpg")))) {
    gLogger.Error(Logcat::Asset, "纹理资产 '{}' 需要存在不为文件夹, 且支持的后缀为png,jpg.", mPath);
    return;
  }
  CPU_PROFILING_SCOPE;
  Int32 Width, Height, Channels;
  const stbi_uc* Pixels = stbi_load(mPath.Data(), &Width, &Height, &Channels, STBI_rgb_alpha);
  if (!Pixels) {
    gLogger.Error(Logcat::Asset, "纹理资产 '{}' 加载失败.", mPath);
    return;
  }
  RHIBufferDesc BufferDesc{};
  const Int32 Size = Width * Height * 4;
  BufferDesc.SetMemoryProperty(ERHIBufferMemoryProperty::HostVisible | ERHIBufferMemoryProperty::HostCoherent).SetUsage(ERHIBufferUsage::TransferSrc).SetSize(Size);
  auto StagingBuffer = GfxContext::GetRef().CreateBufferU(BufferDesc);
  void* Data = StagingBuffer->Map(Size, 0);
  Memcpy(Data, Pixels, Size);
  StagingBuffer->Unmap(Data);

  RHIImageDesc ImageDesc{};
  ImageDesc.SetFormat(ERHIFormat::R8G8B8A8_SRGB).SetWidth(Width).SetHeight(Height).SetUsage(ERHIImageUsage::ShaderRead | ERHIImageUsage::TransferDst);
  mImage = GfxContext::GetRef().CreateImageU(ImageDesc);

  GfxCommandSyncHandle Handle = GfxCommandHelper::CreateSingleTimeCommandBuffer();
  Handle.CommandBuffer->ResourceBarrier(mImage.Get(), ERHIImageLayout::Undefined, ERHIImageLayout::TransferDst);
  Handle.CommandBuffer->CopyBufferToImage(StagingBuffer.Get(), mImage.Get());
  Handle.CommandBuffer->ResourceBarrier(mImage.Get(), ERHIImageLayout::TransferDst, ERHIImageLayout::ShaderReadOnly);
  GfxCommandHelper::SubmitSingleTimeCommandBufferAndWait(Handle);

  RHIImageViewDesc Desc{};
  Desc.SetSourceImage(mImage.Get());
  mImageView = GfxContext::GetRef().CreateImageViewU(Desc);

  gLogger.Info(Logcat::Asset, "纹理资产 '{}' 加载成功.", mPath);
}

void Texture2D::Unload() {
  mImageView = nullptr;
  mImage = nullptr;
  mMeta = {};
  gLogger.Info(Logcat::Asset, "纹理资产 '{}' 卸载成功.", mPath);
}

void TextureCube::Load() {}

void TextureCube::Unload() {}

void TextureCube::ApplyMeta(const AssetMeta& Meta) {}
