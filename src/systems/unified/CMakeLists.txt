set(UNIFIED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/structural_map.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cycle_analysis.c
    ${CMAKE_CURRENT_SOURCE_DIR}/calendar_epoch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/calendar_politics.c
    ${CMAKE_CURRENT_SOURCE_DIR}/convergence.c
    ${CMAKE_CURRENT_SOURCE_DIR}/convergence_detect.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_drama.c
    ${CMAKE_CURRENT_SOURCE_DIR}/frequency.c
    ${CMAKE_CURRENT_SOURCE_DIR}/precession_detect.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wisdom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_engine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fun_facts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/achievement.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/calendar_reform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codon_hexagram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cultural_stories.c
    ${CMAKE_CURRENT_SOURCE_DIR}/knowledge_graph.c
    ${CMAKE_CURRENT_SOURCE_DIR}/number_scanner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/phase_space.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ratio_analysis.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sacred_numbers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/time_philosophy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/birth_profile.c
    ${CMAKE_CURRENT_SOURCE_DIR}/festival_detector.c
    ${CMAKE_CURRENT_SOURCE_DIR}/calendar_convert.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lunar_harmonics.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_structural_map    OBJECT structural_map.c)
    add_library(time_cycle_analysis    OBJECT cycle_analysis.c)
    add_library(time_calendar_epoch    OBJECT calendar_epoch.c)
    add_library(time_calendar_politics OBJECT calendar_politics.c)
    add_library(time_convergence       OBJECT convergence.c)
    add_library(time_convergence_detect OBJECT convergence_detect.c)
    add_library(time_earth_drama       OBJECT earth_drama.c)
    add_library(time_frequency         OBJECT frequency.c)
    add_library(time_precession_detect OBJECT precession_detect.c)
    add_library(time_wisdom            OBJECT wisdom.c)
    add_library(time_wisdom_engine     OBJECT wisdom_engine.c)
    add_library(time_fun_facts         OBJECT fun_facts.c)
    add_library(time_achievement       OBJECT achievement.c)
    add_library(time_audio_data        OBJECT audio_data.c)
    add_library(time_calendar_reform   OBJECT calendar_reform.c)
    add_library(time_codon_hexagram    OBJECT codon_hexagram.c)
    add_library(time_cultural_stories  OBJECT cultural_stories.c)
    add_library(time_knowledge_graph   OBJECT knowledge_graph.c)
    add_library(time_number_scanner    OBJECT number_scanner.c)
    add_library(time_phase_space       OBJECT phase_space.c)
    add_library(time_ratio_analysis    OBJECT ratio_analysis.c)
    add_library(time_sacred_numbers    OBJECT sacred_numbers.c)
    add_library(time_time_philosophy   OBJECT time_philosophy.c)
    add_library(time_birth_profile     OBJECT birth_profile.c)
    add_library(time_festival_detector OBJECT festival_detector.c)
    add_library(time_calendar_convert  OBJECT calendar_convert.c)
    add_library(time_lunar_harmonics   OBJECT lunar_harmonics.c)
endif()
