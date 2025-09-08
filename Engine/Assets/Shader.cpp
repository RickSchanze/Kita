//
// Created by kita on 25-9-5.
//

#include "Shader.h"

#include "AssetsConfig.h"
#include "Core/FileSystem/File.h"
#include "Core/FileSystem/InputFileStream.h"
#include "Core/FileSystem/OutputFileStream.h"
#include "Core/FileSystem/Path.h"
#include "Core/String/StringUtils.h"
#include "Project/Project.h"

#include "Core/Config/ConfigManager.h"
#include "slang-com-ptr.h"

class SlangShaderTranslater {
public:
  SlangShaderTranslater() {
    slang::createGlobalSession(mGlobalSession.writeRef());
    CreateSession();
  }

  Slang::ComPtr<slang::ISession> GetSession() { return mCompileSession; }

private:
  void CreateSession() {
    auto& Config = ConfigManager::GetConfigRef<AssetsConfig>();
    auto SearchPaths = Config.GetShaderSearchPaths();
    Array<const char*> SearchPathsArray;
    for (auto& SearchPath : SearchPaths) {
      SearchPathsArray.Add(SearchPath.Data());
    }
    slang::SessionDesc Desc{};
    slang::TargetDesc TargetDesc{};
    TargetDesc.format = SLANG_SPIRV;
    TargetDesc.profile = mGlobalSession->findProfile("glsl_460");
    Desc.searchPaths = SearchPathsArray.Data();
    Desc.searchPathCount = static_cast<SlangInt>(SearchPathsArray.Count());
    Desc.targets = &TargetDesc;
    Desc.targetCount = 1;
    mGlobalSession->createSession(Desc, mCompileSession.writeRef());
  }

  Slang::ComPtr<slang::IGlobalSession> mGlobalSession;
  Slang::ComPtr<slang::ISession> mCompileSession;
};

static SlangShaderTranslater& GetTranslater() {
  static SlangShaderTranslater Translater;
  return Translater;
}

void Shader::Load() {
  if (mPath.Empty() || !mPath.EndsWith(".slang")) {
    gLogger.Error(Logcat::Asset, "无效的Shader路径.");
    return;
  }
  ReadCache();
  if (NeedReTranslate()) {
    mLoaded = Translate();
  } else {
    mLoaded = ReadBinary();
  }
  if (mLoaded) {
    gLogger.Info(Logcat::Asset, "Shader '{}' 加载成功[ObjectHandle = {}].", mPath, GetHandle());
  } else {
    gLogger.Error(Logcat::Asset, "Shader '{}' 加载失败[ObjectHandle = {}].", mPath, GetHandle());
  }
}

void Shader::Unload() {
  mShaderData.Data.Clear();
  mLoaded = false;
}

void Shader::ApplyMeta(const AssetMeta& Meta) {
  const auto& MyShaderMeta = static_cast<const ShaderMeta&>(Meta);
  mPath = MyShaderMeta.Path;
  // Apply时Handle已经处理
}

constexpr auto ShaderCacheFolder = "Shader/Cache";

String Shader::GetBinaryPath() {
  const auto ThisShaderBinaryFileName = mPath.Replace(".slang", ".shaderbin");
  return Path::Combine(Path::Combine(Project::GetIntermediatePath(), ShaderCacheFolder), ThisShaderBinaryFileName);
}

bool Shader::NeedReTranslate() {
  if (sCache.Contains(mPath)) {
    const ShaderCache& OldCache = sCache[mPath];
    if (const auto NewTime = File::GetLastModifiedTime(mPath); (!NewTime || OldCache.LastTextModifiedTime != *NewTime)) {
      return true;
    }
    if (const auto ThisShaderBinaryPath = GetBinaryPath(); Path::IsExists(ThisShaderBinaryPath)) {
      if (const auto BinaryTime = File::GetLastModifiedTime(ThisShaderBinaryPath); (!BinaryTime || OldCache.LastBinaryModifiedTime != *BinaryTime)) {
        return true;
      }
      return false;
    } else {
      return true;
    }
  }
  return true;
}

#define DIAG()                                                                                                                        \
  if (Diagnostics) {                                                                                                                  \
    gLogger.Error(Logcat::Asset, "读取Shader '{}' 时发生错误: {}", mPath, static_cast<const char*>(Diagnostics->getBufferPointer())); \
    return false;                                                                                                                     \
  }

static bool TranslateCompute(const Slang::ComPtr<slang::ISession>& Session, slang::IModule* Module, const Slang::ComPtr<slang::IEntryPoint>& ComputeEntry, ShaderBinaryData& OutBinaryData) {
  return false;
}

static void ProcessFragmentAttribute(slang::FunctionReflection* FragFuncLayout, Byte& FirstByte) {
  const SlangInt AttributeCount = FragFuncLayout->getUserAttributeCount();
  for (SlangInt Index = 0; Index < AttributeCount; Index++) {
    slang::UserAttribute* Attribute = FragFuncLayout->getUserAttributeByIndex(Index);
    if (Attribute == nullptr)
      continue;
    StringView AttrName = Attribute->getName();
    if (AttrName == "CullMode") {
      Int32 CullMode;
      SlangResult Result = Attribute->getArgumentValueInt(0, &CullMode);
      if (Result == SLANG_OK) {
        if (CullMode == 0) {
          FirstByte.Clear(1);
        } else {
          FirstByte.Set(1);
        }
      }
    }
  }
}

static void ProcessVertexAttribute(slang::FunctionReflection* FragFuncLayout, ShaderBinaryData& OutBinaryData) {}

static bool TranslateGraphics(const Slang::ComPtr<slang::ISession>& Session, slang::IModule* Module, const Slang::ComPtr<slang::IEntryPoint>& VertexEntry,
                              const Slang::ComPtr<slang::IEntryPoint>& FragmentEntry, ShaderBinaryData& OutBinaryData, StringView ShaderPath) {
  Array<slang::IComponentType*> Components = {Module, VertexEntry, FragmentEntry};
  Slang::ComPtr<slang::IComponentType> Program;
  Slang::ComPtr<slang::IBlob> Diagnostics;
  Session->createCompositeComponentType(Components.Data(), static_cast<SlangInt>(Components.Count()), Program.writeRef(), Diagnostics.writeRef());
  if (Diagnostics) {
    return false;
  }
  // 反射
  Byte FirstByte{};
  FirstByte.Set(0); // 设置图像管线为true
  FirstByte.Set(1); // 默认使用CullMode为backface
  OutBinaryData.Data.Add(FirstByte);

  slang::ProgramLayout* ProgLayout = Program->getLayout();
  if (!ProgLayout) {
    return false;
  }

  // 反射获取信息
  bool Processed[std::to_underlying(EShaderStage::Count)]{};
  const SlangInt EntryPointCount = ProgLayout->getEntryPointCount();
  for (SlangInt i = 0; i < EntryPointCount; i++) {
    slang::EntryPointLayout* EntryPoint = ProgLayout->getEntryPointByIndex(i);
    switch (SlangStage Stage = EntryPoint->getStage()) {
    case SLANG_STAGE_VERTEX: {
      if (Processed[std::to_underlying(EShaderStage::Vertex)]) {
        gLogger.Error(Logcat::Asset, "'{}': 一个slang文件里不支持有多个Vertex入口.", ShaderPath);
        return false;
      }
      slang::FunctionReflection* FunctionReflection = EntryPoint->getFunction();
      if (!FunctionReflection) {
        return false;
      }
      ProcessVertexAttribute(FunctionReflection, OutBinaryData);
      Processed[std::to_underlying(EShaderStage::Vertex)] = true;
    } break;
    case SLANG_STAGE_FRAGMENT: {
      if (Processed[std::to_underlying(EShaderStage::Fragment)]) {
        gLogger.Error(Logcat::Asset, "'{}': 一个slang文件里不支持有多个Fragment入口.", ShaderPath);
        return false;
      }
      slang::FunctionReflection* FunctionReflection = EntryPoint->getFunction();
      if (!FunctionReflection) {
        return false;
      }
      ProcessFragmentAttribute(FunctionReflection, FirstByte);
      Processed[std::to_underlying(EShaderStage::Fragment)] = true;
    } break;
    default:
      gLogger.Critical(Logcat::Asset, "暂不支持的Shader Stage: {}", Stage);
    }
  }
  return true;
}

bool Shader::Translate() {
  using namespace slang;
  Slang::ComPtr<IBlob> Diagnostics;
  IModule* Module = GetTranslater().GetSession()->loadModule(mPath.Data(), Diagnostics.writeRef());
  DIAG();
  Slang::ComPtr<IEntryPoint> ComputeEntryPoint;
  Module->findEntryPointByName("ComputeMain", ComputeEntryPoint.writeRef());
  if (ComputeEntryPoint) {
    DIAG();
    return TranslateCompute(GetTranslater().GetSession(), Module, ComputeEntryPoint, mShaderData);
  }
  Slang::ComPtr<IEntryPoint> VertexEntryPoint;
  Module->findEntryPointByName("VertexMain", VertexEntryPoint.writeRef());
  DIAG();
  Slang::ComPtr<IEntryPoint> FragmentEntryPoint;
  Module->findEntryPointByName("FragmentMain", FragmentEntryPoint.writeRef());
  DIAG();
  return TranslateGraphics(GetTranslater().GetSession(), Module, VertexEntryPoint, FragmentEntryPoint, mShaderData, mPath);
}

bool Shader::ReadBinary() {
  InputFileStream InputFile(GetBinaryPath(), std::ios::in | std::ios::binary);
  const auto FileSize = InputFile.GetFileSize();
  if (!FileSize) {
    return false;
  }
  mShaderData.Data.Resize(*FileSize);
  UInt64 BytesRead = 0;
  InputFile.ReadBytes(mShaderData.Data.Data(), mShaderData.Data.Count(), BytesRead);
  if (BytesRead < 12) {
    return false;
  }
  return true;
}

constexpr auto ShaderCacheFile = "Shader/Cache.csv";

void Shader::ReadCache() {
  if (sCacheRead) {
    return;
  }
  if (auto IntermediateShaderCachePath = Path::Combine(Project::GetIntermediatePath(), ShaderCacheFile); Path::IsExists(IntermediateShaderCachePath)) {
    if (Path::IsDirectory(IntermediateShaderCachePath)) {
      gLogger.Error(Logcat::Asset, "ShaderCachePath '{}' 不能是一个目录.", IntermediateShaderCachePath);
      return;
    }
    InputFileStream InputFile(IntermediateShaderCachePath);
    Int32 LineNum = 0;
    while (true) {
      LineNum++;
      if (auto LineResult = InputFile.ReadLine()) {
        auto& Line = *LineResult;
        Array<StringView> Words = StringUtils::Split(Line, ',');
        if (Words.Count() != 3) {
          gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
          continue;
        } else {
          ::ShaderCache ShaderCache{};
          auto LastBinaryModifiedTime = StringUtils::ToNumber<UInt64>(Words[1]);
          if (!LastBinaryModifiedTime) {
            gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
            continue;
          }
          ShaderCache.LastBinaryModifiedTime = *LastBinaryModifiedTime;
          const auto LastTextModifiedTime = StringUtils::ToNumber<UInt64>(Words[2]);
          if (!LastTextModifiedTime) {
            gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
            continue;
          }
          ShaderCache.LastTextModifiedTime = *LastTextModifiedTime;
          if (!Words[0].Empty()) {
            sCache[Words[0].ToString()] = ShaderCache;
          } else {
            gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
          }
        }
      } else {
        if (LineResult.Error() != EFileSystemError::EndOfFile) {
          gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'时发生错误: {}", IntermediateShaderCachePath, LineResult.Error());
        } else {
          break;
        }
      }
    }
  }
  sCacheRead = true;
}

void Shader::WriteCache() {
  const auto IntermediateShaderCachePath = Path::Combine(Project::GetIntermediatePath(), ShaderCacheFile);
  OutputFileStream OutputFile(IntermediateShaderCachePath);
  for (auto& [Path, Cache] : sCache) {
    OutputFile.WriteLine(Path, ",", Cache.LastBinaryModifiedTime, ",", Cache.LastTextModifiedTime);
  }
}