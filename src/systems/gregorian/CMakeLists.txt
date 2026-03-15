set(GREGORIAN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/gregorian.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_gregorian OBJECT gregorian.c)
endif()
