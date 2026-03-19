set(CHAKRA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/chakra.c
    ${CMAKE_CURRENT_SOURCE_DIR}/chakra_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_chakra           OBJECT chakra.c)
    add_library(time_chakra_interpret OBJECT chakra_interpret.c)
endif()
