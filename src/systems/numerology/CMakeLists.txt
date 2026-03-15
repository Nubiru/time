set(NUMEROLOGY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/numerology.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_numerology OBJECT numerology.c)
endif()
