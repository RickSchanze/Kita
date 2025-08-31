//
// Created by kita on 25-8-31.
//
#include "ImGuiConfig.h"

#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Path.h"

#include <imgui.h>

void ImGuiConfig::ApplyImGuiSettings() {
  auto& IO = ImGui::GetIO();
  ImVector<ImWchar> Ranges;
  if (Path::IsExists(FontPath)) {
    ImFontGlyphRangesBuilder Builder;
    String GlyphText = "";
    if (Path::IsExists(CharsetPath)) {
      if (auto Text = File::ReadAllText(CharsetPath)) {
        GlyphText = *Text;
        Builder.AddText(GlyphText.Data());
      } else {
        gLogger.Warn("RHI.ImGui", "读取Charset文件{}失败, 默认使SimplifiedChineseCommon", CharsetPath);
        Builder.AddRanges(IO.Fonts->GetGlyphRangesChineseSimplifiedCommon());
      }
    } else {
      gLogger.Warn("RHI.ImGui", "字体charset文件{}不存在, 默认使SimplifiedChineseCommon", FontPath);
      Builder.AddRanges(IO.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    }
    Builder.AddRanges(IO.Fonts->GetGlyphRangesDefault());
    Builder.BuildRanges(&Ranges);
    IO.Fonts->AddFontFromFileTTF(FontPath.Data(), FontSize, nullptr, Ranges.Data);
    IO.Fonts->Build();
  }
  Theme.ApplyTheme();
}

inline ImVec4 Vector4f2ImVec4(const Vector4f& v) { return ImVec4(v.X(), v.Y(), v.Z(), v.W()); }

inline ImVec2 Vector2f2ImVec2(const Vector2f& v) { return ImVec2(v.X(), v.Y()); }

void ImGuiColorTheme::ApplyTheme() const {
  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text] = Vector4f2ImVec4(Text);
  colors[ImGuiCol_TextDisabled] = Vector4f2ImVec4(TextDisabled);
  colors[ImGuiCol_WindowBg] = Vector4f2ImVec4(WindowBg);
  colors[ImGuiCol_ChildBg] = Vector4f2ImVec4(ChildBg);
  colors[ImGuiCol_PopupBg] = Vector4f2ImVec4(PopupBg);
  colors[ImGuiCol_Border] = Vector4f2ImVec4(Border);
  colors[ImGuiCol_BorderShadow] = Vector4f2ImVec4(BorderShadow);
  colors[ImGuiCol_FrameBg] = Vector4f2ImVec4(FrameBg);
  colors[ImGuiCol_FrameBgHovered] = Vector4f2ImVec4(FrameBgHovered);
  colors[ImGuiCol_FrameBgActive] = Vector4f2ImVec4(FrameBgActive);
  colors[ImGuiCol_TitleBg] = Vector4f2ImVec4(TitleBg);
  colors[ImGuiCol_TitleBgActive] = Vector4f2ImVec4(TitleBgActive);
  colors[ImGuiCol_TitleBgCollapsed] = Vector4f2ImVec4(TitleBgCollapsed);
  colors[ImGuiCol_MenuBarBg] = Vector4f2ImVec4(MenuBarBg);
  colors[ImGuiCol_ScrollbarBg] = Vector4f2ImVec4(ScrollbarBg);
  colors[ImGuiCol_ScrollbarGrab] = Vector4f2ImVec4(ScrollbarGrab);
  colors[ImGuiCol_ScrollbarGrabHovered] = Vector4f2ImVec4(ScrollbarGrabHovered);
  colors[ImGuiCol_ScrollbarGrabActive] = Vector4f2ImVec4(ScrollbarGrabActive);
  colors[ImGuiCol_CheckMark] = Vector4f2ImVec4(CheckMark);
  colors[ImGuiCol_SliderGrab] = Vector4f2ImVec4(SliderGrab);
  colors[ImGuiCol_SliderGrabActive] = Vector4f2ImVec4(SliderGrabActive);
  colors[ImGuiCol_Button] = Vector4f2ImVec4(Button);
  colors[ImGuiCol_ButtonHovered] = Vector4f2ImVec4(ButtonHovered);
  colors[ImGuiCol_ButtonActive] = Vector4f2ImVec4(ButtonActive);
  colors[ImGuiCol_Header] = Vector4f2ImVec4(Header);
  colors[ImGuiCol_HeaderHovered] = Vector4f2ImVec4(HeaderHovered);
  colors[ImGuiCol_HeaderActive] = Vector4f2ImVec4(HeaderActive);
  colors[ImGuiCol_Separator] = Vector4f2ImVec4(Separator);
  colors[ImGuiCol_SeparatorHovered] = Vector4f2ImVec4(SeparatorHovered);
  colors[ImGuiCol_SeparatorActive] = Vector4f2ImVec4(SeparatorActive);
  colors[ImGuiCol_ResizeGrip] = Vector4f2ImVec4(ResizeGrip);
  colors[ImGuiCol_ResizeGripHovered] = Vector4f2ImVec4(ResizeGripHovered);
  colors[ImGuiCol_ResizeGripActive] = Vector4f2ImVec4(ResizeGripActive);
  colors[ImGuiCol_Tab] = Vector4f2ImVec4(Tab);
  colors[ImGuiCol_TabHovered] = Vector4f2ImVec4(TabHovered);
  colors[ImGuiCol_TabActive] = Vector4f2ImVec4(TabActive);
  colors[ImGuiCol_TabUnfocused] = Vector4f2ImVec4(TabUnfocused);
  colors[ImGuiCol_TabUnfocusedActive] = Vector4f2ImVec4(TabUnfocusedActive);
  colors[ImGuiCol_DockingPreview] = Vector4f2ImVec4(DockingPreview);
  colors[ImGuiCol_DockingEmptyBg] = Vector4f2ImVec4(DockingEmptyBg);
  colors[ImGuiCol_PlotLines] = Vector4f2ImVec4(PlotLines);
  colors[ImGuiCol_PlotLinesHovered] = Vector4f2ImVec4(PlotLinesHovered);
  colors[ImGuiCol_PlotHistogram] = Vector4f2ImVec4(PlotHistogram);
  colors[ImGuiCol_PlotHistogramHovered] = Vector4f2ImVec4(PlotHistogramHovered);
  colors[ImGuiCol_TableHeaderBg] = Vector4f2ImVec4(TableHeaderBg);
  colors[ImGuiCol_TableBorderStrong] = Vector4f2ImVec4(TableBorderStrong);
  colors[ImGuiCol_TableBorderLight] = Vector4f2ImVec4(TableBorderLight);
  colors[ImGuiCol_TableRowBg] = Vector4f2ImVec4(TableRowBg);
  colors[ImGuiCol_TableRowBgAlt] = Vector4f2ImVec4(TableRowBgAlt);
  colors[ImGuiCol_TextSelectedBg] = Vector4f2ImVec4(TextSelectedBg);
  colors[ImGuiCol_DragDropTarget] = Vector4f2ImVec4(DragDropTarget);
  colors[ImGuiCol_NavHighlight] = Vector4f2ImVec4(NavHighlight);
  colors[ImGuiCol_NavWindowingHighlight] = Vector4f2ImVec4(NavWindowingHighlight);
  colors[ImGuiCol_NavWindowingDimBg] = Vector4f2ImVec4(NavWindowingDimBg);
  colors[ImGuiCol_ModalWindowDimBg] = Vector4f2ImVec4(ModalWindowDimBg);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding = Vector2f2ImVec2(WindowPadding);
  style.FramePadding = Vector2f2ImVec2(FramePadding);
  style.CellPadding = Vector2f2ImVec2(CellPadding);
  style.ItemSpacing = Vector2f2ImVec2(ItemSpacing);
  style.ItemInnerSpacing = Vector2f2ImVec2(ItemInnerSpacing);
  style.TouchExtraPadding = Vector2f2ImVec2(TouchExtraPadding);
  style.IndentSpacing = IndentSpacing;
  style.ScrollbarSize = ScrollbarSize;
  style.GrabMinSize = GrabMinSize;
  style.WindowBorderSize = WindowBorderSize;
  style.ChildBorderSize = ChildBorderSize;
  style.PopupBorderSize = PopupBorderSize;
  style.FrameBorderSize = FrameBorderSize;
  style.TabBorderSize = TabBorderSize;
  style.WindowRounding = WindowRounding;
  style.ChildRounding = ChildRounding;
  style.FrameRounding = FrameRounding;
  style.PopupRounding = PopupRounding;
  style.ScrollbarRounding = ScrollbarRounding;
  style.GrabRounding = GrabRounding;
  style.LogSliderDeadzone = LogSliderDeadzone;
  style.TabRounding = TabRounding;
}
