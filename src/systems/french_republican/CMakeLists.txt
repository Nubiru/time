set(FRENCH_REPUBLICAN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/french_republican.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_french_republican OBJECT french_republican.c)
endif()
