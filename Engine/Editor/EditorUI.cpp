//
// Created by kita on 25-8-31.
//

#include "EditorUI.h"

#include "Assets/Asset.h"
#include "Core/Performance/ProfilerMark.h"

#include <imgui_internal.h>

void EditorUI::BeginHorizontal(const char* Id) {
  CPU_PROFILING_SCOPE;
  HorizontalLayoutState state;

  // 生成唯一ID
  if (Id) {
    state.LayoutId = Id;
  } else {
    std::ostringstream oss;
    oss << "##HorizontalLayout" << sLayoutCounter++;
    state.LayoutId = oss.str();
  }

  // 记录当前可用区域和光标位置
  state.AvailableSize = ImVec2ToVector2f(ImGui::GetContentRegionAvail());
  state.CursorStartPos = ImVec2ToVector2f(ImGui::GetCursorPos());

  sLayoutStack.Push(state);
}

void EditorUI::EndHorizontal() {
  CPU_PROFILING_SCOPE;
  if (sLayoutStack.Empty()) {
    gLogger.Error(Logcat::Editor, "EndHorizontal: 没有配对的BeginHorizontal!");
    return;
  }

  sLayoutStack.Pop();
}

void EditorUI::BeginChild(const char* Id, const Vector2f& Size, const bool Border) {
  CPU_PROFILING_SCOPE;
  if (sLayoutStack.Empty()) {
    ImGuiChildFlags Flags = ImGuiChildFlags_None;
    if (Border) {
      Flags |= ImGuiChildFlags_Border;
    }
    // 如果不在HorizontalLayout中，使用默认行为
    ImGui::BeginChild(Id ? Id : "##DefaultChild", Vector2fToImVec2(Size), Flags);
    return;
  }

  HorizontalLayoutState& state = sLayoutStack.Top();

  // 如果不是第一个子元素，使用SameLine
  if (!state.FirstChild) {
    ImGui::SameLine();
  }
  state.FirstChild = false;

  // 生成子元素ID
  std::string ChildId;
  if (Id) {
    ChildId = Id;
  } else {
    std::ostringstream oss;
    oss << state.LayoutId << "_Child" << state.ChildCount;
    ChildId = oss.str();
  }

  // 计算子元素大小
  ImVec2 ChildSize = Vector2fToImVec2(Size);
  if (ChildSize.x <= 0.0f) {
    // 如果宽度为0或负数，使用剩余可用宽度
    ChildSize.x = ImGui::GetContentRegionAvail().x;
  }
  if (ChildSize.y <= 0.0f) {
    // 如果高度为0或负数，使用布局的完整高度
    ChildSize.y = state.AvailableSize.Y();
  }
  ImGuiChildFlags Flags = ImGuiChildFlags_None;
  if (Border) {
    Flags |= ImGuiChildFlags_Border;
  }
  ImGui::BeginChild(ChildId.c_str(), ChildSize, Flags);
  state.ChildCount++;
}

void EditorUI::EndChild() {
  CPU_PROFILING_SCOPE;
  ImGui::EndChild();
}

void EditorUI::Separator(float Thickness, const Color& InColor) {
  if (sLayoutStack.Empty()) {
    // 如果不在HorizontalLayout中，使用水平分隔符
    ImGui::Separator();
    return;
  }

  HorizontalLayoutState& state = sLayoutStack.Top();

  // 在水平布局中，我们需要垂直分隔符
  if (!state.FirstChild) {
    ImGui::SameLine();
  }
  state.FirstChild = false;

  // 绘制垂直分隔线
  ImDrawList* DrawList = ImGui::GetWindowDrawList();
  ImVec2 CursorPos = ImGui::GetCursorScreenPos();

  // 设置分隔符颜色
  ImGui::PushStyleColor(ImGuiCol_Separator, ColorToImU32(InColor));

  // 绘制垂直线
  DrawList->AddLine(ImVec2(CursorPos.x + Thickness * 0.5f, CursorPos.y), ImVec2(CursorPos.x + Thickness * 0.5f, CursorPos.y + state.AvailableSize.Y()), ColorToImU32(InColor), Thickness);

  ImGui::PopStyleColor();

  // 移动光标
  ImGui::Dummy(ImVec2(Thickness, state.AvailableSize.Y()));
}

void EditorUI::DraggableSeparator(const char* Id, float& OutWidth, float MinWidth, float MaxWidth) {
  if (sLayoutStack.Empty()) {
    return;
  }

  HorizontalLayoutState& state = sLayoutStack.Top();

  if (!state.FirstChild) {
    ImGui::SameLine();
  }
  state.FirstChild = false;

  // 生成分隔器ID
  const std::string SeparatorId = Id ? Id : "##DraggableSeparator";

  // 创建可拖拽的按钮作为分隔器
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.5f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.7f));

  ImGui::Button(SeparatorId.c_str(), ImVec2(4.0f, state.AvailableSize.Y()));

  // 设置鼠标光标
  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
  }

  // 处理拖拽
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    OutWidth += ImGui::GetIO().MouseDelta.x;
    OutWidth = ImClamp(OutWidth, MinWidth, MaxWidth);
  }

  ImGui::PopStyleColor(3);
}
