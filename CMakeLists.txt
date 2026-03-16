# ===========================================================================
# CMakeLists.txt — Time Project
#
# Visual, interactive, three-dimensional digital artwork displaying time
# through multiple cultural and astronomical systems.
#
# Two build modes:
#   Native (gcc):  cmake -B build && cmake --build build && ctest --test-dir build
#   WASM (emcc):   emcmake cmake -B build-wasm && cmake --build build-wasm
# ===========================================================================
cmake_minimum_required(VERSION 3.16)
project(Time VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

# Emscripten needs GNU extensions (EM_ASM requires -std=gnu11)
# Native build uses strict C11 + pedantic
if(EMSCRIPTEN)
    set(CMAKE_C_EXTENSIONS ON)
else()
    set(CMAKE_C_EXTENSIONS OFF)
    add_compile_options(-pedantic -Wmissing-prototypes -Wmissing-declarations)
endif()

# ===========================================================================
# Source tree — per-directory CMakeLists define sources + OBJECT libraries
# ===========================================================================
add_subdirectory(src)

# ===========================================================================
# Aggregate all sources for WASM build
# ===========================================================================
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${MATH_SOURCES}
    ${RENDER_SOURCES}
    ${SYSTEMS_SOURCES}
    ${UI_SOURCES}
)

# ===========================================================================
# WASM BUILD (emcmake cmake -B build-wasm)
# ===========================================================================
if(EMSCRIPTEN)
    add_executable(time_app ${ALL_SOURCES})
    set_target_properties(time_app PROPERTIES
        OUTPUT_NAME "index"
        SUFFIX ".html"
    )
    target_link_options(time_app PRIVATE
        -sUSE_WEBGL2=1
        -sFULL_ES3=1
        -sWASM=1
        -sSTACK_SIZE=524288
        -sALLOW_MEMORY_GROWTH=1
        "-sEXPORTED_RUNTIME_METHODS=['stringToUTF8']"
        -Os
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/web/index.html"
    )
    # Copy style.css to build dir
    add_custom_command(TARGET time_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/web/style.css
            ${CMAKE_BINARY_DIR}/style.css
    )

# ===========================================================================
# NATIVE BUILD — tests (cmake -B build)
# ===========================================================================
else()
    include(CTest)
    include(cmake/testing.cmake)
endif()
