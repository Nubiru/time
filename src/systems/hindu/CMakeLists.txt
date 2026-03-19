set(HINDU_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/nakshatra.c
    ${CMAKE_CURRENT_SOURCE_DIR}/panchanga.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yuga.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hindu_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_nakshatra        OBJECT nakshatra.c)
    add_library(time_panchanga        OBJECT panchanga.c)
    add_library(time_yuga             OBJECT yuga.c)
    add_library(time_hindu_interpret  OBJECT hindu_interpret.c)
endif()
