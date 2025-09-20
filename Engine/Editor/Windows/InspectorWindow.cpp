//
// Created by kita on 25-9-13.
//

#include "InspectorWindow.h"

InspectorWindow::InspectorWindow() {
  mWindowTitle = "细节";
  mName = "InspectorWindow";
  mShouldDeleteWhenUnVisible = false;
}

InspectorWindow::~InspectorWindow() {}

void InspectorWindow::DrawEditorUI() {
  EditorUI::Text("选择一个对象以查看其属性");
}
