set(EGYPTIAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/egyptian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/egyptian_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_egyptian OBJECT egyptian.c)
    add_library(time_egyptian_interpret OBJECT egyptian_interpret.c)
endif()
