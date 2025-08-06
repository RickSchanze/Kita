#pragma once
#include "Core/FileSystem/FileSystemError.h"
#include "Core/FileSystem/Path.h"
#include "Core/Serialization/OutputArchive.h"
#include "inicpp.h"

class INIOutputArchive : public OutputArchive {
public:
  INIOutputArchive() = default;

  void SetTargetFile(const Path& FilePath);

  virtual void BeginObject(StringView ObjectName) override;
  virtual void EndObject() override;
  virtual void BeginArray() override;
  virtual void EndArray() override;
  virtual void Write(StringView Key, StringView Value) override;
  virtual void Write(StringView Key, Int8 Value) override;
  virtual void Write(StringView Key, Int16 Value) override;
  virtual void Write(StringView Key, Int32 Value) override;
  virtual void Write(StringView Key, Int64 Value) override;
  virtual void Write(StringView Key, UInt8 Value) override;
  virtual void Write(StringView Key, UInt16 Value) override;
  virtual void Write(StringView Key, UInt32 Value) override;
  virtual void Write(StringView Key, UInt64 Value) override;
  virtual void Write(StringView Key, Float32 Value) override;
  virtual void Write(StringView Key, Float64 Value) override;
  virtual void Write(StringView Key, bool Value) override;

private:
  // 判断一个section是否IniManager下的第一层section / 顶级section
  static bool IsFirstLayerSection(const inicpp::section& Section);
  static bool IsSection(const inicpp::section& Section);

  inicpp::IniManager mIni;
  inicpp::section mCurrentSection;
};
