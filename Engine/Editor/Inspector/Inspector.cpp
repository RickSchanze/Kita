//
// Created by kita on 25-9-13.
//

#include "Inspector.h"

#include "Editor/EditorUI.h"

void Inspector::SetInspectedObject(Object* InObject) {
  if (mInspectedObject == InObject) {
    return;
  }
  mInspectedObject = InObject;
  OnSetInspectedObject(InObject);
}

void Inspector::Header(StringView Path, Int32 ObjectHandle) {
  EditorUI::PushBorderColor(Color::HighlightText());
  EditorUI::BeginChild("Header", {0, 2.7f * EditorUI::GetDefaultFontSize()}, EditorUI::EChildFlags::Borders);
  EditorUI::ImageIcon(EEditorImageIcon::Shader, 2);
  EditorUI::SameLine();
  EditorUI::BeginGroup();
  {
    EditorUI::TextColored(Color::HighlightText(), "路径: ");
    EditorUI::SameLine();
    EditorUI::Text(Path);
    EditorUI::TextColored(Color::HighlightText(), "句柄: ");
    EditorUI::SameLine();
    EditorUI::Text("%d", ObjectHandle);
  }
  EditorUI::EndGroup();
  EditorUI::EndChild();
  EditorUI::PopStyleColor();
}

void Inspector::Property(StringView Name, StringView Value) {
  EditorUI::TextColored(Color::HighlightText(), Name);
  EditorUI::SameLine();
  EditorUI::Text(Value);
}
