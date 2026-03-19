set(PERSIAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/persian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/persian_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_persian           OBJECT persian.c)
    add_library(time_persian_interpret OBJECT persian_interpret.c)
endif()
