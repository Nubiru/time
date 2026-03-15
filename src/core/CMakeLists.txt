# src/core/CMakeLists.txt — Core application modules

set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_state.c
    ${CMAKE_CURRENT_SOURCE_DIR}/date_parse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hud.c
    PARENT_SCOPE
)

if(NOT EMSCRIPTEN)
    add_library(time_date_parse OBJECT date_parse.c)
endif()
