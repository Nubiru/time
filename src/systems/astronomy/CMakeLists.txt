set(ASTRONOMY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/orbit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/planets.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lunar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/precession.c
    ${CMAKE_CURRENT_SOURCE_DIR}/solar_events.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cosmic_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/planet_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/moon_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/moon_nodes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/retrograde.c
    ${CMAKE_CURRENT_SOURCE_DIR}/astronomy_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_orbit                OBJECT orbit.c)
    add_library(time_planets              OBJECT planets.c)
    add_library(time_lunar                OBJECT lunar.c)
    add_library(time_precession           OBJECT precession.c)
    add_library(time_solar_events         OBJECT solar_events.c)
    add_library(time_cosmic_time          OBJECT cosmic_time.c)
    add_library(time_planet_data          OBJECT planet_data.c)
    add_library(time_moon_data            OBJECT moon_data.c)
    add_library(time_moon_nodes           OBJECT moon_nodes.c)
    add_library(time_retrograde           OBJECT retrograde.c)
    add_library(time_astronomy_interpret  OBJECT astronomy_interpret.c)
endif()
