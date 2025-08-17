import os
import re
from dataclasses import dataclass, field
from typing import List, Dict, Optional

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
class ClassInfo:
    type: str
    name: str
    attributes: Dict[str, str]
    properties: List[PropertyInfo] = field(default_factory=list)
    functions: List[FunctionInfo] = field(default_factory=list)


@dataclass
class CMakeProjectInfo:
    project_path: str
    project_name: str
    files: Dict[str, str] = field(default_factory=dict)  # abs_path -> relative_path
    parsed_files: Dict[str, List[ClassInfo]] = field(default_factory=dict)  # abs_path -> classes


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

    def parse(self, file_path: str) -> List[ClassInfo]:
        try:
            with open(file_path, "r", encoding="utf-8") as f:
                code = f.read()
        except Exception as e:
            print(f"[WARN] 无法读取 {file_path}: {e}")
            return []

        code = self._remove_comments(code)
        tokens = self._tokenize(code)
        idx = 0
        result: List[ClassInfo] = []

        while idx < len(tokens):
            tok = tokens[idx]
            if tok == "KCLASS":
                attrs, idx = self._parse_attributes(tokens, idx + 1)
                assert tokens[idx] == "class"
                class_name = tokens[idx + 1]
                idx += 2
                assert tokens[idx] == "{"
                idx += 1
                class_info = ClassInfo(
                    type="class",
                    name=class_name,
                    attributes=attrs
                )
                while idx < len(tokens) and tokens[idx] != "}":
                    if tokens[idx] == "KPROPERTY":
                        prop_attrs, idx = self._parse_attributes(tokens, idx + 1)
                        type_parts = []
                        while idx < len(tokens) and tokens[idx + 1] not in ["=", ";", "{", "("]:
                            type_parts.append(tokens[idx])
                            idx += 1
                        prop_type = " ".join(type_parts)
                        prop_name = tokens[idx]
                        idx += 1
                        default_val = None
                        if tokens[idx] in ("=", "{"):
                            default_val, idx = self._parse_default_value(tokens, idx)
                        assert tokens[idx] == ";"
                        idx += 1
                        class_info.properties.append(PropertyInfo(
                            name=prop_name,
                            type=prop_type,
                            attributes=prop_attrs,
                            default=default_val
                        ))
                    elif tokens[idx] == "KFUNCTION":
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
                result.append(class_info)
                idx += 1
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
        self._file_cache: Dict[str, float] = {}  # abs_path -> last modified time

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
        parsed_map: Dict[str, List[ClassInfo]] = {}

        for header in header_files:
            abs_path = os.path.join(project_dir, header).replace("\\", "/")
            rel_global = os.path.relpath(abs_path, self.root_path).replace("\\", "/")

            # 跳过排除列表文件
            if rel_global in self.excluded_headers:
                continue

            if os.path.isfile(abs_path):
                rel_path = os.path.relpath(abs_path, project_dir).replace("\\", "/")
                files_map[abs_path] = rel_path

                # 增量扫描：仅当文件新增或修改时解析
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

    def _extract_header_files(self, cmake_content: str) -> List[str]:
        headers = []
        pattern = re.compile(r"[^\s'\"]+\.(?:h|hpp|hh|hxx)", re.IGNORECASE)
        for match in pattern.findall(cmake_content):
            headers.append(match.strip())
        return headers


# ---------------- 运行示例 ----------------
if __name__ == "__main__":
    root = r"C:\Users\kita\Documents\Projects\Kita\Engine"
    parser = HeaderParser()
    scanner = CMakeScanner(PROJECT_ROOT_PATH, parser, EXCLUDED_HEADERS)
    projects = scanner.scan_projects()
