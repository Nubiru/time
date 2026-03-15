set(AZTEC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/aztec.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_aztec OBJECT aztec.c)
endif()
