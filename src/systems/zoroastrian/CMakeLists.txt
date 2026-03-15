set(ZOROASTRIAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/cosmic_duality.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zoroastrian.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_cosmic_duality OBJECT cosmic_duality.c)
    add_library(time_zoroastrian    OBJECT zoroastrian.c)
endif()
