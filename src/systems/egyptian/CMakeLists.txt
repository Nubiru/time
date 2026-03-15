set(EGYPTIAN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/egyptian.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_egyptian OBJECT egyptian.c)
endif()
