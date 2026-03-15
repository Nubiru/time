set(PERSIAN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/persian.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_persian OBJECT persian.c)
endif()
