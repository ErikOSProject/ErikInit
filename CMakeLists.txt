cmake_minimum_required(VERSION 3.16)
project(init VERSION 0.1)
enable_language(C ASM)

# You can pass -DTARGET_ARCH=<arch> to select an architecture
if(NOT DEFINED TARGET_ARCH)
    set(TARGET_ARCH "x86_64") # Default architecture
endif()

include(${TARGET_ARCH}.cmake OPTIONAL)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Generic")
message(FATAL_ERROR "Please specify a compatible toolchain file. 
For example: \"cmake -DCMAKE_TOOLCHAIN_FILE=../x64.cmake ..\"")
endif()

add_executable(init
    src/init.c
)

target_compile_options(init PRIVATE 
    -Wall -Wextra -pedantic -Werror
    -fshort-wchar
    -mno-red-zone
    -Wno-unused-variable
    -Wno-unused-command-line-argument
    -Wno-c11-extensions
    -Wno-c99-extensions
    -Wno-c2x-extensions
    -Wno-language-extension-token
    -Wno-writable-strings)

target_link_options(init PRIVATE
    -nostdlib)

target_link_libraries(init PRIVATE c erikbus)
