//
// Created by kita on 25-8-23.
//

#include "IConfig.h"

#include "Core/Assert.h"

String IConfig::GetFilePath() {
  String ConfigPath;
  auto FilePath = GetType()->GetAttribute("File");
  if (!FilePath) {
    // 没有配置的话认为在Engine.cfg里
    return "Config/Engine.cfg";
  }
  if (FilePath->Empty()) {
    // 没有配置的话认为在Engine.cfg里
    ConfigPath = "Config/Engine.cfg";
  }
  // 这里获取的string_view应该在字符串常亮存储区, 正确情况下不会崩溃
  // 如果崩了, 那么代码一定有了什么问题
  return ConfigPath;
}

String IConfig::GetCategory() {
  auto CategoryName = GetType()->GetAttribute("Category");
  ASSERT_MSG(CategoryName && !CategoryName->Empty(), "Config 必须有一个Category: {}", GetType()->GetName())
  auto Category = String(*CategoryName);
  return Category;
}
