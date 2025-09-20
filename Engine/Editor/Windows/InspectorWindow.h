#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/EditorWindowManager.h"
#include "Editor/MenuAction.h"

#include "InspectorWindow.generated.h"

class Inspector;
KCLASS()
class InspectorWindow : public EditorWindow {
  GENERATED_BODY(InspectorWindow)
public:
  InspectorWindow();
  virtual ~InspectorWindow() override;
  virtual void DrawEditorUI() override;

private:
  void SetCurrentObjectHandle(Int32 NewHandle);

  void CollectAllInspector();
  UniquePtr<Inspector> CreateInspector(const Type* InType);

  Int32 mCurrentSelectedHandle = 0;
  Object* mCurrentSelectedObject = nullptr;
  UniquePtr<Inspector> mInspector;

  /// 类型到Inspector的映射
  Map<const Type*, const Type*> mInspectorTypeMap;
};

KCLASS(MenuActionItem = "窗口/细节")
class MenuAction_OpenInspectorWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenInspectorWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<InspectorWindow>(); }
};
