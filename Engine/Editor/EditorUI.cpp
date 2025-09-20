//
// Created by kita on 25-8-31.
//

#include "EditorUI.h"

#include "Assets/Asset.h"
#include "Assets/AssetsManager.h"
#include "Core/Assert.h"
#include "Core/Config/ConfigManager.h"
#include "Core/FileSystem/File.h"
#include "Core/Performance/ProfilerMark.h"
#include "RHI/GfxContext.h"
#include "RHI/ImGuiConfig.h"
#include "RHI/Image.h"

#include <imgui_internal.h>
#include <nlohmann/json.hpp>

EditorUI::EditorUI() {}
EditorUI::~EditorUI() {}
void EditorUI::Splitter(float& S1, float& S2, const ESplitterDirection Direction, const float MinSize1, const float MinSize2, const float Thickness) {
  const auto Size = ImGui::GetContentRegionAvail();
  if (S1 == 0 || S2 == 0) {
    S1 = Size.x * 0.5f;
    S2 = Size.x - S1;
  }
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
    UInt32 currentColor = sSplitterNormal;
    if (Dragging)
      currentColor = sSplitterActive;
    else if (ImGui::IsItemHovered())
      currentColor = sSplitterHovered;

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

    UInt32 CurrentColor = sSplitterNormal;
    if (Dragging)
      CurrentColor = sSplitterActive;
    else if (ImGui::IsItemHovered())
      CurrentColor = sSplitterHovered;

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

// clang-format off
static std::unordered_map<std::string, EditorUI::EEditorImageIcon> IconMap = {
    {"Critical.png", EditorUI::EEditorImageIcon::Critical},
    {"Warning.png", EditorUI::EEditorImageIcon::Warning},
    {"Info.png", EditorUI::EEditorImageIcon::Info},
    {"Error.png", EditorUI::EEditorImageIcon::Error},
  {"Shader.png", EditorUI::EEditorImageIcon::Shader},
  {"Mesh.png", EditorUI::EEditorImageIcon::Mesh},
  {"Texture.png", EditorUI::EEditorImageIcon::Texture},
  {"UnknownFile.png", EditorUI::EEditorImageIcon::UnknownFile},
};
// clang-format on

enum class EEditorUITexture {
  IconAtlas,
  Count,
};

struct EditorUI::Impl {
  Texture2D* ImageIconTexture;
  UniquePtr<RHISampler> Sampler;
  void* ImGuiTexture[ToUnderlying(EEditorUITexture::Count)];
};

void EditorUI::StartUp() {
  const auto& Config = ConfigManager::GetConfigRef<ImGuiConfig>();
  sSplitterNormal = ColorToImU32(Config.Theme.Separator);
  sSplitterHovered = ColorToImU32(Config.Theme.SeparatorHovered);
  sSplitterActive = ColorToImU32(Config.Theme.SeparatorActive);
  sDefaultFontSize = Config.FontSize;
  sImpl = New<Impl>();
  RHISamplerDesc SamplerDesc;
  SamplerDesc.SetMagFilter(ERHIFilterMode::Linear)
      .SetMinFilter(ERHIFilterMode::Linear)
      .SetMipmapMode(ERHIMipmapMode::Linear)
      .SetAddressModeU(ERHISamplerAddressMode::ClampToEdge)
      .SetAddressModeV(ERHISamplerAddressMode::ClampToEdge)
      .SetAddressModeW(ERHISamplerAddressMode::ClampToEdge)
      .SetAnisotropyEnable(false)
      .SetMaxAnisotropy(1.0f)
      .SetMinLod(0.0f)
      .SetMaxLod(0.0f); // 不使用 mipmap
  sImpl->Sampler = GfxContext::GetRef().CreateSamplerU(SamplerDesc);
  auto ImportHandle = AssetsManager::ImportAsync("Assets/Texture/EditorIcon/EditorIconAtlas.png", true);
  ImportHandle.WaitSync();
  sImpl->ImageIconTexture = ImportHandle.GetAssetObjectT<Texture2D>();
  ASSERT_MSG(sImpl->ImageIconTexture, "加载编辑器图标纹理图集失败.");

  sImpl->ImGuiTexture[ToUnderlying(EEditorUITexture::IconAtlas)] = GfxContext::GetRef().CreateImGuiTexture(sImpl->Sampler.Get(), sImpl->ImageIconTexture->GetImageView());

  auto Text = File::ReadAllText("Assets/Texture/EditorIcon/EditorIconAtlas_UV.json");
  ASSERT_MSG(Text, "无法获取EditorIconAtlas的UV信息: EditorIconAtlas_UV.json, Result={}", Text.Error())
  nlohmann::json AtlasJson = nlohmann::json::parse(Text->GetStdString());

  for (auto& [Name, UV] : AtlasJson.items()) {
    auto it = IconMap.find(Name);
    if (it != IconMap.end()) {
      const auto IconType = it->second;
      sImageIconUV[ToUnderlying(IconType)].LT.X() = UV["u0"].get<float>();
      sImageIconUV[ToUnderlying(IconType)].LT.Y() = UV["v0"].get<float>();
      sImageIconUV[ToUnderlying(IconType)].RB.X() = UV["u1"].get<float>();
      sImageIconUV[ToUnderlying(IconType)].RB.Y() = UV["v1"].get<float>();
    }
  }
}

void EditorUI::ShutDown() {
  // TODO: AssetManager Unload Texture
  Delete(sImpl);
}

bool EditorUI::Button(const StringView Label, const Vector2f Size, Optional<UInt32> TextColor) {
  if (TextColor) {
    ImGui::PushStyleColor(ImGuiCol_Text, *TextColor);
  }
  const auto Result = ImGui::Button(Label.GetStdStringView().data(), Vector2fToImVec2(Size));
  if (TextColor) {
    ImGui::PopStyleColor();
  }
  return Result;
}

void EditorUI::ImageIcon(EEditorImageIcon Icon, Vector2f ImageSize) {
  if (ImageSize.X() == 0 || ImageSize.Y() == 0) {
    ImageSize.SetX(sDefaultFontSize);
    ImageSize.SetY(sDefaultFontSize);
  }
  ImVec2 UV0 = Vector2fToImVec2(sImageIconUV[ToUnderlying(Icon)].LT);
  ImVec2 UV1 = Vector2fToImVec2(sImageIconUV[ToUnderlying(Icon)].RB);
  ImGui::Image(reinterpret_cast<ImTextureID>(sImpl->ImGuiTexture[ToUnderlying(EEditorUITexture::IconAtlas)]), Vector2fToImVec2(Vector2f(ImageSize)), UV0, UV1);
}

void EditorUI::ImageIcon(EEditorImageIcon Icon, float Scale) { EditorUI::ImageIcon(Icon, Vector2f(sDefaultFontSize * Scale, sDefaultFontSize * Scale)); }

void EditorUI::Image(void* Texture, Vector2f Size) { ImGui::Image(reinterpret_cast<ImTextureID>(Texture), Vector2fToImVec2(Size)); }

void EditorUI::PushBorderColor(Color InColor) { ImGui::PushStyleColor(ImGuiCol_Border, InColor.ToUInt32()); }

RHISampler* EditorUI::GetEditorUsedSampler() { return sImpl->Sampler.Get(); }
