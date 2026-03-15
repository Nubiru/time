set(ETHIOPIAN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ethiopian.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_ethiopian OBJECT ethiopian.c)
endif()
