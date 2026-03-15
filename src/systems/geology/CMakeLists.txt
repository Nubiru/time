set(GEOLOGY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/geo_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/climate_history.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fossil_milestones.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tectonic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/radiometric.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thermo.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_geo_time          OBJECT geo_time.c)
    add_library(time_climate_history   OBJECT climate_history.c)
    add_library(time_fossil_milestones OBJECT fossil_milestones.c)
    add_library(time_tectonic          OBJECT tectonic.c)
    add_library(time_radiometric       OBJECT radiometric.c)
    add_library(time_thermo            OBJECT thermo.c)
endif()
