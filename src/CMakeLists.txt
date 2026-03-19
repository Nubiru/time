# src/CMakeLists.txt — Source tree aggregator

add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(render)
add_subdirectory(systems)
add_subdirectory(ui)
add_subdirectory(platform)

# Propagate all source lists to root for WASM build
set(MATH_SOURCES      ${MATH_SOURCES}      PARENT_SCOPE)
set(CORE_SOURCES      ${CORE_SOURCES}      PARENT_SCOPE)
set(RENDER_SOURCES    ${RENDER_SOURCES}    PARENT_SCOPE)
set(SYSTEMS_SOURCES   ${SYSTEMS_SOURCES}   PARENT_SCOPE)
set(UI_SOURCES        ${UI_SOURCES}        PARENT_SCOPE)
set(PLATFORM_SOURCES  ${PLATFORM_SOURCES}  PARENT_SCOPE)
