//
// Created by kita on 25-8-30.
//

#include "RenderPipeline.h"

#include "Core/Performance/ProfilerMark.h"
#include "Editor/MenuActionManager.h"
#include "RHI/GfxContext.h"

#include <imgui.h>
#include <imgui_impl_vulkan.h>

static void StaticDrawImGui() {
  CPU_PROFILING_SCOPE;
  ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* Viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(Viewport->WorkPos);
  ImGui::SetNextWindowSize(Viewport->WorkSize);
  ImGui::SetNextWindowViewport(Viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  WindowFlags |= ImGuiWindowFlags_NoBackground;
  static ImGuiDockNodeFlags DockSpaceFlags = ImGuiDockNodeFlags_None;
  bool pOpen;
  ImGui::Begin("DockSpace Demo", &pOpen, WindowFlags);
  ImGuiID DockSpaceId = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(DockSpaceId, ImVec2(0.0f, 0.0f), DockSpaceFlags);
  ImGui::PopStyleVar(2);
  MenuActionManager::Render();

  ImGui::End();
}

void RenderPipeline::Draw(const RenderPipelineDrawParams& Params) {
  CPU_PROFILING_SCOPE;
  DrawImGui(Params);
}

void RenderPipeline::DrawImGui(const RenderPipelineDrawParams& Params) {
  static TaskHandle Handle{};
  Handle.WaitSync();
  ImGui_ImplVulkan_NewFrame();
  ImGui::NewFrame();
  StaticDrawImGui();
  EditorWindowManager::Render();
  Handle = RecordImGuiCommands(Params);
}

TaskHandle RenderPipeline::RecordImGuiCommands(const RenderPipelineDrawParams& Params) {
  CPU_PROFILING_SCOPE;
  return GfxContext::GetRef().DrawImGui(Params.Cmd, Params.TargetFramebuffer, Params.Width, Params.Height);
}