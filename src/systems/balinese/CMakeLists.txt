set(BALINESE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pawukon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pawukon_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_pawukon           OBJECT pawukon.c)
    add_library(time_pawukon_interpret OBJECT pawukon_interpret.c)
endif()
