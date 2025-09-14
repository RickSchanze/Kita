#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
PackAtlas.py
将 <目标文件夹>/Images 下所有图片打包成一个 POT 尺寸的纹理图集
并生成 UV 坐标 JSON 文件。
要求：
1. 所有图片尺寸必须一致，否则中断
2. 输出纹理尺寸必须是 2^n
"""

import os
import sys
import math
import json
from PIL import Image


PADDING = 2        # 图标之间的间隔像素
ATLAS_COLUMNS = 8  # 默认列数上限（可根据需求修改）

def next_power_of_two(x: int) -> int:
    """计算大于等于 x 的最小 2 次幂"""
    return 1 if x == 0 else 2 ** math.ceil(math.log2(x))


def pack_atlas(folder_path: str):
    folder_name = os.path.basename(os.path.normpath(folder_path))
    images_dir = os.path.join(folder_path, "Images")

    if not os.path.exists(images_dir):
        print(f"❌ 未找到目录: {images_dir}")
        return False

    # 获取全部图片
    files = [f for f in os.listdir(images_dir)
             if f.lower().endswith((".png", ".jpg", ".jpeg"))]
    if not files:
        print(f"⚠️ {images_dir} 下没有找到图片")
        return False

    # 读取所有图片并验证尺寸一致
    icons = []
    first_size = None
    for filename in sorted(files):
        img_path = os.path.join(images_dir, filename)
        img = Image.open(img_path).convert("RGBA")

        if first_size is None:
            first_size = img.size
        elif img.size != first_size:
            print(f"❌ 图像尺寸不一致: {filename} 尺寸为 {img.size}，而第一个图像尺寸为 {first_size}")
            return False

        icons.append((filename, img))

    icon_w, icon_h = first_size

    # 自动计算行列
    cols = min(ATLAS_COLUMNS, len(icons))
    rows = math.ceil(len(icons) / cols)

    real_atlas_w = cols * (icon_w + PADDING) - PADDING
    real_atlas_h = rows * (icon_h + PADDING) - PADDING

    # 🆕 输出尺寸强制为 POT
    atlas_w = next_power_of_two(real_atlas_w)
    atlas_h = next_power_of_two(real_atlas_h)

    print(f"📦 目标图集实际内容大小: {real_atlas_w}x{real_atlas_h}")
    print(f"📐 输出 POT 尺寸: {atlas_w}x{atlas_h}")

    # 创建空白图集（透明背景）
    atlas = Image.new("RGBA", (atlas_w, atlas_h), (0, 0, 0, 0))
    uv_data = {}

    for idx, (filename, icon) in enumerate(icons):
        col = idx % cols
        row = idx // cols
        x = col * (icon_w + PADDING)
        y = row * (icon_h + PADDING)

        atlas.paste(icon, (x, y), mask=icon)

        # UV 使用标准化 POT 尺寸
        u0 = x / atlas_w
        v0 = y / atlas_h
        u1 = (x + icon_w) / atlas_w
        v1 = (y + icon_h) / atlas_h

        uv_data[filename] = {
            "u0": round(u0, 6),
            "v0": round(v0, 6),
            "u1": round(u1, 6),
            "v1": round(v1, 6)
        }

    atlas_path = os.path.join(folder_path, f"{folder_name}Atlas.png")
    uv_path = os.path.join(folder_path, f"{folder_name}Atlas_UV.json")

    atlas.save(atlas_path)
    with open(uv_path, "w", encoding="utf-8") as f:
        json.dump(uv_data, f, indent=4, ensure_ascii=False)

    print(f"✅ 图集已生成: {atlas_path}")
    print(f"✅ UV 数据已生成: {uv_path}")
    return True


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python PackAtlas.py <目标文件夹>")
        print("示例: python PackAtlas.py EditorIcon")
        sys.exit(1)

    success = pack_atlas(sys.argv[1])
    if not success:
        sys.exit(1)