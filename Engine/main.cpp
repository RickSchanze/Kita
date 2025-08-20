#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Reflection/MetaMark.h"
#include "Core/Reflection/TypeRegistry.h"
#include "Core/Serialization/TOML/TOMLInputArchive.h"
#include "Core/Serialization/TOML/TOMLOutputArchive.h"
#include "Core/Serialization/YAML/YAMLInputArchive.h"
#include "Core/Serialization/YAML/YAMLOutputArchive.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/ThreadUtils.h"
#include "Windows.h"

struct Wait5sTask : TaskNode {
  virtual ETaskNodeResult Run() override {
    LOG_INFO_TAG("Test", "Wait5s Start");
    ThreadUtils::WaitFor(std::chrono::seconds(5));
    LOG_INFO_TAG("Test", "Wait5s End");
    return ETaskNodeResult::Success;
  }

  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }
};

struct Wait6sTask : TaskNode {
  virtual ETaskNodeResult Run() override {
    LOG_INFO_TAG("Test", "Wait6s Start");
    ThreadUtils::WaitFor(std::chrono::seconds(6));
    LOG_INFO_TAG("Test", "Wait6s End");
    return ETaskNodeResult::Success;
  }

  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::IO; }
};

struct Wait7sTask : TaskNode {
  virtual ETaskNodeResult Run() override {
    LOG_INFO_TAG("Test", "Wait7s Start");
    ThreadUtils::WaitFor(std::chrono::seconds(7));
    LOG_INFO_TAG("Test", "Wait7s End");
    return ETaskNodeResult::Success;
  }

  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }
};

int main() {
  SetConsoleOutputCP(CP_UTF8);
  TaskGraph::StartUp();

  auto T1 = TaskGraph::CreateTask<Wait5sTask>("Wait5s", {});
  auto T2 = TaskGraph::CreateTask<Wait6sTask>("Wait6s", {});
  auto T3 = TaskGraph::CreateTask<Wait7sTask>("Wait7s", {T1, T2});
  T3.WaitSync();
  TaskGraph::ShutDown();
}

KCLASS(Attr1 = "Value1", Attr2 = Value2) // 这里有没有双引号都可以
class MyClass {

  KPROPERTY(Attr1 = "Value1", Attr2 = Value2)
  int MyInt = 0;

  KPROPERTY()
  String MyInt1;

  KPROPERTY(Transient) // Transient没有值 默认为"true"
  String MyInt2;

  KFUNCTION() // 和Field一样, 需要提取参数信息
  void Test(int a, int b);
};
