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

# Coverage build: cmake -B build-coverage -DCMAKE_BUILD_TYPE=Coverage
if(NOT EMSCRIPTEN AND CMAKE_BUILD_TYPE STREQUAL "Coverage")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

# Sanitizer build: cmake -B build-sanitize -DCMAKE_BUILD_TYPE=Sanitize
if(NOT EMSCRIPTEN AND CMAKE_BUILD_TYPE STREQUAL "Sanitize")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

# Benchmark build: optimized with no debug overhead
if(NOT EMSCRIPTEN AND CMAKE_BUILD_TYPE STREQUAL "Benchmark")
    add_compile_options(-O2 -DNDEBUG)
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
    # Environment configuration: -DPRODUCTION=ON or -DSTAGING=ON
    option(PRODUCTION "Production build — optimized, no debug" OFF)
    option(STAGING "Staging build — optimized with debug info" OFF)

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
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/web/index.html"
    )

    # Per-environment optimization and defines
    if(PRODUCTION)
        target_compile_definitions(time_app PRIVATE PRODUCTION=1)
        target_link_options(time_app PRIVATE -O3 --closure 1 -flto -sASSERTIONS=0)
    elseif(STAGING)
        target_compile_definitions(time_app PRIVATE STAGING=1)
        target_link_options(time_app PRIVATE -O2 -g2 -sASSERTIONS=1)
    else()
        # Development: debug info, assertions, source maps, moderate optimization
        target_compile_definitions(time_app PRIVATE DEVELOPMENT=1)
        target_compile_options(time_app PRIVATE -g)
        target_link_options(time_app PRIVATE -Os -sASSERTIONS=2 -gsource-map)
    endif()

    # Copy style.css to build dir
    add_custom_command(TARGET time_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/web/style.css
            ${CMAKE_BINARY_DIR}/style.css
    )

    # Copy service-worker.js to build dir (offline PWA support)
    add_custom_command(TARGET time_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/web/service-worker.js
            ${CMAKE_BINARY_DIR}/service-worker.js
    )

# ===========================================================================
# NATIVE BUILD — tests (cmake -B build)
# ===========================================================================
else()
    include(CTest)
    include(cmake/testing.cmake)
endif()
