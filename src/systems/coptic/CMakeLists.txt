set(COPTIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/coptic.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_coptic OBJECT coptic.c)
endif()
