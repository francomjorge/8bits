# PRISMA-PI PS1 — V2-SOUND

Second PlayStation 1 prototype for PRISMA-PI.

## Purpose

Prove the complete controller -> parameter -> SPU playback path before adding the full NIGHT DRIVE image and patch bank.

## Existing code reused

The audio path is adapted directly from PSn00bSDK's official `examples/sound/vagsample` example:

- `SpuInit()`
- VAG header parsing
- DMA upload to SPU RAM
- SPU channel address/frequency/volume setup
- controller polling
- key on / key off

The build downloads the `proyt.vag` sample from the official PSn00bSDK v0.24 repository and embeds it as `night_sample` using the same `psn00bsdk_target_incbin()` mechanism as the example.

## Screen

The screen intentionally stays simple in this version and shows the NIGHT DRIVE patch values:

- DRIVE 00
- VOLUME 12
- ATTACK 05
- DECAY 10
- SUSTAIN 13
- RELEASE 08
- RATE 06
- TONE 09

Only two parameters are live in V2:

- `UP/DOWN` — VOLUME 0..15
- `LEFT/RIGHT` — TONE/pitch 0..15

Controls:

- `X` — play / trigger the sample
- `O` — stop the voice

V2 uses one monophonic SPU voice, while the PlayStation SPU provides 24 hardware voices. Proper NIGHT DRIVE waveform assets and ADSR mapping are intentionally left for the next audio step.
