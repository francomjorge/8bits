# V1-NIGHT / PRISMA-PI PS1
# Based on PSn00bSDK beginner/hello + cdrom/cdbrowse build patterns.
cmake_minimum_required(VERSION 3.21)

project(
    v1night
    LANGUAGES C ASM
    VERSION 1.0.0
    DESCRIPTION "PRISMA-PI PS1 V1 NIGHT"
)

psn00bsdk_add_executable(v1night GPREL main.c)

psn00bsdk_add_cd_image(
    v1night_iso v1night iso.xml
    DEPENDS v1night system.cnf
)

install(
    FILES
        ${PROJECT_BINARY_DIR}/v1night.exe
        ${PROJECT_BINARY_DIR}/v1night.bin
        ${PROJECT_BINARY_DIR}/v1night.cue
    TYPE BIN
)
