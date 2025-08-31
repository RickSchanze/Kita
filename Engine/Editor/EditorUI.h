#pragma once
#include "Core/String/StringView.h"
#include "Math/Color.h"
#include "Math/Vector.h"

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

  enum ETableFlagBits {
    // Features
    TFB_None = 0,
    TFB_Resizable = 1 << 0,         // Enable resizing columns.
    TFB_Reorderable = 1 << 1,       // Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
    TFB_Hideable = 1 << 2,          // Enable hiding/disabling columns in context menu.
    TFB_Sortable = 1 << 3,          // Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see TFB_SortMulti and TFB_SortTristate.
    TFB_NoSavedSettings = 1 << 4,   // Disable persisting columns order, width and sort settings in the .ini file.
    TFB_ContextMenuInBody = 1 << 5, // Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().
                                    // Decorations
    TFB_RowBg = 1 << 6,          // Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)
    TFB_BordersInnerH = 1 << 7,  // Draw horizontal borders between rows.
    TFB_BordersOuterH = 1 << 8,  // Draw horizontal borders at the top and bottom.
    TFB_BordersInnerV = 1 << 9,  // Draw vertical borders between columns.
    TFB_BordersOuterV = 1 << 10, // Draw vertical borders on the left and right sides.
    TFB_BordersH = TFB_BordersInnerH | TFB_BordersOuterH,     // Draw horizontal borders.
    TFB_BordersV = TFB_BordersInnerV | TFB_BordersOuterV,     // Draw vertical borders.
    TFB_BordersInner = TFB_BordersInnerV | TFB_BordersInnerH, // Draw inner borders.
    TFB_BordersOuter = TFB_BordersOuterV | TFB_BordersOuterH, // Draw outer borders.
    TFB_Borders = TFB_BordersInner | TFB_BordersOuter,        // Draw all borders.
    TFB_NoBordersInBody = 1 << 11,                            // [ALPHA] Disable vertical borders in columns Body (borders will always appear in Headers). -> May move to style
    TFB_NoBordersInBodyUntilResize = 1 << 12,                 // [ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers). -> May move to style
                                                              // Sizing Policy (read above for defaults)
    TFB_SizingFixedFit = 1 << 13,                             // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.
    TFB_SizingFixedSame = 2 << 13,      // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Implicitly enable
                                        // TFB_NoKeepColumnsVisible.
    TFB_SizingStretchProp = 3 << 13,    // Columns default to _WidthStretch with default weights proportional to each columns contents widths.
    TFB_SizingStretchSame = 4 << 13,    // Columns default to _WidthStretch with default weights all equal, unless overridden by TableSetupColumn().
                                        // Sizing Extra Options
    TFB_NoHostExtendX = 1 << 16,        // Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
    TFB_NoHostExtendY = 1 << 17,        // Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data
                                        // below the limit will be clipped and not visible.
    TFB_NoKeepColumnsVisible = 1 << 18, // Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
    TFB_PreciseWidths = 1 << 19,        // Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this
                                        // flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth. Clipping
    TFB_NoClip = 1 << 20,       // Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible
                                // with TableSetupScrollFreeze(). Padding
    TFB_PadOuterX = 1 << 21,    // Default if BordersOuterV is on. Enable outermost padding. Generally desirable if you have headers.
    TFB_NoPadOuterX = 1 << 22,  // Default if BordersOuterV is off. Disable outermost padding.
    TFB_NoPadInnerX = 1 << 23,  // Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
                                // Scrolling
    TFB_ScrollX = 1 << 24,      // Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this creates
                                // a child window, ScrollY is currently generally recommended when using ScrollX.
    TFB_ScrollY = 1 << 25,      // Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.
                                // Sorting
    TFB_SortMulti = 1 << 26,    // Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).
    TFB_SortTristate = 1 << 27, // Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).
                                // Miscellaneous
    TFB_HighlightHoveredColumn = 1 << 28, // Highlight column headers when hovered (may evolve into a fuller highlight)

    // [Internal] Combinations and masks
    TFB_SizingMask_ = TFB_SizingFixedFit | TFB_SizingFixedSame | TFB_SizingStretchProp | TFB_SizingStretchSame,
  };
  using ETableFlags = int;
  static bool BeginTable(const StringView Id, const int Columns, const ImGuiTableFlags Flags = 0, const Vector2f& OuterSize = Vector2f(0.0f, 0.0f), const float InnerWidth = 0.0f) {
    return ImGui::BeginTable(Id.Data(), Columns, Flags, ImVec2(OuterSize.X(), OuterSize.Y()), InnerWidth);
  }

  static void EndTable() { ImGui::EndTable(); }

  // Flags for ImGui::TableSetupColumn()
  enum ETableColumnFlagBits {
    // Input configuration flags
    TCFB_None = 0,
    TCFB_Disabled = 1 << 0,              // Overriding/master disable flag: hide column, won't show in context menu (unlike calling TableSetColumnEnabled() which manipulates the user accessible state)
    TCFB_DefaultHide = 1 << 1,           // Default as a hidden/disabled column.
    TCFB_DefaultSort = 1 << 2,           // Default as a sorting column.
    TCFB_WidthStretch = 1 << 3,          // Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).
    TCFB_WidthFixed = 1 << 4,            // Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).
    TCFB_NoResize = 1 << 5,              // Disable manual resizing.
    TCFB_NoReorder = 1 << 6,             // Disable manual reordering this column, this will also prevent other columns from crossing over this column.
    TCFB_NoHide = 1 << 7,                // Disable ability to hide/disable this column.
    TCFB_NoClip = 1 << 8,                // Disable clipping for this column (all NoClip columns will render in a same draw command).
    TCFB_NoSort = 1 << 9,                // Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table).
    TCFB_NoSortAscending = 1 << 10,      // Disable ability to sort in the ascending direction.
    TCFB_NoSortDescending = 1 << 11,     // Disable ability to sort in the descending direction.
    TCFB_NoHeaderLabel = 1 << 12,        // TableHeadersRow() will submit an empty label for this column. Convenient for some small columns. Name will still appear in context menu or in
                                         // angled headers. You may append into this cell by calling TableSetColumnIndex() right after the TableHeadersRow() call.
    TCFB_NoHeaderWidth = 1 << 13,        // Disable header text width contribution to automatic column width.
    TCFB_PreferSortAscending = 1 << 14,  // Make the initial sort direction Ascending when first sorting on this column (default).
    TCFB_PreferSortDescending = 1 << 15, // Make the initial sort direction Descending when first sorting on this column.
    TCFB_IndentEnable = 1 << 16,         // Use current Indent value when entering cell (default for column 0).
    TCFB_IndentDisable = 1 << 17,        // Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.
    TCFB_AngledHeader = 1 << 18,         // TableHeadersRow() will submit an angled header row for this column. Note this will add an extra row.

    // Output status flags, read-only via TableGetColumnFlags()
    TCFB_IsEnabled = 1 << 24, // Status: is enabled == not hidden by user/api (referred to as "Hide" in _DefaultHide and _NoHide) flags.
    TCFB_IsVisible = 1 << 25, // Status: is visible == is enabled AND not clipped by scrolling.
    TCFB_IsSorted = 1 << 26,  // Status: is currently part of the sort specs
    TCFB_IsHovered = 1 << 27, // Status: is hovered by mouse

    // [Internal] Combinations and masks
    TCFB_WidthMask_ = TCFB_WidthStretch | TCFB_WidthFixed,
    TCFB_IndentMask_ = TCFB_IndentEnable | TCFB_IndentDisable,
    TCFB_StatusMask_ = TCFB_IsEnabled | TCFB_IsVisible | TCFB_IsSorted | TCFB_IsHovered,
    TCFB_NoDirectResize_ = 1 << 30, // [Internal] Disable user resizing this column directly (it may however we resized indirectly from its left edge)
  };
  using ETableColumnFlags = int;
  static void TableSetupColumn(StringView Name, ETableColumnFlags Flags = 0, float Width = 0.0f, ImGuiID UserID = 0) { return ImGui::TableSetupColumn(Name.Data(), Flags, Width, UserID); }
  static void TableHeadersRow() { ImGui::TableHeadersRow(); }
  static void TableNextRow() { ImGui::TableNextRow(); }
  static void TableSetColumnIndex(int ColumnIndex) { ImGui::TableSetColumnIndex(ColumnIndex); }

  enum ComboFlagBits {
    CFB_None = 0,
    CFB_PopupAlignLeft = 1 << 0,  // Align the popup toward the left by default
    CFB_HeightSmall = 1 << 1,     // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    CFB_HeightRegular = 1 << 2,   // Max ~8 items visible (default)
    CFB_HeightLarge = 1 << 3,     // Max ~20 items visible
    CFB_HeightLargest = 1 << 4,   // As many fitting items as possible
    CFB_NoArrowButton = 1 << 5,   // Display on the preview box without the square arrow button
    CFB_NoPreview = 1 << 6,       // Display only a square arrow button
    CFB_WidthFitPreview = 1 << 7, // Width dynamically calculated from preview contents
    CFB_HeightMask_ = CFB_HeightSmall | CFB_HeightRegular | CFB_HeightLarge | CFB_HeightLargest,
  };
  using ComboFlags = int;
  static bool BeginCombo(StringView Label, StringView PreviewValue, ComboFlags Flags = 0) { return ImGui::BeginCombo(Label.Data(), PreviewValue.Data(), Flags); }
  static void EndCombo() { ImGui::EndCombo(); }

  enum SelectableFlagBits {
    SFB_None = 0,
    SFB_NoAutoClosePopups = 1 << 0, // Clicking this doesn't close parent popup window (overrides ImGuiItemFlags_AutoClosePopups)
    SFB_SpanAllColumns = 1 << 1,    // Frame will span all columns of its container table (text will still fit in current column)
    SFB_AllowDoubleClick = 1 << 2,  // Generate press events on double clicks too
    SFB_Disabled = 1 << 3,          // Cannot be selected, display grayed out text
    SFB_AllowOverlap = 1 << 4,      // (WIP) Hit testing to allow subsequent widgets to overlap this one
    SFB_Highlight = 1 << 5,         // Make the item be displayed as if it is hovered

  };
  using ESelectableFlags = int;
  static bool Selectable(StringView Label, bool Selected = false, ESelectableFlags Flags = 0, Vector2f Size = Vector2f(0.0f, 0.0f)) {
    return ImGui::Selectable(Label.Data(), Selected, Flags, {Size.X(), Size.Y()});
  }

  static void SameLine(float Offset = 0.0f) { ImGui::SameLine(Offset); }
};
