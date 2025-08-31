// Auto-generated header file
#pragma once

#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/InputArchive.h"
#include "Core/Serialization/OutputArchive.h"
#define GENERATED_HEADER_ImGuiColorTheme \
typedef ImGuiColorTheme ThisStruct; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
const Type* GetType(); \
void WriteArchive(OutputArchive& Archive) const;\
void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ImGuiColorTheme(); }static void DestructSelf(void* Ptr) { ((ImGuiColorTheme*)(Ptr))->~ImGuiColorTheme(); }struct Z_TypeRegister_ImGuiColorTheme { \
Z_TypeRegister_ImGuiColorTheme() { \
TypeBuilder Builder{}; \
Builder.CreateType<ImGuiColorTheme>("ImGuiColorTheme"); \
Builder.SetConstructor(ImGuiColorTheme::ConstructSelf).SetDestructor(ImGuiColorTheme::DestructSelf); \
Builder.AddField("Text", &ImGuiColorTheme::Text); \
Builder.AddField("TextDisabled", &ImGuiColorTheme::TextDisabled); \
Builder.AddField("WindowBg", &ImGuiColorTheme::WindowBg); \
Builder.AddField("ChildBg", &ImGuiColorTheme::ChildBg); \
Builder.AddField("PopupBg", &ImGuiColorTheme::PopupBg); \
Builder.AddField("Border", &ImGuiColorTheme::Border); \
Builder.AddField("BorderShadow", &ImGuiColorTheme::BorderShadow); \
Builder.AddField("FrameBg", &ImGuiColorTheme::FrameBg); \
Builder.AddField("FrameBgHovered", &ImGuiColorTheme::FrameBgHovered); \
Builder.AddField("FrameBgActive", &ImGuiColorTheme::FrameBgActive); \
Builder.AddField("TitleBg", &ImGuiColorTheme::TitleBg); \
Builder.AddField("TitleBgActive", &ImGuiColorTheme::TitleBgActive); \
Builder.AddField("TitleBgCollapsed", &ImGuiColorTheme::TitleBgCollapsed); \
Builder.AddField("MenuBarBg", &ImGuiColorTheme::MenuBarBg); \
Builder.AddField("ScrollbarBg", &ImGuiColorTheme::ScrollbarBg); \
Builder.AddField("ScrollbarGrab", &ImGuiColorTheme::ScrollbarGrab); \
Builder.AddField("ScrollbarGrabHovered", &ImGuiColorTheme::ScrollbarGrabHovered); \
Builder.AddField("ScrollbarGrabActive", &ImGuiColorTheme::ScrollbarGrabActive); \
Builder.AddField("CheckMark", &ImGuiColorTheme::CheckMark); \
Builder.AddField("SliderGrab", &ImGuiColorTheme::SliderGrab); \
Builder.AddField("SliderGrabActive", &ImGuiColorTheme::SliderGrabActive); \
Builder.AddField("Button", &ImGuiColorTheme::Button); \
Builder.AddField("ButtonHovered", &ImGuiColorTheme::ButtonHovered); \
Builder.AddField("ButtonActive", &ImGuiColorTheme::ButtonActive); \
Builder.AddField("Header", &ImGuiColorTheme::Header); \
Builder.AddField("HeaderHovered", &ImGuiColorTheme::HeaderHovered); \
Builder.AddField("HeaderActive", &ImGuiColorTheme::HeaderActive); \
Builder.AddField("Separator", &ImGuiColorTheme::Separator); \
Builder.AddField("SeparatorHovered", &ImGuiColorTheme::SeparatorHovered); \
Builder.AddField("SeparatorActive", &ImGuiColorTheme::SeparatorActive); \
Builder.AddField("ResizeGrip", &ImGuiColorTheme::ResizeGrip); \
Builder.AddField("ResizeGripHovered", &ImGuiColorTheme::ResizeGripHovered); \
Builder.AddField("ResizeGripActive", &ImGuiColorTheme::ResizeGripActive); \
Builder.AddField("Tab", &ImGuiColorTheme::Tab); \
Builder.AddField("TabHovered", &ImGuiColorTheme::TabHovered); \
Builder.AddField("TabActive", &ImGuiColorTheme::TabActive); \
Builder.AddField("TabUnfocused", &ImGuiColorTheme::TabUnfocused); \
Builder.AddField("TabUnfocusedActive", &ImGuiColorTheme::TabUnfocusedActive); \
Builder.AddField("DockingPreview", &ImGuiColorTheme::DockingPreview); \
Builder.AddField("DockingEmptyBg", &ImGuiColorTheme::DockingEmptyBg); \
Builder.AddField("PlotLines", &ImGuiColorTheme::PlotLines); \
Builder.AddField("PlotLinesHovered", &ImGuiColorTheme::PlotLinesHovered); \
Builder.AddField("PlotHistogram", &ImGuiColorTheme::PlotHistogram); \
Builder.AddField("PlotHistogramHovered", &ImGuiColorTheme::PlotHistogramHovered); \
Builder.AddField("TableHeaderBg", &ImGuiColorTheme::TableHeaderBg); \
Builder.AddField("TableBorderStrong", &ImGuiColorTheme::TableBorderStrong); \
Builder.AddField("TableBorderLight", &ImGuiColorTheme::TableBorderLight); \
Builder.AddField("TableRowBg", &ImGuiColorTheme::TableRowBg); \
Builder.AddField("TableRowBgAlt", &ImGuiColorTheme::TableRowBgAlt); \
Builder.AddField("TextSelectedBg", &ImGuiColorTheme::TextSelectedBg); \
Builder.AddField("DragDropTarget", &ImGuiColorTheme::DragDropTarget); \
Builder.AddField("NavHighlight", &ImGuiColorTheme::NavHighlight); \
Builder.AddField("NavWindowingHighlight", &ImGuiColorTheme::NavWindowingHighlight); \
Builder.AddField("NavWindowingDimBg", &ImGuiColorTheme::NavWindowingDimBg); \
Builder.AddField("ModalWindowDimBg", &ImGuiColorTheme::ModalWindowDimBg); \
Builder.AddField("WindowPadding", &ImGuiColorTheme::WindowPadding); \
Builder.AddField("FramePadding", &ImGuiColorTheme::FramePadding); \
Builder.AddField("CellPadding", &ImGuiColorTheme::CellPadding); \
Builder.AddField("ItemSpacing", &ImGuiColorTheme::ItemSpacing); \
Builder.AddField("ItemInnerSpacing", &ImGuiColorTheme::ItemInnerSpacing); \
Builder.AddField("TouchExtraPadding", &ImGuiColorTheme::TouchExtraPadding); \
Builder.AddField("IndentSpacing", &ImGuiColorTheme::IndentSpacing); \
Builder.AddField("ScrollbarSize", &ImGuiColorTheme::ScrollbarSize); \
Builder.AddField("GrabMinSize", &ImGuiColorTheme::GrabMinSize); \
Builder.AddField("WindowBorderSize", &ImGuiColorTheme::WindowBorderSize); \
Builder.AddField("ChildBorderSize", &ImGuiColorTheme::ChildBorderSize); \
Builder.AddField("PopupBorderSize", &ImGuiColorTheme::PopupBorderSize); \
Builder.AddField("FrameBorderSize", &ImGuiColorTheme::FrameBorderSize); \
Builder.AddField("TabBorderSize", &ImGuiColorTheme::TabBorderSize); \
Builder.AddField("WindowRounding", &ImGuiColorTheme::WindowRounding); \
Builder.AddField("ChildRounding", &ImGuiColorTheme::ChildRounding); \
Builder.AddField("FrameRounding", &ImGuiColorTheme::FrameRounding); \
Builder.AddField("PopupRounding", &ImGuiColorTheme::PopupRounding); \
Builder.AddField("ScrollbarRounding", &ImGuiColorTheme::ScrollbarRounding); \
Builder.AddField("GrabRounding", &ImGuiColorTheme::GrabRounding); \
Builder.AddField("LogSliderDeadzone", &ImGuiColorTheme::LogSliderDeadzone); \
Builder.AddField("TabRounding", &ImGuiColorTheme::TabRounding); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ImGuiColorTheme __Z_TypeRegister_ImGuiColorTheme_Instance; \

#define GENERATED_HEADER_ImGuiConfig \
public: \
typedef ThisClass Super; \
typedef ImGuiConfig ThisClass; \
static FORCE_INLINE const Type* GetStaticType(); \
static FORCE_INLINE constexpr bool IsReflected() { return true; } \
virtual const Type* GetType(); \
virtual void WriteArchive(OutputArchive& Archive) const;\
virtual void ReadArchive(InputArchive& Archive);\
static void ConstructSelf(void* Ptr) { new (Ptr) ImGuiConfig(); }static void DestructSelf(void* Ptr) { ((ImGuiConfig*)(Ptr))->~ImGuiConfig(); }struct Z_TypeRegister_ImGuiConfig { \
Z_TypeRegister_ImGuiConfig() { \
TypeBuilder Builder{}; \
Builder.CreateType<ImGuiConfig>("ImGuiConfig"); \
Builder.AddParent<IConfig>(); \
Builder.SetConstructor(ImGuiConfig::ConstructSelf).SetDestructor(ImGuiConfig::DestructSelf); \
Builder.SetTypeAttribute("Category", "ImGui"); \
Builder.AddField("FontPath", &ImGuiConfig::FontPath); \
Builder.AddField("CharsetPath", &ImGuiConfig::CharsetPath); \
Builder.AddField("FontSize", &ImGuiConfig::FontSize); \
Builder.AddField("Theme", &ImGuiConfig::Theme); \
Builder.Register(); \
} \
}; \
static inline Z_TypeRegister_ImGuiConfig __Z_TypeRegister_ImGuiConfig_Instance; \
private: \

