# PRISMA SOUND BANK V2

Single-module NES prototype with a preloaded 32-patch APU bank.

## Controls
- LEFT / RIGHT: previous / next patch
- B: jump +8 patches
- UP / DOWN: AMOUNT 0..7
- A: PLAY ON/OFF
- START: reset

## Sound bank
32 data-driven presets using the NES APU: Pulse 1, Pulse 2, Triangle and Noise, including steady tones, vibrato, major/minor arpeggios, dual-voice intervals, siren and glitch behaviors.

## Visual asset
The moving bird uses exact `SeagullIdleAnim_0..3.png` frames from the CC0 Nat's 8x8 Starter Pack already imported in this repository. Only technical conversion to NES sprite format is applied.

## Current ROM build
- Mapper 0 / NROM-128
- 16 KiB PRG + 8 KiB CHR
- ROM size: 24592 bytes
- SHA256: `d71052ff89fdc006dacb9b8fa2e8b0d6177334260680d7fa0189b68525a0c0a6`

The playable `.nes` build is generated as `PRISMA_SOUND_BANK_V2.nes` during development; the reusable patch database lives in `PRISMA_APU_BANK_32.json`.
