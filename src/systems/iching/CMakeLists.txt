set(ICHING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/iching.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_iching OBJECT iching.c)
endif()
