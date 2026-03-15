set(JAPANESE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/japanese.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_japanese OBJECT japanese.c)
endif()
