set(THAI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/thai_calendar.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_thai_calendar OBJECT thai_calendar.c)
endif()
