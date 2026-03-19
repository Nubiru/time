set(KOREAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/korean_calendar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/korean_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_korean_calendar  OBJECT korean_calendar.c)
    add_library(time_korean_interpret OBJECT korean_interpret.c)
endif()
