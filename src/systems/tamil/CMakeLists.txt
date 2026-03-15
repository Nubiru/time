set(TAMIL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tamil_calendar.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_tamil_calendar OBJECT tamil_calendar.c)
endif()
