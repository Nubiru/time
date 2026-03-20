set(ISLAMIC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hijri.c
    ${CMAKE_CURRENT_SOURCE_DIR}/prayer_times.c
    ${CMAKE_CURRENT_SOURCE_DIR}/islamic_interpret.c
    ${CMAKE_CURRENT_SOURCE_DIR}/islamic_today.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_hijri              OBJECT hijri.c)
    add_library(time_prayer_times       OBJECT prayer_times.c)
    add_library(time_islamic_interpret  OBJECT islamic_interpret.c)
    add_library(time_islamic_today      OBJECT islamic_today.c)
endif()
