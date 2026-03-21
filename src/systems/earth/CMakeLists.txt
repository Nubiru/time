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
    ${CMAKE_CURRENT_SOURCE_DIR}/local_events.c
    ${CMAKE_CURRENT_SOURCE_DIR}/event_filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/neighbor_roulette.c
    ${CMAKE_CURRENT_SOURCE_DIR}/personal_space.c
    ${CMAKE_CURRENT_SOURCE_DIR}/content_curator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/persona_persist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/persona_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/onboarding.c
    ${CMAKE_CURRENT_SOURCE_DIR}/settings_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/birth_entry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/author_card.c
    ${CMAKE_CURRENT_SOURCE_DIR}/surf_display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/human_population.c
    ${CMAKE_CURRENT_SOURCE_DIR}/today_for_you.c
    ${CMAKE_CURRENT_SOURCE_DIR}/notification_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/moment_snapshot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_stats.c
    ${CMAKE_CURRENT_SOURCE_DIR}/heartbeat_counter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pop_counter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/persona_card.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usage_interest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/diamond_room.c
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
    add_library(time_local_events       OBJECT local_events.c)
    add_library(time_event_filter       OBJECT event_filter.c)
    add_library(time_neighbor_roulette  OBJECT neighbor_roulette.c)
    add_library(time_personal_space    OBJECT personal_space.c)
    add_library(time_content_curator   OBJECT content_curator.c)
    add_library(time_persona_persist  OBJECT persona_persist.c)
    add_library(time_persona_io      OBJECT persona_io.c)
    add_library(time_onboarding      OBJECT onboarding.c)
    add_library(time_settings_panel  OBJECT settings_panel.c)
    add_library(time_birth_entry     OBJECT birth_entry.c)
    add_library(time_author_card     OBJECT author_card.c)
    add_library(time_surf_display    OBJECT surf_display.c)
    add_library(time_human_population OBJECT human_population.c)
    add_library(time_today_for_you    OBJECT today_for_you.c)
    add_library(time_notification_data OBJECT notification_data.c)
    add_library(time_moment_snapshot  OBJECT moment_snapshot.c)
    add_library(time_earth_stats      OBJECT earth_stats.c)
    add_library(time_heartbeat_counter OBJECT heartbeat_counter.c)
    add_library(time_pop_counter      OBJECT pop_counter.c)
    add_library(time_persona_card    OBJECT persona_card.c)
    add_library(time_usage_interest  OBJECT usage_interest.c)
    add_library(time_diamond_room    OBJECT diamond_room.c)
endif()
