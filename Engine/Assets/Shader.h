#pragma once
#include "Asset.h"
#include "Core/Container/Byte.h"
#include "Core/Reflection/MetaMark.h"
#include "Core/String/String.h"
#include "Core/TypeDefs.h"

#include "Shader.generated.h"

KENUM()
enum class EShaderLanguage {
  Slang,
  Spirv,
  GLSL,
  HLSL,
  Count,
};

enum class EShaderParameterType {
  Float4x4,
  Float4,
  TextureCube,
  SamplerState,
  Texture2D,
  Custom, // 结构体
  Count
};

struct ShaderMemberInfo {
  String Name;
  EShaderParameterType Type = EShaderParameterType::Count;
  Int32 Size = 0;
  Int32 Offset = 0;
  Int32 Binding = 0;
#if KITA_EDITOR
  String Label;
  bool IsHideInEditor = false;
#endif
};

struct ShaderParameterInfo {
  String Name;
  EShaderParameterType Type = EShaderParameterType::Count;
  Int32 Size = 0;
  Int32 Space = 0;
  /// 如果不为空则代表Shared
  String SharedName;
#if KITA_EDITOR
  String Label;
  bool IsHideInEditor = false;
#endif
  // for dynamic uniform buffer
  bool IsDynamic = false;
  Array<ShaderMemberInfo> Members;
};

KSTRUCT()
struct ShaderMeta : AssetMeta {
  GENERATED_BODY(ShaderMeta)
  /// 数据库主键Id
  KPROPERTY()
  Int32 Id = 0;

  /// 资产路径
  KPROPERTY()
  String Path;

  /// 对象Handle
  KPROPERTY()
  Int32 ObjectHandle = 0;
};

enum class EShaderStage {
  Vertex,
  Fragment,
  Count,
};

struct ShaderBinaryData {
  /// Shader的二进制数据
  /// 第1~4个字节记录一些管线属性
  ///   第一位: 此Shader是不是图形管线
  ///   第二位: CullMode是否是Backface
  /// 第5~8个字节 参数字节长度N
  /// 接下来是N个字节是表示参数的Json字符串
  /// 随后1个字节标识当前是什么阶段的Shader
  /// 随后四个字节标识该Shader长度
  /// 随后长度字节的标识该Shader的Spirv 接下来是这样数据的循环...
  Array<Byte> Data;

  [[nodiscard]] bool IsValid() const { return Data.Count() > 12; }
  [[nodiscard]] bool IsGraphicsShader() const { return IsValid() ? Data[0].Test(0) : false; }
  [[nodiscard]] bool IsCompute() const { return IsValid() ? !IsGraphicsShader() : false; }
  [[nodiscard]] bool IsCullModeBackface() const { return Data[0].Test(1); }

  [[nodiscard]] Array<ShaderParameterInfo> ParseParameterInfo();
};

struct ShaderCache {
  UInt64 LastTextModifiedTime;
  UInt64 LastBinaryModifiedTime;
};

/// Shader的功能只有保存Shader的二进制数据
/// 以及编译Shader
/// Material会负责持有RHI的ShaderModule
KCLASS()
class Shader : public Asset {
  GENERATED_BODY(Shader)
public:
  virtual EAssetType GetAssetType() override { return EAssetType::Shader; }

  virtual void Load() override;
  virtual void Unload() override;
  virtual void ApplyMeta(const AssetMeta& Meta) override;

  [[nodiscard]] const ShaderBinaryData& GetBinaryData() const { return mShaderData; }

  String GetBinaryPath();

  static void ReadCache();
  static void WriteCache();

  /// 将生成的Shader文件写入到
  /// Intermediate/Shader/{ShaderName}.glsl
  /// 中
  void GenerateTempCodeGLSL();
  void GenerateTempCodeHLSL();

  /// 将Shader文件写入到 {Folder}/{ShaderName}.{Lang}等
  /// @param Folder 将生成的Shader文件写入到哪个目录下
  /// @param Language 目标语言
  void GenerateTempCode(StringView Folder, EShaderLanguage Language);

  [[nodiscard]] bool IsCompute() const;
  [[nodiscard]] bool IsGraphics() const;

protected:
  /// 是否需要重新将Shader编译为spirv
  bool NeedReTranslate();

  /// 将slang shader编译为spirv二进制数据
  bool Translate();

  /// 读取Shader的二进制数据
  bool ReadBinary();

private:
  /// Shader编译后的二进制数据
  /// 如果Shader本身文本文件没变 二进制数据也没变
  /// 那么会直接从二进制文件读
  /// 否则重新编译Shader 并写入而二进制
  ShaderBinaryData mShaderData;
  const char* mEntryPointNames[std::to_underlying(EShaderStage::Count)] = {"VertexMain", "FragmentMain"};

  static inline Map<String, ShaderCache> sCache;
  static inline bool sCacheRead = false;
};
