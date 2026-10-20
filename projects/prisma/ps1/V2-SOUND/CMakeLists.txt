# V2-SOUND / PRISMA-PI PS1
# Audio playback path copied from the official PSn00bSDK vagsample pattern.
cmake_minimum_required(VERSION 3.21)

project(
    v2sound
    LANGUAGES C ASM
    VERSION 1.0.0
    DESCRIPTION "PRISMA-PI PS1 V2 SOUND"
)

psn00bsdk_add_executable(v2sound GPREL main.c)

# night.vag is downloaded from the official PSn00bSDK v0.24 vagsample example
# by the CI workflow before configure/build.
psn00bsdk_target_incbin(v2sound PRIVATE night_sample night.vag)

psn00bsdk_add_cd_image(
    v2sound_iso v2sound iso.xml
    DEPENDS v2sound system.cnf
)

install(
    FILES
        ${PROJECT_BINARY_DIR}/v2sound.exe
        ${PROJECT_BINARY_DIR}/v2sound.bin
        ${PROJECT_BINARY_DIR}/v2sound.cue
    TYPE BIN
)
