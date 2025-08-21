#pragma once
#include "Core/Container/Map.h"
#include "Core/Singleton/Singleton.h"
#include "Object.h"

class ObjectTable : public Singleton<ObjectTable> {
public:
  [[nodiscard]] bool IsObjectAlive(const Int32 Handle) const { return mObjectTable.Contains(Handle); }

  /// 注册一个Object 如果此Object对应的Handle已存在则Log错误并返回false
  bool RegisterObject(Object* Object);

  /// 从ObjectTable中删除一个Object, 同时释放内存 TODO: 延迟内存释放
  void UnregisterObject(Int32 ObjectHandle);
  /// 从ObjectTable中删除一个Object, 同时释放内存
  void UnregisterObject(const Object* Object);


private:
  Map<Int32, Object*> mObjectTable;
};
