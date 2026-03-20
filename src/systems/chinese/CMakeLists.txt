set(CHINESE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/chinese.c
    ${CMAKE_CURRENT_SOURCE_DIR}/chinese_interpret.c
    ${CMAKE_CURRENT_SOURCE_DIR}/chinese_today.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_chinese OBJECT chinese.c)
    add_library(time_chinese_interpret OBJECT chinese_interpret.c)
    add_library(time_chinese_today    OBJECT chinese_today.c)
endif()
