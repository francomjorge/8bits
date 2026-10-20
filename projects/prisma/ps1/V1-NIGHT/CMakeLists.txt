# PRISMA-PI / PlayStation 1 - V1-NIGHT
# Based directly on PSn00bSDK examples/graphics/rgb24/CMakeLists.txt
cmake_minimum_required(VERSION 3.21)

project(
    prisma_pi_v1_night
    LANGUAGES C ASM
    VERSION 1.0.0
    DESCRIPTION "PRISMA-PI PS1 V1 NIGHT DRIVE"
)

psn00bsdk_add_executable(v1night GPREL main.c)

psn00bsdk_add_cd_image(
    v1night_iso v1night iso.xml
    DEPENDS v1night system.cnf
)

# Same mechanism used by the official rgb24 example.
psn00bsdk_target_incbin(v1night PRIVATE tim_image night.tim)

install(
    FILES
        ${PROJECT_BINARY_DIR}/v1night.exe
        ${PROJECT_BINARY_DIR}/v1night.bin
        ${PROJECT_BINARY_DIR}/v1night.cue
    TYPE BIN
)
