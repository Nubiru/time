# cmake/testing.cmake — Unity test framework integration
#
# Provides time_add_test() for registering test executables
# with per-test link dependencies.

# Unity test framework (vendored)
add_library(unity STATIC tests/unity/unity.c)
target_include_directories(unity PUBLIC tests/unity)

# Register a test executable.
#
# Usage:
#   time_add_test(NAME test_vec3 TEST tests/math/test_vec3.c DEPS vec3)
#   time_add_test(NAME test_mat4 TEST tests/math/test_mat4.c DEPS mat4 vec3)
#
# NAME: test executable name (also CTest name)
# TEST: path to test source file
# DEPS: list of OBJECT library names (without time_ prefix)
function(time_add_test)
    cmake_parse_arguments(T "" "NAME;TEST" "DEPS" ${ARGN})

    # Build dep target names: "vec3" -> "time_vec3"
    set(_link_targets "")
    foreach(_dep ${T_DEPS})
        list(APPEND _link_targets "time_${_dep}")
    endforeach()

    add_executable(${T_NAME} ${T_TEST})
    target_link_libraries(${T_NAME} PRIVATE unity ${_link_targets} m)
    target_include_directories(${T_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    add_test(NAME ${T_NAME} COMMAND ${T_NAME})
endfunction()
