//
// Created by kita on 25-9-13.
//

#include "InspectorWindow.h"

#include "Assets/AssetsManager.h"
#include "Core/Reflection/ReflUtils.h"
#include "Editor/Inspector/Inspector.h"
#include "Editor/Selection.h"
#include "Object/ObjectTable.h"

InspectorWindow::InspectorWindow() {
  mWindowTitle = "细节";
  mName = "InspectorWindow";
  mShouldDeleteWhenUnVisible = false;
  CollectAllInspector();
}

InspectorWindow::~InspectorWindow() {}

void InspectorWindow::DrawEditorUI() {
  CPU_PROFILING_SCOPE;
  if (mCurrentSelectedHandle != Selection::GetFirstSelectedObjectHandle()) {
    SetCurrentObjectHandle(Selection::GetFirstSelectedObjectHandle());
  }
  if (mCurrentSelectedHandle == 0) {
    EditorUI::ImageIcon(EditorUI::EEditorImageIcon::Warning);
    EditorUI::SameLine();
    EditorUI::Text("选择一个对象以查看其细节属性");
    return;
  }
  if (!ObjectTable::GetRef().IsObjectAlive(mCurrentSelectedHandle)) {
    mCurrentSelectedObject = nullptr;
  }
  if (mCurrentSelectedObject == nullptr) {
    EditorUI::ImageIcon(EditorUI::EEditorImageIcon::Warning);
    EditorUI::SameLine();
    EditorUI::Text("选择一个对象以查看其细节属性");
    return;
  }
  if (mInspector) {
    if (mInspector->GetInspectedObject() != mCurrentSelectedObject) {
      mInspector = CreateInspector(mCurrentSelectedObject->GetType());
      if (!mInspector) {
        // TODO: 没有专门的Inspector 转为自动生成的
      }
    }
  } else {
    mInspector = CreateInspector(mCurrentSelectedObject->GetType());
    if (!mInspector) {
      // TODO: 没有专门的Inspector 转为自动生成的
    }
  }
  if (mInspector) {
    mInspector->SetInspectedObject(mCurrentSelectedObject);
    mInspector->DrawGUI();
  } else {
    EditorUI::ImageIcon(EditorUI::EEditorImageIcon::Error);
    EditorUI::SameLine();
    EditorUI::Text("未找到该对象的Inspector('%d')", mCurrentSelectedHandle);
  }
}

void InspectorWindow::SetCurrentObjectHandle(Int32 NewHandle) {
  mCurrentSelectedHandle = NewHandle;
  if (mCurrentSelectedHandle > 0) {
    if (!ObjectTable::GetRef().IsObjectAlive(mCurrentSelectedHandle)) {
      auto Handle = AssetsManager::LoadAsync(mCurrentSelectedHandle);
      // TODO: 转圈圈
      Handle.WaitSync();
      mCurrentSelectedObject = Handle.GetAssetObject();
    } else {
      mCurrentSelectedObject = ObjectTable::GetObject(mCurrentSelectedHandle);
    }
  } else {
    if (ObjectTable::GetRef().IsObjectAlive(mCurrentSelectedHandle)) {
      mCurrentSelectedObject = ObjectTable::GetObject(mCurrentSelectedHandle);
    } else {
      mCurrentSelectedObject = nullptr;
    }
  }
}

void InspectorWindow::CollectAllInspector() {
  Array<const Type*> ReflectedTypes = ReflUtils::GetTypes([](const Type* InType) -> bool { return InType->GetAttribute("Inspector").HasValue(); });
  for (auto ReflectedType : ReflectedTypes) {
    auto InspectedTypeName = *ReflectedType->GetAttribute("Inspector");
    const Type* InspectedType = ReflUtils::GetType(InspectedTypeName);
    if (InspectedType == nullptr) {
      gLogger.Warn(Logcat::Editor, "无法为不存在的类型'{}'添加Inspector.", InspectedTypeName);
      continue;
    }
    if (mInspectorTypeMap.Contains(InspectedType)) {
      gLogger.Warn(Logcat::Editor, "重复为类型'{}'添加Inspector. 忽略后来添加的.", InspectedTypeName);
      continue;
    }
    mInspectorTypeMap.Add(InspectedType, ReflectedType);
  }
}

UniquePtr<Inspector> InspectorWindow::CreateInspector(const Type* InType) {
  if (mInspectorTypeMap.Contains(InType)) {
    const Type* ReflectedType = mInspectorTypeMap[InType];
    return ReflectedType->CreateInstanceT<Inspector>();
  } else {
    return nullptr;
  }
}
