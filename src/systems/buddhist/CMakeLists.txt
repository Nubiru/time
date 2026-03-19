set(BUDDHIST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/buddhist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kalpa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/buddhist_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_buddhist OBJECT buddhist.c)
    add_library(time_kalpa    OBJECT kalpa.c)
    add_library(time_buddhist_interpret OBJECT buddhist_interpret.c)
endif()
