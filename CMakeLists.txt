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
    src/core/input.c
    src/core/hud.c
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
    src/render/planet_surface_pack.c
    src/render/orbit_trail_pack.c
    src/render/saturn_ring_pack.c
    src/render/zodiac_pack.c
    src/render/bodygraph_pack.c
    src/render/card_pack.c
    src/render/moon_pack.c
    src/render/diffraction.c
    src/render/deep_sky_pack.c
    src/render/milkyway_pack.c
    src/render/earth_globe.c
    src/render/earth_atmosphere.c
    src/render/earth_pack.c
    src/render/tree_pack.c
    src/render/eclipse_geometry.c
    src/render/catalog_ingest.c
    src/render/text_render.c
    src/render/hexagram_geometry.c
    src/render/mesh_shader.c
    src/render/noise_shader.c
    src/render/sun_shader.c
    src/render/constellation_bounds.c
    src/render/horizon_geometry.c
    src/render/weather_overlay.c
    src/render/post_process.c
    src/render/seasonal_lighting.c
    src/render/passes/star_pass.c
    src/render/passes/planet_pass.c
    src/render/passes/zodiac_pass.c
    src/render/passes/diffraction_pass.c
    src/render/passes/constellation_pass.c
    src/render/passes/saturn_pass.c
    src/render/passes/milkyway_pass.c
    src/render/passes/moon_pass.c
    src/render/passes/earth_pass.c
    src/render/passes/deep_sky_pass.c
    src/render/passes/bodygraph_pass.c
    src/render/passes/hexagram_pass.c
    src/render/passes/tree_pass.c
    src/render/passes/orbit_trail_pass.c
    src/render/passes/card_pass.c
    src/render/passes/post_pass.c
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
    src/systems/tzolkin/kin_social.c
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
    # Persian
    src/systems/persian/persian.c
    # Bahai
    src/systems/bahai/bahai.c
    # Japanese
    src/systems/japanese/japanese.c
    # Aztec
    src/systems/aztec/aztec.c
    # French Republican
    src/systems/french_republican/french_republican.c
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
    src/systems/earth/interest_profile.c
    src/systems/earth/tidal.c
    src/systems/earth/wind_patterns.c
    src/systems/earth/tide_predict.c
    src/systems/earth/location_alerts.c
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
    src/systems/unified/time_philosophy.c
    src/systems/unified/calendar_convert.c
    src/systems/unified/lunar_harmonics.c
    # Celtic
    src/systems/celtic/wheel_of_year.c
    # Chakra
    src/systems/chakra/chakra.c
    # Coptic
    src/systems/coptic/coptic.c
    # Celtic Tree
    src/systems/celtic/celtic_tree.c
    # Korean
    src/systems/korean/korean_calendar.c
    # Balinese
    src/systems/balinese/pawukon.c
    # Tamil
    src/systems/tamil/tamil_calendar.c
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
    src/ui/help_overlay.c
    src/ui/ui_state.c
    src/ui/layer_panel.c
    src/ui/command_palette.c
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
        -sSTACK_SIZE=524288
        -sALLOW_MEMORY_GROWTH=1
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
    add_library(time_bodygraph_pack  OBJECT src/render/bodygraph_pack.c)
    add_library(time_card_pack       OBJECT src/render/card_pack.c)
    add_library(time_catalog_ingest  OBJECT src/render/catalog_ingest.c)
    add_library(time_diffraction     OBJECT src/render/diffraction.c)
    add_library(time_earth_atmosphere OBJECT src/render/earth_atmosphere.c)
    add_library(time_earth_globe     OBJECT src/render/earth_globe.c)
    add_library(time_earth_pack      OBJECT src/render/earth_pack.c)
    add_library(time_eclipse_geometry OBJECT src/render/eclipse_geometry.c)
    add_library(time_tree_pack       OBJECT src/render/tree_pack.c)
    add_library(time_hexagram_geometry OBJECT src/render/hexagram_geometry.c)
    add_library(time_milkyway_pack   OBJECT src/render/milkyway_pack.c)
    add_library(time_moon_pack       OBJECT src/render/moon_pack.c)
    add_library(time_orbit_trail_pack OBJECT src/render/orbit_trail_pack.c)
    add_library(time_planet_pack     OBJECT src/render/planet_pack.c)
    add_library(time_planet_surface_pack OBJECT src/render/planet_surface_pack.c)
    add_library(time_saturn_ring_pack OBJECT src/render/saturn_ring_pack.c)
    add_library(time_star_field      OBJECT src/render/star_field.c)
    add_library(time_text_render     OBJECT src/render/text_render.c)
    add_library(time_zodiac_pack     OBJECT src/render/zodiac_pack.c)
    add_library(time_mesh_shader    OBJECT src/render/mesh_shader.c)
    add_library(time_noise_shader   OBJECT src/render/noise_shader.c)
    add_library(time_sun_shader     OBJECT src/render/sun_shader.c)
    add_library(time_constellation_bounds OBJECT src/render/constellation_bounds.c)
    add_library(time_horizon_geometry OBJECT src/render/horizon_geometry.c)
    add_library(time_weather_overlay OBJECT src/render/weather_overlay.c)
    add_library(time_post_process OBJECT src/render/post_process.c)
    add_library(time_seasonal_lighting OBJECT src/render/seasonal_lighting.c)
    add_library(time_shader_builder OBJECT src/render/shader_builder.c)
    add_library(time_billboard_shader OBJECT src/render/billboard_shader.c)
    add_library(time_shader_constants OBJECT src/render/shader_constants.c)
    add_library(time_lens_flare OBJECT src/render/lens_flare.c)
    add_library(time_click_target OBJECT src/render/click_target.c)

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
    add_library(time_dreamspell      OBJECT src/systems/tzolkin/dreamspell.c)
    add_library(time_tzolkin_board   OBJECT src/systems/tzolkin/tzolkin_board.c)
    add_library(time_kin_social      OBJECT src/systems/tzolkin/kin_social.c)
    add_library(time_iching          OBJECT src/systems/iching/iching.c)
    add_library(time_chinese         OBJECT src/systems/chinese/chinese.c)
    add_library(time_numerology      OBJECT src/systems/numerology/numerology.c)
    add_library(time_human_design    OBJECT src/systems/human_design/human_design.c)
    add_library(time_bodygraph       OBJECT src/systems/human_design/bodygraph.c)
    add_library(time_hebrew          OBJECT src/systems/hebrew/hebrew.c)
    add_library(time_sabbatical      OBJECT src/systems/hebrew/sabbatical.c)
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

    # Systems — Persian
    add_library(time_persian         OBJECT src/systems/persian/persian.c)

    # Systems — Bahai
    add_library(time_bahai           OBJECT src/systems/bahai/bahai.c)

    # Systems — Japanese
    add_library(time_japanese        OBJECT src/systems/japanese/japanese.c)

    # Systems — Aztec
    add_library(time_aztec           OBJECT src/systems/aztec/aztec.c)

    # Systems — French Republican
    add_library(time_french_republican OBJECT src/systems/french_republican/french_republican.c)

    # Systems — Geology
    add_library(time_geo_time        OBJECT src/systems/geology/geo_time.c)
    add_library(time_climate_history OBJECT src/systems/geology/climate_history.c)
    add_library(time_fossil_milestones OBJECT src/systems/geology/fossil_milestones.c)
    add_library(time_tectonic        OBJECT src/systems/geology/tectonic.c)
    add_library(time_radiometric     OBJECT src/systems/geology/radiometric.c)
    add_library(time_thermo          OBJECT src/systems/geology/thermo.c)

    # Systems — Earth
    add_library(time_biorhythm       OBJECT src/systems/earth/biorhythm.c)
    add_library(time_daylight        OBJECT src/systems/earth/daylight.c)
    add_library(time_seasons         OBJECT src/systems/earth/seasons.c)
    add_library(time_ski_resorts     OBJECT src/systems/earth/ski_resorts.c)
    add_library(time_snow_season     OBJECT src/systems/earth/snow_season.c)
    add_library(time_solar_radiation OBJECT src/systems/earth/solar_radiation.c)
    add_library(time_storm_data      OBJECT src/systems/earth/storm_data.c)
    add_library(time_surf_spots        OBJECT src/systems/earth/surf_spots.c)
    add_library(time_interest_profile OBJECT src/systems/earth/interest_profile.c)
    add_library(time_tidal           OBJECT src/systems/earth/tidal.c)
    add_library(time_wind_patterns   OBJECT src/systems/earth/wind_patterns.c)
    add_library(time_tide_predict    OBJECT src/systems/earth/tide_predict.c)
    add_library(time_location_alerts OBJECT src/systems/earth/location_alerts.c)

    # Systems — Unified
    add_library(time_structural_map  OBJECT src/systems/unified/structural_map.c)
    add_library(time_cycle_analysis  OBJECT src/systems/unified/cycle_analysis.c)
    add_library(time_calendar_epoch  OBJECT src/systems/unified/calendar_epoch.c)
    add_library(time_calendar_politics OBJECT src/systems/unified/calendar_politics.c)
    add_library(time_convergence     OBJECT src/systems/unified/convergence.c)
    add_library(time_convergence_detect OBJECT src/systems/unified/convergence_detect.c)
    add_library(time_earth_drama     OBJECT src/systems/unified/earth_drama.c)
    add_library(time_frequency       OBJECT src/systems/unified/frequency.c)
    add_library(time_precession_detect OBJECT src/systems/unified/precession_detect.c)
    add_library(time_wisdom          OBJECT src/systems/unified/wisdom.c)
    add_library(time_wisdom_engine   OBJECT src/systems/unified/wisdom_engine.c)
    add_library(time_fun_facts       OBJECT src/systems/unified/fun_facts.c)
    add_library(time_achievement     OBJECT src/systems/unified/achievement.c)
    add_library(time_audio_data      OBJECT src/systems/unified/audio_data.c)
    add_library(time_calendar_reform OBJECT src/systems/unified/calendar_reform.c)
    add_library(time_codon_hexagram  OBJECT src/systems/unified/codon_hexagram.c)
    add_library(time_cultural_stories OBJECT src/systems/unified/cultural_stories.c)
    add_library(time_knowledge_graph OBJECT src/systems/unified/knowledge_graph.c)
    add_library(time_number_scanner  OBJECT src/systems/unified/number_scanner.c)
    add_library(time_phase_space     OBJECT src/systems/unified/phase_space.c)
    add_library(time_ratio_analysis  OBJECT src/systems/unified/ratio_analysis.c)
    add_library(time_sacred_numbers  OBJECT src/systems/unified/sacred_numbers.c)
    add_library(time_time_philosophy OBJECT src/systems/unified/time_philosophy.c)
    add_library(time_birth_profile  OBJECT src/systems/unified/birth_profile.c)
    add_library(time_festival_detector OBJECT src/systems/unified/festival_detector.c)
    add_library(time_calendar_convert OBJECT src/systems/unified/calendar_convert.c)
    add_library(time_lunar_harmonics OBJECT src/systems/unified/lunar_harmonics.c)

    # Systems — Celtic
    add_library(time_wheel_of_year   OBJECT src/systems/celtic/wheel_of_year.c)

    # Systems — Chakra
    add_library(time_chakra          OBJECT src/systems/chakra/chakra.c)

    # Systems — Coptic
    add_library(time_coptic          OBJECT src/systems/coptic/coptic.c)

    # Systems — Celtic Tree
    add_library(time_celtic_tree     OBJECT src/systems/celtic/celtic_tree.c)

    # Systems — Korean
    add_library(time_korean_calendar OBJECT src/systems/korean/korean_calendar.c)

    # Systems — Balinese
    add_library(time_pawukon         OBJECT src/systems/balinese/pawukon.c)

    # Systems — Tamil
    add_library(time_tamil_calendar  OBJECT src/systems/tamil/tamil_calendar.c)

    # Systems — Egyptian
    add_library(time_egyptian        OBJECT src/systems/egyptian/egyptian.c)

    # Systems — Ethiopian
    add_library(time_ethiopian       OBJECT src/systems/ethiopian/ethiopian.c)

    # Systems — Myanmar
    add_library(time_myanmar         OBJECT src/systems/myanmar/myanmar.c)

    # Systems — Thai
    add_library(time_thai_calendar   OBJECT src/systems/thai/thai_calendar.c)

    # Systems — Zoroastrian
    add_library(time_cosmic_duality  OBJECT src/systems/zoroastrian/cosmic_duality.c)
    add_library(time_zoroastrian     OBJECT src/systems/zoroastrian/zoroastrian.c)

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
    add_library(time_accessibility   OBJECT src/ui/accessibility.c)
    add_library(time_chinese_fmt     OBJECT src/ui/chinese_fmt.c)
    add_library(time_hd_card         OBJECT src/ui/hd_card.c)
    add_library(time_i18n            OBJECT src/ui/i18n.c)
    add_library(time_iching_card     OBJECT src/ui/iching_card.c)
    add_library(time_lunar_display   OBJECT src/ui/lunar_display.c)
    add_library(time_rtl_layout      OBJECT src/ui/rtl_layout.c)
    add_library(time_scroll_layers   OBJECT src/ui/scroll_layers.c)
    add_library(time_help_overlay   OBJECT src/ui/help_overlay.c)
    add_library(time_ui_state       OBJECT src/ui/ui_state.c)
    add_library(time_layer_panel    OBJECT src/ui/layer_panel.c)
    add_library(time_command_palette OBJECT src/ui/command_palette.c)

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
    time_add_test(NAME test_milkyway_pack  TEST tests/render/test_milkyway_pack.c  DEPS milkyway_pack noise_shader shader_builder)
    time_add_test(NAME test_bodygraph_pack TEST tests/render/test_bodygraph_pack.c DEPS bodygraph_pack bodygraph)
    time_add_test(NAME test_earth_pack    TEST tests/render/test_earth_pack.c    DEPS earth_pack earth_globe earth_atmosphere)
    time_add_test(NAME test_tree_pack    TEST tests/render/test_tree_pack.c    DEPS tree_pack tree_geometry sefirot)
    time_add_test(NAME test_eclipse_geometry TEST tests/render/test_eclipse_geometry.c DEPS eclipse_geometry lunar solar_events)
    time_add_test(NAME test_glyph_batch     TEST tests/render/test_glyph_batch.c     DEPS glyph_batch vec3)
    time_add_test(NAME test_color_theory    TEST tests/render/test_color_theory.c    DEPS color_theory color)
    time_add_test(NAME test_star_catalog_ext TEST tests/render/test_star_catalog_ext.c DEPS star_catalog_ext)
    time_add_test(NAME test_decan_stars     TEST tests/render/test_decan_stars.c     DEPS decan_stars)
    time_add_test(NAME test_megalithic      TEST tests/render/test_megalithic.c      DEPS megalithic solar_events)
    time_add_test(NAME test_tarot_visual    TEST tests/render/test_tarot_visual.c    DEPS tarot_visual color_theory color tree_geometry sefirot)
    time_add_test(NAME test_moon_data       TEST tests/systems/test_moon_data.c      DEPS moon_data kepler)
    time_add_test(NAME test_card_pack      TEST tests/render/test_card_pack.c       DEPS card_pack card_layout)
    time_add_test(NAME test_catalog_ingest TEST tests/render/test_catalog_ingest.c  DEPS catalog_ingest)
    time_add_test(NAME test_diffraction    TEST tests/render/test_diffraction.c     DEPS diffraction)
    time_add_test(NAME test_earth_atmosphere TEST tests/render/test_earth_atmosphere.c DEPS earth_atmosphere)
    time_add_test(NAME test_earth_globe    TEST tests/render/test_earth_globe.c     DEPS earth_globe)
    time_add_test(NAME test_hexagram_geometry TEST tests/render/test_hexagram_geometry.c DEPS hexagram_geometry iching color_theory color)
    time_add_test(NAME test_moon_pack      TEST tests/render/test_moon_pack.c       DEPS moon_pack moon_data kepler planets orbit)
    time_add_test(NAME test_orbit_trail_pack TEST tests/render/test_orbit_trail_pack.c DEPS orbit_trail_pack orbit kepler planet_data)
    time_add_test(NAME test_planet_pack    TEST tests/render/test_planet_pack.c     DEPS planet_pack planets orbit kepler planet_data atmo_ring)
    time_add_test(NAME test_planet_surface_pack TEST tests/render/test_planet_surface_pack.c DEPS planet_surface_pack orbit kepler planet_data noise_shader shader_builder)
    time_add_test(NAME test_saturn_ring_pack TEST tests/render/test_saturn_ring_pack.c DEPS saturn_ring_pack)
    time_add_test(NAME test_star_field     TEST tests/render/test_star_field.c      DEPS star_field star_catalog star_catalog_ext constellation star_colors)
    time_add_test(NAME test_text_render    TEST tests/render/test_text_render.c     DEPS text_render)
    time_add_test(NAME test_zodiac_pack    TEST tests/render/test_zodiac_pack.c     DEPS zodiac_pack ring_geometry cusp_lines aspect_lines billboard color_palette color_theory color vec3 mat4)
    time_add_test(NAME test_constellation_bounds TEST tests/render/test_constellation_bounds.c DEPS constellation_bounds)
    time_add_test(NAME test_horizon_geometry TEST tests/render/test_horizon_geometry.c DEPS horizon_geometry)
    time_add_test(NAME test_weather_overlay TEST tests/render/test_weather_overlay.c DEPS weather_overlay wind_patterns storm_data)
    time_add_test(NAME test_post_process TEST tests/render/test_post_process.c DEPS post_process noise_shader)
    time_add_test(NAME test_seasonal_lighting TEST tests/render/test_seasonal_lighting.c DEPS seasonal_lighting)
    time_add_test(NAME test_shader_builder TEST tests/render/test_shader_builder.c DEPS shader_builder)
    time_add_test(NAME test_mesh_shader TEST tests/render/test_mesh_shader.c DEPS mesh_shader)
    time_add_test(NAME test_noise_shader TEST tests/render/test_noise_shader.c DEPS noise_shader)
    time_add_test(NAME test_sun_shader TEST tests/render/test_sun_shader.c DEPS sun_shader noise_shader shader_builder)
    time_add_test(NAME test_billboard_shader TEST tests/render/test_billboard_shader.c DEPS billboard_shader)
    time_add_test(NAME test_shader_constants TEST tests/render/test_shader_constants.c DEPS shader_constants)
    time_add_test(NAME test_lens_flare TEST tests/render/test_lens_flare.c DEPS lens_flare)
    time_add_test(NAME test_click_target TEST tests/render/test_click_target.c DEPS click_target)

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
    time_add_test(NAME test_sabbatical     TEST tests/systems/hebrew/test_sabbatical.c    DEPS sabbatical hebrew)
    time_add_test(NAME test_dreamspell     TEST tests/systems/tzolkin/test_dreamspell.c   DEPS dreamspell)
    time_add_test(NAME test_tzolkin_board  TEST tests/systems/tzolkin/test_tzolkin_board.c DEPS tzolkin_board)
    time_add_test(NAME test_kin_social    TEST tests/systems/tzolkin/test_kin_social.c   DEPS kin_social dreamspell)
    time_add_test(NAME test_coptic         TEST tests/systems/coptic/test_coptic.c        DEPS coptic)
    time_add_test(NAME test_egyptian       TEST tests/systems/egyptian/test_egyptian.c    DEPS egyptian)
    time_add_test(NAME test_ethiopian      TEST tests/systems/ethiopian/test_ethiopian.c  DEPS ethiopian)
    time_add_test(NAME test_wheel_of_year  TEST tests/systems/celtic/test_wheel_of_year.c DEPS wheel_of_year)
    time_add_test(NAME test_chakra         TEST tests/systems/chakra/test_chakra.c        DEPS chakra)
    time_add_test(NAME test_myanmar        TEST tests/systems/myanmar/test_myanmar.c      DEPS myanmar)
    time_add_test(NAME test_thai_calendar  TEST tests/systems/thai/test_thai_calendar.c   DEPS thai_calendar)
    time_add_test(NAME test_cosmic_duality TEST tests/systems/zoroastrian/test_cosmic_duality.c DEPS cosmic_duality)
    time_add_test(NAME test_zoroastrian    TEST tests/systems/zoroastrian/test_zoroastrian.c   DEPS zoroastrian)

    # Tarot tests
    time_add_test(NAME test_tarot           TEST tests/systems/tarot/test_tarot.c   DEPS tarot)

    # Persian tests
    time_add_test(NAME test_persian         TEST tests/systems/persian/test_persian.c DEPS persian)

    # Baha'i tests
    time_add_test(NAME test_bahai           TEST tests/systems/bahai/test_bahai.c     DEPS bahai)

    # Japanese tests
    time_add_test(NAME test_japanese        TEST tests/systems/japanese/test_japanese.c DEPS japanese)

    # Aztec tests
    time_add_test(NAME test_aztec           TEST tests/systems/aztec/test_aztec.c      DEPS aztec)

    # Tamil tests
    time_add_test(NAME test_tamil_calendar  TEST tests/systems/tamil/test_tamil_calendar.c DEPS tamil_calendar)

    # Celtic Tree tests
    time_add_test(NAME test_celtic_tree     TEST tests/systems/celtic/test_celtic_tree.c   DEPS celtic_tree)

    # Korean tests
    time_add_test(NAME test_korean_calendar TEST tests/systems/korean/test_korean_calendar.c DEPS korean_calendar)

    # Balinese tests
    time_add_test(NAME test_pawukon         TEST tests/systems/balinese/test_pawukon.c       DEPS pawukon)

    # French Republican tests
    time_add_test(NAME test_french_republican TEST tests/systems/french_republican/test_french_republican.c DEPS french_republican)

    # Geology tests
    time_add_test(NAME test_geo_time        TEST tests/systems/test_geo_time.c              DEPS geo_time)
    time_add_test(NAME test_climate_history TEST tests/systems/geology/test_climate_history.c DEPS climate_history)
    time_add_test(NAME test_fossil_milestones TEST tests/systems/geology/test_fossil_milestones.c DEPS fossil_milestones)
    time_add_test(NAME test_tectonic        TEST tests/systems/geology/test_tectonic.c      DEPS tectonic)
    time_add_test(NAME test_radiometric    TEST tests/systems/geology/test_radiometric.c   DEPS radiometric)
    time_add_test(NAME test_thermo         TEST tests/systems/geology/test_thermo.c        DEPS thermo)

    # Earth tests
    time_add_test(NAME test_biorhythm       TEST tests/systems/earth/test_biorhythm.c       DEPS biorhythm)
    time_add_test(NAME test_daylight        TEST tests/systems/earth/test_daylight.c        DEPS daylight solar_events)
    time_add_test(NAME test_seasons         TEST tests/systems/earth/test_seasons.c         DEPS seasons)
    time_add_test(NAME test_ski_resorts     TEST tests/systems/earth/test_ski_resorts.c     DEPS ski_resorts)
    time_add_test(NAME test_snow_season     TEST tests/systems/earth/test_snow_season.c     DEPS snow_season)
    time_add_test(NAME test_solar_radiation TEST tests/systems/earth/test_solar_radiation.c DEPS solar_radiation)
    time_add_test(NAME test_storm_data      TEST tests/systems/earth/test_storm_data.c      DEPS storm_data)
    time_add_test(NAME test_surf_spots      TEST tests/systems/earth/test_surf_spots.c      DEPS surf_spots)
    time_add_test(NAME test_interest_profile TEST tests/systems/earth/test_interest_profile.c DEPS interest_profile)
    time_add_test(NAME test_tidal           TEST tests/systems/earth/test_tidal.c           DEPS tidal)
    time_add_test(NAME test_wind_patterns   TEST tests/systems/earth/test_wind_patterns.c   DEPS wind_patterns)
    time_add_test(NAME test_tide_predict    TEST tests/systems/earth/test_tide_predict.c    DEPS tide_predict tidal surf_spots lunar julian)
    time_add_test(NAME test_location_alerts TEST tests/systems/earth/test_location_alerts.c DEPS location_alerts tide_predict tidal surf_spots ski_resorts snow_season daylight solar_events solar_radiation prayer_times storm_data convergence_detect julian lunar tzolkin tzolkin_board iching chinese hebrew hijri)

    # Tzolkin extended tests
    time_add_test(NAME test_cr_cycle        TEST tests/systems/tzolkin/test_cr_cycle.c      DEPS cr_cycle haab tzolkin julian)

    # Unified tests
    time_add_test(NAME test_structural_map  TEST tests/systems/unified/test_structural_map.c  DEPS structural_map)
    time_add_test(NAME test_cycle_analysis  TEST tests/systems/test_cycle_analysis.c          DEPS cycle_analysis)
    time_add_test(NAME test_calendar_epoch  TEST tests/systems/unified/test_calendar_epoch.c  DEPS calendar_epoch)
    time_add_test(NAME test_calendar_politics TEST tests/systems/unified/test_calendar_politics.c DEPS calendar_politics)
    time_add_test(NAME test_convergence     TEST tests/systems/unified/test_convergence.c     DEPS convergence)
    time_add_test(NAME test_convergence_detect TEST tests/systems/unified/test_convergence_detect.c DEPS convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri)
    time_add_test(NAME test_earth_drama     TEST tests/systems/unified/test_earth_drama.c     DEPS earth_drama climate_history fossil_milestones geo_time)
    time_add_test(NAME test_frequency       TEST tests/systems/unified/test_frequency.c       DEPS frequency)
    time_add_test(NAME test_precession_detect TEST tests/systems/unified/test_precession_detect.c DEPS precession_detect)
    time_add_test(NAME test_wisdom          TEST tests/systems/unified/test_wisdom.c          DEPS wisdom)
    time_add_test(NAME test_wisdom_engine   TEST tests/systems/unified/test_wisdom_engine.c   DEPS wisdom_engine wisdom)
    time_add_test(NAME test_fun_facts       TEST tests/systems/unified/test_fun_facts.c       DEPS fun_facts)
    time_add_test(NAME test_achievement    TEST tests/systems/unified/test_achievement.c    DEPS achievement)
    time_add_test(NAME test_audio_data     TEST tests/systems/unified/test_audio_data.c     DEPS audio_data frequency)
    time_add_test(NAME test_calendar_reform TEST tests/systems/unified/test_calendar_reform.c DEPS calendar_reform)
    time_add_test(NAME test_codon_hexagram TEST tests/systems/unified/test_codon_hexagram.c DEPS codon_hexagram)
    time_add_test(NAME test_cultural_stories TEST tests/systems/unified/test_cultural_stories.c DEPS cultural_stories)
    time_add_test(NAME test_knowledge_graph TEST tests/systems/unified/test_knowledge_graph.c DEPS knowledge_graph)
    time_add_test(NAME test_number_scanner TEST tests/systems/unified/test_number_scanner.c DEPS number_scanner)
    time_add_test(NAME test_phase_space    TEST tests/systems/unified/test_phase_space.c    DEPS phase_space)
    time_add_test(NAME test_ratio_analysis TEST tests/systems/unified/test_ratio_analysis.c DEPS ratio_analysis)
    time_add_test(NAME test_sacred_numbers TEST tests/systems/unified/test_sacred_numbers.c DEPS sacred_numbers)
    time_add_test(NAME test_time_philosophy TEST tests/systems/unified/test_time_philosophy.c DEPS time_philosophy)
    time_add_test(NAME test_birth_profile TEST tests/systems/unified/test_birth_profile.c
        DEPS birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
    time_add_test(NAME test_festival_detector TEST tests/systems/unified/test_festival_detector.c
        DEPS festival_detector korean_calendar hebrew sabbatical hijri buddhist lunar wheel_of_year thai_calendar tamil_calendar myanmar persian zoroastrian)
    time_add_test(NAME test_calendar_convert TEST tests/systems/unified/test_calendar_convert.c
        DEPS calendar_convert julian tzolkin haab cr_cycle chinese iching hebrew hijri persian coptic ethiopian egyptian french_republican japanese bahai korean_calendar thai_calendar tamil_calendar myanmar zoroastrian celtic_tree)
    time_add_test(NAME test_lunar_harmonics TEST tests/systems/unified/test_lunar_harmonics.c
        DEPS lunar_harmonics lunar julian)

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
    time_add_test(NAME test_accessibility  TEST tests/ui/test_accessibility.c   DEPS accessibility)
    time_add_test(NAME test_astro_fmt      TEST tests/ui/test_astro_fmt.c       DEPS astro_fmt)
    time_add_test(NAME test_chinese_fmt    TEST tests/ui/test_chinese_fmt.c     DEPS chinese_fmt chinese julian)
    time_add_test(NAME test_hd_card        TEST tests/ui/test_hd_card.c         DEPS hd_card human_design)
    time_add_test(NAME test_i18n           TEST tests/ui/test_i18n.c            DEPS i18n)
    time_add_test(NAME test_iching_card    TEST tests/ui/test_iching_card.c     DEPS iching_card hexagram_visual iching)
    time_add_test(NAME test_lunar_display  TEST tests/ui/test_lunar_display.c   DEPS lunar_display lunar julian zodiac)
    time_add_test(NAME test_rtl_layout     TEST tests/ui/test_rtl_layout.c      DEPS rtl_layout)
    time_add_test(NAME test_scroll_layers  TEST tests/ui/test_scroll_layers.c   DEPS scroll_layers)
    time_add_test(NAME test_help_overlay  TEST tests/ui/test_help_overlay.c   DEPS help_overlay)
    time_add_test(NAME test_ui_state     TEST tests/ui/test_ui_state.c       DEPS ui_state)
    time_add_test(NAME test_layer_panel TEST tests/ui/test_layer_panel.c   DEPS layer_panel)
    time_add_test(NAME test_command_palette TEST tests/ui/test_command_palette.c DEPS command_palette)

    # Cross-system validation test
    time_add_test(NAME test_cross_validation TEST tests/systems/test_cross_validation.c
        DEPS julian sidereal ecliptic kepler orbit planets gregorian zodiac aspects observer houses tzolkin chinese)

endif()
