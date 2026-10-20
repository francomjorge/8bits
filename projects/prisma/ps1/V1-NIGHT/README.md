# PRISMA-PI PS1 — V1-NIGHT

First PlayStation 1 branch of PRISMA-PI.

## Development rule

No custom console bootstrap and no hand-written GPU bring-up. We start from working PSn00bSDK examples and change one subsystem at a time.

## V1-NIGHT

This version is based directly on two existing PSn00bSDK examples:

- `examples/graphics/rgb24` — GPU/display initialization plus `GetTimInfo()` / `LoadImage()`
- `examples/cdrom/cdbrowse` — bootable CD layout (`SYSTEM.CNF`, `iso.xml`, `psn00bsdk_add_cd_image`)

V1 does one thing only: boot and show NIGHT DRIVE at 320×240.

The committed `night.png` is converted during the build to standard Sony 24-bit TIM format, then embedded with the same `psn00bsdk_target_incbin()` mechanism used by the RGB24 example.

## Files

- `main.c` — adapted from PSn00bSDK `graphics/rgb24`
- `night.png` — current NIGHT DRIVE image, 320×240
- `png2tim24.py` — deterministic asset packaging step
- `CMakeLists.txt`
- `SYSTEM.CNF` source: `system.cnf`
- `iso.xml`

## Incremental plan

- `V1-NIGHT` — static image and bootable `.bin/.cue`
- `V2-PAD` — controller input copied from `examples/io/pads`
- `V3-UI` — drawable 15-bit/CLUT scene assets and interactive UI
- `V4-AUDIO` — VAG/SPU playback copied from `examples/sound/vagsample`
- `V5-BANK` — PRISMA-PI patch/sample bank

The CI artifact names stay short: `V1-NIGHT.bin`, `V1-NIGHT.cue`, `V1-NIGHT.exe`.
