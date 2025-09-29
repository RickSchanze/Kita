#pragma once
#include "Core/Container/Map.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/Singleton/Singleton.h"

class RHIDescriptorSetLayout;
struct RHIRenderPassDesc;
class RHIRenderPass;

class RHIResourceRegistry : Singleton<RHIResourceRegistry> {
public:
  RHIResourceRegistry();
  ~RHIResourceRegistry();

  static void StartUp();
  static void ShutDown();

  static RHIRenderPass* GetRenderPass(UInt64 InHash);
  static RHIRenderPass* GetRenderPass(const RHIRenderPassDesc& Desc);

  static RHIDescriptorSetLayout* GetCameraDescriptorSetLayout();
  static RHIDescriptorSetLayout* GetModelDescriptorSetLayout();

private:
  Map<UInt64, UniquePtr<RHIRenderPass>> mRenderPasses;
  UniquePtr<RHIDescriptorSetLayout> mCameraDescriptorSetLayout;
  UniquePtr<RHIDescriptorSetLayout> mModelDescriptorSetLayout;
};
