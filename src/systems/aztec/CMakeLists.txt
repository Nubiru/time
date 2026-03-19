set(AZTEC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/aztec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aztec_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_aztec           OBJECT aztec.c)
    add_library(time_aztec_interpret OBJECT aztec_interpret.c)
endif()
