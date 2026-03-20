set(GEOLOGY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/geo_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/climate_history.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fossil_milestones.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tectonic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/radiometric.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thermo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/geology_interpret.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_voyage.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fossil_narrative.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_story.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_geo_time          OBJECT geo_time.c)
    add_library(time_climate_history   OBJECT climate_history.c)
    add_library(time_fossil_milestones OBJECT fossil_milestones.c)
    add_library(time_tectonic          OBJECT tectonic.c)
    add_library(time_radiometric       OBJECT radiometric.c)
    add_library(time_thermo            OBJECT thermo.c)
    add_library(time_geology_interpret OBJECT geology_interpret.c)
    add_library(time_earth_voyage      OBJECT earth_voyage.c)
    add_library(time_fossil_narrative  OBJECT fossil_narrative.c)
    add_library(time_earth_story      OBJECT earth_story.c)
endif()
