#pragma once
#include "Core/Config/IConfig.h"

#include "ImGuiConfig.generated.h"
#include "Math/Vector.h"

KSTRUCT()
struct ImGuiColorTheme {
  GENERATED_BODY(ImGuiColorTheme)

  // Colors
  KPROPERTY() Vector4f Text { 1.00f, 1.00f, 1.00f, 1.00f };
  KPROPERTY() Vector4f TextDisabled { 0.50f, 0.50f, 0.50f, 1.00f };
  KPROPERTY() Vector4f WindowBg { 0.10f, 0.10f, 0.10f, 1.00f };
  KPROPERTY() Vector4f ChildBg { 0.00f, 0.00f, 0.00f, 0.00f };
  KPROPERTY() Vector4f PopupBg { 0.19f, 0.19f, 0.19f, 0.92f };
  KPROPERTY() Vector4f Border { 0.19f, 0.19f, 0.19f, 0.29f };
  KPROPERTY() Vector4f BorderShadow { 0.00f, 0.00f, 0.00f, 0.24f };
  KPROPERTY() Vector4f FrameBg { 0.05f, 0.05f, 0.05f, 0.54f };
  KPROPERTY() Vector4f FrameBgHovered { 0.19f, 0.19f, 0.19f, 0.54f };
  KPROPERTY() Vector4f FrameBgActive { 0.20f, 0.22f, 0.23f, 1.00f };
  KPROPERTY() Vector4f TitleBg { 0.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f TitleBgActive { 0.06f, 0.06f, 0.06f, 1.00f };
  KPROPERTY() Vector4f TitleBgCollapsed { 0.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f MenuBarBg { 0.14f, 0.14f, 0.14f, 1.00f };
  KPROPERTY() Vector4f ScrollbarBg { 0.05f, 0.05f, 0.05f, 0.54f };
  KPROPERTY() Vector4f ScrollbarGrab { 0.34f, 0.34f, 0.34f, 0.54f };
  KPROPERTY() Vector4f ScrollbarGrabHovered { 0.40f, 0.40f, 0.40f, 0.54f };
  KPROPERTY() Vector4f ScrollbarGrabActive { 0.56f, 0.56f, 0.56f, 0.54f };
  KPROPERTY() Vector4f CheckMark { 0.33f, 0.67f, 0.86f, 1.00f };
  KPROPERTY() Vector4f SliderGrab { 0.34f, 0.34f, 0.34f, 0.54f };
  KPROPERTY() Vector4f SliderGrabActive { 0.56f, 0.56f, 0.56f, 0.54f };
  KPROPERTY() Vector4f Button { 0.05f, 0.05f, 0.05f, 0.54f };
  KPROPERTY() Vector4f ButtonHovered { 0.19f, 0.19f, 0.19f, 0.54f };
  KPROPERTY() Vector4f ButtonActive { 0.20f, 0.22f, 0.23f, 1.00f };
  KPROPERTY() Vector4f Header { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Vector4f HeaderHovered { 0.00f, 0.00f, 0.00f, 0.36f };
  KPROPERTY() Vector4f HeaderActive { 0.20f, 0.22f, 0.23f, 0.33f };
  KPROPERTY() Vector4f Separator { 0.28f, 0.28f, 0.28f, 0.29f };
  KPROPERTY() Vector4f SeparatorHovered { 0.44f, 0.44f, 0.44f, 0.29f };
  KPROPERTY() Vector4f SeparatorActive { 0.40f, 0.44f, 0.47f, 1.00f };
  KPROPERTY() Vector4f ResizeGrip { 0.28f, 0.28f, 0.28f, 0.29f };
  KPROPERTY() Vector4f ResizeGripHovered { 0.44f, 0.44f, 0.44f, 0.29f };
  KPROPERTY() Vector4f ResizeGripActive { 0.40f, 0.44f, 0.47f, 1.00f };
  KPROPERTY() Vector4f Tab { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Vector4f TabHovered { 0.14f, 0.14f, 0.14f, 1.00f };
  KPROPERTY() Vector4f TabActive { 0.20f, 0.20f, 0.20f, 0.36f };
  KPROPERTY() Vector4f TabUnfocused { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Vector4f TabUnfocusedActive { 0.14f, 0.14f, 0.14f, 1.00f };
  KPROPERTY() Vector4f DockingPreview { 0.33f, 0.67f, 0.86f, 1.00f };
  KPROPERTY() Vector4f DockingEmptyBg { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f PlotLines { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f PlotLinesHovered { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f PlotHistogram { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f PlotHistogramHovered { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f TableHeaderBg { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Vector4f TableBorderStrong { 0.00f, 0.00f, 0.00f, 0.52f };
  KPROPERTY() Vector4f TableBorderLight { 0.28f, 0.28f, 0.28f, 0.29f };
  KPROPERTY() Vector4f TableRowBg { 0.00f, 0.00f, 0.00f, 0.00f };
  KPROPERTY() Vector4f TableRowBgAlt { 1.00f, 1.00f, 1.00f, 0.06f };
  KPROPERTY() Vector4f TextSelectedBg { 0.20f, 0.22f, 0.23f, 1.00f };
  KPROPERTY() Vector4f DragDropTarget { 0.33f, 0.67f, 0.86f, 1.00f };
  KPROPERTY() Vector4f NavHighlight { 1.00f, 0.00f, 0.00f, 1.00f };
  KPROPERTY() Vector4f NavWindowingHighlight { 1.00f, 0.00f, 0.00f, 0.70f };
  KPROPERTY() Vector4f NavWindowingDimBg { 1.00f, 0.00f, 0.00f, 0.20f };
  KPROPERTY() Vector4f ModalWindowDimBg { 1.00f, 0.00f, 0.00f, 0.35f };

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

  void ApplyImGuiSettings();
};