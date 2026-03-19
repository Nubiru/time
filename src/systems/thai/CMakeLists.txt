set(THAI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/thai_calendar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thai_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_thai_calendar  OBJECT thai_calendar.c)
    add_library(time_thai_interpret OBJECT thai_interpret.c)
endif()
