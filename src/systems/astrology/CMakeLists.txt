set(ASTROLOGY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/zodiac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aspects.c
    ${CMAKE_CURRENT_SOURCE_DIR}/observer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/houses.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dignity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/planetary_hours.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wheel_layout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/astrology_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_zodiac          OBJECT zodiac.c)
    add_library(time_aspects         OBJECT aspects.c)
    add_library(time_observer        OBJECT observer.c)
    add_library(time_houses          OBJECT houses.c)
    add_library(time_dignity         OBJECT dignity.c)
    add_library(time_planetary_hours OBJECT planetary_hours.c)
    add_library(time_astro_wheel     OBJECT wheel_layout.c)
    add_library(time_astrology_interpret OBJECT astrology_interpret.c)
endif()
