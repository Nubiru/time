set(BAHAI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bahai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bahai_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_bahai           OBJECT bahai.c)
    add_library(time_bahai_interpret OBJECT bahai_interpret.c)
endif()
