//
// Created by kita on 25-9-28.
//

#include "RenderPassManager.h"

#include "RHI/RenderPass.h"

RenderPassManager::RenderPassManager() {}

RenderPassManager::~RenderPassManager() { mRenderPasses = {}; }

void RenderPassManager::StartUp() {}
void RenderPassManager::ShutDown() {}

RHIRenderPass* RenderPassManager::GetRenderPass(const RHIRenderPassDesc& Desc) {

}