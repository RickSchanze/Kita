#pragma once
#include "Core/String/StringView.h"

#include <imgui.h>

/// 封装了ImGui的UI
class EditorUI {
public:
  enum EWindowFlagBits {
    WFB_None = 0,
    WFB_NoTitleBar = 1 << 0,                 // Disable title-bar
    WFB_NoResize = 1 << 1,                   // Disable user resizing with the lower-right grip
    WFB_NoMove = 1 << 2,                     // Disable user moving the window
    WFB_NoScrollbar = 1 << 3,                // Disable scrollbars (window can still scroll with mouse or programmatically)
    WFB_NoScrollWithMouse = 1 << 4,          // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    WFB_NoCollapse = 1 << 5,                 // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    WFB_AlwaysAutoResize = 1 << 6,           // Resize every window to its content every frame
    WFB_NoBackground = 1 << 7,               // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    WFB_NoSavedSettings = 1 << 8,            // Never load/save settings in .ini file
    WFB_NoMouseInputs = 1 << 9,              // Disable catching mouse, hovering test with pass through.
    WFB_MenuBar = 1 << 10,                   // Has a menu-bar
    WFB_HorizontalScrollbar = 1 << 11,       // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to
                                             // specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    WFB_NoFocusOnAppearing = 1 << 12,        // Disable taking focus when transitioning from hidden to visible state
    WFB_NoBringToFrontOnFocus = 1 << 13,     // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    WFB_AlwaysVerticalScrollbar = 1 << 14,   // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    WFB_AlwaysHorizontalScrollbar = 1 << 15, // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    WFB_NoNavInputs = 1 << 16,               // No keyboard/gamepad navigation within the window
    WFB_NoNavFocus = 1 << 17,                // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    WFB_UnsavedDocument = 1 << 18,           // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user
                                             // to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    WFB_NoDocking = 1 << 19,                 // Disable docking of this window
    WFB_NoNav = WFB_NoNavInputs | WFB_NoNavFocus,
    WFB_NoDecoration = WFB_NoTitleBar | WFB_NoResize | WFB_NoScrollbar | WFB_NoCollapse,
    WFB_NoInputs = WFB_NoMouseInputs | WFB_NoNavInputs | WFB_NoNavFocus,
  };
  using EWindowFlags = Int32;

  static void Begin(const StringView Title, bool* Open = nullptr, EWindowFlags Flags = WFB_None) { ImGui::Begin(Title.Data(), Open, Flags); }
  static void End() { ImGui::End(); }

  static bool BeginMenuBar() { return ImGui::BeginMenuBar(); }
  static void EndMenuBar() { ImGui::EndMenuBar(); }
  static void BeginMainMenuBar() { ImGui::BeginMainMenuBar(); }
  static void EndMainMenuBar() { ImGui::EndMainMenuBar(); }

  static bool BeginMenu(StringView Name) { return ImGui::BeginMenu(Name.Data()); }
  static void EndMenu() { ImGui::EndMenu(); }

  static bool MenuItem(StringView Name) { return ImGui::MenuItem(Name.Data()); }

  template <typename... Args> static inline void Text(const char* fmt, Args&&... args) {
    // 先处理你自己的逻辑，比如颜色/前缀
    ImGui::Text(fmt, std::forward<Args>(args)...);
  }
};
