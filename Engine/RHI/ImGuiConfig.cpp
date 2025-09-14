//
// Created by kita on 25-8-31.
//
#include "ImGuiConfig.h"

#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Path.h"
#include "IconFontHeader.h"

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

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphOffset.y = 4.f;
    static constexpr ImWchar IconFontRages[] = {ICON_MIN_MS, ICON_MAX_MS, 0};
    IO.Fonts->AddFontFromFileTTF(IconFontPath.Data(), FontSize, &config, IconFontRages);
    IO.Fonts->Build();
  }
  Theme.ApplyTheme();
}

inline ImVec4 Color2ImVec4(const Color& v) { return ImVec4(v.R(), v.G(), v.B(), v.A()); }

inline ImVec2 Vector2f2ImVec2(const Vector2f& v) { return ImVec2(v.X(), v.Y()); }

void ImGuiColorTheme::ApplyTheme() const {
  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text] = Color2ImVec4(Text);
  colors[ImGuiCol_TextDisabled] = Color2ImVec4(TextDisabled);
  colors[ImGuiCol_WindowBg] = Color2ImVec4(WindowBg);
  colors[ImGuiCol_ChildBg] = Color2ImVec4(ChildBg);
  colors[ImGuiCol_PopupBg] = Color2ImVec4(PopupBg);
  colors[ImGuiCol_Border] = Color2ImVec4(Border);
  colors[ImGuiCol_BorderShadow] = Color2ImVec4(BorderShadow);
  colors[ImGuiCol_FrameBg] = Color2ImVec4(FrameBg);
  colors[ImGuiCol_FrameBgHovered] = Color2ImVec4(FrameBgHovered);
  colors[ImGuiCol_FrameBgActive] = Color2ImVec4(FrameBgActive);
  colors[ImGuiCol_TitleBg] = Color2ImVec4(TitleBg);
  colors[ImGuiCol_TitleBgActive] = Color2ImVec4(TitleBgActive);
  colors[ImGuiCol_TitleBgCollapsed] = Color2ImVec4(TitleBgCollapsed);
  colors[ImGuiCol_MenuBarBg] = Color2ImVec4(MenuBarBg);
  colors[ImGuiCol_ScrollbarBg] = Color2ImVec4(ScrollbarBg);
  colors[ImGuiCol_ScrollbarGrab] = Color2ImVec4(ScrollbarGrab);
  colors[ImGuiCol_ScrollbarGrabHovered] = Color2ImVec4(ScrollbarGrabHovered);
  colors[ImGuiCol_ScrollbarGrabActive] = Color2ImVec4(ScrollbarGrabActive);
  colors[ImGuiCol_CheckMark] = Color2ImVec4(CheckMark);
  colors[ImGuiCol_SliderGrab] = Color2ImVec4(SliderGrab);
  colors[ImGuiCol_SliderGrabActive] = Color2ImVec4(SliderGrabActive);
  colors[ImGuiCol_Button] = Color2ImVec4(Button);
  colors[ImGuiCol_ButtonHovered] = Color2ImVec4(ButtonHovered);
  colors[ImGuiCol_ButtonActive] = Color2ImVec4(ButtonActive);
  colors[ImGuiCol_Header] = Color2ImVec4(Header);
  colors[ImGuiCol_HeaderHovered] = Color2ImVec4(HeaderHovered);
  colors[ImGuiCol_HeaderActive] = Color2ImVec4(HeaderActive);
  colors[ImGuiCol_Separator] = Color2ImVec4(Separator);
  colors[ImGuiCol_SeparatorHovered] = Color2ImVec4(SeparatorHovered);
  colors[ImGuiCol_SeparatorActive] = Color2ImVec4(SeparatorActive);
  colors[ImGuiCol_ResizeGrip] = Color2ImVec4(ResizeGrip);
  colors[ImGuiCol_ResizeGripHovered] = Color2ImVec4(ResizeGripHovered);
  colors[ImGuiCol_ResizeGripActive] = Color2ImVec4(ResizeGripActive);
  colors[ImGuiCol_Tab] = Color2ImVec4(Tab);
  colors[ImGuiCol_TabHovered] = Color2ImVec4(TabHovered);
  colors[ImGuiCol_TabActive] = Color2ImVec4(TabActive);
  colors[ImGuiCol_TabUnfocused] = Color2ImVec4(TabUnfocused);
  colors[ImGuiCol_TabUnfocusedActive] = Color2ImVec4(TabUnfocusedActive);
  colors[ImGuiCol_DockingPreview] = Color2ImVec4(DockingPreview);
  colors[ImGuiCol_DockingEmptyBg] = Color2ImVec4(DockingEmptyBg);
  colors[ImGuiCol_PlotLines] = Color2ImVec4(PlotLines);
  colors[ImGuiCol_PlotLinesHovered] = Color2ImVec4(PlotLinesHovered);
  colors[ImGuiCol_PlotHistogram] = Color2ImVec4(PlotHistogram);
  colors[ImGuiCol_PlotHistogramHovered] = Color2ImVec4(PlotHistogramHovered);
  colors[ImGuiCol_TableHeaderBg] = Color2ImVec4(TableHeaderBg);
  colors[ImGuiCol_TableBorderStrong] = Color2ImVec4(TableBorderStrong);
  colors[ImGuiCol_TableBorderLight] = Color2ImVec4(TableBorderLight);
  colors[ImGuiCol_TableRowBg] = Color2ImVec4(TableRowBg);
  colors[ImGuiCol_TableRowBgAlt] = Color2ImVec4(TableRowBgAlt);
  colors[ImGuiCol_TextSelectedBg] = Color2ImVec4(TextSelectedBg);
  colors[ImGuiCol_DragDropTarget] = Color2ImVec4(DragDropTarget);
  colors[ImGuiCol_NavHighlight] = Color2ImVec4(NavHighlight);
  colors[ImGuiCol_NavWindowingHighlight] = Color2ImVec4(NavWindowingHighlight);
  colors[ImGuiCol_NavWindowingDimBg] = Color2ImVec4(NavWindowingDimBg);
  colors[ImGuiCol_ModalWindowDimBg] = Color2ImVec4(ModalWindowDimBg);

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
