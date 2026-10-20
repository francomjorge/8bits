#!/usr/bin/env python3
"""
Minimal 24-bit TIM writer for PRISMA-PI V1-NIGHT.

The runtime/display code is not custom: it comes from PSn00bSDK's official
rgb24 example. This tool only packages a 320x240 PNG into the standard Sony TIM
container expected by GetTimInfo()/LoadImage().

TIM reference:
- magic: 0x00000010
- flags: 0x00000003 (24-bit direct color)
- image width field: 16-bit VRAM words, so RGB888 width * 3 / 2
"""
from pathlib import Path
import struct
import sys
from PIL import Image


def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: png2tim24.py INPUT.png OUTPUT.tim")

    src = Path(sys.argv[1])
    dst = Path(sys.argv[2])
    image = Image.open(src).convert("RGB")

    if image.size != (320, 240):
        raise SystemExit(
            f"expected 320x240 image, got {image.size[0]}x{image.size[1]}"
        )

    pixels = image.tobytes()
    row_bytes = image.width * 3
    if row_bytes & 1:
        raise SystemExit("24-bit TIM row size must be even for this converter")

    vram_width_words = row_bytes // 2
    block_size = 12 + len(pixels)

    with dst.open("wb") as fp:
        fp.write(struct.pack("<I", 0x10))
        fp.write(struct.pack("<I", 0x03))
        fp.write(struct.pack("<I", block_size))
        fp.write(struct.pack("<HHHH", 0, 0, vram_width_words, image.height))
        fp.write(pixels)


if __name__ == "__main__":
    main()
