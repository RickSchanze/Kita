import os
import re
from dataclasses import dataclass, field
from typing import List, Dict, Optional, TextIO

# ======== 配置 ========
EXCLUDED_HEADERS = [
    "Core/Reflection/MetaMark.h"
]
PROJECT_ROOT_PATH = "C:/Users/kita/Documents/Projects/Kita/Engine"


# ---------------- 数据模型 ----------------
@dataclass
class ArgumentInfo:
    type: str
    name: str


@dataclass
class PropertyInfo:
    name: str
    type: str
    attributes: Dict[str, str]
    default: Optional[str] = None


@dataclass
class FunctionInfo:
    name: str
    return_type: str
    attributes: Dict[str, str]
    args: List[ArgumentInfo] = field(default_factory=list)


@dataclass
class EnumMemberInfo:
    name: str
    value: Optional[str] = None
    attributes: Dict[str, str] = field(default_factory=dict)


@dataclass
class EnumInfo:
    name: str
    attributes: Dict[str, str]
    members: List[EnumMemberInfo] = field(default_factory=list)


@dataclass
class ClassInfo:
    type: str  # "class" or "struct"
    name: str
    attributes: Dict[str, str]
    bases: List[str] = field(default_factory=list)  # 新增：父类完整名字
    properties: List[PropertyInfo] = field(default_factory=list)
    functions: List[FunctionInfo] = field(default_factory=list)


@dataclass
class CMakeProjectInfo:
    project_path: str
    project_name: str
    files: Dict[str, str] = field(default_factory=dict)  # abs_path -> relative_path
    parsed_files: Dict[str, List[object]] = field(default_factory=dict)  # abs_path -> parsed structures


# ---------------- HeaderParser ----------------
class HeaderParser:
    @staticmethod
    def _remove_comments(code: str) -> str:
        result = []
        i = 0
        in_block_comment = False
        in_line_comment = False

        while i < len(code):
            if in_block_comment:
                if code[i:i + 2] == "*/":
                    in_block_comment = False
                    i += 2
                else:
                    if code[i] == "\n":
                        result.append("\n")
                    i += 1
            elif in_line_comment:
                if code[i] == "\n":
                    in_line_comment = False
                    result.append("\n")
                i += 1
            else:
                if code[i:i + 2] == "/*":
                    in_block_comment = True
                    i += 2
                elif code[i:i + 2] == "//":
                    in_line_comment = True
                    i += 2
                else:
                    result.append(code[i])
                    i += 1

        return "".join(result)

    @staticmethod
    def _tokenize(code: str) -> List[str]:
        tokens = []
        i = 0
        while i < len(code):
            c = code[i]
            if c.isspace():
                i += 1
                continue
            if c.isalpha() or c == '_' or c.isdigit():
                start = i
                while (
                        i < len(code)
                        and (code[i].isalnum() or code[i] in ['_', ':', '<', '>', '*', '&'])
                ):
                    i += 1
                tokens.append(code[start:i])
                continue
            if c == '"':
                start = i
                i += 1
                while i < len(code) and code[i] != '"':
                    if code[i] == '\\':
                        i += 1
                    i += 1
                i += 1
                tokens.append(code[start:i])
                continue
            tokens.append(c)
            i += 1
        return tokens

    @staticmethod
    def _parse_attributes(tokens: List[str], idx: int) -> (Dict[str, str], int):
        attrs: Dict[str, str] = {}
        assert tokens[idx] == "("
        idx += 1
        key: Optional[str] = None
        while idx < len(tokens) and tokens[idx] != ")":
            tok = tokens[idx]
            if tok == ",":
                idx += 1
                continue
            if key is None:
                key = tok
                idx += 1
            elif tok == "=":
                idx += 1
                value = tokens[idx]
                if value.startswith('"') and value.endswith('"'):
                    value = value[1:-1]
                attrs[key] = value
                key = None
                idx += 1
            else:
                attrs[key] = "true"
                key = None
        if key is not None:
            attrs[key] = "true"
        return attrs, idx + 1

    @staticmethod
    def _parse_default_value(tokens: List[str], idx: int) -> (Optional[str], int):
        if tokens[idx] == "=":
            idx += 1
            val_tokens = []
            while idx < len(tokens) and tokens[idx] != ";":
                val_tokens.append(tokens[idx])
                idx += 1
            return " ".join(val_tokens), idx
        elif tokens[idx] == "{":
            val_tokens = []
            brace_level = 0
            while idx < len(tokens) and not (brace_level == 0 and tokens[idx] == ";"):
                if tokens[idx] == "{":
                    brace_level += 1
                elif tokens[idx] == "}":
                    brace_level -= 1
                val_tokens.append(tokens[idx])
                idx += 1
            return " ".join(val_tokens), idx
        return None, idx

    @staticmethod
    def _parse_function_args(tokens: List[str], idx: int) -> (List[ArgumentInfo], int):
        args: List[ArgumentInfo] = []
        while tokens[idx] != ")":
            type_parts = []
            arg_name = ""
            while idx < len(tokens) and tokens[idx] not in [",", ")"]:
                if (tokens[idx + 1] in [",", ")"]) or (tokens[idx + 1] == "="):
                    arg_name = tokens[idx]
                    idx += 1
                    break
                else:
                    type_parts.append(tokens[idx])
                    idx += 1
            arg_type = " ".join(type_parts)
            args.append(ArgumentInfo(type=arg_type, name=arg_name))
            if tokens[idx] == ",":
                idx += 1
        idx += 1
        return args, idx

    def _parse_class_or_struct(self, tokens: List[str], idx: int, kind: str) -> (ClassInfo, int):
        """解析 KCLASS / KSTRUCT"""
        attrs, idx = self._parse_attributes(tokens, idx + 1)
        assert tokens[idx] == kind
        class_name = tokens[idx + 1]
        idx += 2

        # ===== 解析继承 =====
        bases = []
        if idx < len(tokens) and tokens[idx] == ":":
            idx += 1
            while idx < len(tokens) and tokens[idx] != "{":
                if tokens[idx] in ("public", "protected", "private"):
                    idx += 1
                    continue
                base_parts = []
                while idx < len(tokens) and tokens[idx] not in [",", "{"]:
                    base_parts.append(tokens[idx])
                    idx += 1
                if base_parts:
                    bases.append("".join(base_parts))
                if idx < len(tokens) and tokens[idx] == ",":
                    idx += 1

        assert tokens[idx] == "{"
        idx += 1

        class_info = ClassInfo(
            type=kind,
            name=class_name,
            attributes=attrs,
            bases=bases
        )

        ### FIXED: 用 brace_level 控制
        brace_level = 1
        while idx < len(tokens) and brace_level > 0:
            tok = tokens[idx]

            if tok == "{":
                brace_level += 1
                idx += 1
                continue
            elif tok == "}":
                brace_level -= 1
                idx += 1
                continue

            if tok == "KPROPERTY":
                prop_attrs, idx = self._parse_attributes(tokens, idx + 1)

                # 类型和变量名解析
                type_parts = []
                prop_name = ""
                while idx < len(tokens):
                    if tokens[idx] in (";", "=", "{", "("):
                        break
                    type_parts.append(tokens[idx])
                    idx += 1
                if type_parts:
                    prop_name = type_parts[-1]
                    prop_type = " ".join(type_parts[:-1])
                else:
                    prop_type = ""

                default_val = None
                if idx < len(tokens) and tokens[idx] in ("=", "{"):
                    default_val, idx = self._parse_default_value(tokens, idx)

                assert tokens[idx] == ";"
                idx += 1

                class_info.properties.append(PropertyInfo(
                    name=prop_name,
                    type=prop_type,
                    attributes=prop_attrs,
                    default=default_val
                ))

            elif tok == "KFUNCTION":
                func_attrs, idx = self._parse_attributes(tokens, idx + 1)
                ret_type_parts = []
                while idx < len(tokens) and tokens[idx + 1] != "(":
                    ret_type_parts.append(tokens[idx])
                    idx += 1
                ret_type = " ".join(ret_type_parts)
                func_name = tokens[idx]
                idx += 1
                assert tokens[idx] == "("
                idx += 1
                args, idx = self._parse_function_args(tokens, idx)
                assert tokens[idx] == ";"
                idx += 1
                class_info.functions.append(FunctionInfo(
                    name=func_name,
                    return_type=ret_type,
                    attributes=func_attrs,
                    args=args
                ))
            else:
                idx += 1

        return class_info, idx

    def parse(self, file_path: str) -> List[object]:
        try:
            with open(file_path, "r", encoding="utf-8") as f:
                code = f.read()
        except Exception as e:
            print(f"[WARN] 无法读取 {file_path}: {e}")
            return []

        code = self._remove_comments(code)
        tokens = self._tokenize(code)
        idx = 0
        result: List[object] = []

        while idx < len(tokens):
            tok = tokens[idx]
            if tok == "KCLASS":
                class_info, idx = self._parse_class_or_struct(tokens, idx, "class")
                result.append(class_info)
            elif tok == "KSTRUCT":
                struct_info, idx = self._parse_class_or_struct(tokens, idx, "struct")
                result.append(struct_info)
            elif tok == "KENUM":
                enum_attrs, idx = self._parse_attributes(tokens, idx + 1)
                assert tokens[idx] == "enum"
                idx += 1
                if tokens[idx] in ("class", "struct"):
                    idx += 1
                enum_name = tokens[idx]
                idx += 1
                assert tokens[idx] == "{"
                idx += 1

                enum_info = EnumInfo(
                    name=enum_name,
                    attributes=enum_attrs
                )

                current_value = -1
                while idx < len(tokens) and tokens[idx] != "}":
                    member_attrs = {}
                    if tokens[idx] == "KVALUE":
                        member_attrs, idx = self._parse_attributes(tokens, idx + 1)

                    member_name = tokens[idx]
                    idx += 1

                    member_value = None
                    if tokens[idx] == "=":
                        idx += 1
                        val_tokens = []
                        while idx < len(tokens) and tokens[idx] not in [",", "}"]:
                            val_tokens.append(tokens[idx])
                            idx += 1
                        member_value = " ".join(val_tokens)
                        try:
                            current_value = int(member_value, 0)
                        except ValueError:
                            pass
                    else:
                        current_value += 1
                        member_value = str(current_value)

                    enum_info.members.append(EnumMemberInfo(
                        name=member_name,
                        value=member_value,
                        attributes=member_attrs
                    ))

                    if tokens[idx] == ",":
                        idx += 1

                idx += 1
                result.append(enum_info)
            else:
                idx += 1
        return result


# ---------------- CMake 扫描器 ----------------
class CMakeScanner:
    HEADER_EXTENSIONS = {".h", ".hpp", ".hh", ".hxx"}

    def __init__(self, root_path: str, parser: HeaderParser, excluded_headers: List[str] = None):
        self.root_path = os.path.abspath(root_path).replace("\\", "/")
        self.parser = parser
        self.excluded_headers = set(excluded_headers or [])
        self._file_cache: Dict[str, float] = {}

    def scan_projects(self) -> List[CMakeProjectInfo]:
        cmake_projects: List[CMakeProjectInfo] = []
        for dirpath, _, filenames in os.walk(self.root_path):
            if "CMakeLists.txt" in filenames:
                project_info = self._parse_cmake_project(dirpath)
                if project_info:
                    cmake_projects.append(project_info)
        return cmake_projects

    def _parse_cmake_project(self, project_dir: str) -> Optional[CMakeProjectInfo]:
        cmake_file = os.path.join(project_dir, "CMakeLists.txt")
        try:
            with open(cmake_file, "r", encoding="utf-8") as f:
                cmake_content = f.read()
        except Exception as e:
            print(f"[WARN] 无法读取 {cmake_file}: {e}")
            return None

        header_files = self._extract_header_files(cmake_content)
        files_map: Dict[str, str] = {}
        parsed_map: Dict[str, List[object]] = {}

        for header in header_files:
            abs_path = os.path.join(project_dir, header).replace("\\", "/")
            rel_global = os.path.relpath(abs_path, self.root_path).replace("\\", "/")

            if rel_global in self.excluded_headers:
                continue

            if os.path.isfile(abs_path):
                # 这里改成相对于 PROJECT_ROOT_PATH 的相对路径
                rel_path = os.path.relpath(abs_path, self.root_path).replace("\\", "/")
                files_map[abs_path] = rel_path

                mtime = os.path.getmtime(abs_path)
                if abs_path not in self._file_cache or self._file_cache[abs_path] < mtime:
                    parsed_map[abs_path] = self.parser.parse(abs_path)
                    self._file_cache[abs_path] = mtime
            else:
                parsed_map[abs_path] = parsed_map.get(abs_path, [])

        project_name = os.path.basename(project_dir)
        return CMakeProjectInfo(
            project_path=project_dir.replace("\\", "/"),
            project_name=project_name,
            files=files_map,
            parsed_files=parsed_map
        )

    @staticmethod
    def _extract_header_files(cmake_content: str) -> List[str]:
        headers = []
        pattern = re.compile(r"[^\s'\"]+\.(?:h|hpp|hh|hxx)", re.IGNORECASE)
        for match in pattern.findall(cmake_content):
            headers.append(match.strip())
        return headers


class CodeGenerator:
    def __init__(self, generated_directory: str):
        self.generated_directory = os.path.abspath(generated_directory).replace("\\", "/")

    def generate_projects(self, projs: List[CMakeProjectInfo]):
        for proj in projs:
            self.generate_project(proj)

    def generate_project(self, proj: CMakeProjectInfo):
        has_class_or_enum = any(
            any(item.__class__.__name__ in ("ClassInfo", "EnumInfo") for item in parsed_items)
            for parsed_items in proj.parsed_files.values()
        )
        if not has_class_or_enum:
            return

        output_dir = os.path.join(self.generated_directory, proj.project_name)
        os.makedirs(output_dir, exist_ok=True)

        for abs_header_path, parsed_items in proj.parsed_files.items():
            if not parsed_items:
                continue
            has_code = any(item.__class__.__name__ in ("ClassInfo", "EnumInfo") for item in parsed_items)
            if not has_code:
                continue

            base_name = os.path.splitext(os.path.basename(abs_header_path))[0]
            gen_h_path = os.path.join(output_dir, f"{base_name}.generated.h")
            gen_cpp_path = os.path.join(output_dir, f"{base_name}.generated.cpp")

            self._write_generated_header(gen_h_path, base_name, parsed_items)
            self._write_generated_cpp(gen_cpp_path, base_name, parsed_items, proj.files[abs_header_path])

    def _get_true_name(self, obj: ClassInfo | EnumInfo | FunctionInfo | PropertyInfo | EnumMemberInfo):
        if "Name" in obj.attributes:
            return obj.attributes["Name"]
        return obj.name

    def _write_generated_source_class(self, f: TextIO, class_info: ClassInfo):
        f.write(f"const Type* {class_info.name}::GetStaticType() {{ return TypeOf<{class_info.name}>(); }}\n")
        f.write(f"const Type* {class_info.name}::GetType() {{ return TypeOf<{class_info.name}>(); }}\n")
        f.write(f"void {class_info.name}::WriteArchive(OutputArchive& Archive) const {{ \n")
        if len(class_info.bases) != 0:
            f.write(f"Super::WriteArchive(Archive); \n")
        for my_property in class_info.properties:
            f.write(f"Archive.WriteType(\"{self._get_true_name(my_property)}\", {my_property.name}); \n")
        f.write(f"}} \n")
        f.write(f"void {class_info.name}::ReadArchive(InputArchive& Archive) {{ \n")
        if len(class_info.bases) != 0:
            f.write(f"Super::ReadArchive(Archive); \n")
        for my_property in class_info.properties:
            f.write(f"Archive.ReadType(\"{self._get_true_name(my_property)}\", {my_property.name}); \n")
        f.write(f"}} \n")

    def _write_generated_header_class(self, f: TextIO, class_info: ClassInfo):
        f.write(f"#define GENERATED_HEADER_{class_info.name} \\\n")
        if class_info.type == "class":
            f.write(f"public: \\\n")
        if len(class_info.bases) == 0:
            if class_info.type == "class":
                f.write(f"typedef {class_info.name} ThisClass; \\\n")
            else:
                f.write(f"typedef {class_info.name} ThisStruct; \\\n")
        else:
            if class_info.type == "class":
                f.write(f"typedef ThisClass Super; \\\n")
                f.write(f"typedef {class_info.name} ThisClass; \\\n")
            else:
                f.write(f"typedef ThisStruct Super; \\\n")
                f.write(f"typedef {class_info.name} ThisStruct; \\\n")
        f.write(f"static FORCE_INLINE const Type* GetStaticType(); \\\n")
        f.write(f"static FORCE_INLINE constexpr bool IsReflected() {{ return true; }} \\\n")
        if class_info.type == "class":
            f.write(f"virtual const Type* GetType(); \\\n")
        else:
            # struct里不能有虚函数
            f.write(f"const Type* GetType(); \\\n")
        if not "CustomSerialization" in class_info.attributes:
            if class_info.type == "class":
                f.write(f"virtual void WriteArchive(OutputArchive& Archive) const;\\\n")
                f.write(f"virtual void ReadArchive(InputArchive& Archive);\\\n")
            else:
                f.write(f"void WriteArchive(OutputArchive& Archive) const;\\\n")
                f.write(f"void ReadArchive(InputArchive& Archive);\\\n")
        f.write(f"struct Z_TypeRegister_{class_info.name} {{ \\\n")
        f.write(f"Z_TypeRegister_{class_info.name}() {{ \\\n")
        f.write("TypeBuilder Builder{}; \\\n")
        f.write(f"Builder.CreateType<{class_info.name}>(\"{self._get_true_name(class_info)}\"); \\\n")
        for base in class_info.bases:
            f.write(f"Builder.AddParent<{base}>(); \\\n")
        for my_property in class_info.properties:
            f.write(f"Builder.AddField(\"{self._get_true_name(my_property)}\", &{class_info.name}::{my_property.name})")
            for attr_name, attr_value in my_property.attributes:
                if not attr_name in "Name":
                    f.write(f".SetAttribute(\"{attr_name}\", \"{attr_value}\")")
            f.write("; \\\n")
        f.write("Builder.Register(); \\\n")
        f.write(f"}} \\\n")
        f.write(f"}}; \\\n")
        f.write(f"static inline Z_TypeRegister_{class_info.name} __Z_TypeRegister_{class_info.name}_Instance; \\\n")
        ############## ObjectReferencingGenerate ################
        has_object_ptr = False
        for my_property in class_info.properties:
            if "ObjectPtr" in my_property.type:
                has_object_ptr = True
                break
        if has_object_ptr:
            f.write("void GetReferencingObject(Array<Int32>& OutArray) \\\n")
            f.write("{ \\\n")
            for my_property in class_info.properties:
                if "ObjectPtr" in my_property.type:
                    f.write(f"ExtractReferencingObjectHandle({my_property.name}, OutArray); \\\n")
            f.write("} \\\n")

        if class_info.type == "class":
            f.write("private: \\\n")
        f.write("\n")

    def _write_generated_header_enum(self, f: TextIO, enum_info: EnumInfo):
        f.write(f"void Z_Reflection_Register_Func_Enum_{enum_info.name}();")
        f.write(
            f"struct Z_Reflection_Register_Enum_{enum_info.name} {{ "
            f"Z_Reflection_Register_Enum_{enum_info.name}() {{"
            f"Z_Reflection_Register_Func_Enum_{enum_info.name}(); "
            f"}}}};\n")
        f.write(
            f"inline Z_Reflection_Register_Enum_{enum_info.name} Z_Reflection_Register_Enum_{enum_info.name}___Instance{{}}; \n")

    def _write_generated_source_enum(self, f: TextIO, enum_info: EnumInfo):
        f.write(f"void Z_Reflection_Register_Func_Enum_{enum_info.name}() {{")
        f.write("TypeBuilder Builder{};")
        f.write(f"Builder.CreateType<{enum_info.name}>(\"{self._get_true_name(enum_info)}\")")
        for attr_name, attr_value in enum_info.attributes:
            if attr_name not in ["Name"]:
                f.write(f".SetTypeAttribute(\"{attr_name}\", \"{attr_value}\")")
        f.write(";\n")
        for enum_value in enum_info.members:
            f.write(f"Builder.AddField(\"{self._get_true_name(enum_value)}\", {enum_info.name}::{enum_value.name})")
            for attr_name, attr_value in enum_value.attributes:
                if not attr_name in "Name":
                    f.write(f".SetAttribute(\"{attr_name}\", \"{attr_value}\")")
            f.write(";\n")
        f.write("Builder.Register();\n")
        f.write("}")

    def _write_generated_header(self, path: str, base_name: str, parsed_items: List[object]):
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            f.write("// Auto-generated header file\n")
            f.write("#pragma once\n\n")
            f.write(f"#include \"Core/Reflection/MetaMark.h\"\n")
            f.write(f"#include \"Core/Reflection/TypeRegistry.h\"\n")
            f.write(f"#include \"Core/Serialization/InputArchive.h\"\n")
            f.write(f"#include \"Core/Serialization/OutputArchive.h\"\n")
            for item in parsed_items:
                if isinstance(item, ClassInfo):
                    self._write_generated_header_class(f, item)
                if isinstance(item, EnumInfo):
                    self._write_generated_header_enum(f, item)

    def _write_generated_cpp(self, path: str, base_name: str, parsed_items: List[object], relative_path: str):
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            f.write("// Auto-generated source file\n")
            f.write(f'#include "{relative_path}"\n\n')
            for item in parsed_items:
                if isinstance(item, ClassInfo):
                    self._write_generated_source_class(f, item)
                if isinstance(item, EnumInfo):
                    self._write_generated_source_enum(f, item)


# ---------------- 运行示例 ----------------
if __name__ == "__main__":
    root = PROJECT_ROOT_PATH
    parser = HeaderParser()
    scanner = CMakeScanner(PROJECT_ROOT_PATH, parser, EXCLUDED_HEADERS)
    projects = scanner.scan_projects()
    generator = CodeGenerator(os.path.join(root, "Generated"))
    generator.generate_projects(projects)
