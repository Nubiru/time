set(LAO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lao_calendar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lao_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_lao_calendar  OBJECT lao_calendar.c)
    add_library(time_lao_interpret OBJECT lao_interpret.c)
endif()
