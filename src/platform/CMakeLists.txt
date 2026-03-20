# src/platform/CMakeLists.txt — Platform abstraction modules

if(EMSCRIPTEN)
    set(PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/storage_bridge_wasm.c
        PARENT_SCOPE
    )
else()
    set(PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/storage_bridge_native.c
        PARENT_SCOPE
    )
    add_library(time_storage_bridge OBJECT storage_bridge_native.c)
endif()
