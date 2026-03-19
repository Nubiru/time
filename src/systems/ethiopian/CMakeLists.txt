set(ETHIOPIAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ethiopian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ethiopian_interpret.c
    PARENT_SCOPE
)
if(NOT EMSCRIPTEN)
    add_library(time_ethiopian           OBJECT ethiopian.c)
    add_library(time_ethiopian_interpret OBJECT ethiopian_interpret.c)
endif()
