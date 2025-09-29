//
// Created by kita on 25-9-28.
//

#include "RHIResourceRegistry.h"

#include "RHI/DescriptorSet.h"
#include "RHI/GfxContext.h"
#include "RHI/RenderPass.h"

RHIResourceRegistry::RHIResourceRegistry() = default;

RHIResourceRegistry::~RHIResourceRegistry() { mRenderPasses = {}; }

void RHIResourceRegistry::StartUp() {}

void RHIResourceRegistry::ShutDown() {
  GetGfxContextRef().WaitDeviceIdle();
  GetRef().mRenderPasses.Clear();
  GetRef().mCameraDescriptorSetLayout = nullptr;
  GetRef().mModelDescriptorSetLayout = nullptr;
}

RHIRenderPass* RHIResourceRegistry::GetRenderPass(const UInt64 InHash) {
  auto& Self = GetRef();
  const auto Iter = Self.mRenderPasses.Find(InHash);
  return Iter != Self.mRenderPasses.End() ? Iter->second.Get() : nullptr;
}

RHIRenderPass* RHIResourceRegistry::GetRenderPass(const RHIRenderPassDesc& Desc) {
  const UInt64 Hash = Desc.GetHashCode();
  auto& Self = GetRef();
  const auto Iter = Self.mRenderPasses.Find(Hash);
  if (Iter != Self.mRenderPasses.End()) {
    return Iter->second.Get();
  }
  const auto RenderPass = GetGfxContextRef().CreateRenderPassU(Desc);
  RHIRenderPass* RenderPassPtr = RenderPass.Get();
  Self.mRenderPasses.Add(Hash, RenderPass);
  return RenderPassPtr;
}

RHIDescriptorSetLayout* RHIResourceRegistry::GetCameraDescriptorSetLayout() {
  auto& Self = GetRef();
  if (Self.mCameraDescriptorSetLayout) {
    return Self.mCameraDescriptorSetLayout.Get();
  }
  RHIDescriptorSetLayoutDesc Desc{};
  RHIDescriptorSetLayoutBinding Binding{};
  Binding.SetBinding(0).SetStage(KITA_RHI_ALL_GRAPHICS_SHADER_STAGES).SetDescriptorType(ERHIDescriptorType::UniformBuffer).SetDescriptorCount(1);
  Desc.Bindings.Add(Binding);
  Self.mCameraDescriptorSetLayout = GetGfxContextRef().CreateDescriptorSetLayoutU(Desc);
  return Self.mCameraDescriptorSetLayout.Get();
}

RHIDescriptorSetLayout* RHIResourceRegistry::GetModelDescriptorSetLayout() {}