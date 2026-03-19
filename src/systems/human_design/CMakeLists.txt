set(HUMAN_DESIGN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/human_design.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bodygraph.c
    ${CMAKE_CURRENT_SOURCE_DIR}/human_design_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_human_design OBJECT human_design.c)
    add_library(time_bodygraph    OBJECT bodygraph.c)
    add_library(time_human_design_interpret OBJECT human_design_interpret.c)
endif()
