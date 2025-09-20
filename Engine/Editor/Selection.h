#pragma once
#include "Core/Container/Array.h"
#include "Core/Singleton/Singleton.h"

class Object;
class Selection : public Singleton<Selection> {
public:
  static void StartUp();
  static void ShutDown();

  // 设置一个选中的Object
  static void SetSelected(const Object* Object);
  static void SetSelected(const Array<Object*>& Objects);
  static void SetSelected(Int32 ObjHandle);
  static void SetSelected(const Array<Int32>& ObjHandles);

  static const Array<Int32>& GetSelectedObjectHandles() { return GetRef().mObjects; }
  static Int32 GetFirstSelectedObjectHandle();

private:
  Array<Int32> mObjects;
};
