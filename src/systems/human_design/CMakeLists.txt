set(HUMAN_DESIGN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/human_design.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bodygraph.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_human_design OBJECT human_design.c)
    add_library(time_bodygraph    OBJECT bodygraph.c)
endif()
