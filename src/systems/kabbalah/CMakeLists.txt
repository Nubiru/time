set(KABBALAH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sefirot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/four_worlds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tree_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kabbalah_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_sefirot              OBJECT sefirot.c)
    add_library(time_four_worlds          OBJECT four_worlds.c)
    add_library(time_tree_geometry        OBJECT tree_geometry.c)
    add_library(time_kabbalah_interpret   OBJECT kabbalah_interpret.c)
endif()
