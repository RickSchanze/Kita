#pragma once
#include "Core/Config/IConfig.h"
#include "Math/Color.h"
#include "Math/Vector.h"

#include "ImGuiConfig.generated.h"

KSTRUCT()
struct ImGuiColorTheme {
  GENERATED_BODY(ImGuiColorTheme)

  // Colors
  KPROPERTY() Color Text { 1.00f, 1.00f, 1.00f, 1.00f };
  KPROPERTY() Color TextDisabled { 0.50f, 0.50f, 0.50f, 1.00f };
  KPROPERTY() Color WindowBg { 0.10f, 0.10f, 0.10f, 1.00f };
  KPROPERTY() Color ChildBg { 0.00f, 0.00f, 0.00f, 0.00f };
  KPROPERTY() Color PopupBg { 0.19f, 0.19f, 0.19f, 0.92f };
  KPROPERTY() Color Border { 0.19f, 0.19f, 0.19f, 0.29f };
  KPROPERTY() Color BorderShadow { 0.00f, 0.00f, 0.00f, 0.24f };
  KPROPERTY() Color FrameBg { 0.05f, 0.05f, 0.05f, 0.54f };
  KPROPERTY() Color FrameBgHovered { 0.19f, 0.19f, 0.19f, 0.54f };
  KPROPERTY() Color FrameBgActive { 0.20f, 0.22f, 0.23f, 1.00f };
  KPROPERTY() Color TitleBg { 0.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color TitleBgActive { 0.06f, 0.06f, 0.06f, 1.00f };
  KPROPERTY() Color TitleBgCollapsed { 0.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color MenuBarBg { 0.14f, 0.14f, 0.14f, 1.00f };
  KPROPERTY() Color ScrollbarBg { 0.05f, 0.05f, 0.05f, 0.54f };
  KPROPERTY() Color ScrollbarGrab { 0.34f, 0.34f, 0.34f, 0.54f };
  KPROPERTY() Color ScrollbarGrabHovered { 0.40f, 0.40f, 0.40f, 0.54f };
  KPROPERTY() Color ScrollbarGrabActive { 0.56f, 0.56f, 0.56f, 0.54f };
  KPROPERTY() Color CheckMark { 0.33f, 0.67f, 0.86f, 1.00f };
  KPROPERTY() Color SliderGrab { 0.34f, 0.34f, 0.34f, 0.54f };
  KPROPERTY() Color SliderGrabActive { 0.56f, 0.56f, 0.56f, 0.54f };
  KPROPERTY() Color Button { 0.05f, 0.05f, 0.05f, 0.54f };
  KPROPERTY() Color ButtonHovered { 0.19f, 0.19f, 0.19f, 0.54f };
  KPROPERTY() Color ButtonActive { 0.20f, 0.22f, 0.23f, 1.00f };
  KPROPERTY() Color Header { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Color HeaderHovered { 0.00f, 0.00f, 0.00f, 0.36f };
  KPROPERTY() Color HeaderActive { 0.20f, 0.22f, 0.23f, 0.33f };
  KPROPERTY() Color Separator { 0.28f, 0.28f, 0.28f, 0.29f };
  KPROPERTY() Color SeparatorHovered { 0.44f, 0.44f, 0.44f, 0.29f };
  KPROPERTY() Color SeparatorActive { 0.40f, 0.44f, 0.47f, 1.00f };
  KPROPERTY() Color ResizeGrip { 0.28f, 0.28f, 0.28f, 0.29f };
  KPROPERTY() Color ResizeGripHovered { 0.44f, 0.44f, 0.44f, 0.29f };
  KPROPERTY() Color ResizeGripActive { 0.40f, 0.44f, 0.47f, 1.00f };
  KPROPERTY() Color Tab { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Color TabHovered { 0.14f, 0.14f, 0.14f, 1.00f };
  KPROPERTY() Color TabActive { 0.20f, 0.20f, 0.20f, 0.36f };
  KPROPERTY() Color TabUnfocused { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Color TabUnfocusedActive { 0.14f, 0.14f, 0.14f, 1.00f };
  KPROPERTY() Color DockingPreview { 0.33f, 0.67f, 0.86f, 1.00f };
  KPROPERTY() Color DockingEmptyBg { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color PlotLines { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color PlotLinesHovered { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color PlotHistogram { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color PlotHistogramHovered { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color TableHeaderBg { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Color TableBorderStrong { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Color TableBorderLight { 0.28f, 0.28f, 0.28f, 0.29f };
  KPROPERTY() Color TableRowBg { 0.00f, 0.00f, 0.00f, 0.00f };
  KPROPERTY() Color TableRowBgAlt { 1.00f, 1.00f, 1.00f, 0.06f };
  KPROPERTY() Color TextSelectedBg { 0.20f, 0.22f, 0.23f, 1.00f };
  KPROPERTY() Color DragDropTarget { 0.33f, 0.67f, 0.86f, 1.00f };
  KPROPERTY() Color NavHighlight { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Color NavWindowingHighlight { 1.00f, 0.00f, 0.00f, 0.70f };
  KPROPERTY() Color NavWindowingDimBg { 1.00f, 0.00f, 0.00f, 0.20f };
  KPROPERTY() Color ModalWindowDimBg { 1.00f, 0.00f, 0.00f, 0.35f };

  // Style
  KPROPERTY() Vector2f WindowPadding { 8.00f, 8.00f };
  KPROPERTY() Vector2f FramePadding { 5.00f, 2.00f };
  KPROPERTY() Vector2f CellPadding { 6.00f, 6.00f };
  KPROPERTY() Vector2f ItemSpacing { 6.00f, 6.00f };
  KPROPERTY() Vector2f ItemInnerSpacing { 6.00f, 6.00f };
  KPROPERTY() Vector2f TouchExtraPadding { 0.00f, 0.00f };
  KPROPERTY() float IndentSpacing = 25.0f;
  KPROPERTY() float ScrollbarSize = 15.0f;
  KPROPERTY() float GrabMinSize = 10.0f;
  KPROPERTY() float WindowBorderSize = 1.0f;
  KPROPERTY() float ChildBorderSize = 1.0f;
  KPROPERTY() float PopupBorderSize = 1.0f;
  KPROPERTY() float FrameBorderSize = 1.0f;
  KPROPERTY() float TabBorderSize = 1.0f;
  KPROPERTY() float WindowRounding = 7.0f;
  KPROPERTY() float ChildRounding = 4.0f;
  KPROPERTY() float FrameRounding = 3.0f;
  KPROPERTY() float PopupRounding = 4.0f;
  KPROPERTY() float ScrollbarRounding = 9.0f;
  KPROPERTY() float GrabRounding = 3.0f;
  KPROPERTY() float LogSliderDeadzone = 4.0f;
  KPROPERTY() float TabRounding = 4.0f;

  void ApplyTheme() const;
};

KCLASS(Category = ImGui)
class ImGuiConfig : public IConfig {
  GENERATED_BODY(ImGuiConfig)

public:
  KPROPERTY()
  String FontPath = "Assets/Font/Default.ttf";

  KPROPERTY()
  String CharsetPath = "Assets/Font/Charset.txt";

  KPROPERTY()
  float FontSize = 16.0f;

  KPROPERTY()
  ImGuiColorTheme Theme;

  KPROPERTY()
  String IconFontPath = "Assets/Font/IconFont.ttf";

  void ApplyImGuiSettings();
};