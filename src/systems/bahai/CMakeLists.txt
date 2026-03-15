set(BAHAI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/bahai.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_bahai OBJECT bahai.c)
endif()
