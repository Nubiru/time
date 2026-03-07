# ===========================================================================
# CMakeLists.txt — Time Project
#
# Visual, interactive, three-dimensional digital artwork displaying time
# through multiple cultural and astronomical systems.
#
# Two build modes:
#   Native (gcc):  cmake -B build && cmake --build build && ctest --test-dir build
#   WASM (emcc):   emcmake cmake -B build-wasm && cmake --build build-wasm
# ===========================================================================
cmake_minimum_required(VERSION 3.16)
project(Time VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

# Emscripten needs GNU extensions (EM_ASM requires -std=gnu11)
# Native build uses strict C11 + pedantic
if(EMSCRIPTEN)
    set(CMAKE_C_EXTENSIONS ON)
else()
    set(CMAKE_C_EXTENSIONS OFF)
    add_compile_options(-pedantic)
endif()

# ===========================================================================
# Source file lists (explicit — no GLOB)
# ===========================================================================

# --- Core ---
set(CORE_SOURCES
    src/core/main.c
    src/core/app_state.c
    src/core/date_parse.c
)

# --- Math ---
set(MATH_SOURCES
    src/math/vec3.c
    src/math/mat4.c
    src/math/julian.c
    src/math/kepler.c
    src/math/sidereal.c
    src/math/ecliptic.c
    src/math/easing.c
    src/math/arc_geometry.c
    src/math/color.c
    src/math/bezier.c
    src/math/sacred_geometry.c
    src/math/wheel_layout.c
    src/math/projection.c
)

# --- Render ---
set(RENDER_SOURCES
    src/render/gl_init.c
    src/render/shader.c
    src/render/camera.c
    src/render/camera_scale.c
    src/render/render_layers.c
    src/render/mesh.c
    src/render/color_palette.c
    src/render/color_theory.c
    src/render/ring_geometry.c
    src/render/ring_data.c
    src/render/aspect_lines.c
    src/render/cusp_lines.c
    src/render/galaxy_geometry.c
    src/render/billboard.c
    src/render/star_catalog.c
    src/render/star_catalog_ext.c
    src/render/star_colors.c
    src/render/constellation.c
    src/render/atmo_ring.c
    src/render/deep_sky.c
    src/render/font_atlas.c
    src/render/glyph_batch.c
    src/render/decan_stars.c
    src/render/megalithic.c
    src/render/tarot_visual.c
    src/render/star_field.c
    src/render/planet_pack.c
    src/render/zodiac_pack.c
    src/render/card_pack.c
    src/render/moon_pack.c
    src/render/diffraction.c
    src/render/deep_sky_pack.c
    src/render/earth_globe.c
    src/render/earth_atmosphere.c
    src/render/catalog_ingest.c
    src/render/text_render.c
    src/render/hexagram_geometry.c
)

# --- Systems ---
set(SYSTEMS_SOURCES
    # Astronomy
    src/systems/astronomy/orbit.c
    src/systems/astronomy/planets.c
    src/systems/astronomy/lunar.c
    src/systems/astronomy/precession.c
    src/systems/astronomy/solar_events.c
    src/systems/astronomy/cosmic_time.c
    src/systems/astronomy/planet_data.c
    src/systems/astronomy/moon_data.c
    src/systems/astronomy/moon_nodes.c
    src/systems/astronomy/retrograde.c
    # Astrology
    src/systems/astrology/zodiac.c
    src/systems/astrology/aspects.c
    src/systems/astrology/observer.c
    src/systems/astrology/houses.c
    src/systems/astrology/dignity.c
    src/systems/astrology/planetary_hours.c
    src/systems/astrology/wheel_layout.c
    # Gregorian
    src/systems/gregorian/gregorian.c
    # Tzolkin
    src/systems/tzolkin/tzolkin.c
    src/systems/tzolkin/haab.c
    src/systems/tzolkin/cr_cycle.c
    src/systems/tzolkin/dreamspell.c
    src/systems/tzolkin/tzolkin_board.c
    # I Ching
    src/systems/iching/iching.c
    # Chinese
    src/systems/chinese/chinese.c
    # Numerology
    src/systems/numerology/numerology.c
    # Human Design
    src/systems/human_design/human_design.c
    src/systems/human_design/bodygraph.c
    # Hebrew
    src/systems/hebrew/hebrew.c
    src/systems/hebrew/sabbatical.c
    # Islamic
    src/systems/islamic/hijri.c
    src/systems/islamic/prayer_times.c
    # Buddhist
    src/systems/buddhist/buddhist.c
    src/systems/buddhist/kalpa.c
    # Hindu
    src/systems/hindu/nakshatra.c
    src/systems/hindu/panchanga.c
    src/systems/hindu/yuga.c
    # Kabbalah
    src/systems/kabbalah/sefirot.c
    src/systems/kabbalah/four_worlds.c
    src/systems/kabbalah/tree_geometry.c
    # Tarot
    src/systems/tarot/tarot.c
    # Geology
    src/systems/geology/geo_time.c
    src/systems/geology/climate_history.c
    src/systems/geology/fossil_milestones.c
    src/systems/geology/tectonic.c
    src/systems/geology/radiometric.c
    src/systems/geology/thermo.c
    # Earth
    src/systems/earth/biorhythm.c
    src/systems/earth/daylight.c
    src/systems/earth/seasons.c
    src/systems/earth/ski_resorts.c
    src/systems/earth/snow_season.c
    src/systems/earth/solar_radiation.c
    src/systems/earth/storm_data.c
    src/systems/earth/surf_spots.c
    src/systems/earth/tidal.c
    src/systems/earth/wind_patterns.c
    src/systems/earth/tide_predict.c
    # Unified
    src/systems/unified/structural_map.c
    src/systems/unified/cycle_analysis.c
    src/systems/unified/calendar_epoch.c
    src/systems/unified/calendar_politics.c
    src/systems/unified/convergence.c
    src/systems/unified/earth_drama.c
    src/systems/unified/frequency.c
    src/systems/unified/precession_detect.c
    src/systems/unified/wisdom.c
    src/systems/unified/wisdom_engine.c
    src/systems/unified/fun_facts.c
    src/systems/unified/achievement.c
    src/systems/unified/audio_data.c
    src/systems/unified/calendar_reform.c
    src/systems/unified/codon_hexagram.c
    src/systems/unified/cultural_stories.c
    src/systems/unified/knowledge_graph.c
    src/systems/unified/number_scanner.c
    src/systems/unified/phase_space.c
    src/systems/unified/ratio_analysis.c
    src/systems/unified/sacred_numbers.c
    # Celtic
    src/systems/celtic/wheel_of_year.c
    # Chakra
    src/systems/chakra/chakra.c
    # Coptic
    src/systems/coptic/coptic.c
    # Egyptian
    src/systems/egyptian/egyptian.c
    # Ethiopian
    src/systems/ethiopian/ethiopian.c
    # Myanmar
    src/systems/myanmar/myanmar.c
    # Thai
    src/systems/thai/thai_calendar.c
    # Zoroastrian
    src/systems/zoroastrian/cosmic_duality.c
    src/systems/zoroastrian/zoroastrian.c
)

# --- UI ---
set(UI_SOURCES
    src/ui/time_hud.c
    src/ui/fmt.c
    src/ui/animation.c
    src/ui/glyph_layout.c
    src/ui/card_data.c
    src/ui/zodiac_glyphs.c
    src/ui/hexagram_visual.c
    src/ui/location_presets.c
    src/ui/scale_hud.c
    src/ui/system_scale_map.c
    src/ui/time_format.c
    src/ui/card_layout.c
    src/ui/tzolkin_card.c
    src/ui/astro_summary.c
    src/ui/astro_fmt.c
    src/ui/golden_layout.c
    src/ui/theme.c
    src/ui/accessibility.c
    src/ui/chinese_fmt.c
    src/ui/hd_card.c
    src/ui/i18n.c
    src/ui/iching_card.c
    src/ui/lunar_display.c
    src/ui/rtl_layout.c
    src/ui/scroll_layers.c
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${MATH_SOURCES}
    ${RENDER_SOURCES}
    ${SYSTEMS_SOURCES}
    ${UI_SOURCES}
)

# ===========================================================================
# WASM BUILD (emcmake cmake -B build-wasm)
# ===========================================================================
if(EMSCRIPTEN)
    add_executable(time_app ${ALL_SOURCES})
    set_target_properties(time_app PROPERTIES
        OUTPUT_NAME "index"
        SUFFIX ".html"
    )
    target_link_options(time_app PRIVATE
        -sUSE_WEBGL2=1
        -sFULL_ES3=1
        -sWASM=1
        -Os
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/web/index.html"
    )
    # Copy style.css to build dir
    add_custom_command(TARGET time_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/web/style.css
            ${CMAKE_BINARY_DIR}/style.css
    )

# ===========================================================================
# NATIVE BUILD (cmake -B build)
# ===========================================================================
else()
    # ---------------------------------------------------------------
    # OBJECT libraries — one per source file for granular test linking
    # ---------------------------------------------------------------

    # Math
    add_library(time_vec3            OBJECT src/math/vec3.c)
    add_library(time_mat4            OBJECT src/math/mat4.c)
    add_library(time_julian          OBJECT src/math/julian.c)
    add_library(time_kepler          OBJECT src/math/kepler.c)
    add_library(time_sidereal        OBJECT src/math/sidereal.c)
    add_library(time_ecliptic        OBJECT src/math/ecliptic.c)
    add_library(time_easing          OBJECT src/math/easing.c)
    add_library(time_arc_geometry    OBJECT src/math/arc_geometry.c)
    add_library(time_color           OBJECT src/math/color.c)
    add_library(time_bezier          OBJECT src/math/bezier.c)
    add_library(time_sacred_geometry OBJECT src/math/sacred_geometry.c)
    add_library(time_wheel_layout    OBJECT src/math/wheel_layout.c)
    add_library(time_projection      OBJECT src/math/projection.c)

    # Render
    add_library(time_camera          OBJECT src/render/camera.c)
    add_library(time_camera_scale    OBJECT src/render/camera_scale.c)
    add_library(time_render_layers   OBJECT src/render/render_layers.c)
    add_library(time_color_palette   OBJECT src/render/color_palette.c)
    add_library(time_color_theory    OBJECT src/render/color_theory.c)
    add_library(time_ring_geometry   OBJECT src/render/ring_geometry.c)
    add_library(time_ring_data       OBJECT src/render/ring_data.c)
    add_library(time_aspect_lines    OBJECT src/render/aspect_lines.c)
    add_library(time_cusp_lines      OBJECT src/render/cusp_lines.c)
    add_library(time_galaxy_geometry OBJECT src/render/galaxy_geometry.c)
    add_library(time_billboard       OBJECT src/render/billboard.c)
    add_library(time_star_catalog    OBJECT src/render/star_catalog.c)
    add_library(time_star_catalog_ext OBJECT src/render/star_catalog_ext.c)
    add_library(time_star_colors     OBJECT src/render/star_colors.c)
    add_library(time_constellation   OBJECT src/render/constellation.c)
    add_library(time_atmo_ring       OBJECT src/render/atmo_ring.c)
    add_library(time_deep_sky        OBJECT src/render/deep_sky.c)
    add_library(time_deep_sky_pack   OBJECT src/render/deep_sky_pack.c)
    add_library(time_font_atlas      OBJECT src/render/font_atlas.c)
    add_library(time_glyph_batch     OBJECT src/render/glyph_batch.c)
    add_library(time_decan_stars     OBJECT src/render/decan_stars.c)
    add_library(time_megalithic      OBJECT src/render/megalithic.c)
    add_library(time_tarot_visual    OBJECT src/render/tarot_visual.c)

    # Systems — Astronomy
    add_library(time_orbit           OBJECT src/systems/astronomy/orbit.c)
    add_library(time_planets         OBJECT src/systems/astronomy/planets.c)
    add_library(time_lunar           OBJECT src/systems/astronomy/lunar.c)
    add_library(time_precession      OBJECT src/systems/astronomy/precession.c)
    add_library(time_solar_events    OBJECT src/systems/astronomy/solar_events.c)
    add_library(time_cosmic_time     OBJECT src/systems/astronomy/cosmic_time.c)
    add_library(time_planet_data     OBJECT src/systems/astronomy/planet_data.c)
    add_library(time_moon_data       OBJECT src/systems/astronomy/moon_data.c)
    add_library(time_moon_nodes      OBJECT src/systems/astronomy/moon_nodes.c)
    add_library(time_retrograde      OBJECT src/systems/astronomy/retrograde.c)

    # Systems — Astrology
    add_library(time_zodiac          OBJECT src/systems/astrology/zodiac.c)
    add_library(time_aspects         OBJECT src/systems/astrology/aspects.c)
    add_library(time_observer        OBJECT src/systems/astrology/observer.c)
    add_library(time_houses          OBJECT src/systems/astrology/houses.c)
    add_library(time_dignity         OBJECT src/systems/astrology/dignity.c)
    add_library(time_planetary_hours OBJECT src/systems/astrology/planetary_hours.c)
    add_library(time_astro_wheel     OBJECT src/systems/astrology/wheel_layout.c)

    # Systems — Calendars
    add_library(time_gregorian       OBJECT src/systems/gregorian/gregorian.c)
    add_library(time_tzolkin         OBJECT src/systems/tzolkin/tzolkin.c)
    add_library(time_haab            OBJECT src/systems/tzolkin/haab.c)
    add_library(time_cr_cycle        OBJECT src/systems/tzolkin/cr_cycle.c)
    add_library(time_iching          OBJECT src/systems/iching/iching.c)
    add_library(time_chinese         OBJECT src/systems/chinese/chinese.c)
    add_library(time_numerology      OBJECT src/systems/numerology/numerology.c)
    add_library(time_human_design    OBJECT src/systems/human_design/human_design.c)
    add_library(time_bodygraph       OBJECT src/systems/human_design/bodygraph.c)
    add_library(time_hebrew          OBJECT src/systems/hebrew/hebrew.c)
    add_library(time_hijri           OBJECT src/systems/islamic/hijri.c)
    add_library(time_prayer_times    OBJECT src/systems/islamic/prayer_times.c)
    add_library(time_buddhist        OBJECT src/systems/buddhist/buddhist.c)
    add_library(time_kalpa           OBJECT src/systems/buddhist/kalpa.c)
    add_library(time_nakshatra       OBJECT src/systems/hindu/nakshatra.c)
    add_library(time_panchanga       OBJECT src/systems/hindu/panchanga.c)
    add_library(time_yuga            OBJECT src/systems/hindu/yuga.c)
    add_library(time_sefirot         OBJECT src/systems/kabbalah/sefirot.c)
    add_library(time_four_worlds     OBJECT src/systems/kabbalah/four_worlds.c)
    add_library(time_tree_geometry   OBJECT src/systems/kabbalah/tree_geometry.c)

    # Systems — Tarot
    add_library(time_tarot           OBJECT src/systems/tarot/tarot.c)

    # Systems — Geology
    add_library(time_geo_time        OBJECT src/systems/geology/geo_time.c)
    add_library(time_climate_history OBJECT src/systems/geology/climate_history.c)
    add_library(time_fossil_milestones OBJECT src/systems/geology/fossil_milestones.c)
    add_library(time_tectonic        OBJECT src/systems/geology/tectonic.c)

    # Systems — Earth
    add_library(time_biorhythm       OBJECT src/systems/earth/biorhythm.c)
    add_library(time_daylight        OBJECT src/systems/earth/daylight.c)
    add_library(time_seasons         OBJECT src/systems/earth/seasons.c)
    add_library(time_ski_resorts     OBJECT src/systems/earth/ski_resorts.c)
    add_library(time_snow_season     OBJECT src/systems/earth/snow_season.c)
    add_library(time_solar_radiation OBJECT src/systems/earth/solar_radiation.c)
    add_library(time_storm_data      OBJECT src/systems/earth/storm_data.c)
    add_library(time_surf_spots      OBJECT src/systems/earth/surf_spots.c)
    add_library(time_tidal           OBJECT src/systems/earth/tidal.c)
    add_library(time_wind_patterns   OBJECT src/systems/earth/wind_patterns.c)
    add_library(time_tide_predict    OBJECT src/systems/earth/tide_predict.c)

    # Systems — Unified
    add_library(time_structural_map  OBJECT src/systems/unified/structural_map.c)
    add_library(time_cycle_analysis  OBJECT src/systems/unified/cycle_analysis.c)
    add_library(time_calendar_epoch  OBJECT src/systems/unified/calendar_epoch.c)
    add_library(time_calendar_politics OBJECT src/systems/unified/calendar_politics.c)
    add_library(time_convergence     OBJECT src/systems/unified/convergence.c)
    add_library(time_earth_drama     OBJECT src/systems/unified/earth_drama.c)
    add_library(time_frequency       OBJECT src/systems/unified/frequency.c)
    add_library(time_precession_detect OBJECT src/systems/unified/precession_detect.c)
    add_library(time_wisdom          OBJECT src/systems/unified/wisdom.c)
    add_library(time_wisdom_engine   OBJECT src/systems/unified/wisdom_engine.c)
    add_library(time_fun_facts       OBJECT src/systems/unified/fun_facts.c)

    # Core
    add_library(time_date_parse      OBJECT src/core/date_parse.c)

    # UI
    add_library(time_time_hud        OBJECT src/ui/time_hud.c)
    add_library(time_fmt             OBJECT src/ui/fmt.c)
    add_library(time_animation       OBJECT src/ui/animation.c)
    add_library(time_glyph_layout    OBJECT src/ui/glyph_layout.c)
    add_library(time_card_data       OBJECT src/ui/card_data.c)
    add_library(time_zodiac_glyphs   OBJECT src/ui/zodiac_glyphs.c)
    add_library(time_hexagram_visual OBJECT src/ui/hexagram_visual.c)
    add_library(time_location_presets OBJECT src/ui/location_presets.c)
    add_library(time_scale_hud       OBJECT src/ui/scale_hud.c)
    add_library(time_system_scale_map OBJECT src/ui/system_scale_map.c)
    add_library(time_time_format     OBJECT src/ui/time_format.c)
    add_library(time_card_layout     OBJECT src/ui/card_layout.c)
    add_library(time_tzolkin_card    OBJECT src/ui/tzolkin_card.c)
    add_library(time_astro_summary   OBJECT src/ui/astro_summary.c)
    add_library(time_astro_fmt       OBJECT src/ui/astro_fmt.c)
    add_library(time_golden_layout   OBJECT src/ui/golden_layout.c)
    add_library(time_theme           OBJECT src/ui/theme.c)

    # ---------------------------------------------------------------
    # Tests — 116 suites, Unity framework, per-test dependencies
    # ---------------------------------------------------------------
    include(CTest)
    include(cmake/testing.cmake)

    # Math tests
    time_add_test(NAME test_vec3            TEST tests/math/test_vec3.c            DEPS vec3)
    time_add_test(NAME test_mat4            TEST tests/math/test_mat4.c            DEPS mat4 vec3)
    time_add_test(NAME test_julian          TEST tests/math/test_julian.c          DEPS julian)
    time_add_test(NAME test_kepler          TEST tests/math/test_kepler.c          DEPS kepler)
    time_add_test(NAME test_sidereal        TEST tests/math/test_sidereal.c        DEPS sidereal)
    time_add_test(NAME test_ecliptic        TEST tests/math/test_ecliptic.c        DEPS ecliptic)
    time_add_test(NAME test_easing          TEST tests/math/test_easing.c          DEPS easing)
    time_add_test(NAME test_arc_geometry    TEST tests/math/test_arc_geometry.c    DEPS arc_geometry)
    time_add_test(NAME test_color           TEST tests/math/test_color.c           DEPS color)
    time_add_test(NAME test_bezier          TEST tests/math/test_bezier.c          DEPS bezier vec3)
    time_add_test(NAME test_sacred_geometry TEST tests/math/test_sacred_geometry.c DEPS sacred_geometry vec3)
    time_add_test(NAME test_wheel_layout    TEST tests/math/test_wheel_layout.c    DEPS wheel_layout)
    time_add_test(NAME test_projection      TEST tests/math/test_projection.c      DEPS projection mat4 vec3)

    # Render tests
    time_add_test(NAME test_camera          TEST tests/render/test_camera.c          DEPS camera vec3 mat4)
    time_add_test(NAME test_camera_scale    TEST tests/render/test_camera_scale.c    DEPS camera_scale easing)
    time_add_test(NAME test_render_layers   TEST tests/render/test_render_layers.c   DEPS render_layers camera_scale easing golden_layout)
    time_add_test(NAME test_font_atlas      TEST tests/render/test_font_atlas.c      DEPS font_atlas)
    time_add_test(NAME test_color_palette   TEST tests/render/test_color_palette.c   DEPS color_palette color_theory color)
    time_add_test(NAME test_ring_geometry   TEST tests/render/test_ring_geometry.c   DEPS ring_geometry)
    time_add_test(NAME test_aspect_lines    TEST tests/render/test_aspect_lines.c    DEPS aspect_lines color_palette color_theory color)
    time_add_test(NAME test_cusp_lines      TEST tests/render/test_cusp_lines.c      DEPS cusp_lines)
    time_add_test(NAME test_galaxy_geometry TEST tests/render/test_galaxy_geometry.c  DEPS galaxy_geometry)
    time_add_test(NAME test_billboard       TEST tests/render/test_billboard.c       DEPS billboard mat4 vec3)
    time_add_test(NAME test_ring_data       TEST tests/render/test_ring_data.c       DEPS ring_data ring_geometry)
    time_add_test(NAME test_star_catalog    TEST tests/render/test_star_catalog.c    DEPS star_catalog)
    time_add_test(NAME test_constellation   TEST tests/render/test_constellation.c   DEPS constellation star_catalog)
    time_add_test(NAME test_atmo_ring       TEST tests/render/test_atmo_ring.c       DEPS atmo_ring)
    time_add_test(NAME test_star_colors     TEST tests/render/test_star_colors.c     DEPS star_colors)
    time_add_test(NAME test_deep_sky        TEST tests/render/test_deep_sky.c        DEPS deep_sky)
    time_add_test(NAME test_deep_sky_pack  TEST tests/render/test_deep_sky_pack.c  DEPS deep_sky_pack deep_sky)
    time_add_test(NAME test_glyph_batch     TEST tests/render/test_glyph_batch.c     DEPS glyph_batch vec3)
    time_add_test(NAME test_color_theory    TEST tests/render/test_color_theory.c    DEPS color_theory color)
    time_add_test(NAME test_star_catalog_ext TEST tests/render/test_star_catalog_ext.c DEPS star_catalog_ext)
    time_add_test(NAME test_decan_stars     TEST tests/render/test_decan_stars.c     DEPS decan_stars)
    time_add_test(NAME test_megalithic      TEST tests/render/test_megalithic.c      DEPS megalithic solar_events)
    time_add_test(NAME test_tarot_visual    TEST tests/render/test_tarot_visual.c    DEPS tarot_visual color_theory color tree_geometry sefirot)
    time_add_test(NAME test_moon_data       TEST tests/systems/test_moon_data.c      DEPS moon_data kepler)

    # Core tests
    time_add_test(NAME test_date_parse      TEST tests/core/test_date_parse.c       DEPS date_parse julian)

    # Astronomy tests
    time_add_test(NAME test_orbit           TEST tests/systems/test_orbit.c          DEPS orbit kepler)
    time_add_test(NAME test_planets         TEST tests/systems/test_planets.c        DEPS planets orbit kepler)
    time_add_test(NAME test_lunar           TEST tests/systems/test_lunar.c          DEPS lunar julian)
    time_add_test(NAME test_precession      TEST tests/systems/test_precession.c     DEPS precession)
    time_add_test(NAME test_solar_events    TEST tests/systems/test_solar_events.c   DEPS solar_events)
    time_add_test(NAME test_cosmic_time     TEST tests/systems/test_cosmic_time.c    DEPS cosmic_time)
    time_add_test(NAME test_planet_data     TEST tests/systems/test_planet_data.c    DEPS planet_data)
    time_add_test(NAME test_moon_nodes      TEST tests/systems/test_moon_nodes.c     DEPS moon_nodes)
    time_add_test(NAME test_retrograde      TEST tests/systems/test_retrograde.c     DEPS retrograde planets orbit kepler)

    # Astrology tests
    time_add_test(NAME test_zodiac          TEST tests/systems/test_zodiac.c         DEPS zodiac)
    time_add_test(NAME test_aspects         TEST tests/systems/test_aspects.c        DEPS aspects)
    time_add_test(NAME test_observer        TEST tests/systems/test_observer.c       DEPS observer)
    time_add_test(NAME test_houses          TEST tests/systems/test_houses.c         DEPS houses zodiac)
    time_add_test(NAME test_dignity         TEST tests/systems/test_dignity.c        DEPS dignity)
    time_add_test(NAME test_planetary_hours TEST tests/systems/test_planetary_hours.c DEPS planetary_hours)
    time_add_test(NAME test_astro_wheel     TEST tests/systems/test_wheel_layout.c   DEPS astro_wheel vec3)

    # Calendar tests
    time_add_test(NAME test_gregorian       TEST tests/systems/test_gregorian.c      DEPS gregorian julian)
    time_add_test(NAME test_tzolkin         TEST tests/systems/test_tzolkin.c        DEPS tzolkin julian)
    time_add_test(NAME test_haab            TEST tests/systems/test_haab.c           DEPS haab tzolkin julian)
    time_add_test(NAME test_chinese         TEST tests/systems/test_chinese.c        DEPS chinese julian)
    time_add_test(NAME test_iching          TEST tests/systems/test_iching.c         DEPS iching)
    time_add_test(NAME test_numerology      TEST tests/systems/test_numerology.c     DEPS numerology)
    time_add_test(NAME test_human_design    TEST tests/systems/test_human_design.c   DEPS human_design)
    time_add_test(NAME test_bodygraph       TEST tests/systems/test_bodygraph.c      DEPS bodygraph)
    time_add_test(NAME test_hebrew          TEST tests/systems/hebrew/test_hebrew.c  DEPS hebrew)
    time_add_test(NAME test_hijri           TEST tests/systems/islamic/test_hijri.c  DEPS hijri)
    time_add_test(NAME test_prayer_times    TEST tests/systems/islamic/test_prayer_times.c DEPS prayer_times solar_events julian)
    time_add_test(NAME test_buddhist        TEST tests/systems/buddhist/test_buddhist.c    DEPS buddhist lunar)
    time_add_test(NAME test_kalpa           TEST tests/systems/buddhist/test_kalpa.c       DEPS kalpa)
    time_add_test(NAME test_nakshatra       TEST tests/systems/test_nakshatra.c     DEPS nakshatra)
    time_add_test(NAME test_panchanga       TEST tests/systems/hindu/test_panchanga.c DEPS panchanga nakshatra)
    time_add_test(NAME test_yuga            TEST tests/systems/hindu/test_yuga.c     DEPS yuga)
    time_add_test(NAME test_sefirot         TEST tests/systems/kabbalah/test_sefirot.c      DEPS sefirot)
    time_add_test(NAME test_four_worlds     TEST tests/systems/kabbalah/test_four_worlds.c  DEPS four_worlds)
    time_add_test(NAME test_tree_geometry   TEST tests/systems/kabbalah/test_tree_geometry.c DEPS tree_geometry sefirot)

    # Tarot tests
    time_add_test(NAME test_tarot           TEST tests/systems/tarot/test_tarot.c   DEPS tarot)

    # Geology tests
    time_add_test(NAME test_geo_time        TEST tests/systems/test_geo_time.c              DEPS geo_time)
    time_add_test(NAME test_climate_history TEST tests/systems/geology/test_climate_history.c DEPS climate_history)
    time_add_test(NAME test_fossil_milestones TEST tests/systems/geology/test_fossil_milestones.c DEPS fossil_milestones)
    time_add_test(NAME test_tectonic        TEST tests/systems/geology/test_tectonic.c      DEPS tectonic)

    # Earth tests
    time_add_test(NAME test_biorhythm       TEST tests/systems/earth/test_biorhythm.c       DEPS biorhythm)
    time_add_test(NAME test_daylight        TEST tests/systems/earth/test_daylight.c        DEPS daylight solar_events)
    time_add_test(NAME test_seasons         TEST tests/systems/earth/test_seasons.c         DEPS seasons)
    time_add_test(NAME test_ski_resorts     TEST tests/systems/earth/test_ski_resorts.c     DEPS ski_resorts)
    time_add_test(NAME test_snow_season     TEST tests/systems/earth/test_snow_season.c     DEPS snow_season)
    time_add_test(NAME test_solar_radiation TEST tests/systems/earth/test_solar_radiation.c DEPS solar_radiation)
    time_add_test(NAME test_storm_data      TEST tests/systems/earth/test_storm_data.c      DEPS storm_data)
    time_add_test(NAME test_surf_spots      TEST tests/systems/earth/test_surf_spots.c      DEPS surf_spots)
    time_add_test(NAME test_tidal           TEST tests/systems/earth/test_tidal.c           DEPS tidal)
    time_add_test(NAME test_wind_patterns   TEST tests/systems/earth/test_wind_patterns.c   DEPS wind_patterns)
    time_add_test(NAME test_tide_predict    TEST tests/systems/earth/test_tide_predict.c    DEPS tide_predict tidal surf_spots lunar julian)

    # Tzolkin extended tests
    time_add_test(NAME test_cr_cycle        TEST tests/systems/tzolkin/test_cr_cycle.c      DEPS cr_cycle haab tzolkin julian)

    # Unified tests
    time_add_test(NAME test_structural_map  TEST tests/systems/unified/test_structural_map.c  DEPS structural_map)
    time_add_test(NAME test_cycle_analysis  TEST tests/systems/test_cycle_analysis.c          DEPS cycle_analysis)
    time_add_test(NAME test_calendar_epoch  TEST tests/systems/unified/test_calendar_epoch.c  DEPS calendar_epoch)
    time_add_test(NAME test_calendar_politics TEST tests/systems/unified/test_calendar_politics.c DEPS calendar_politics)
    time_add_test(NAME test_convergence     TEST tests/systems/unified/test_convergence.c     DEPS convergence)
    time_add_test(NAME test_earth_drama     TEST tests/systems/unified/test_earth_drama.c     DEPS earth_drama climate_history fossil_milestones geo_time)
    time_add_test(NAME test_frequency       TEST tests/systems/unified/test_frequency.c       DEPS frequency)
    time_add_test(NAME test_precession_detect TEST tests/systems/unified/test_precession_detect.c DEPS precession_detect)
    time_add_test(NAME test_wisdom          TEST tests/systems/unified/test_wisdom.c          DEPS wisdom)
    time_add_test(NAME test_wisdom_engine   TEST tests/systems/unified/test_wisdom_engine.c   DEPS wisdom_engine wisdom)
    time_add_test(NAME test_fun_facts       TEST tests/systems/unified/test_fun_facts.c       DEPS fun_facts)

    # UI tests
    time_add_test(NAME test_time_hud        TEST tests/ui/test_time_hud.c
        DEPS time_hud gregorian julian sidereal tzolkin zodiac planets orbit kepler)
    time_add_test(NAME test_fmt             TEST tests/ui/test_fmt.c              DEPS fmt)
    time_add_test(NAME test_animation       TEST tests/ui/test_animation.c        DEPS animation easing)
    time_add_test(NAME test_glyph_layout    TEST tests/ui/test_glyph_layout.c    DEPS glyph_layout)
    time_add_test(NAME test_card_data       TEST tests/ui/test_card_data.c        DEPS card_data)
    time_add_test(NAME test_zodiac_glyphs   TEST tests/ui/test_zodiac_glyphs.c   DEPS zodiac_glyphs)
    time_add_test(NAME test_hexagram_visual TEST tests/ui/test_hexagram_visual.c  DEPS hexagram_visual iching)
    time_add_test(NAME test_location_presets TEST tests/ui/test_location_presets.c DEPS location_presets)
    time_add_test(NAME test_scale_hud       TEST tests/ui/test_scale_hud.c       DEPS scale_hud)
    time_add_test(NAME test_system_scale_map TEST tests/ui/test_system_scale_map.c DEPS system_scale_map)
    time_add_test(NAME test_time_format     TEST tests/ui/test_time_format.c      DEPS time_format)
    time_add_test(NAME test_card_layout     TEST tests/ui/test_card_layout.c      DEPS card_layout)
    time_add_test(NAME test_tzolkin_card    TEST tests/ui/test_tzolkin_card.c     DEPS tzolkin_card tzolkin julian)
    time_add_test(NAME test_astro_summary   TEST tests/ui/test_astro_summary.c
        DEPS astro_summary astro_fmt zodiac dignity aspects planets orbit kepler)
    time_add_test(NAME test_golden_layout   TEST tests/ui/test_golden_layout.c   DEPS golden_layout)
    time_add_test(NAME test_theme           TEST tests/ui/test_theme.c           DEPS theme golden_layout color_theory color)

    # Cross-system validation test
    time_add_test(NAME test_cross_validation TEST tests/systems/test_cross_validation.c
        DEPS julian sidereal ecliptic kepler orbit planets gregorian zodiac aspects observer houses tzolkin chinese)

endif()
