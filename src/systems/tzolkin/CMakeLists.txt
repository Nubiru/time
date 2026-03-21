set(TZOLKIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/haab.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cr_cycle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dreamspell.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_board.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kin_social.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_interpret.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_today.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thirteen_moon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/heptad_gate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hunab_ku_21.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lenses_of_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clear_signs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/telektonon_board.c
    ${CMAKE_CURRENT_SOURCE_DIR}/telektonon_journey.c
    ${CMAKE_CURRENT_SOURCE_DIR}/force_lines.c
    ${CMAKE_CURRENT_SOURCE_DIR}/telektonon_cards.c
    ${CMAKE_CURRENT_SOURCE_DIR}/synchronotron.c
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_441.c
    ${CMAKE_CURRENT_SOURCE_DIR}/haab_vinal.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_tzolkin       OBJECT tzolkin.c)
    add_library(time_haab          OBJECT haab.c)
    add_library(time_cr_cycle      OBJECT cr_cycle.c)
    add_library(time_dreamspell    OBJECT dreamspell.c)
    add_library(time_tzolkin_board OBJECT tzolkin_board.c)
    add_library(time_kin_social    OBJECT kin_social.c)
    add_library(time_tzolkin_interpret OBJECT tzolkin_interpret.c)
    add_library(time_tzolkin_today    OBJECT tzolkin_today.c)
    add_library(time_thirteen_moon   OBJECT thirteen_moon.c)
    add_library(time_heptad_gate     OBJECT heptad_gate.c)
    add_library(time_hunab_ku_21    OBJECT hunab_ku_21.c)
    add_library(time_lenses_of_time  OBJECT lenses_of_time.c)
    add_library(time_clear_signs     OBJECT clear_signs.c)
    add_library(time_telektonon_board   OBJECT telektonon_board.c)
    add_library(time_telektonon_journey OBJECT telektonon_journey.c)
    add_library(time_force_lines        OBJECT force_lines.c)
    add_library(time_telektonon_cards    OBJECT telektonon_cards.c)
    add_library(time_synchronotron       OBJECT synchronotron.c)
    add_library(time_matrix_441          OBJECT matrix_441.c)
    add_library(time_haab_vinal          OBJECT haab_vinal.c)
endif()
