//
// Created by kita on 25-8-31.
//

#include "EditorUI.h"

#include "Assets/AssetsManager.h"
#include "Core/Assert.h"
#include "Core/Config/ConfigManager.h"
#include "Core/FileSystem/File.h"
#include "EditorWindowManager.h"
#include "MenuActionManager.h"
#include "RHI/GfxContext.h"
#include "RHI/ImGuiConfig.h"
#include "RHI/Image.h"

#include <imgui_internal.h>
#include <nlohmann/json.hpp>

EditorUI::EditorUI() = default;
EditorUI::~EditorUI() = default;

static ImVec2 Vector2fToImVec2(const Vector2f Vec) { return ImVec2(Vec.X(), Vec.Y()); }

bool EditorUI::InvisibleButton(const StringView StrId, const Vector2f Size) { return ImGui::InvisibleButton(StrId.GetStdStringView().data(), Vector2fToImVec2(Size)); }

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
static std::unordered_map<std::string, EEditorImageIcon> IconMap = {
    {"Critical.png", EEditorImageIcon::Critical},
    {"Warning.png", EEditorImageIcon::Warning},
    {"Info.png", EEditorImageIcon::Info},
    {"Error.png", EEditorImageIcon::Error},
  {"Shader.png", EEditorImageIcon::Shader},
  {"Mesh.png", EEditorImageIcon::Mesh},
  {"Texture.png", EEditorImageIcon::Texture},
  {"UnknownFile.png", EEditorImageIcon::UnknownFile},
  {"Folder.png", EEditorImageIcon::Folder},
};
// clang-format on

enum class EEditorUITexture {
  IconAtlas,
  Count,
};

struct EditorUI::Impl {
  Texture2D* ImageIconTexture{};
  UniquePtr<RHISampler> Sampler;
  void* ImGuiTexture[ToUnderlying(EEditorUITexture::Count)]{};

  ~Impl() {
    GetGfxContext()->WaitDeviceIdle();
    for (auto Texture : ImGuiTexture) {
      GetGfxContext()->DestroyImGuiTexture(Texture);
    }
  }
};

void EditorUI::StartUp() {
  const auto& Config = ConfigManager::GetConfigRef<ImGuiConfig>();
  sSplitterNormal = ColorToImU32(Config.Theme.Separator);
  sSplitterHovered = ColorToImU32(Config.Theme.SeparatorHovered);
  sSplitterActive = ColorToImU32(Config.Theme.SeparatorActive);
  sSpacing = Config.Theme.ItemSpacing;
  sDefaultFontSize = Config.FontSize;
  sTextColor = Config.Theme.Text;
  sTextColorUInt32 = sTextColor.ToUInt32();
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

  MenuActionManager::StartUp();
  EditorWindowManager::StartUp();
}

void EditorUI::ShutDown() {
  EditorWindowManager::ShutDown();
  MenuActionManager::ShutDown();
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

void EditorUI::ImageIcon(const EEditorImageIcon Icon, Vector2f ImageSize) {
  if (ImageSize.X() == 0 || ImageSize.Y() == 0) {
    ImageSize.SetX(sDefaultFontSize);
    ImageSize.SetY(sDefaultFontSize);
  }
  ImVec2 UV0 = Vector2fToImVec2(sImageIconUV[ToUnderlying(Icon)].LT);
  ImVec2 UV1 = Vector2fToImVec2(sImageIconUV[ToUnderlying(Icon)].RB);
  ImGui::Image(reinterpret_cast<ImTextureID>(sImpl->ImGuiTexture[ToUnderlying(EEditorUITexture::IconAtlas)]), Vector2fToImVec2(Vector2f(ImageSize)), UV0, UV1);
}

void EditorUI::ImageIcon(const EEditorImageIcon Icon, const float Scale) { EditorUI::ImageIcon(Icon, Vector2f(sDefaultFontSize * Scale, sDefaultFontSize * Scale)); }

void EditorUI::Image(void* Texture, const Vector2f Size) { ImGui::Image(reinterpret_cast<ImTextureID>(Texture), Vector2fToImVec2(Size)); }

void EditorUI::PushBorderColor(const Color InColor) { ImGui::PushStyleColor(ImGuiCol_Border, InColor.ToUInt32()); }

RHISampler* EditorUI::GetEditorUsedSampler() { return sImpl->Sampler.Get(); }

void* EditorUI::GetIconImage() { return sImpl->ImGuiTexture[0]; }

EditorUI::ImageIconUV EditorUI::GetImageIconUV(const EEditorImageIcon Icon) { return sImageIconUV[ToUnderlying(Icon)]; }

String EditorUI::TruncateText(const StringView StringOrig, const float MaxWidth) {
  // 计算完整文本宽度
  Vector2f FullTextSize = CalcTextSize(StringOrig);
  const float FullTextWidth = FullTextSize.X();

  // 如果文本不超出最大宽度，直接返回原文本
  if (FullTextWidth <= MaxWidth) {
    return StringOrig.ToString();
  }

  // 计算省略号的宽度
  Vector2f EllipsisSize = EditorUI::CalcTextSize("...");
  const float EllipsisWidth = EllipsisSize.X();
  const float AvailableWidth = MaxWidth - EllipsisWidth;

  // 如果可用宽度太小，只返回省略号
  if (AvailableWidth <= 0) {
    return "...";
  }

  // 通过二分查找找到合适的截断位置
  int Left = 0;
  int Right = static_cast<int>(StringOrig.Count());
  int BestLength = 0;

  while (Left <= Right) {
    const int Mid = (Left + Right) / 2;
    String TestText = StringOrig.SubStr(0, Mid).ToString();
    Vector2f TestSize = EditorUI::CalcTextSize(TestText);

    if (TestSize.X() <= AvailableWidth) {
      BestLength = Mid;
      Left = Mid + 1;
    } else {
      Right = Mid - 1;
    }
  }

  return StringOrig.SubStr(0, BestLength).ToString() + "...";
}
EEditorImageIcon EditorUI::GetIconFromAsseType(const EAssetType Type) {
  switch (Type) {
  case EAssetType::Shader:
    return EEditorImageIcon::Shader;
  case EAssetType::Mesh:
    return EEditorImageIcon::Mesh;
  case EAssetType::Texture2D:
    return EEditorImageIcon::Texture;
  case EAssetType::TextureCube:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Material:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Scene:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Animation:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Audio:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Font:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Prefab:
    return EEditorImageIcon::UnknownFile;
  case EAssetType::Count:
    return EEditorImageIcon::UnknownFile;
  }
  return EEditorImageIcon::UnknownFile;
}

void EditorUIDrawList::AddImage(void* Texture, const Vector2f Min, const Vector2f Max) const {
  mDrawList->AddImage(reinterpret_cast<ImTextureID>(Texture), Vector2fToImVec2(Min), Vector2fToImVec2(Max));
}

void EditorUIDrawList::AddText(const StringView Text, const Vector2f Pos) const { mDrawList->AddText(Vector2fToImVec2(Pos), EditorUI::GetTextColorUInt32(), Text.Data()); }

void EditorUIDrawList::AddImageIcon(const EEditorImageIcon Icon, const Vector2f Min, const Vector2f Max) const {
  const EditorUI::ImageIconUV UV = EditorUI::GetImageIconUV(Icon);
  ImTextureID Texture = reinterpret_cast<ImTextureID>(EditorUI::GetIconImage());
  mDrawList->AddImage(Texture, Vector2fToImVec2(Min), Vector2fToImVec2(Max), Vector2fToImVec2(UV.LT), Vector2fToImVec2(UV.RB));
}

void EditorUIDrawList::AddRect(const Vector2f Min, const Vector2f Max, const UInt32 Color, const float Rounding, const float Thickness) const {
  mDrawList->AddRect(Vector2fToImVec2(Min), Vector2fToImVec2(Max), Color, Rounding, 0, Thickness);
}

