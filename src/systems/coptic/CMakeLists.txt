set(COPTIC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/coptic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/coptic_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_coptic           OBJECT coptic.c)
    add_library(time_coptic_interpret OBJECT coptic_interpret.c)
endif()
