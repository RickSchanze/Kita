//
// Created by kita on 25-8-31.
//

#include "EditorUI.h"

#include "Assets/Asset.h"
#include "Core/Config/ConfigManager.h"
#include "Core/Performance/ProfilerMark.h"
#include "RHI/ImGuiConfig.h"

#include <imgui_internal.h>

void EditorUI::Splitter(float& S1, float& S2, const ESplitterDirection Direction, const float MinSize1, const float MinSize2, const float Thickness) {
  const auto Size = ImGui::GetContentRegionAvail();
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // 紧贴窗口
  bool Dragging = false;

  if (Direction == ESplitterDirection::Vertical) {
    ImGui::SameLine();
    ImGui::InvisibleButton("##Splitter", ImVec2(Thickness, Size.y));
    Dragging = ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left);

    // 悬停/拖拽时修改鼠标光标
    if (ImGui::IsItemHovered() || Dragging)
      ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

    // 平滑颜色（简单插值，可改成更高级的动画系统）
    UInt32 currentColor = SplitterNormal;
    if (Dragging)
      currentColor = SplitterActive;
    else if (ImGui::IsItemHovered())
      currentColor = SplitterHovered;

    const auto p0 = ImGui::GetItemRectMin();
    const auto p1 = ImGui::GetItemRectMax();
    ImGui::GetWindowDrawList()->AddRectFilled(p0, p1, currentColor);

    // 拖拽逻辑
    if (Dragging) {
      const float Delta = ImGui::GetIO().MouseDelta.x;
      S1 += Delta;
      S2 -= Delta;
      S1 = ImMax(S1, MinSize1);
      S2 = ImMax(S2, MinSize2);
    }
  } else { // Horizontal
    ImGui::InvisibleButton("##Splitter", ImVec2(Size.x, Thickness));
    Dragging = ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left);

    if (ImGui::IsItemHovered() || Dragging)
      ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);

    UInt32 CurrentColor = SplitterNormal;
    if (Dragging)
      CurrentColor = SplitterActive;
    else if (ImGui::IsItemHovered())
      CurrentColor = SplitterHovered;

    const auto P0 = ImGui::GetItemRectMin();
    const auto P1 = ImGui::GetItemRectMax();
    ImGui::GetWindowDrawList()->AddRectFilled(P0, P1, ImGui::GetColorU32(CurrentColor));

    if (Dragging) {
      const float Delta = ImGui::GetIO().MouseDelta.y;
      S1 += Delta;
      S2 -= Delta;
      S1 = ImMax(S1, MinSize1);
      S2 = ImMax(S2, MinSize2);
    }
  }

  ImGui::PopStyleVar();
}

void EditorUI::InitializeColors() {
  const auto& Config = ConfigManager::GetConfigRef<ImGuiConfig>();
  SplitterNormal = ColorToImU32(Config.Theme.Separator);
  SplitterHovered = ColorToImU32(Config.Theme.SeparatorHovered);
  SplitterActive = ColorToImU32(Config.Theme.SeparatorActive);
}
