set(GREGORIAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gregorian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gregorian_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_gregorian           OBJECT gregorian.c)
    add_library(time_gregorian_interpret OBJECT gregorian_interpret.c)
endif()
