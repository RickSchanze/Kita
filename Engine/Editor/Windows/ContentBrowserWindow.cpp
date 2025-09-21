//
// Created by kita on 25-9-13.
//

#include "ContentBrowserWindow.h"

#include "Assets/AssetsManager.h"
#include "Core/String/StringUtils.h"
#include "Editor/Selection.h"
#include "RHI/IconFontHeader.h"

static void BuildAssetTreeNodeFromIndex(const AssetIndex& InAssetIndex, ContentBrowserWindow::DirectoryTreeNode* Root) {
  const StringView Path = InAssetIndex.Path;
  const auto PathSplit = StringUtils::Split(Path, '/');
  ContentBrowserWindow::DirectoryTreeNode* Current = Root;
  for (Int32 Index = 0; Index < PathSplit.Count() - 1; Index++) {
    auto Split = PathSplit[Index];
    ContentBrowserWindow::DirectoryTreeNode* Founded = nullptr;
    for (auto& Value : Current->Children) {
      if (Value->Name == Split) {
        Founded = Value.Get();
        break;
      }
    }
    if (Founded == nullptr) {
      UniquePtr<ContentBrowserWindow::DirectoryTreeNode> New = MakeUnique<ContentBrowserWindow::DirectoryTreeNode>();
      New->Name = Split.ToString();
      New->Indent = static_cast<Int8>(Index);
      New->IsOpened = false;
      Founded = New.Get();
      New->Parent = Current;
      Current->Children.Add(std::move(New));
    }
    Current = Founded;
  }
  ContentBrowserWindow::FileEntry FileEntry;
  FileEntry.Name = PathSplit.Last().ToString();
  FileEntry.ObjectHandle = InAssetIndex.ObjectHandle;
  FileEntry.Type = InAssetIndex.Type;
  Current->Files.Add(std::move(FileEntry));
}

static UniquePtr<ContentBrowserWindow::DirectoryTreeNode> BuildAssetTree() {
  Array<AssetIndex> Index = AssetsManager::GetRef().internalGetAllAssetIndex();
  UniquePtr<ContentBrowserWindow::DirectoryTreeNode> Root = MakeUnique<ContentBrowserWindow::DirectoryTreeNode>();
  for (auto& AssetIndex : Index) {
    BuildAssetTreeNodeFromIndex(AssetIndex, Root.Get());
  }
  return Root;
}

ContentBrowserWindow::ContentBrowserWindow() {
  mWindowTitle = "内容浏览器";
  mName = "ContentBrowserWindow";
  mShouldDeleteWhenUnVisible = false;
  mAssetTreeRoot = BuildAssetTree();
  if (!mAssetTreeRoot->Children.Empty()) {
    mCurrentSelectedTreeNode = mAssetTreeRoot->Children[0].Get();
  }
}

ContentBrowserWindow::~ContentBrowserWindow() {}

static void DrawDirectoryTreeRecursive(ContentBrowserWindow::DirectoryTreeNode* Root, ContentBrowserWindow::DirectoryTreeNode*& CurrentSelected) {
  if (Root != nullptr) {
    if (Root->Name.Empty()) {
      for (const auto& Node : Root->Children) {
        DrawDirectoryTreeRecursive(Node.Get(), CurrentSelected);
      }
    } else {
      EditorUI::PushID(Root->Name);
      const String Text = Format("{} {}", Root->IsOpened ? KITA_ICON_FOLDER_OPEN : KITA_ICON_FOLDER, Root->Name);
      for (int Index = 0; Index < Root->Indent; Index++) {
        EditorUI::Indent();
      }
      const bool PushColor = CurrentSelected ==  Root;
      if (PushColor) {
        EditorUI::PushButtonColor(Color::HighlightBackground());
      }
      if (EditorUI::Button(Text.Data())) {
        Root->IsOpened = !Root->IsOpened;
        if (!Root->IsOpened) {
          if (Root->Parent && !Root->Parent->Name.Empty()) {
            CurrentSelected = Root->Parent;
          }
        } else {
          CurrentSelected = Root;
        }
      }
      if (PushColor) {
        EditorUI::PopButtonColor();
      }
      if (Root->IsOpened) {
        for (const auto& TreeNode : Root->Children) {
          DrawDirectoryTreeRecursive(TreeNode.Get(), CurrentSelected);
        }
        EditorUI::Indent();
        for (const auto& File : Root->Files) {
          const EEditorImageIcon Icon = EditorUI::GetIconFromAsseType(File.Type);
          EditorUI::ImageIcon(Icon);
          EditorUI::SameLine();
          if (EditorUI::Button(File.Name)) {
            // 选中了一个文件
            Selection::SetSelected(File.ObjectHandle);
          }
        }
        EditorUI::Unindent();
      }
      EditorUI::PopID();
    }
    for (int Index = 0; Index < Root->Indent; Index++) {
      EditorUI::Unindent();
    }
  }
}

static void VerticalImageIconTextButton(const EEditorImageIcon Icon, const StringView Text, Vector2f ImageSize, bool* SingleClicked = nullptr, bool* DoubleClicked = nullptr) {
  const String NewText = EditorUI::TruncateText(Text, ImageSize.X());
  const float ImageWidth = ImageSize.X();
  const float ImageHeight = ImageSize.Y();
  Vector2f TextSize = EditorUI::CalcTextSize(NewText);
  const float TextWidth = TextSize.X();
  const float TextHeight = TextSize.Y();
  constexpr float VerticalSpacing = 3;    // 图片和文字的垂直间隔
  constexpr float BorderThickness = 2.0f; // 边框厚度
  constexpr float BorderPadding = 2.0f;   // 边框内边距

  // 计算按钮尺寸时包含边框和内边距
  const float ButtonWidth = ImageWidth + BorderThickness * 2 + BorderPadding * 2;
  const float ButtonHeight = ImageHeight + TextHeight + VerticalSpacing + BorderThickness * 2 + BorderPadding * 2;
  const auto ButtonSize = Vector2f(ButtonWidth, ButtonHeight);

  const bool Clicked = EditorUI::InvisibleButton(Text, ButtonSize);
  Vector2f ButtonPos = EditorUI::GetItemRectMin();
  const auto DrawList = EditorUI::GetDrawList();

  // 检测单击和双击
  if (SingleClicked) {
    *SingleClicked = Clicked;
  }

  if (DoubleClicked) {
    *DoubleClicked = EditorUI::IsLeftMouseDoubleClicked() && EditorUI::IsItemHovered();
  }

  // 计算内容区域的偏移（考虑边框和内边距）
  constexpr float ContentOffsetX = BorderThickness + BorderPadding;
  constexpr float ContentOffsetY = BorderThickness + BorderPadding;

  // 计算图片和文字的位置
  const Vector2f ImagePos = ButtonPos + Vector2f(ContentOffsetX, ContentOffsetY);
  float TextX = ButtonPos.X() + ContentOffsetX + (ImageWidth - TextWidth) * 0.5f;
  float TextY = ButtonPos.Y() + ContentOffsetY + ImageHeight + VerticalSpacing;

  // 绘制图片和文字
  DrawList.AddImageIcon(Icon, ImagePos, ImagePos + Vector2f(ImageWidth, ImageHeight));
  DrawList.AddText(NewText, {TextX, TextY});

  // 如果悬停，绘制边框
  if (EditorUI::IsItemHovered()) {
    // 获取边框颜色（你可能需要根据你的主题调整这个颜色）
    constexpr auto BorderColor = Color::HighlightText().ToUInt32();

    // 绘制边框矩形
    DrawList.AddRect(ButtonPos, ButtonPos + ButtonSize, BorderColor, 0.0f, BorderThickness);

    EditorUI::SetTooltip(Text);
  }
}

void ContentBrowserWindow::DrawDirectoryContent(ContentBrowserWindow::DirectoryTreeNode* Root) {
  if (Root == nullptr)
    return;
  constexpr auto IMAGE_WIDTH = 128.f;
  const Vector2f Size = EditorUI::GetContentRegionAvail();
  const float Spacing = EditorUI::GetItemSpacing().X();
  int Columns = static_cast<int>((Size.X() + Spacing) / (IMAGE_WIDTH + Spacing));
  if (Columns < 1) {
    Columns = 1;
  }
  if (EditorUI::BeginTable("DirContentTable", Columns, EditorUI::ETableFlags::SizingFixedFit | EditorUI::ETableFlags::NoHostExtendX)) {
    if (Root->Parent != nullptr) {
      if (!Root->Parent->Name.Empty()) {
        bool LastDoubleClicked = false;
        EditorUI::TableNextColumn();
        EditorUI::BeginGroup();
        VerticalImageIconTextButton(EEditorImageIcon::Folder, "..", Vector2f(IMAGE_WIDTH, IMAGE_WIDTH), nullptr, &LastDoubleClicked);
        if (LastDoubleClicked) {
          SetCurrentSelectedFolder(Root->Parent);
          EditorUI::EndGroup();
          EditorUI::EndTable();
          return;
        }
        EditorUI::EndGroup();
      }
    } else {
      EditorUI::EndTable();
    }
    for (const auto& Directory : Root->Children) {
      EditorUI::TableNextColumn();
      EditorUI::BeginGroup();
      bool DoubleClicked = false;
      VerticalImageIconTextButton(EEditorImageIcon::Folder, Directory->Name, Vector2f(IMAGE_WIDTH, IMAGE_WIDTH), nullptr, &DoubleClicked);
      if (DoubleClicked) {
        SetCurrentSelectedFolder(Directory.Get());
        EditorUI::EndGroup();
        break;
      }
      EditorUI::EndGroup();
    }
    for (const auto& File : Root->Files) {
      EditorUI::TableNextColumn();
      EditorUI::BeginGroup();
      bool SingleClicked = false;
      VerticalImageIconTextButton(EditorUI::GetIconFromAsseType(File.Type), File.Name, Vector2f(IMAGE_WIDTH, IMAGE_WIDTH), &SingleClicked);
      if (SingleClicked) {
        Selection::SetSelected(File.ObjectHandle);
      }
      EditorUI::EndGroup();
    }
    EditorUI::EndTable();
  }
}

void ContentBrowserWindow::DrawEditorUI() {
  EditorUI::BeginChild("A", {}, EditorUI::EChildFlags::ResizeX);
  DrawDirectoryTreeRecursive(mAssetTreeRoot.Get(), mCurrentSelectedTreeNode);
  EditorUI::EndChild();
  EditorUI::SameLine();
  EditorUI::SameLine();

  EditorUI::BeginChild("B", {});
  DrawDirectoryContent(mCurrentSelectedTreeNode);
  EditorUI::EndChild();
}

ContentBrowserWindow::DirectoryTreeNode::DirectoryTreeNode() {}

ContentBrowserWindow::DirectoryTreeNode::~DirectoryTreeNode() {}

static void SetCurrentSelectedFolderRecursive(ContentBrowserWindow::DirectoryTreeNode* Node) {
  if (Node != nullptr) {
    Node->IsOpened = true;
    SetCurrentSelectedFolderRecursive(Node->Parent);
  }
}

void ContentBrowserWindow::SetCurrentSelectedFolder(DirectoryTreeNode* Node) {
  mCurrentSelectedTreeNode = Node;
  SetCurrentSelectedFolderRecursive(Node);
}