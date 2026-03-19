set(TAMIL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tamil_calendar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tamil_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_tamil_calendar  OBJECT tamil_calendar.c)
    add_library(time_tamil_interpret OBJECT tamil_interpret.c)
endif()
