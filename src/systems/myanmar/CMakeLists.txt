set(MYANMAR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/myanmar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/myanmar_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_myanmar           OBJECT myanmar.c)
    add_library(time_myanmar_interpret OBJECT myanmar_interpret.c)
endif()
