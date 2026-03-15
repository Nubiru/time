set(BALINESE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/pawukon.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_pawukon OBJECT pawukon.c)
endif()
