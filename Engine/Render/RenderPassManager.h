#pragma once
#include "Core/Container/Map.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/Singleton/Singleton.h"

struct RHIRenderPassDesc;
class RHIRenderPass;
class RenderPassManager : Singleton<RenderPassManager> {
public:
  RenderPassManager();
  ~RenderPassManager();

  static void StartUp();
  static void ShutDown();

  static RHIRenderPass* GetRenderPass(UInt64 InHash);
  static RHIRenderPass* GetRenderPass(const RHIRenderPassDesc& Desc);

private:
  Map<UInt64, UniquePtr<RHIRenderPass>> mRenderPasses;
};
