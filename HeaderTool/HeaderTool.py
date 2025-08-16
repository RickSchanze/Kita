import json
from dataclasses import dataclass, field
from typing import Optional, List, Dict


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
    type: str  # 固定为 "class"
    name: str
    attributes: Dict[str, str]
    properties: List[PropertyInfo] = field(default_factory=list)
    functions: List[FunctionInfo] = field(default_factory=list)


# ---------------- 解析器 ----------------
class HeaderParser:
    def _remove_comments(self, code: str) -> str:
        result = []
        i = 0
        in_block_comment = False
        in_line_comment = False

        while i < len(code):
            if in_block_comment:
                if code[i:i+2] == "*/":
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
                if code[i:i+2] == "/*":
                    in_block_comment = True
                    i += 2
                elif code[i:i+2] == "//":
                    in_line_comment = True
                    i += 2
                else:
                    result.append(code[i])
                    i += 1

        return "".join(result)

    def _tokenize(self, code: str) -> List[str]:
        tokens = []
        i = 0
        while i < len(code):
            c = code[i]

            # 跳过空白
            if c.isspace():
                i += 1
                continue

            # 标识符 / 关键字 / 类型
            if c.isalpha() or c == '_' or c.isdigit():
                start = i
                while (
                        i < len(code)
                        and (code[i].isalnum() or code[i] in ['_', ':', '<', '>', '*', '&'])
                ):
                    i += 1
                tokens.append(code[start:i])
                continue

            # 字符串常量
            if c == '"':
                start = i
                i += 1
                while i < len(code) and code[i] != '"':
                    if code[i] == '\\':
                        i += 1  # 跳过转义字符
                    i += 1
                i += 1
                tokens.append(code[start:i])
                continue

            # 单字符符号（, ; ( ) { } = 等）
            tokens.append(c)
            i += 1

        return tokens

    def _parse_attributes(self, tokens: List[str], idx: int) -> (Dict[str, str], int):
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

    def _parse_default_value(self, tokens: List[str], idx: int) -> (Optional[str], int):
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

    def _parse_function_args(self, tokens: List[str], idx: int) -> (List[ArgumentInfo], int):
        args: List[ArgumentInfo] = []
        while tokens[idx] != ")":
            type_parts = []
            arg_name = ""

            while idx < len(tokens) and tokens[idx] not in [",", ")"]:
                # 如果下一个 token 是逗号、右括号或等号，则当前是参数名
                if (tokens[idx+1] in [",", ")"]) or (tokens[idx+1] == "="):
                    arg_name = tokens[idx]
                    idx += 1
                    break
                else:
                    type_parts.append(tokens[idx])
                    idx += 1

            arg_type = " ".join(type_parts)
            args.append(ArgumentInfo(type=arg_type, name=arg_name))

            if tokens[idx] == ",":
                idx += 1  # 跳过逗号

        idx += 1  # 跳过右括号
        return args, idx

    def parse(self, file_path: str) -> List[ClassInfo]:
        with open(file_path, "r", encoding="utf-8") as f:
            code = f.read()

        code = self._remove_comments(code)
        tokens = self._tokenize(code)

        idx = 0
        result: List[ClassInfo] = []

        while idx < len(tokens):
            tok = tokens[idx]

            if tok in ("KCLASS", "CLASS"):
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
                    if tokens[idx] in ("KPROPERTY", "PROPERTY"):
                        prop_attrs, idx = self._parse_attributes(tokens, idx + 1)

                        type_parts = []
                        while idx < len(tokens) and tokens[idx+1] not in ["=", ";", "{", "("]:
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

                    elif tokens[idx] in ("KFUNCTION", "FUNCTION"):
                        func_attrs, idx = self._parse_attributes(tokens, idx + 1)

                        ret_type_parts = []
                        while idx < len(tokens) and tokens[idx+1] != "(":
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


# ---------------- 调试入口 ----------------
if __name__ == "__main__":
    parser = HeaderParser()
    parsed: List[ClassInfo] = parser.parse(r"C:\Users\kita\Documents\Projects\Kita\Engine\main.cpp")
    print(json.dumps([c.__dict__ for c in parsed], indent=2, ensure_ascii=False, default=lambda o: o.__dict__))