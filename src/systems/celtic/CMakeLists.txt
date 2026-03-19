set(CELTIC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/wheel_of_year.c
    ${CMAKE_CURRENT_SOURCE_DIR}/celtic_tree.c
    ${CMAKE_CURRENT_SOURCE_DIR}/celtic_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_wheel_of_year  OBJECT wheel_of_year.c)
    add_library(time_celtic_tree    OBJECT celtic_tree.c)
    add_library(time_celtic_interpret OBJECT celtic_interpret.c)
endif()
