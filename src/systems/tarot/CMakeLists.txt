set(TAROT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tarot.c PARENT_SCOPE)
if(NOT EMSCRIPTEN)
    add_library(time_tarot OBJECT tarot.c)
endif()
