set(TZOLKIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/haab.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cr_cycle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dreamspell.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_board.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kin_social.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_interpret.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_today.c
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
endif()
