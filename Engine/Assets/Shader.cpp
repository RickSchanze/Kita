//
// Created by kita on 25-9-5.
//

#include "Shader.h"

#include "AssetsConfig.h"
#include "Core/Config/ConfigManager.h"
#include "Core/FileSystem/File.h"
#include "Core/FileSystem/InputFileStream.h"
#include "Core/FileSystem/OutputFileStream.h"
#include "Core/FileSystem/Path.h"
#include "Core/String/StringUtils.h"
#include "Project/Project.h"
#include "nlohmann/json.hpp"
#include "slang-com-ptr.h"

class SlangShaderTranslater {
public:
  SlangShaderTranslater() {
    slang::createGlobalSession(mGlobalSession.writeRef());
    CreateSession();
  }

  Slang::ComPtr<slang::ISession> GetSession() { return mCompileSession; }

  [[nodiscard]] SlangInt GetLanguageIndex(const EShaderLanguage Language) const { return mLanguageIndices[ToUnderlying(Language)]; }

private:
  void CreateSession() {
    Ranges::Fill(mLanguageIndices, INVALID_INDEX);
    const auto& Config = ConfigManager::GetConfigRef<AssetsConfig>();
    auto SearchPaths = Config.GetShaderSearchPaths();
    Array<const char*> SearchPathsArray;
    for (auto& SearchPath : SearchPaths) {
      SearchPathsArray.Add(SearchPath.Data());
    }
    slang::SessionDesc Desc{};
    slang::TargetDesc TargetDesc[ToUnderlying(EShaderLanguage::Count)];
    TargetDesc[0].format = SLANG_SPIRV;
    TargetDesc[0].profile = mGlobalSession->findProfile("glsl_460");
    mLanguageIndices[ToUnderlying(EShaderLanguage::Spirv)] = 0;
    TargetDesc[1].format = SLANG_GLSL;
    TargetDesc[1].profile = mGlobalSession->findProfile("glsl_460");
    mLanguageIndices[ToUnderlying(EShaderLanguage::GLSL)] = 1;
    Desc.searchPaths = SearchPathsArray.Data();
    Desc.searchPathCount = static_cast<SlangInt>(SearchPathsArray.Count());
    Desc.targets = TargetDesc;
    Desc.targetCount = 2;
    mGlobalSession->createSession(Desc, mCompileSession.writeRef());
  }

  Slang::ComPtr<slang::IGlobalSession> mGlobalSession;
  Slang::ComPtr<slang::ISession> mCompileSession;
  SlangInt mLanguageIndices[ToUnderlying(EShaderLanguage::Count)]{};
};

static SlangShaderTranslater& GetTranslater() {
  static SlangShaderTranslater Translater;
  return Translater;
}

Array<ShaderParameterInfo> ShaderBinaryData::ParseParameterInfo() {
  if (Data.Count() < 8) {
    return {};
  }
  Int32 ParamLength = *reinterpret_cast<Int32*>(Data.Data() + 4);
  if (ParamLength <= 0 || Data.Count() < 8 + ParamLength) {
    return {};
  }
  StringView ParameterInfoString(reinterpret_cast<const char*>(Data.Data() + 8), ParamLength);
  try {
    nlohmann::json Json = nlohmann::json::parse(ParameterInfoString.GetStdStringView());
    Array<ShaderParameterInfo> Infos{};
    Infos.Reserve(Json.size());
    for (const auto& Param : Json) {
      ShaderParameterInfo Info{};
      Info.Name = Param["A"].get<std::string>();
      Info.Type = static_cast<EShaderParameterType>(Param["B"].get<Int32>());
      Info.Size = Param["C"].get<Int32>();
      Info.Space = Param["D"].get<Int32>();
      Info.IsDynamic = Param["E"].get<bool>();
      Info.IsHideInEditor = Param["F"].get<bool>();
      Info.Label = Param["G"].get<std::string>();
      Info.SharedName = Param["H"].get<std::string>();
      if (Param.contains("I")) {
        for (const auto& Member : Param["I"]) {
          ShaderMemberInfo MemberInfo{};
          MemberInfo.Name = Member["A"].get<std::string>();
          MemberInfo.Type = static_cast<EShaderParameterType>(Member["B"].get<Int32>());
          MemberInfo.Offset = Member["C"].get<Int32>();
          MemberInfo.Size = Member["D"].get<Int32>();
          MemberInfo.Binding = Member["E"].get<Int32>();
          MemberInfo.IsHideInEditor = Member["F"].get<bool>();
          MemberInfo.Label = Member["G"].get<std::string>();
          Info.Members.Add(MemberInfo);
        }
      }
      Infos.Add(Info);
    }
    return Infos;
  } catch (std::exception& E) {
    gLogger.Error(Logcat::Asset, "解析Shader参数出错: {}.", E.what());
    return {};
  }
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
    mParameterInfos = mShaderData.ParseParameterInfo();
    gLogger.Info(Logcat::Asset, "Shader '{}' 加载成功[ObjectHandle = {}].", mPath, GetHandle());
  } else {
    gLogger.Error(Logcat::Asset, "Shader '{}' 加载失败[ObjectHandle = {}].", mPath, GetHandle());
  }
}

void Shader::Unload() {
  mShaderData.Data.Clear();
  mLoaded = false;
  gLogger.Info(Logcat::Asset, "卸载Shader {}, ObjectHandle = {}.", mPath, GetHandle());
}

void Shader::ApplyMeta(const AssetMeta& Meta) {
  const auto& MyShaderMeta = static_cast<const ShaderMeta&>(Meta);
  mPath = MyShaderMeta.Path;
  // Apply时Handle已经处理
}

constexpr auto ShaderCacheFolder = "Shader/Cache";

String Shader::GetBinaryPath() const {
  const auto ThisShaderBinaryFileName = mPath.Replace(".slang", ".shaderbin");
  return Path::Combine(Path::Combine(Project::GetIntermediatePath(), ShaderCacheFolder), ThisShaderBinaryFileName);
}

void Shader::GenerateTempCode(StringView Folder, const EShaderLanguage Language) {
  if (!IsLoaded()) {
    Load();
    if (!mLoaded) {
      gLogger.Error(Logcat::Asset, "加载Shader '{}' 的临时 {} 失败.", mPath, EnumToString(Language));
      return;
    }
  }
  using namespace slang;
  Slang::ComPtr<IBlob> Diagnostics;
  IModule* Module = GetTranslater().GetSession()->loadModule(mPath.Data(), Diagnostics.writeRef());
  if (IsGraphics()) {
    Slang::ComPtr<IEntryPoint> VertexEntryPoint;
    Module->findEntryPointByName("VertexMain", VertexEntryPoint.writeRef());
    Slang::ComPtr<IEntryPoint> FragmentEntryPoint;
    Module->findEntryPointByName("FragmentMain", FragmentEntryPoint.writeRef());
    Array<IComponentType*> Components = {Module, VertexEntryPoint, FragmentEntryPoint};
    Slang::ComPtr<IComponentType> LinkedProgram;
    GetTranslater().GetSession()->createCompositeComponentType(Components.Data(), 3, LinkedProgram.writeRef());
    if (LinkedProgram) {
      const String FileName = Format("{}.glsl", Path::GetFileNameWithoutExtension(mPath));
      OutputFileStream OutputFile{Path::Combine(Folder, FileName)};
      SlangInt StageIndices[ToUnderlying(EShaderStage::Count)];
      const auto Layout = LinkedProgram->getLayout();
      for (auto Index = 0; Index < Layout->getEntryPointCount(); Index++) {
        const auto EntryPoint = Layout->getEntryPointByIndex(Index);
        if (EntryPoint->getStage() == SLANG_STAGE_VERTEX) {
          StageIndices[ToUnderlying(EShaderStage::Vertex)] = Index;
        } else if (EntryPoint->getStage() == SLANG_STAGE_FRAGMENT) {
          StageIndices[ToUnderlying(EShaderStage::Fragment)] = Index;
        }
      }
      Slang::ComPtr<slang::IBlob> Code;
      LinkedProgram->getEntryPointCode(StageIndices[ToUnderlying(EShaderStage::Vertex)], GetTranslater().GetLanguageIndex(Language), Code.writeRef());
      OutputFile.WriteLine("Vertex:");
      OutputFile << static_cast<const char*>(Code->getBufferPointer());
      OutputFile.WriteLine();
      OutputFile.WriteLine("Fragment:");
      OutputFile << static_cast<const char*>(Code->getBufferPointer());
    } else {
      gLogger.Error(Logcat::Asset, "GenerateTempCode: 链接Shader '{}' 失败.", mPath);
    }
  } else {
    Slang::ComPtr<IEntryPoint> ComputeEntryPoint;
    Module->findEntryPointByName("ComputeMain", ComputeEntryPoint.writeRef());
    // TODO
  }
}

bool Shader::IsCompute() const { return mShaderData.IsGraphicsShader(); }

bool Shader::IsGraphics() const { return !mShaderData.IsGraphicsShader(); }

bool Shader::NeedReTranslate() const {
  if (sCache.Contains(mPath)) {
    const ShaderCache& OldCache = sCache[mPath];
    if (const auto NewTime = File::GetLastModifiedTime(mPath)) {
      if (*NewTime != OldCache.LastTextModifiedTime) {
        return true;
      }
    }
    if (const auto ThisShaderBinaryPath = GetBinaryPath(); Path::IsExists(ThisShaderBinaryPath)) {
      if (const auto BinaryTime = File::GetLastModifiedTime(ThisShaderBinaryPath)) {
        if (*BinaryTime == OldCache.LastBinaryModifiedTime) {
          return false;
        }
      }
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
      const SlangResult Result = Attribute->getArgumentValueInt(0, &CullMode);
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

static EShaderParameterType GetType(slang::TypeLayoutReflection* TypeLayout) {
  using namespace slang;
  if (!TypeLayout) {
    return EShaderParameterType::Count;
  }
  switch (TypeLayout->getKind()) {
  case TypeReflection::Kind::Vector: {
    if (TypeLayout->getScalarType() == TypeReflection::Float32) {
      if (TypeLayout->getColumnCount() == 4) {
        return EShaderParameterType::Float4;
      }
    }
  } break;
  case TypeReflection::Kind::Matrix: {
    if (TypeLayout->getScalarType() == TypeReflection::Float32 && TypeLayout->getRowCount() == 4 && TypeLayout->getColumnCount() == 4) {
      return EShaderParameterType::Float4x4;
    }
  } break;
  case TypeReflection::Kind::Resource: {
    switch (TypeLayout->getResourceShape()) {
    case SLANG_TEXTURE_2D:
      return EShaderParameterType::Texture2D;
    case SLANG_TEXTURE_CUBE:
      return EShaderParameterType::TextureCube;
    default:
      break;
    }
  } break;
  case TypeReflection::Kind::SamplerState:
    return EShaderParameterType::SamplerState;
  default:
    break;
  }
  return EShaderParameterType::Custom;
}

template <typename T> static String ProcessVariableAttribute(T& Target, slang::Attribute* Attr) {
  const auto Name = Attr->getName();
  if (Name != nullptr) {
    const StringView NameView = Name;
#if KITA_EDITOR
    if (NameView == "HideInEditor") {
      Target.IsHideInEditor = true;
    }
    if (NameView == "Label") {
      SizeType Size;
      const char* Label = Attr->getArgumentValueString(0, &Size);
      if (Label != nullptr && Size != 0) {
        Target.Label = StringView(Label, Size).Trim('"').ToString();
        return {};
      }
      return "Label属性需要指定一个Name";
    }
#endif
    if constexpr (Traits::SameAs<T, ShaderParameterInfo>) {
      if (NameView == "Dynamic") {
        Target.IsDynamic = true;
      }
      if (NameView == "Shared") {
        SizeType Size;
        const char* SharedName = Attr->getArgumentValueString(0, &Size);
        if (Size != 0 && SharedName != nullptr) {
          const auto SharedNameView = StringView(SharedName, Size).Trim('"');
          Target.SharedName = SharedNameView.ToString();
        } else {
          return Format("Shared属性必须指定一个Name.");
        }
      }
    }
  }
  return {};
}

static String ReflectStruct(slang::TypeLayoutReflection* LayoutReflection, Array<ShaderMemberInfo>& OutMemberInfos) {
  const SlangInt MemberCount = LayoutReflection->getFieldCount();
  for (SlangInt Index = 0; Index < MemberCount; Index++) {
    const auto VarLayout = LayoutReflection->getFieldByIndex(Index);
    if (!VarLayout) {
      continue;
    }
    const auto Var = VarLayout->getVariable();
    if (!Var) {
      continue;
    }
    ShaderMemberInfo MemberInfo;
    MemberInfo.Name = StringView(Var->getName()).Trim('"').ToString();
    const auto FieldTypeLayout = VarLayout->getTypeLayout();
    MemberInfo.Type = GetType(FieldTypeLayout);

    switch (MemberInfo.Type) {
    case EShaderParameterType::Float4x4:
    case EShaderParameterType::Float4:
      MemberInfo.Size = static_cast<Int32>(FieldTypeLayout->getSize());
      MemberInfo.Offset = static_cast<Int32>(VarLayout->getOffset());
      MemberInfo.Binding = 0;
      break;
    case EShaderParameterType::TextureCube:
    case EShaderParameterType::Texture2D:
    case EShaderParameterType::SamplerState:
      MemberInfo.Size = 0;
      MemberInfo.Offset = 0;
      MemberInfo.Binding = static_cast<Int32>(VarLayout->getBindingIndex());
      break;
    case EShaderParameterType::Custom:
      return "暂时不支持嵌套结构体";
    case EShaderParameterType::Count:
      break;
    }
    const auto AttrCount = Var->getUserAttributeCount();
    for (SlangInt AttrIndex = 0; AttrIndex < AttrCount; AttrIndex++) {
      const auto Attr = Var->getUserAttributeByIndex(AttrIndex);
      const auto Error = ProcessVariableAttribute(MemberInfo, Attr);
      if (!Error.Empty()) {
        return Error;
      }
    }
    OutMemberInfos.Add(MemberInfo);
  }
  return "";
};

static String ReflectParameter(slang::VariableLayoutReflection* Param, Array<ShaderParameterInfo>& OutShaderInfos) {
  if (!Param)
    return {};

  const auto Variable = Param->getVariable();
  if (!Variable)
    return {};

  const auto TypeLayout = Param->getTypeLayout();
  ShaderParameterInfo ParameterInfo;
  ParameterInfo.Name = StringView(Variable->getName()).Trim('"').ToString();
  const auto ParamType = TypeLayout->getKind();
  if (ParamType != slang::TypeReflection::Kind::ParameterBlock) {
    gLogger.Error(Logcat::Asset, "Shader的所有参数必须由ParameterBlock包裹. 但是'{}'违反了这一规则.", ParameterInfo.Name);
    return Format("Shader的参数都必须由ParameterBlock包裹, 但是参数'{}'违反了这一规则.", ParameterInfo.Name);
  }
  const auto ElementTypeLayout = TypeLayout->getElementTypeLayout();
  ParameterInfo.Type = GetType(ElementTypeLayout);
  ParameterInfo.Size = static_cast<Int32>(ElementTypeLayout->getSize());
  ParameterInfo.Space = static_cast<Int32>(Param->getBindingIndex());
  if (ParameterInfo.Type == EShaderParameterType::Custom) {
    ReflectStruct(ElementTypeLayout, ParameterInfo.Members);
  }
  const SlangInt AttrCount = Variable->getUserAttributeCount();
  for (SlangInt Index = 0; Index < AttrCount; Index++) {
    const auto Attr = Variable->getUserAttributeByIndex(Index);
    String Message = ProcessVariableAttribute(ParameterInfo, Attr);
    if (!Message.Empty()) {
      return Message;
    }
  }
  OutShaderInfos.Add(ParameterInfo);
  return {};
}

String ToJson(const Array<ShaderParameterInfo>& AllParams) {
  nlohmann::json Json;
  for (const auto& Param : AllParams) {
    nlohmann::json ParamJson;
    ParamJson["A"] = Param.Name.GetStdString();
    ParamJson["B"] = Param.Type;
    ParamJson["C"] = Param.Size;
    ParamJson["D"] = Param.Space;
    ParamJson["E"] = Param.IsDynamic;
    ParamJson["F"] = Param.IsHideInEditor;
    ParamJson["G"] = Param.Label.GetStdString();
    ParamJson["H"] = Param.SharedName.GetStdString();
    for (const auto& Member : Param.Members) {
      nlohmann::json MemberJson;
      MemberJson["A"] = Member.Name.GetStdString();
      MemberJson["B"] = Member.Type;
      MemberJson["C"] = Member.Offset;
      MemberJson["D"] = Member.Size;
      MemberJson["E"] = Member.Binding;
      MemberJson["F"] = Member.IsHideInEditor;
      MemberJson["G"] = Member.Label.GetStdString();
      ParamJson["I"].push_back(MemberJson);
    }
    Json.push_back(ParamJson);
  }
  return Json.dump();
}

static String ProcessParameters(slang::ProgramLayout* Layout, ShaderBinaryData& OutBinaryData, UInt64& InOutCursor) {
  Array<ShaderParameterInfo> ParamInfos;
  const SlangInt GlobalParamCount = Layout->getParameterCount();
  for (SlangInt Index = 0; Index < GlobalParamCount; Index++) {
    const auto Param = Layout->getParameterByIndex(Index);
    if (!Param)
      continue;
    String Error = ReflectParameter(Param, ParamInfos);
    if (!Error.Empty()) {
      return Error;
    }
  }
  const String ParamJson = ToJson(ParamInfos);
  OutBinaryData.Data.Resize(OutBinaryData.Data.Count() + ParamJson.Count() + 4);
  Byte* Current = OutBinaryData.Data.Data() + InOutCursor;
  *reinterpret_cast<Int32*>(Current) = static_cast<Int32>(ParamJson.Count());
  Current += 4;
  Memcpy(Current, ParamJson.Data(), ParamJson.Count());
  InOutCursor += ParamJson.Count() + 4;
  return {};
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
  UInt64 Cursor = 0;
  // 反射
  Byte FirstByte{};
  FirstByte.Set(0); // 设置图像管线为true
  FirstByte.Set(1); // 默认使用CullMode为backface
  // 前四个字节为管线属性 先占个位
  OutBinaryData.Data.Add(FirstByte);
  Byte SecondByte{};
  OutBinaryData.Data.Add(SecondByte);
  Byte ThirdByte{};
  OutBinaryData.Data.Add(ThirdByte);
  Byte FourthByte{};
  OutBinaryData.Data.Add(FourthByte);
  Cursor = 4;

  slang::ProgramLayout* ProgLayout = Program->getLayout();
  if (!ProgLayout) {
    return false;
  }

  // 反射获取入口点信息
  bool Processed[std::to_underlying(EShaderStage::Count)]{};
  SlangInt StageIndex[std::to_underlying(EShaderStage::Count)]{};
  const SlangInt EntryPointCount = static_cast<Int32>(ProgLayout->getEntryPointCount());
  for (SlangInt Index = 0; Index < EntryPointCount; Index++) {
    slang::EntryPointLayout* EntryPoint = ProgLayout->getEntryPointByIndex(Index);
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
      StageIndex[std::to_underlying(EShaderStage::Vertex)] = Index;
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
      StageIndex[std::to_underlying(EShaderStage::Fragment)] = Index;
    } break;
    default:
      gLogger.Critical(Logcat::Asset, "暂不支持的Shader Stage: {}", Stage);
    }
  }
  // 接下来是参数
  String ErrorMessage = ProcessParameters(ProgLayout, OutBinaryData, Cursor);
  if (!ErrorMessage.Empty()) {
    gLogger.Error(Logcat::Asset, "处理Shader '{}' 时参数出现错误: {}", ShaderPath, ErrorMessage);
    return false;
  }
  // 接下来是SpirvCode
  Slang::ComPtr<slang::IBlob> VertSpirvCode;
  SlangResult Result =
      Program->getEntryPointCode(StageIndex[ToUnderlying(EShaderStage::Vertex)], GetTranslater().GetLanguageIndex(EShaderLanguage::Spirv), VertSpirvCode.writeRef(), Diagnostics.writeRef());
  if (SLANG_FAILED(Result)) {
    gLogger.Error(Logcat::Asset, "处理Shader '{}' 时获取Vertex SpirvCode 出错(Code = {}): {}", ShaderPath, Result,
                  Diagnostics->getBufferPointer() == nullptr ? "Unknown Error" : static_cast<const char*>(Diagnostics->getBufferPointer()));
    return false;
  }
  Slang::ComPtr<slang::IBlob> FragSpirvCode;
  Result = Program->getEntryPointCode(StageIndex[ToUnderlying(EShaderStage::Fragment)], GetTranslater().GetLanguageIndex(EShaderLanguage::Spirv), FragSpirvCode.writeRef(), Diagnostics.writeRef());
  if (SLANG_FAILED(Result)) {
    gLogger.Error(Logcat::Asset, "处理Shader '{}' 时获取Fragment SpirvCode 出错(Code = {}): {}", ShaderPath, Result,
                  Diagnostics->getBufferPointer() == nullptr ? "Unknown Error" : static_cast<const char*>(Diagnostics->getBufferPointer()));
    return false;
  }
  // 写入Vert长度和Frag长度
  OutBinaryData.Data.Resize(OutBinaryData.Data.Count() + 4 * 2 + VertSpirvCode->getBufferSize() + FragSpirvCode->getBufferSize());
  *reinterpret_cast<Int32*>(OutBinaryData.Data.Data() + Cursor) = static_cast<Int32>(VertSpirvCode->getBufferSize());
  Cursor += 4;
  *reinterpret_cast<Int32*>(OutBinaryData.Data.Data() + Cursor) = static_cast<Int32>(FragSpirvCode->getBufferSize());
  Cursor += 4;
  Memcpy(OutBinaryData.Data.Data() + Cursor, VertSpirvCode->getBufferPointer(), VertSpirvCode->getBufferSize());
  Cursor += VertSpirvCode->getBufferSize();
  Memcpy(OutBinaryData.Data.Data() + Cursor, FragSpirvCode->getBufferPointer(), FragSpirvCode->getBufferSize());
  Cursor += FragSpirvCode->getBufferSize();
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
  if (TranslateGraphics(GetTranslater().GetSession(), Module, VertexEntryPoint, FragmentEntryPoint, mShaderData, mPath)) {
    // 写入Binary文件
    const String BinaryPath = GetBinaryPath();
    {
      OutputFileStream OutputFile(BinaryPath, std::ios::out | std::ios::binary);
      OutputFile.WriteBytes(reinterpret_cast<const char*>(mShaderData.Data.Data()), static_cast<std::streamsize>(mShaderData.Data.Count()));
    }
    const UInt64 BinaryModifiedTime = *File::GetLastModifiedTime(BinaryPath);
    const UInt64 TextModifiedTime = *File::GetLastModifiedTime(mPath);
    sCache[mPath] = ShaderCache{TextModifiedTime, BinaryModifiedTime};
    WriteCache();
    return true;
  } else {
    mShaderData.Data.Clear();
    return false;
  }
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

void Shader::GenerateTempCodeGLSL() {
  const String TempPath = Path::Combine(Project::GetIntermediatePath(), "Shader");
  GenerateTempCode(TempPath, EShaderLanguage::GLSL);
}

void Shader::GenerateTempCodeHLSL() {
  const String TempPath = Path::Combine(Project::GetIntermediatePath(), "Shader");
  GenerateTempCode(TempPath, EShaderLanguage::HLSL);
}
