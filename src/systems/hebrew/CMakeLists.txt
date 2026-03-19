set(HEBREW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hebrew.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sabbatical.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hebrew_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_hebrew     OBJECT hebrew.c)
    add_library(time_sabbatical OBJECT sabbatical.c)
    add_library(time_hebrew_interpret OBJECT hebrew_interpret.c)
endif()
