set(MYANMAR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/myanmar.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_myanmar OBJECT myanmar.c)
endif()
