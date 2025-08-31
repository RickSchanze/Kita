from pathlib import Path

input_dir = "C:/Users/kita/Documents/Projects/Kita/Engine"
out_file = r"C:\Users\kita\Documents\Projects\Kita\Content\Assets\Font\Charset.txt"

from pathlib import Path


def collect_unique_chars(root_dir):
    root_path = Path(root_dir)
    unique_chars = set()

    for file_path in root_path.rglob("*"):
        if file_path.is_file() and (
                file_path.suffix in {".cpp", ".h"} or file_path.name == "CMakeLists.txt"
        ):
            try:
                text = file_path.read_text(encoding="utf-8", errors="ignore")
                unique_chars.update(text)
            except Exception as e:
                print(f"读取文件失败: {file_path}，错误: {e}")

    return unique_chars


if __name__ == "__main__":
    chars = collect_unique_chars(input_dir)
    with open(out_file, "w", encoding="utf-8") as f:
        for char in chars:
            f.write(char)
