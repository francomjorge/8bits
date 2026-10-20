# PRISMA-PI PS1 V1-BOOT
# Based on PSn00bSDK examples/beginner/hello/CMakeLists.txt
cmake_minimum_required(VERSION 3.21)

project(
    v1boot
    LANGUAGES C
    VERSION 1.0.0
    DESCRIPTION "PRISMA-PI PS1 V1-BOOT"
)

psn00bsdk_add_executable(v1boot GPREL main.c)
psn00bsdk_add_cd_image(v1boot_iso v1boot iso.xml DEPENDS v1boot system.cnf)

install(FILES ${PROJECT_BINARY_DIR}/v1boot.exe TYPE BIN)
