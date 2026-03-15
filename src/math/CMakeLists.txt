# src/math/CMakeLists.txt — Math library modules

set(MATH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vec3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mat4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/julian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kepler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sidereal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ecliptic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/easing.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arc_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/color.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bezier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sacred_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wheel_layout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/projection.c
    PARENT_SCOPE
)

if(NOT EMSCRIPTEN)
    add_library(time_vec3            OBJECT vec3.c)
    add_library(time_mat4            OBJECT mat4.c)
    add_library(time_julian          OBJECT julian.c)
    add_library(time_kepler          OBJECT kepler.c)
    add_library(time_sidereal        OBJECT sidereal.c)
    add_library(time_ecliptic        OBJECT ecliptic.c)
    add_library(time_easing          OBJECT easing.c)
    add_library(time_arc_geometry    OBJECT arc_geometry.c)
    add_library(time_color           OBJECT color.c)
    add_library(time_bezier          OBJECT bezier.c)
    add_library(time_sacred_geometry OBJECT sacred_geometry.c)
    add_library(time_wheel_layout    OBJECT wheel_layout.c)
    add_library(time_projection      OBJECT projection.c)
endif()
