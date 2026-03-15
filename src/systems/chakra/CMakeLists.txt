set(CHAKRA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/chakra.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_chakra OBJECT chakra.c)
endif()
