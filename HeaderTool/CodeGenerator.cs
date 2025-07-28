using CppAst;

namespace HeaderTool;

public class FileEntities
{
    public List<CppClass> Classes = [];
    public List<CppEnum> Enums = [];
    public string FilePath = "";
}

public class CppReflectedEnumField
{
    public CppEnumItem Item;
    public string Name;
    public Dictionary<string, string> Attributes;

    public string GetModifiedName()
    {
        string name = Name;
        if (Attributes.ContainsKey("Name"))
        {
            name = Attributes["Name"];
        }

        if (name.StartsWith($"\"")) name = name.Trim('"');

        return name;
    }
}

public class CppReflectedField
{
    public CppField Field;
    public string Name;
    public Dictionary<string, string> Attributes;
    public string FullTypeName;

    public string GetModifiedName()
    {
        string name = Name;
        if (Attributes.ContainsKey("Name"))
        {
            name = Attributes["Name"];
        }

        if (name.StartsWith('"')) name = name.Trim('"');
        return name;
    }
}

public class CppReflectedEnum
{
    public CppEnum Enum;
    public string Name;
    public string FullName;
    public string OriginFullName;
    public bool IsEnumClass;
    public Dictionary<string, string> Attributes = new();
    public List<CppReflectedEnumField> Fields = new();

    public string GetModifiedName()
    {
        string name = Name;
        if (Attributes.ContainsKey("Name"))
        {
            name = Attributes["Name"];
        }

        return name;
    }
}

public class CppOriginalParent
{
    public CppClass Class;
    public string OriginalFullName;
}

public class CppReflectedClass
{
    public CppClass Class;
    public string Name;
    public string FullName;
    public string OriginFullName;
    public List<CppOriginalParent> OriginParents = new();
    public bool IsStruct;
    public Dictionary<string, string> Attributes = new();
    public List<CppReflectedField> Fields = new();
}

public class CppReflectedEntities
{
    public List<CppReflectedEnum> Enums = new();
    public List<CppReflectedClass> Classes = new();
}

public class CodeGenerator
{
    static Dictionary<CppClass, CppReflectedClass> cppClassToReflected = new();
    static Dictionary<CppEnum, CppReflectedEnum> cppEnumToReflected = new();

    static bool HasReflectionAttribute(List<CppAttribute> attributes)
    {
        if (attributes.Count == 0) return false;
        foreach (var attr in attributes)
        {
            if (attr.Kind == AttributeKind.AnnotateAttribute)
            {
                return true;
            }
        }

        return false;
    }

    static void TraverseNamespacesAndFindAllEntities(Dictionary<string, FileEntities> result,
        CppNamespace cppNamespace, Dictionary<string, GeneratedFileInfo> processingFile)
    {
        foreach (var cppClass in cppNamespace.Classes)
        {
            if (cppClass.SourceFile == null) continue;
            if (!processingFile.ContainsKey(cppClass.SourceFile)) continue;
            if (HasReflectionAttribute(cppClass.Attributes))
            {
                if (!result.ContainsKey(cppClass.SourceFile))
                {
                    result.Add(cppClass.SourceFile, new FileEntities());
                }

                result[cppClass.SourceFile].Classes.Add(cppClass);
            }
        }

        foreach (var cppEnum in cppNamespace.Enums)
        {
            if (cppEnum.SourceFile == null) continue;
            if (!processingFile.ContainsKey(cppEnum.SourceFile)) continue;
            if (HasReflectionAttribute(cppEnum.Attributes))
            {
                if (!result.ContainsKey(cppEnum.SourceFile))
                {
                    result.Add(cppEnum.SourceFile, new FileEntities());
                }

                result[cppEnum.SourceFile].Enums.Add(cppEnum);
            }
        }

        foreach (var newCppNamespace in cppNamespace.Namespaces)
        {
            TraverseNamespacesAndFindAllEntities(result, newCppNamespace, processingFile);
        }
    }

    public static Dictionary<string, FileEntities> GetFileMapToClasses(CppCompilation compilation,
        Dictionary<string, GeneratedFileInfo> processingFile)
    {
        Dictionary<string, FileEntities> fileMapToClasses = new();
        foreach (var cppClass in compilation.Classes)
        {
            if (cppClass.SourceFile == null) continue;
            string sourceFile = cppClass.SourceFile.Replace("\\", "/");
            if (!processingFile.ContainsKey(sourceFile)) 
                continue;
            if (HasReflectionAttribute(cppClass.Attributes))
            {
                if (!fileMapToClasses.ContainsKey(sourceFile))
                {
                    fileMapToClasses.Add(sourceFile, new FileEntities());
                }

                fileMapToClasses[sourceFile].Classes.Add(cppClass);
            }
        }

        foreach (var cppEnum in compilation.Enums)
        {
            if (!processingFile.ContainsKey(cppEnum.SourceFile)) continue;
            if (HasReflectionAttribute(cppEnum.Attributes))
            {
                string sourceFile = cppEnum.SourceFile.Replace("\\", "/");
                if (!fileMapToClasses.ContainsKey(sourceFile))
                {
                    fileMapToClasses.Add(sourceFile, new FileEntities());
                }

                fileMapToClasses[sourceFile].Enums.Add(cppEnum);
            }
        }

        foreach (var newCppNamespace in compilation.Namespaces)
        {
            TraverseNamespacesAndFindAllEntities(fileMapToClasses, newCppNamespace, processingFile);
        }

        return fileMapToClasses;
    }

    static Dictionary<string, string>? ParseAttributes(List<CppAttribute> attributes)
    {
        Dictionary<string, string> result = new();
        foreach (var cppAttribute in attributes)
        {
            if (cppAttribute.Kind != AttributeKind.AnnotateAttribute)
            {
                continue;
            }

            string[] pairs = cppAttribute.Arguments.Split("=");
            if (pairs.Length == 1)
            {
                result.Add(pairs[0], "true");
            }
            else if (pairs.Length == 2)
            {
                result.Add(pairs[0], pairs[1]);
            }
            else
            {
                return null;
            }
        }

        return result;
    }

    static void FindEnumFields(CppReflectedEnum reflectedEnum)
    {
        CppEnum cppEnum = reflectedEnum.Enum;
        foreach (var cppEnumItem in cppEnum.Items)
        {
            CppReflectedEnumField reflectedField = new();
            reflectedField.Item = cppEnumItem;
            reflectedField.Name = cppEnumItem.Name;
            var attrs = ParseAttributes(cppEnumItem.Attributes);
            if (attrs == null)
            {
                reflectedField.Attributes = new();
            }
            else
            {
                reflectedField.Attributes = attrs;
            }

            reflectedEnum.Fields.Add(reflectedField);
        }
    }

    static void FindClassFields(CppReflectedClass reflectedClass)
    {
        CppClass cppClass = reflectedClass.Class;
        foreach (var cppField in cppClass.Fields)
        {
            if (!HasReflectionAttribute(cppField.Attributes))
            {
                continue;
            }

            CppReflectedField reflectedField = new();
            reflectedField.Field = cppField;
            reflectedField.Name = cppField.Name;
            reflectedField.FullTypeName = cppField.Type.FullName;
            var attrs = ParseAttributes(cppField.Attributes);
            if (attrs == null)
            {
                Console.Error.WriteLine($"类{reflectedClass.Name}的属性{cppField.Name}的属性有误");
                throw new Exception();
            }
            else
            {
                reflectedField.Attributes = attrs;
            }

            reflectedClass.Fields.Add(reflectedField);
        }
    }

    private static Dictionary<string, CppReflectedEntities> TransferFileEntitiesToReflectedEntities(
        Dictionary<string, FileEntities> entities, Config config)
    {
        Dictionary<string, CppReflectedEntities> result = new();
        foreach (var (file, entity) in entities)
        {
            CppReflectedEntities newEntity = new();
            foreach (var entityClass in entity.Classes)
            {
                CppReflectedClass reflectedClass = new()
                {
                    Name = entityClass.Name,
                    OriginFullName = entityClass.FullName,
                    Class = entityClass
                };
                reflectedClass.IsStruct = entityClass.ClassKind == CppClassKind.Struct;
                reflectedClass.FullName = reflectedClass.OriginFullName.Replace("::", ".");
                var attrs = ParseAttributes(entityClass.Attributes);
                if (attrs == null)
                {
                    Console.Error.WriteLine($"类{entityClass.Name}的属性有误");
                    throw new Exception();
                }
                else
                {
                    reflectedClass.Attributes = attrs;
                }

                foreach (var baseType in entityClass.BaseTypes)
                {
                    CppOriginalParent parent = new()
                    {
                        Class = baseType.Type as CppClass,
                    };
                    parent.OriginalFullName = parent.Class!.FullName;
                    reflectedClass.OriginParents.Add(parent);
                }

                FindClassFields(reflectedClass);
                newEntity.Classes.Add(reflectedClass);
                cppClassToReflected.Add(entityClass, reflectedClass);
            }

            foreach (var entityEnum in entity.Enums)
            {
                CppReflectedEnum reflectedEnum = new()
                {
                    Name = entityEnum.Name,
                    Enum = entityEnum,
                    IsEnumClass = entityEnum.IsScoped,
                    OriginFullName = entityEnum.FullName
                };
                reflectedEnum.FullName = reflectedEnum.OriginFullName.Replace("::", ".");
                var attrs = ParseAttributes(entityEnum.Attributes);
                if (attrs == null)
                {
                    Console.Error.WriteLine($"枚举{entityEnum.Name}的属性有误");
                    throw new Exception();
                }
                else
                {
                    reflectedEnum.Attributes = attrs;
                }

                FindEnumFields(reflectedEnum);
                newEntity.Enums.Add(reflectedEnum);
                cppEnumToReflected.Add(entityEnum, reflectedEnum);
            }

            result.Add(file, newEntity);
        }

        return result;
    }

    public static bool Generate(Dictionary<string, GeneratedFileInfo> filesToProcess, Config khtConfig)
    {
        List<string> fileProcessList = new();
        foreach (var file in filesToProcess)
        {
            fileProcessList.Add(file.Value.Path);
        }

        CppParserOptions options = new();
        options.IncludeFolders.AddRange(khtConfig.ExtraIncludePath);
        options.Defines.Add("KITA_PROCESSING_METADATA_MARK");
        options.AdditionalArguments.Add("-std=c++26");
        options.Defines.Add("TRACY_ENABLE");
        options.ConfigureForWindowsMsvc();
        CppCompilation compilation = CppParser.ParseFiles(fileProcessList, options);
        foreach (var message in compilation.Diagnostics.Messages)
        {
            if (message.Type is CppLogMessageType.Error)
            {
                Console.Error.WriteLine(message);
            }
        }

        if (compilation.HasErrors)
        {
            Console.Error.WriteLine("Has Error!");
            return false;
        }

        var fileToClasses = GetFileMapToClasses(compilation, filesToProcess);
        var fileToReflectedEntities = TransferFileEntitiesToReflectedEntities(fileToClasses, khtConfig);
        Generate(filesToProcess, fileToReflectedEntities);
        return true;
    }

    static void Generate(Dictionary<string, GeneratedFileInfo> fileInfo,
        Dictionary<string, CppReflectedEntities> entitiesInfo)
    {
        foreach (var (file, entities) in entitiesInfo)
        {
            if (!fileInfo.TryGetValue(Path.GetFullPath(file).Replace("\\", "/"), out var outInfo))
            {
                Console.Error.WriteLine($"{file}需要生成却没有对应信息");
                throw new Exception();
            }

            string headerFolder = Path.GetDirectoryName(outInfo.GeneratedHeaderFilePath)!;
            Directory.CreateDirectory(headerFolder);
            using StreamWriter headerWriter = new(new FileStream(outInfo.GeneratedHeaderFilePath, FileMode.Create));
            using StreamWriter sourceWriter = new(new FileStream(outInfo.GeneratedSourceFilePath, FileMode.Create));
            headerWriter.WriteLine("// 此文件由代码生成工具自动生成, 若无必要请勿改动");
            headerWriter.WriteLine("#pragma once");
            string currentFileId = file.GetHashCode().ToString().Replace("-", "_");
            headerWriter.WriteLine("#undef CURRENT_FILE_ID");
            headerWriter.WriteLine($"#define CURRENT_FILE_ID {currentFileId}");
            headerWriter.WriteLine("class InputArchive;");
            headerWriter.WriteLine("class OutputArchive;");
            sourceWriter.WriteLine($"#include \"{file}\"");
            foreach (var cppReflectedClass in entities.Classes)
            {
                headerWriter.WriteLine($"#define GENERATED_{cppReflectedClass.Name}_{currentFileId} \\");
                GenerateClassHeader(headerWriter, cppReflectedClass);
                GenerateClassSource(sourceWriter, cppReflectedClass);
            }

            foreach (var cppReflectedEnum in entities.Enums)
            {
                headerWriter.WriteLine($"void Z_register_enum_{currentFileId}_{cppReflectedEnum.Name}();");
                headerWriter.WriteLine($"struct Z_enum_register_{currentFileId}_{cppReflectedEnum.Name} {{");
                headerWriter.WriteLine(
                    $"Z_enum_register_{currentFileId}_{cppReflectedEnum.Name}() {{ Z_register_enum_{currentFileId}_{cppReflectedEnum.Name}(); }}");
                headerWriter.WriteLine($"}};");
                headerWriter.WriteLine(
                    $"static inline Z_enum_register_{currentFileId}_{cppReflectedEnum.Name} z_enum_register_{currentFileId}_{cppReflectedEnum.Name}{{}};");

                sourceWriter.WriteLine($"void Z_register_enum_{currentFileId}_{cppReflectedEnum.Name}() {{");

                string enumName = cppReflectedEnum.GetModifiedName();
                sourceWriter.WriteLine(
                    $"TypeBuilder{{}}.CreateType<{cppReflectedEnum.OriginFullName}>({enumName})");
                sourceWriter.WriteLine();

                for (int i = 0; i < cppReflectedEnum.Fields.Count - 1; i++)
                {
                    var cppReflectedEnumField = cppReflectedEnum.Fields[i];
                    sourceWriter.Write(
                        $".AddField(\"{cppReflectedEnumField.GetModifiedName()}\", {cppReflectedEnum.OriginFullName}::{cppReflectedEnumField.Name})");
                    foreach (var attrPair in cppReflectedEnumField.Attributes.Where(attrPair => attrPair.Key != "Reflected"))
                    {
                        sourceWriter.Write($".SetFieldAttribute(\"{attrPair.Key}\",\"{attrPair.Value}\")");
                    }

                    sourceWriter.WriteLine($"");
                }
                sourceWriter.WriteLine(";}");
            }
        }
    }

    static void GenerateClassSource(StreamWriter writer, CppReflectedClass reflectedClass)
    {
        string className = reflectedClass.FullName;
        if (reflectedClass.Attributes.ContainsKey("Name"))
        {
            className = reflectedClass.Attributes["Name"];
        }

        if (!className.StartsWith('"'))
        {
            className = $"\"{className}\"";
        }

        writer.WriteLine($"void {reflectedClass.OriginFullName}::Z_RegisterMetaData() {{");
        writer.Write($"TypeBuilder{{}}.CreateType<{reflectedClass.OriginFullName}>(\"{className}\")");
        {
            // 在注册元信息内的部分
            foreach (var attrPair in reflectedClass.Attributes.Where(attrPair => attrPair.Key != "Reflected"))
            {
                writer.Write($".SetTypeAttribute(\"{attrPair.Key}\", \"{attrPair.Value}\")");
            }

            writer.WriteLine();
            if (!reflectedClass.Attributes.ContainsKey("Abstract"))
            {
                // TODO: Register Constructor
            }

            foreach (var field in reflectedClass.Fields)
            {
                string fieldName = field.Name;
                if (field.Attributes.ContainsKey("Name"))
                {
                    fieldName = field.Attributes["Name"];
                }

                if (!fieldName.StartsWith('"'))
                {
                    fieldName = $"\"{fieldName}\"";
                }

                writer.Write($".AddField(\"{fieldName}\", &{reflectedClass.OriginFullName}::{field.Name})");
                foreach (var attrPair in field.Attributes.Where(attrPair => attrPair.Key != "Reflected"))
                {
                    writer.Write($".SetFieldAttribute(\"{attrPair.Key}\", \"{attrPair.Value}\")");
                }
            }

            writer.WriteLine(";}");
        }
        // 注册获取类型的部分
        writer.WriteLine(
            $"const Type*  {reflectedClass.OriginFullName}::GetType() const {{ return TypeOf<{reflectedClass.OriginFullName}>(); }}");
        writer.WriteLine(
            $"const Type*  {reflectedClass.OriginFullName}::GetStaticType() {{ return TypeOf<{reflectedClass.OriginFullName}>(); }}");

        // 注册序列化/反序列
        if (reflectedClass.Attributes.ContainsKey("Transient") ||
            reflectedClass.Attributes.ContainsKey("CustomSerialization")) return;
        writer.WriteLine(
            $"void {reflectedClass.OriginFullName}::Serialization_Load(kita::InputArchive& Archive) {{");
        foreach (var reflectedClassField in reflectedClass.Fields.Where(reflectedClassField =>
                     !reflectedClassField.Attributes.ContainsKey("Transient")))
        {
            writer.WriteLine(
                $"Archive.ReadType(\"{reflectedClassField.GetModifiedName()}\", {reflectedClassField.Name});");
        }

        writer.WriteLine($"}}");
        writer.WriteLine(
            $"void {reflectedClass.OriginFullName}::Serialization_Save(kita::OutputArchive& Archive) const {{");
        foreach (var reflectedClassField in reflectedClass.Fields)
        {
            if (!reflectedClassField.Attributes.ContainsKey("Transient"))
            {
                writer.WriteLine(
                    $"Archive.WriteType(\"{reflectedClassField.GetModifiedName()}\", {reflectedClassField.Name});");
            }
        }

        writer.WriteLine($"}}");
    }

    static void GenerateClassHeader(StreamWriter writer, CppReflectedClass reflectedClass)
    {
        // 注册类信息的函数
        if (!reflectedClass.IsStruct)
            writer.WriteLine("public: \\");
        writer.WriteLine("static void Z_RegisterMetaData(); \\");
        writer.WriteLine($"struct Z_{reflectedClass.Name}_MetaRegister {{ \\");
        writer.WriteLine(
            $"Z_{reflectedClass.Name}_MetaRegister() {{ {reflectedClass.Name}::Z_RegisterMetaData(); }} \\");
        writer.WriteLine($"}}; \\");
        writer.WriteLine($"static inline Z_{reflectedClass.Name}_MetaRegister z_Register{{}}; \\");
        if (reflectedClass.OriginParents.Count == 0)
        {
            writer.WriteLine(reflectedClass.IsStruct
                ? $"typedef {reflectedClass.Name} ThisStruct; \\"
                : $"typedef {reflectedClass.Name} ThisClass; \\");
        }
        else
        {
            if (reflectedClass.IsStruct)
            {
                writer.WriteLine($"typedef ThisStruct Super; \\");
                writer.WriteLine($"typedef {reflectedClass.Name} ThisStruct; \\");
            }
            else
            {
                writer.WriteLine($"typedef ThisClass Super; \\");
                writer.WriteLine($"typedef {reflectedClass.Name} ThisClass; \\");
            }
        }

        if (!reflectedClass.IsStruct)
        {
            writer.WriteLine("public: \\");
            writer.WriteLine("virtual const Type* GetType() const; \\");
            writer.WriteLine("static const Type* GetStaticType(); \\");
        }
        else
        {
            writer.WriteLine("const Type* GetType() const; \\");
            writer.WriteLine("static const Type* GetStaticType(); \\");
        }

        // 有Transient代表不需要序列化
        if (!reflectedClass.Attributes.ContainsKey("Transient"))
        {
            if (reflectedClass.IsStruct)
            {
                // struct不用虚函数
                writer.WriteLine("void Serialization_Load(InputArchive& Archive); \\");
                writer.WriteLine("void Serialization_Save(OutputArchive& Archive) const; \\");
            }
            else
            {
                writer.WriteLine("virtual void Serialization_Load(InputArchive& Archive); \\");
                writer.WriteLine("virtual void Serialization_Save(OutputArchive& Archive) const; \\");
            }
        }

        if (!reflectedClass.IsStruct)
            writer.WriteLine("private:");
    }
}