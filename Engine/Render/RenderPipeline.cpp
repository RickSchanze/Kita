//
// Created by kita on 25-8-30.
//

#include "RenderPipeline.h"

#include "RHI/GfxContext.h"

void RenderPipeline::Draw(const RenderPipelineDrawParams& Params) { DrawImGui(Params); }

void RenderPipeline::DrawImGui(const RenderPipelineDrawParams& Params) { GfxContext::GetRef().DrawImGui(Params.Cmd, Params.TargetFramebuffer, Params.Width, Params.Height); }