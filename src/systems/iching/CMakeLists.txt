set(ICHING_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/iching.c
    ${CMAKE_CURRENT_SOURCE_DIR}/iching_interpret.c
    PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_iching OBJECT iching.c)
    add_library(time_iching_interpret OBJECT iching_interpret.c)
endif()
