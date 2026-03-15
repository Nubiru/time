set(CHINESE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/chinese.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_chinese OBJECT chinese.c)
endif()
