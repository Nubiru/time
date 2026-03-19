set(JAPANESE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/japanese.c
    ${CMAKE_CURRENT_SOURCE_DIR}/japanese_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_japanese           OBJECT japanese.c)
    add_library(time_japanese_interpret OBJECT japanese_interpret.c)
endif()
