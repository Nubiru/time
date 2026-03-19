set(TAROT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tarot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tarot_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_tarot           OBJECT tarot.c)
    add_library(time_tarot_interpret OBJECT tarot_interpret.c)
endif()
