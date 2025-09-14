#pragma once
#include "Core/Container/Stack.h"
#include "Core/String/StringView.h"
#include "Math/Color.h"
#include "Math/Vector.h"

#include <imgui.h>

namespace Logcat {
inline constexpr auto Editor = "Editor";
}

/// 封装了ImGui的UI
class EditorUI {
public:
  static ImVec2 Vector2fToImVec2(const Vector2f Vec) { return ImVec2(Vec.X(), Vec.Y()); }
  static Vector2f ImVec2ToVector2f(const ImVec2 Vec) { return Vector2f(Vec.x, Vec.y); }
  static ImU32 ColorToImU32(const Color& InColor) { return ImGui::ColorConvertFloat4ToU32(ImVec4(InColor.Data.X(), InColor.Data.Y(), InColor.Data.Z(), InColor.Data.W())); }
  static void Indent() { ImGui::Indent(); }

  enum class EWindowFlags {
    None = 0,
    NoTitleBar = 1 << 0,                 // Disable title-bar
    NoResize = 1 << 1,                   // Disable user resizing with the lower-right grip
    NoMove = 1 << 2,                     // Disable user moving the window
    NoScrollbar = 1 << 3,                // Disable scrollbars (window can still scroll with mouse or programmatically)
    NoScrollWithMouse = 1 << 4,          // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    NoCollapse = 1 << 5,                 // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    AlwaysAutoResize = 1 << 6,           // Resize every window to its content every frame
    NoBackground = 1 << 7,               // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    NoSavedSettings = 1 << 8,            // Never load/save settings in .ini file
    NoMouseInputs = 1 << 9,              // Disable catching mouse, hovering test with pass through.
    MenuBar = 1 << 10,                   // Has a menu-bar
    HorizontalScrollbar = 1 << 11,       // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to
                                         // specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    NoFocusOnAppearing = 1 << 12,        // Disable taking focus when transitioning from hidden to visible state
    NoBringToFrontOnFocus = 1 << 13,     // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    AlwaysVerticalScrollbar = 1 << 14,   // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    AlwaysHorizontalScrollbar = 1 << 15, // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    NoNavInputs = 1 << 16,               // No keyboard/gamepad navigation within the window
    NoNavFocus = 1 << 17,                // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    UnsavedDocument = 1 << 18,           // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user
                                         // to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    NoDocking = 1 << 19,                 // Disable docking of this window
    NoNav = NoNavInputs | NoNavFocus,
    NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
    NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,
  };

  static void Begin(const StringView Title, bool* Open = nullptr, const EWindowFlags Flags = EWindowFlags::None) { ImGui::Begin(Title.Data(), Open, static_cast<ImGuiWindowFlags>(Flags)); }
  static void End() { ImGui::End(); }

  static bool BeginMenuBar() { return ImGui::BeginMenuBar(); }
  static void EndMenuBar() { ImGui::EndMenuBar(); }
  static void BeginMainMenuBar() { ImGui::BeginMainMenuBar(); }
  static void EndMainMenuBar() { ImGui::EndMainMenuBar(); }

  static bool BeginMenu(const StringView Name) { return ImGui::BeginMenu(Name.Data()); }
  static void EndMenu() { ImGui::EndMenu(); }

  static bool MenuItem(const StringView Name) { return ImGui::MenuItem(Name.Data()); }

  template <typename... Args> static void Text(const char* fmt, Args&&... args) {
    // 先处理你自己的逻辑，比如颜色/前缀
    ImGui::Text(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args> static void TextColored(Color InColor, const char* fmt, Args&&... args) {
    // 先处理你自己的逻辑，比如颜色/前缀
    ImGui::TextColored(ImVec4(InColor.R(), InColor.G(), InColor.B(), InColor.A()), fmt, std::forward<Args>(args)...);
  }

  static Vector2f GetContentRegionAvail() {
    ImVec2 Vec = ImGui::GetContentRegionAvail();
    return {Vec.x, Vec.y};
  }

  enum class ETableFlags {
    // Features
    None = 0,
    Resizable = 1 << 0,         // Enable resizing columns.
    Reorderable = 1 << 1,       // Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
    Hideable = 1 << 2,          // Enable hiding/disabling columns in context menu.
    Sortable = 1 << 3,          // Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see SortMulti and SortTristate.
    NoSavedSettings = 1 << 4,   // Disable persisting columns order, width and sort settings in the .ini file.
    ContextMenuInBody = 1 << 5, // Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().
                                // Decorations
    RowBg = 1 << 6,             // Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)
    BordersInnerH = 1 << 7,     // Draw horizontal borders between rows.
    BordersOuterH = 1 << 8,     // Draw horizontal borders at the top and bottom.
    BordersInnerV = 1 << 9,     // Draw vertical borders between columns.
    BordersOuterV = 1 << 10,    // Draw vertical borders on the left and right sides.
    BordersH = BordersInnerH | BordersOuterH,     // Draw horizontal borders.
    BordersV = BordersInnerV | BordersOuterV,     // Draw vertical borders.
    BordersInner = BordersInnerV | BordersInnerH, // Draw inner borders.
    BordersOuter = BordersOuterV | BordersOuterH, // Draw outer borders.
    Borders = BordersInner | BordersOuter,        // Draw all borders.
    NoBordersInBody = 1 << 11,                    // [ALPHA] Disable vertical borders in columns Body (borders will always appear in Headers). -> May move to style
    NoBordersInBodyUntilResize = 1 << 12,         // [ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers). -> May move to style
                                                  // Sizing Policy (read above for defaults)
    SizingFixedFit = 1 << 13,                     // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.
    SizingFixedSame = 2 << 13,                    // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Implicitly enable
                                                  // NoKeepColumnsVisible.
    SizingStretchProp = 3 << 13,                  // Columns default to _WidthStretch with default weights proportional to each columns contents widths.
    SizingStretchSame = 4 << 13,                  // Columns default to _WidthStretch with default weights all equal, unless overridden by TableSetupColumn().
                                                  // Sizing Extra Options
    NoHostExtendX = 1 << 16,          // Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
    NoHostExtendY = 1 << 17,          // Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data
                                      // below the limit will be clipped and not visible.
    NoKeepColumnsVisible = 1 << 18,   // Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
    PreciseWidths = 1 << 19,          // Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this
                                      // flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth. Clipping
    NoClip = 1 << 20,                 // Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible
                                      // with TableSetupScrollFreeze(). Padding
    PadOuterX = 1 << 21,              // Default if BordersOuterV is on. Enable outermost padding. Generally desirable if you have headers.
    NoPadOuterX = 1 << 22,            // Default if BordersOuterV is off. Disable outermost padding.
    NoPadInnerX = 1 << 23,            // Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
                                      // Scrolling
    ScrollX = 1 << 24,                // Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this creates
                                      // a child window, ScrollY is currently generally recommended when using ScrollX.
    ScrollY = 1 << 25,                // Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.
                                      // Sorting
    SortMulti = 1 << 26,              // Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).
    SortTristate = 1 << 27,           // Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).
                                      // Miscellaneous
    HighlightHoveredColumn = 1 << 28, // Highlight column headers when hovered (may evolve into a fuller highlight)
  };
  static bool BeginTable(const StringView Id, const int Columns, const ETableFlags Flags = ETableFlags::None, const Vector2f& OuterSize = Vector2f(0.0f, 0.0f), const float InnerWidth = 0.0f) {
    return ImGui::BeginTable(Id.Data(), Columns, static_cast<ImGuiTableFlags>(Flags), ImVec2(OuterSize.X(), OuterSize.Y()), InnerWidth);
  }

  static void EndTable() { ImGui::EndTable(); }

  // Flags for ImGui::TableSetupColumn()
  enum class ETableColumnFlags {
    // Input configuration flags
    None = 0,
    Disabled = 1 << 0,              // Overriding/master disable flag: hide column, won't show in context menu (unlike calling TableSetColumnEnabled() which manipulates the user accessible state)
    DefaultHide = 1 << 1,           // Default as a hidden/disabled column.
    DefaultSort = 1 << 2,           // Default as a sorting column.
    WidthStretch = 1 << 3,          // Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).
    WidthFixed = 1 << 4,            // Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).
    NoResize = 1 << 5,              // Disable manual resizing.
    NoReorder = 1 << 6,             // Disable manual reordering this column, this will also prevent other columns from crossing over this column.
    NoHide = 1 << 7,                // Disable ability to hide/disable this column.
    NoClip = 1 << 8,                // Disable clipping for this column (all NoClip columns will render in a same draw command).
    NoSort = 1 << 9,                // Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table).
    NoSortAscending = 1 << 10,      // Disable ability to sort in the ascending direction.
    NoSortDescending = 1 << 11,     // Disable ability to sort in the descending direction.
    NoHeaderLabel = 1 << 12,        // TableHeadersRow() will submit an empty label for this column. Convenient for some small columns. Name will still appear in context menu or in
                                    // angled headers. You may append into this cell by calling TableSetColumnIndex() right after the TableHeadersRow() call.
    NoHeaderWidth = 1 << 13,        // Disable header text width contribution to automatic column width.
    PreferSortAscending = 1 << 14,  // Make the initial sort direction Ascending when first sorting on this column (default).
    PreferSortDescending = 1 << 15, // Make the initial sort direction Descending when first sorting on this column.
    IndentEnable = 1 << 16,         // Use current Indent value when entering cell (default for column 0).
    IndentDisable = 1 << 17,        // Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.
    AngledHeader = 1 << 18,         // TableHeadersRow() will submit an angled header row for this column. Note this will add an extra row.

    // Output status flags, read-only via TableGetColumnFlags()
    IsEnabled = 1 << 24, // Status: is enabled == not hidden by user/api (referred to as "Hide" in _DefaultHide and _NoHide) flags.
    IsVisible = 1 << 25, // Status: is visible == is enabled AND not clipped by scrolling.
    IsSorted = 1 << 26,  // Status: is currently part of the sort specs
    IsHovered = 1 << 27, // Status: is hovered by mouse

    // [Internal] Combinations and masks
    WidthMask_ = WidthStretch | WidthFixed,
    IndentMask_ = IndentEnable | IndentDisable,
    StatusMask_ = IsEnabled | IsVisible | IsSorted | IsHovered,
    NoDirectResize_ = 1 << 30, // [Internal] Disable user resizing this column directly (it may however we resized indirectly from its left edge)
  };
  static void TableSetupColumn(const StringView Name, const ETableColumnFlags Flags = ETableColumnFlags::None, const float Width = 0.0f, const ImGuiID UserID = 0) {
    return ImGui::TableSetupColumn(Name.Data(), static_cast<ImGuiTableColumnFlags>(Flags), Width, UserID);
  }
  static void TableHeadersRow() { ImGui::TableHeadersRow(); }
  static void TableNextRow() { ImGui::TableNextRow(); }
  static void TableSetColumnIndex(const int ColumnIndex) { ImGui::TableSetColumnIndex(ColumnIndex); }

  enum class EComboFlags {
    None = 0,
    PopupAlignLeft = 1 << 0,  // Align the popup toward the left by default
    HeightSmall = 1 << 1,     // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    HeightRegular = 1 << 2,   // Max ~8 items visible (default)
    HeightLarge = 1 << 3,     // Max ~20 items visible
    HeightLargest = 1 << 4,   // As many fitting items as possible
    NoArrowButton = 1 << 5,   // Display on the preview box without the square arrow button
    NoPreview = 1 << 6,       // Display only a square arrow button
    WidthFitPreview = 1 << 7, // Width dynamically calculated from preview contents
    HeightMask_ = HeightSmall | HeightRegular | HeightLarge | HeightLargest,
  };
  static bool BeginCombo(const StringView Label, const StringView PreviewValue, const EComboFlags Flags = EComboFlags::None) {
    return ImGui::BeginCombo(Label.Data(), PreviewValue.Data(), static_cast<ImGuiComboFlags>(Flags));
  }
  static void EndCombo() { ImGui::EndCombo(); }

  enum class ESelectableFlags {
    None = 0,
    NoAutoClosePopups = 1 << 0, // Clicking this doesn't close parent popup window (overrides ImGuiItemFlags_AutoClosePopups)
    SpanAllColumns = 1 << 1,    // Frame will span all columns of its container table (text will still fit in current column)
    AllowDoubleClick = 1 << 2,  // Generate press events on double clicks too
    Disabled = 1 << 3,          // Cannot be selected, display grayed out text
    AllowOverlap = 1 << 4,      // (WIP) Hit testing to allow subsequent widgets to overlap this one
    Highlight = 1 << 5,         // Make the item be displayed as if it is hovered
  };

  static bool Selectable(const StringView Label, const bool Selected = false, const ESelectableFlags Flags = ESelectableFlags::None, Vector2f Size = Vector2f(0.0f, 0.0f)) {
    return ImGui::Selectable(Label.Data(), Selected, static_cast<ImGuiSelectableFlags>(Flags), {Size.X(), Size.Y()});
  }

  static void SameLine(const float Offset = 0.0f) { ImGui::SameLine(Offset); }

  enum class EChildFlags {
    None = 0,
    Borders = 1 << 0,                // Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason)
    AlwaysUseWindowPadding = 1 << 1, // Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    ResizeX = 1 << 2,                // Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags)
    ResizeY = 1 << 3,                // Allow resize from bottom border (layout direction). "
    AutoResizeX = 1 << 4,            // Enable auto-resizing width. Read "IMPORTANT: Size measurement" details above.
    AutoResizeY = 1 << 5,            // Enable auto-resizing height. Read "IMPORTANT: Size measurement" details above.
    AlwaysAutoResize = 1 << 6,       // Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
    FrameStyle = 1 << 7,   // Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.
    NavFlattened = 1 << 8, // [BETA] Share focus scope, allow keyboard/gamepad navigation to cross over parent border to this child or between sibling child windows.
  };

  enum class ESplitterDirection { Horizontal, Vertical };

  // 使用Button模拟的Splitter
  static void Splitter(float& S1, float& S2, ESplitterDirection Direction = ESplitterDirection::Vertical, float MinSize1 = 0.0f, float MinSize2 = 0.0f, float Thickness = 5);

  static void InitializeColors();

  static bool Button(StringView Label, Vector2f Size = {}, Optional<UInt32> TextColor = NullOpt);

private:
  static inline UInt32 SplitterNormal;
  static inline UInt32 SplitterHovered;
  static inline UInt32 SplitterActive;
};

ENABLE_BITMASK_OPERATORS(EditorUI::EChildFlags)
ENABLE_BITMASK_OPERATORS(EditorUI::EWindowFlags)
ENABLE_BITMASK_OPERATORS(EditorUI::ESelectableFlags)
ENABLE_BITMASK_OPERATORS(EditorUI::ETableColumnFlags)
ENABLE_BITMASK_OPERATORS(EditorUI::ETableFlags)
ENABLE_BITMASK_OPERATORS(EditorUI::EComboFlags)
