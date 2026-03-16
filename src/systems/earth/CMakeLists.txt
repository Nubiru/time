set(EARTH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/biorhythm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/daylight.c
    ${CMAKE_CURRENT_SOURCE_DIR}/seasons.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ski_resorts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/snow_season.c
    ${CMAKE_CURRENT_SOURCE_DIR}/solar_radiation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/storm_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/surf_spots.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interest_profile.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tidal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wind_patterns.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tide_predict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/location_alerts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/privacy_framework.c
    ${CMAKE_CURRENT_SOURCE_DIR}/user_prefs.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_biorhythm        OBJECT biorhythm.c)
    add_library(time_daylight         OBJECT daylight.c)
    add_library(time_seasons          OBJECT seasons.c)
    add_library(time_ski_resorts      OBJECT ski_resorts.c)
    add_library(time_snow_season      OBJECT snow_season.c)
    add_library(time_solar_radiation  OBJECT solar_radiation.c)
    add_library(time_storm_data       OBJECT storm_data.c)
    add_library(time_surf_spots       OBJECT surf_spots.c)
    add_library(time_interest_profile OBJECT interest_profile.c)
    add_library(time_tidal            OBJECT tidal.c)
    add_library(time_wind_patterns    OBJECT wind_patterns.c)
    add_library(time_tide_predict     OBJECT tide_predict.c)
    add_library(time_location_alerts     OBJECT location_alerts.c)
    add_library(time_privacy_framework  OBJECT privacy_framework.c)
    add_library(time_user_prefs         OBJECT user_prefs.c)
endif()
