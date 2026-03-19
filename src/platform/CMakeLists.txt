# src/platform/CMakeLists.txt — Platform abstraction modules

set(PLATFORM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/storage_bridge_native.c
    PARENT_SCOPE
)

if(NOT EMSCRIPTEN)
    add_library(time_storage_bridge OBJECT storage_bridge_native.c)
endif()
