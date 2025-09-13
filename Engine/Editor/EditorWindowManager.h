#pragma once
#include "Core/Container/Map.h"
#include "Core/Singleton/Singleton.h"

class EditorWindow;
struct Type;
class EditorWindowManager : Singleton<EditorWindowManager> {
public:
  static EditorWindow* Open(const Type* InType);
  static void Close(const Type* InType);
  static void StartUp();
  static void ShutDown();

  template <typename T> static T* Open() { return static_cast<T*>(GetRef().Open(T::GetStaticType())); }

  static void Render();
  static EditorWindow* GetWindow(const Type* WindowType);
  static void MakeSureReflections();

private:
  Map<const Type*, EditorWindow*> mEditorWindows;
};
