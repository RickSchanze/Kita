#pragma once
#include "EditorUI.h"
#include "Object/Object.h"

#include "EditorWindow.generated.h"
#include "EditorWindowManager.h"

KCLASS()
class EditorWindow : public Object {
  GENERATED_BODY(EditorWindow)
public:
  void Render();

  virtual void Draw() {}

  /// 获取窗口标题
  [[nodiscard]] StringView GetWindowTitle() const { return mWindowTitle; }

  void Show() { mShowWindow = true; }
  void Hide() { mShowWindow = false; }

  bool IsVisible() const { return mShowWindow; }
  bool ShouldDeleteWhenUnVisible() const { return mShouldDeleteWhenUnVisible; }

  template <typename T> static void Open() { EditorWindowManager::Open(T::GetStaticType()); }

protected:
  bool mShowWindow = true;
  EditorUI::EWindowFlags mFlags = EditorUI::WFB_None;
  String mWindowTitle = "窗口";
  bool mShouldDeleteWhenUnVisible = true;
};
