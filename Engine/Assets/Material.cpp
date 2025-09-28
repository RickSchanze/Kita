//
// Created by kita on 25-9-27.
//

#include "Material.h"

#include "RHI/Pipeline.h"
#include "Shader.h"

void Material::Load() {}

void Material::Unload() {}

void Material::Save() { Asset::Save(); }

void Material::ApplyMeta(const AssetMeta& Meta) {
  const auto& MatMeta = static_cast<const MaterialMeta&>(Meta);
  mMeta = MatMeta;
}

SharedMaterialImpl::SharedMaterialImpl() = default;

SharedMaterialImpl::~SharedMaterialImpl() {
  mPipelineLayout = nullptr;
  mPipeline = nullptr;
}

SharedMaterialImpl::SharedMaterialImpl(const Shader* InShader) {
  if (InShader == nullptr) {
    return;
  }
  const auto& Params = InShader->GetParameterInfo();

}

bool SharedMaterialImpl::IsValid() const { return mPipeline->IsValid(); }

SharedMaterial::~SharedMaterial() = default;

SharedMaterial SharedMaterialManager::Create(Shader* InShader) { return SharedMaterial{nullptr}; }

SharedMaterial SharedMaterialManager::Get(const Shader* InShader) {
  if (InShader == nullptr) {
    return SharedMaterial{nullptr};
  }
  const UInt64 Hash = InShader->GetHashCode();
  auto& Self = GetRef();
  const auto Iter = Self.mSharedMaterials.Find(Hash);
  if (Iter != Self.mSharedMaterials.end()) {
    return SharedMaterial{Iter->second};
  }
  return SharedMaterial{nullptr};
}

SharedMaterial SharedMaterialManager::Acquire(Shader* InShader) {
  if (InShader == nullptr) {
    return SharedMaterial{nullptr};
  }
  SharedMaterial Mat = Get(InShader);
  if (Mat.IsValid()) {
    return Mat;
  }
  return Create(InShader);
}
