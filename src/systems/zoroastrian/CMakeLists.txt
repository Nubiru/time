set(ZOROASTRIAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/cosmic_duality.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zoroastrian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zoroastrian_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_cosmic_duality        OBJECT cosmic_duality.c)
    add_library(time_zoroastrian           OBJECT zoroastrian.c)
    add_library(time_zoroastrian_interpret OBJECT zoroastrian_interpret.c)
endif()
