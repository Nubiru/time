set(NUMEROLOGY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/numerology.c
    ${CMAKE_CURRENT_SOURCE_DIR}/numerology_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_numerology           OBJECT numerology.c)
    add_library(time_numerology_interpret OBJECT numerology_interpret.c)
endif()
