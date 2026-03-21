# cmake/testing.cmake — Unity test framework integration + test registrations
#
# Provides time_add_test() for registering test executables
# with per-test link dependencies, plus all test registrations.

# Unity test framework (vendored)
add_library(unity STATIC tests/unity/unity.c)
target_include_directories(unity PUBLIC tests/unity)

# Register a test executable.
#
# Usage:
#   time_add_test(NAME test_vec3 TEST tests/math/test_vec3.c DEPS vec3)
#   time_add_test(NAME test_mat4 TEST tests/math/test_mat4.c DEPS mat4 vec3 LABELS unit)
#
# NAME:   test executable name (also CTest name)
# TEST:   path to test source file
# DEPS:   list of OBJECT library names (without time_ prefix)
# LABELS: optional test labels (unit, contract, integration, smoke)
function(time_add_test)
    cmake_parse_arguments(T "" "NAME;TEST" "DEPS;LABELS" ${ARGN})

    # Build dep target names: "vec3" -> "time_vec3"
    set(_link_targets "")
    foreach(_dep ${T_DEPS})
        list(APPEND _link_targets "time_${_dep}")
    endforeach()

    add_executable(${T_NAME} ${T_TEST})
    target_link_libraries(${T_NAME} PRIVATE unity ${_link_targets} m)
    target_include_directories(${T_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Unity test functions (void test_xxx) lack prototypes by design
    target_compile_options(${T_NAME} PRIVATE -Wno-missing-prototypes -Wno-missing-declarations)
    add_test(NAME ${T_NAME} COMMAND ${T_NAME})

    # 10-second timeout prevents hanging tests from blocking CI
    set_tests_properties(${T_NAME} PROPERTIES TIMEOUT 10)

    # Apply labels if provided
    if(T_LABELS)
        set_tests_properties(${T_NAME} PROPERTIES LABELS "${T_LABELS}")
    endif()
endfunction()

# ===========================================================================
# Test registrations — 208 tests
# ===========================================================================

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
time_add_test(NAME test_spring          TEST tests/math/test_spring.c          DEPS spring          LABELS unit)
time_add_test(NAME test_vec_interp     TEST tests/math/test_vec_interp.c     DEPS vec_interp vec3 spring LABELS unit)
time_add_test(NAME test_equatorial_horizontal TEST tests/math/test_equatorial_horizontal.c DEPS equatorial_horizontal ecliptic sidereal)

# Render tests
time_add_test(NAME test_camera          TEST tests/render/test_camera.c          DEPS camera vec3 mat4)
time_add_test(NAME test_camera_scale    TEST tests/render/test_camera_scale.c    DEPS camera_scale easing)
time_add_test(NAME test_render_layers   TEST tests/render/test_render_layers.c   DEPS render_layers camera_scale easing golden_layout)
time_add_test(NAME test_font_atlas      TEST tests/render/test_font_atlas.c      DEPS font_atlas)
time_add_test(NAME test_font_bitmap    TEST tests/render/test_font_bitmap.c    DEPS font_bitmap)
time_add_test(NAME test_color_palette   TEST tests/render/test_color_palette.c   DEPS color_palette color_theory color)
time_add_test(NAME test_ring_geometry   TEST tests/render/test_ring_geometry.c   DEPS ring_geometry)
time_add_test(NAME test_concentric_ring TEST tests/render/test_concentric_ring.c DEPS concentric_ring)
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
time_add_test(NAME test_horizon_camera  TEST tests/render/test_horizon_camera.c  DEPS horizon_camera mat4 vec3)
time_add_test(NAME test_sky_dome        TEST tests/render/test_sky_dome.c        DEPS sky_dome earth_atmosphere)
time_add_test(NAME test_earth_view_frame TEST tests/render/test_earth_view_frame.c DEPS earth_view_frame horizon_camera horizon_geometry sky_dome earth_atmosphere solar_events ecliptic sidereal mat4 vec3)
time_add_test(NAME test_weather_overlay TEST tests/render/test_weather_overlay.c DEPS weather_overlay wind_patterns storm_data)
time_add_test(NAME test_post_process TEST tests/render/test_post_process.c DEPS post_process noise_shader)
time_add_test(NAME test_shared_uniforms TEST tests/render/test_shared_uniforms.c DEPS shared_uniforms)
time_add_test(NAME test_seasonal_lighting TEST tests/render/test_seasonal_lighting.c DEPS seasonal_lighting)
time_add_test(NAME test_shader_builder TEST tests/render/test_shader_builder.c DEPS shader_builder)
time_add_test(NAME test_mesh_shader TEST tests/render/test_mesh_shader.c DEPS mesh_shader)
time_add_test(NAME test_noise_shader TEST tests/render/test_noise_shader.c DEPS noise_shader)
time_add_test(NAME test_sun_shader TEST tests/render/test_sun_shader.c DEPS sun_shader noise_shader shader_builder)
time_add_test(NAME test_billboard_shader TEST tests/render/test_billboard_shader.c DEPS billboard_shader)
time_add_test(NAME test_shader_constants TEST tests/render/test_shader_constants.c DEPS shader_constants)
time_add_test(NAME test_lens_flare TEST tests/render/test_lens_flare.c DEPS lens_flare)
time_add_test(NAME test_click_target TEST tests/render/test_click_target.c DEPS click_target)
time_add_test(NAME test_camera_motion TEST tests/render/test_camera_motion.c DEPS camera_motion LABELS unit)
time_add_test(NAME test_scale_spring TEST tests/render/test_scale_spring.c DEPS scale_spring spring camera_scale easing LABELS unit)
time_add_test(NAME test_camera_path TEST tests/render/test_camera_path.c DEPS camera_path spring vec_interp vec3 easing LABELS unit)
time_add_test(NAME test_render_lod TEST tests/render/test_render_lod.c DEPS render_lod)
time_add_test(NAME test_system_rings TEST tests/render/test_system_rings.c DEPS system_rings concentric_ring color_theory color)
time_add_test(NAME test_convergence_visual TEST tests/render/test_convergence_visual.c DEPS convergence_visual concentric_ring)
time_add_test(NAME test_pass_schedule TEST tests/render/test_pass_schedule.c DEPS pass_schedule)
time_add_test(NAME test_view_state TEST tests/render/test_view_state.c DEPS view_state pass_schedule)
time_add_test(NAME test_ring_today TEST tests/render/test_ring_today.c
    DEPS ring_today system_rings concentric_ring color_theory color gregorian julian tzolkin haab chinese hebrew hijri buddhist lunar nakshatra iching zodiac human_design coptic ethiopian persian japanese korean_calendar thai_calendar geo_time calendar_fixed)
time_add_test(NAME test_birth_sky TEST tests/render/test_birth_sky.c
    DEPS birth_sky ring_today system_rings concentric_ring color_theory color birth_profile gregorian julian tzolkin dreamspell haab chinese hebrew hijri buddhist lunar nakshatra iching zodiac aspects human_design planets orbit kepler coptic ethiopian persian japanese korean_calendar thai_calendar geo_time calendar_fixed)

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
time_add_test(NAME test_astronomy_interpret TEST tests/systems/astronomy/test_astronomy_interpret.c DEPS astronomy_interpret content_i18n i18n)
time_add_test(NAME test_astronomy_i18n TEST tests/systems/astronomy/test_astronomy_i18n.c DEPS astronomy_interpret content_i18n i18n)

# Astrology tests
time_add_test(NAME test_zodiac          TEST tests/systems/test_zodiac.c         DEPS zodiac)
time_add_test(NAME test_aspects         TEST tests/systems/test_aspects.c        DEPS aspects)
time_add_test(NAME test_observer        TEST tests/systems/test_observer.c       DEPS observer)
time_add_test(NAME test_houses          TEST tests/systems/test_houses.c         DEPS houses zodiac)
time_add_test(NAME test_dignity         TEST tests/systems/test_dignity.c        DEPS dignity)
time_add_test(NAME test_planetary_hours TEST tests/systems/test_planetary_hours.c DEPS planetary_hours)
time_add_test(NAME test_astro_wheel     TEST tests/systems/test_wheel_layout.c   DEPS astro_wheel vec3)
time_add_test(NAME test_astrology_interpret TEST tests/systems/astrology/test_astrology_interpret.c DEPS astrology_interpret zodiac content_i18n i18n)
time_add_test(NAME test_astrology_i18n TEST tests/systems/astrology/test_astrology_i18n.c DEPS astrology_interpret zodiac content_i18n i18n)
time_add_test(NAME test_astrology_today TEST tests/systems/astrology/test_astrology_today.c DEPS astrology_today zodiac astrology_interpret planetary_hours aspects dignity content_i18n i18n)

# Calendar tests
time_add_test(NAME test_gregorian       TEST tests/systems/test_gregorian.c      DEPS gregorian julian)
time_add_test(NAME test_gregorian_interpret TEST tests/systems/gregorian/test_gregorian_interpret.c DEPS gregorian_interpret content_i18n i18n)
time_add_test(NAME test_gregorian_i18n TEST tests/systems/gregorian/test_gregorian_i18n.c DEPS gregorian_interpret content_i18n i18n)
time_add_test(NAME test_gregorian_today TEST tests/systems/gregorian/test_gregorian_today.c DEPS gregorian_today gregorian gregorian_interpret content_i18n i18n julian)
time_add_test(NAME test_tzolkin         TEST tests/systems/test_tzolkin.c        DEPS tzolkin julian)
time_add_test(NAME test_haab            TEST tests/systems/test_haab.c           DEPS haab tzolkin julian)
time_add_test(NAME test_chinese         TEST tests/systems/test_chinese.c        DEPS chinese julian)
time_add_test(NAME test_chinese_interpret TEST tests/systems/chinese/test_chinese_interpret.c DEPS chinese_interpret chinese julian content_i18n i18n)
time_add_test(NAME test_chinese_i18n TEST tests/systems/chinese/test_chinese_i18n.c DEPS chinese_interpret chinese julian content_i18n i18n)
time_add_test(NAME test_chinese_today TEST tests/systems/chinese/test_chinese_today.c DEPS chinese_today chinese chinese_interpret julian content_i18n i18n)
time_add_test(NAME test_iching          TEST tests/systems/test_iching.c         DEPS iching)
time_add_test(NAME test_iching_interpret TEST tests/systems/iching/test_iching_interpret.c DEPS iching_interpret iching content_i18n i18n)
time_add_test(NAME test_iching_i18n TEST tests/systems/iching/test_iching_i18n.c DEPS iching_interpret iching content_i18n i18n)
time_add_test(NAME test_iching_today TEST tests/systems/iching/test_iching_today.c DEPS iching_today iching iching_interpret content_i18n i18n)
time_add_test(NAME test_numerology      TEST tests/systems/test_numerology.c     DEPS numerology)
time_add_test(NAME test_numerology_interpret TEST tests/systems/numerology/test_numerology_interpret.c DEPS numerology_interpret content_i18n i18n)
time_add_test(NAME test_numerology_i18n TEST tests/systems/numerology/test_numerology_i18n.c DEPS numerology_interpret content_i18n i18n)
time_add_test(NAME test_human_design    TEST tests/systems/test_human_design.c   DEPS human_design)
time_add_test(NAME test_bodygraph       TEST tests/systems/test_bodygraph.c      DEPS bodygraph)
time_add_test(NAME test_human_design_interpret TEST tests/systems/human_design/test_human_design_interpret.c DEPS human_design_interpret human_design content_i18n i18n)
time_add_test(NAME test_human_design_i18n TEST tests/systems/human_design/test_human_design_i18n.c DEPS human_design_interpret human_design content_i18n i18n)
time_add_test(NAME test_human_design_today TEST tests/systems/human_design/test_human_design_today.c DEPS human_design_today human_design human_design_interpret bodygraph content_i18n i18n)
time_add_test(NAME test_hebrew          TEST tests/systems/hebrew/test_hebrew.c  DEPS hebrew)
time_add_test(NAME test_hijri           TEST tests/systems/islamic/test_hijri.c  DEPS hijri)
time_add_test(NAME test_prayer_times    TEST tests/systems/islamic/test_prayer_times.c DEPS prayer_times solar_events julian)
time_add_test(NAME test_islamic_interpret TEST tests/systems/islamic/test_islamic_interpret.c DEPS islamic_interpret hijri content_i18n i18n)
time_add_test(NAME test_islamic_i18n TEST tests/systems/islamic/test_islamic_i18n.c DEPS islamic_interpret hijri content_i18n i18n)
time_add_test(NAME test_islamic_today TEST tests/systems/islamic/test_islamic_today.c DEPS islamic_today hijri prayer_times islamic_interpret solar_events julian content_i18n i18n)
time_add_test(NAME test_buddhist        TEST tests/systems/buddhist/test_buddhist.c    DEPS buddhist lunar)
time_add_test(NAME test_kalpa           TEST tests/systems/buddhist/test_kalpa.c       DEPS kalpa)
time_add_test(NAME test_buddhist_interpret TEST tests/systems/buddhist/test_buddhist_interpret.c DEPS buddhist_interpret content_i18n i18n)
time_add_test(NAME test_buddhist_i18n TEST tests/systems/buddhist/test_buddhist_i18n.c DEPS buddhist_interpret content_i18n i18n)
time_add_test(NAME test_buddhist_today TEST tests/systems/buddhist/test_buddhist_today.c DEPS buddhist_today buddhist kalpa buddhist_interpret lunar julian content_i18n i18n)
time_add_test(NAME test_nakshatra       TEST tests/systems/test_nakshatra.c     DEPS nakshatra)
time_add_test(NAME test_panchanga       TEST tests/systems/hindu/test_panchanga.c DEPS panchanga nakshatra)
time_add_test(NAME test_yuga            TEST tests/systems/hindu/test_yuga.c     DEPS yuga)
time_add_test(NAME test_hindu_interpret TEST tests/systems/hindu/test_hindu_interpret.c DEPS hindu_interpret panchanga nakshatra content_i18n i18n)
time_add_test(NAME test_hindu_i18n TEST tests/systems/hindu/test_hindu_i18n.c DEPS hindu_interpret panchanga nakshatra content_i18n i18n)
time_add_test(NAME test_hindu_today TEST tests/systems/hindu/test_hindu_today.c DEPS hindu_today panchanga nakshatra yuga hindu_interpret content_i18n i18n)
time_add_test(NAME test_sefirot         TEST tests/systems/kabbalah/test_sefirot.c      DEPS sefirot)
time_add_test(NAME test_four_worlds     TEST tests/systems/kabbalah/test_four_worlds.c  DEPS four_worlds)
time_add_test(NAME test_tree_geometry   TEST tests/systems/kabbalah/test_tree_geometry.c DEPS tree_geometry sefirot)
time_add_test(NAME test_kabbalah_interpret TEST tests/systems/kabbalah/test_kabbalah_interpret.c DEPS kabbalah_interpret content_i18n i18n)
time_add_test(NAME test_kabbalah_i18n TEST tests/systems/kabbalah/test_kabbalah_i18n.c DEPS kabbalah_interpret content_i18n i18n)
time_add_test(NAME test_kabbalah_today TEST tests/systems/kabbalah/test_kabbalah_today.c DEPS kabbalah_today sefirot four_worlds tree_geometry kabbalah_interpret content_i18n i18n)
time_add_test(NAME test_sabbatical     TEST tests/systems/hebrew/test_sabbatical.c    DEPS sabbatical hebrew)
time_add_test(NAME test_hebrew_interpret TEST tests/systems/hebrew/test_hebrew_interpret.c DEPS hebrew_interpret hebrew content_i18n i18n)
time_add_test(NAME test_hebrew_i18n TEST tests/systems/hebrew/test_hebrew_i18n.c DEPS hebrew_interpret hebrew content_i18n i18n)
time_add_test(NAME test_hebrew_today TEST tests/systems/hebrew/test_hebrew_today.c DEPS hebrew_today hebrew sabbatical hebrew_interpret content_i18n i18n)
time_add_test(NAME test_dreamspell     TEST tests/systems/tzolkin/test_dreamspell.c   DEPS dreamspell)
time_add_test(NAME test_tzolkin_board  TEST tests/systems/tzolkin/test_tzolkin_board.c DEPS tzolkin_board)
time_add_test(NAME test_kin_social    TEST tests/systems/tzolkin/test_kin_social.c   DEPS kin_social dreamspell)
time_add_test(NAME test_tzolkin_interpret TEST tests/systems/tzolkin/test_tzolkin_interpret.c DEPS tzolkin_interpret dreamspell content_i18n i18n)
time_add_test(NAME test_tzolkin_i18n TEST tests/systems/tzolkin/test_tzolkin_i18n.c DEPS tzolkin_interpret dreamspell content_i18n i18n)
time_add_test(NAME test_tzolkin_today TEST tests/systems/tzolkin/test_tzolkin_today.c DEPS tzolkin_today tzolkin dreamspell tzolkin_board haab cr_cycle julian tzolkin_interpret content_i18n i18n)
time_add_test(NAME test_heptad_gate  TEST tests/systems/tzolkin/test_heptad_gate.c  DEPS heptad_gate)
time_add_test(NAME test_hunab_ku_21 TEST tests/systems/tzolkin/test_hunab_ku_21.c DEPS hunab_ku_21)
time_add_test(NAME test_lenses_of_time TEST tests/systems/tzolkin/test_lenses_of_time.c DEPS lenses_of_time)
time_add_test(NAME test_clear_signs  TEST tests/systems/tzolkin/test_clear_signs.c  DEPS clear_signs)
time_add_test(NAME test_telektonon_board TEST tests/systems/tzolkin/test_telektonon_board.c DEPS telektonon_board)
time_add_test(NAME test_coptic         TEST tests/systems/coptic/test_coptic.c        DEPS coptic calendar_fixed)
time_add_test(NAME test_coptic_interpret TEST tests/systems/coptic/test_coptic_interpret.c DEPS coptic_interpret content_i18n i18n)
time_add_test(NAME test_coptic_i18n TEST tests/systems/coptic/test_coptic_i18n.c DEPS coptic_interpret content_i18n i18n)
time_add_test(NAME test_egyptian       TEST tests/systems/egyptian/test_egyptian.c    DEPS egyptian calendar_fixed)
time_add_test(NAME test_egyptian_interpret TEST tests/systems/egyptian/test_egyptian_interpret.c DEPS egyptian_interpret content_i18n i18n)
time_add_test(NAME test_egyptian_i18n TEST tests/systems/egyptian/test_egyptian_i18n.c DEPS egyptian_interpret content_i18n i18n)
time_add_test(NAME test_ethiopian      TEST tests/systems/ethiopian/test_ethiopian.c  DEPS ethiopian calendar_fixed)
time_add_test(NAME test_ethiopian_interpret TEST tests/systems/ethiopian/test_ethiopian_interpret.c DEPS ethiopian_interpret content_i18n i18n)
time_add_test(NAME test_ethiopian_i18n TEST tests/systems/ethiopian/test_ethiopian_i18n.c DEPS ethiopian_interpret content_i18n i18n)
time_add_test(NAME test_wheel_of_year  TEST tests/systems/celtic/test_wheel_of_year.c DEPS wheel_of_year)
time_add_test(NAME test_chakra         TEST tests/systems/chakra/test_chakra.c        DEPS chakra)
time_add_test(NAME test_chakra_interpret TEST tests/systems/chakra/test_chakra_interpret.c DEPS chakra_interpret chakra content_i18n i18n)
time_add_test(NAME test_chakra_i18n TEST tests/systems/chakra/test_chakra_i18n.c DEPS chakra_interpret chakra content_i18n i18n)
time_add_test(NAME test_myanmar        TEST tests/systems/myanmar/test_myanmar.c      DEPS myanmar)
time_add_test(NAME test_myanmar_interpret TEST tests/systems/myanmar/test_myanmar_interpret.c DEPS myanmar_interpret content_i18n i18n)
time_add_test(NAME test_myanmar_i18n TEST tests/systems/myanmar/test_myanmar_i18n.c DEPS myanmar_interpret content_i18n i18n)
time_add_test(NAME test_lao_calendar   TEST tests/systems/lao/test_lao_calendar.c     DEPS lao_calendar)
time_add_test(NAME test_lao_interpret  TEST tests/systems/lao/test_lao_interpret.c    DEPS lao_interpret content_i18n i18n)
time_add_test(NAME test_lao_i18n TEST tests/systems/lao/test_lao_i18n.c DEPS lao_interpret content_i18n i18n)
time_add_test(NAME test_thai_calendar  TEST tests/systems/thai/test_thai_calendar.c   DEPS thai_calendar)
time_add_test(NAME test_thai_interpret TEST tests/systems/thai/test_thai_interpret.c  DEPS thai_interpret content_i18n i18n)
time_add_test(NAME test_thai_i18n TEST tests/systems/thai/test_thai_i18n.c DEPS thai_interpret content_i18n i18n)
time_add_test(NAME test_cosmic_duality TEST tests/systems/zoroastrian/test_cosmic_duality.c DEPS cosmic_duality)
time_add_test(NAME test_zoroastrian    TEST tests/systems/zoroastrian/test_zoroastrian.c   DEPS zoroastrian calendar_fixed)
time_add_test(NAME test_zoroastrian_interpret TEST tests/systems/zoroastrian/test_zoroastrian_interpret.c DEPS zoroastrian_interpret zoroastrian calendar_fixed content_i18n i18n)
time_add_test(NAME test_zoroastrian_i18n TEST tests/systems/zoroastrian/test_zoroastrian_i18n.c DEPS zoroastrian_interpret zoroastrian calendar_fixed content_i18n i18n)

# Tarot tests
time_add_test(NAME test_tarot           TEST tests/systems/tarot/test_tarot.c   DEPS tarot)
time_add_test(NAME test_tarot_interpret TEST tests/systems/tarot/test_tarot_interpret.c DEPS tarot_interpret content_i18n i18n)
time_add_test(NAME test_tarot_i18n TEST tests/systems/tarot/test_tarot_i18n.c DEPS tarot_interpret content_i18n i18n)

# Persian tests
time_add_test(NAME test_persian         TEST tests/systems/persian/test_persian.c DEPS persian)
time_add_test(NAME test_persian_interpret TEST tests/systems/persian/test_persian_interpret.c DEPS persian_interpret content_i18n i18n)
time_add_test(NAME test_persian_i18n TEST tests/systems/persian/test_persian_i18n.c DEPS persian_interpret content_i18n i18n)

# Baha'i tests
time_add_test(NAME test_bahai           TEST tests/systems/bahai/test_bahai.c     DEPS bahai)
time_add_test(NAME test_bahai_interpret TEST tests/systems/bahai/test_bahai_interpret.c DEPS bahai_interpret content_i18n i18n)
time_add_test(NAME test_bahai_i18n TEST tests/systems/bahai/test_bahai_i18n.c DEPS bahai_interpret content_i18n i18n)

# Japanese tests
time_add_test(NAME test_japanese        TEST tests/systems/japanese/test_japanese.c DEPS japanese)
time_add_test(NAME test_japanese_interpret TEST tests/systems/japanese/test_japanese_interpret.c DEPS japanese_interpret content_i18n i18n)
time_add_test(NAME test_japanese_i18n TEST tests/systems/japanese/test_japanese_i18n.c DEPS japanese_interpret content_i18n i18n)

# Aztec tests
time_add_test(NAME test_aztec           TEST tests/systems/aztec/test_aztec.c      DEPS aztec)
time_add_test(NAME test_aztec_interpret TEST tests/systems/aztec/test_aztec_interpret.c DEPS aztec_interpret content_i18n i18n)
time_add_test(NAME test_aztec_i18n TEST tests/systems/aztec/test_aztec_i18n.c DEPS aztec_interpret content_i18n i18n)

# Tamil tests
time_add_test(NAME test_tamil_calendar  TEST tests/systems/tamil/test_tamil_calendar.c DEPS tamil_calendar)
time_add_test(NAME test_tamil_interpret TEST tests/systems/tamil/test_tamil_interpret.c DEPS tamil_interpret content_i18n i18n)
time_add_test(NAME test_tamil_i18n TEST tests/systems/tamil/test_tamil_i18n.c DEPS tamil_interpret content_i18n i18n)

# Celtic Tree tests
time_add_test(NAME test_celtic_tree     TEST tests/systems/celtic/test_celtic_tree.c   DEPS celtic_tree)
time_add_test(NAME test_celtic_interpret TEST tests/systems/celtic/test_celtic_interpret.c DEPS celtic_interpret content_i18n i18n)
time_add_test(NAME test_celtic_i18n TEST tests/systems/celtic/test_celtic_i18n.c DEPS celtic_interpret content_i18n i18n)

# Korean tests
time_add_test(NAME test_korean_calendar  TEST tests/systems/korean/test_korean_calendar.c DEPS korean_calendar)
time_add_test(NAME test_korean_interpret TEST tests/systems/korean/test_korean_interpret.c DEPS korean_interpret korean_calendar content_i18n i18n)
time_add_test(NAME test_korean_i18n TEST tests/systems/korean/test_korean_i18n.c DEPS korean_interpret korean_calendar content_i18n i18n)

# Balinese tests
time_add_test(NAME test_pawukon         TEST tests/systems/balinese/test_pawukon.c       DEPS pawukon)
time_add_test(NAME test_pawukon_interpret TEST tests/systems/balinese/test_pawukon_interpret.c DEPS pawukon_interpret content_i18n i18n)
time_add_test(NAME test_pawukon_i18n TEST tests/systems/balinese/test_pawukon_i18n.c DEPS pawukon_interpret content_i18n i18n)

# French Republican tests
time_add_test(NAME test_french_republican TEST tests/systems/french_republican/test_french_republican.c DEPS french_republican)
time_add_test(NAME test_french_republican_interpret TEST tests/systems/french_republican/test_french_republican_interpret.c DEPS french_republican_interpret content_i18n i18n)
time_add_test(NAME test_french_republican_i18n TEST tests/systems/french_republican/test_french_republican_i18n.c DEPS french_republican_interpret content_i18n i18n)

# Geology tests
time_add_test(NAME test_geo_time        TEST tests/systems/test_geo_time.c              DEPS geo_time)
time_add_test(NAME test_climate_history TEST tests/systems/geology/test_climate_history.c DEPS climate_history)
time_add_test(NAME test_fossil_milestones TEST tests/systems/geology/test_fossil_milestones.c DEPS fossil_milestones)
time_add_test(NAME test_tectonic        TEST tests/systems/geology/test_tectonic.c      DEPS tectonic)
time_add_test(NAME test_radiometric    TEST tests/systems/geology/test_radiometric.c   DEPS radiometric)
time_add_test(NAME test_thermo         TEST tests/systems/geology/test_thermo.c        DEPS thermo)
time_add_test(NAME test_geology_interpret TEST tests/systems/geology/test_geology_interpret.c DEPS geology_interpret content_i18n i18n)
time_add_test(NAME test_geology_i18n TEST tests/systems/geology/test_geology_i18n.c DEPS geology_interpret content_i18n i18n)
time_add_test(NAME test_earth_voyage      TEST tests/systems/geology/test_earth_voyage.c      DEPS earth_voyage)
time_add_test(NAME test_fossil_narrative  TEST tests/systems/geology/test_fossil_narrative.c  DEPS fossil_narrative)
time_add_test(NAME test_earth_story      TEST tests/systems/geology/test_earth_story.c      DEPS earth_story)
time_add_test(NAME test_geology_today    TEST tests/systems/geology/test_geology_today.c    DEPS geology_today geo_time geology_interpret content_i18n i18n)

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
time_add_test(NAME test_location_alerts TEST tests/systems/earth/test_location_alerts.c DEPS location_alerts tide_predict tidal surf_spots ski_resorts snow_season daylight solar_events solar_radiation prayer_times storm_data convergence_detect julian lunar tzolkin tzolkin_board iching chinese hebrew hijri buddhist persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_privacy_framework TEST tests/systems/earth/test_privacy_framework.c DEPS privacy_framework)
time_add_test(NAME test_user_prefs      TEST tests/systems/earth/test_user_prefs.c      DEPS user_prefs interest_profile privacy_framework theme golden_layout color_theory color)
time_add_test(NAME test_local_events   TEST tests/systems/earth/test_local_events.c   DEPS local_events interest_profile solar_events)
time_add_test(NAME test_event_filter  TEST tests/systems/earth/test_event_filter.c  DEPS event_filter local_events interest_profile solar_events)
time_add_test(NAME test_neighbor_roulette TEST tests/systems/earth/test_neighbor_roulette.c DEPS neighbor_roulette interest_profile privacy_framework dreamspell tzolkin julian zodiac)
time_add_test(NAME test_personal_space TEST tests/systems/earth/test_personal_space.c DEPS personal_space)
time_add_test(NAME test_content_curator TEST tests/systems/earth/test_content_curator.c DEPS content_curator personal_space interest_profile wisdom fun_facts cultural_stories)
time_add_test(NAME test_persona_persist TEST tests/systems/earth/test_persona_persist.c DEPS persona_persist user_prefs interest_profile personal_space privacy_framework theme golden_layout color_theory color)
time_add_test(NAME test_persona_io TEST tests/systems/earth/test_persona_io.c DEPS persona_io persona_persist storage_bridge user_prefs interest_profile personal_space privacy_framework theme golden_layout color_theory color)
time_add_test(NAME test_onboarding TEST tests/systems/earth/test_onboarding.c DEPS onboarding)
time_add_test(NAME test_settings_panel TEST tests/systems/earth/test_settings_panel.c DEPS settings_panel user_prefs interest_profile privacy_framework theme golden_layout color_theory color)
time_add_test(NAME test_birth_entry TEST tests/systems/earth/test_birth_entry.c DEPS birth_entry)
time_add_test(NAME test_author_card TEST tests/systems/earth/test_author_card.c DEPS author_card)
time_add_test(NAME test_surf_display TEST tests/systems/earth/test_surf_display.c DEPS surf_display tide_predict tidal surf_spots lunar julian)
time_add_test(NAME test_human_population TEST tests/systems/earth/test_human_population.c DEPS human_population)
time_add_test(NAME test_today_for_you TEST tests/systems/earth/test_today_for_you.c DEPS today_for_you)
time_add_test(NAME test_notification_data TEST tests/systems/earth/test_notification_data.c DEPS notification_data)
time_add_test(NAME test_moment_snapshot TEST tests/systems/earth/test_moment_snapshot.c DEPS moment_snapshot)
time_add_test(NAME test_earth_stats TEST tests/systems/earth/test_earth_stats.c DEPS earth_stats human_population)
time_add_test(NAME test_heartbeat_counter TEST tests/systems/earth/test_heartbeat_counter.c DEPS heartbeat_counter human_population)
time_add_test(NAME test_pop_counter TEST tests/systems/earth/test_pop_counter.c DEPS pop_counter human_population)
time_add_test(NAME test_persona_card TEST tests/systems/earth/test_persona_card.c DEPS persona_card card_data)
time_add_test(NAME test_usage_interest TEST tests/systems/earth/test_usage_interest.c DEPS usage_interest)
time_add_test(NAME test_diamond_room TEST tests/systems/earth/test_diamond_room.c DEPS diamond_room)

# Tzolkin extended tests
time_add_test(NAME test_cr_cycle        TEST tests/systems/tzolkin/test_cr_cycle.c      DEPS cr_cycle haab tzolkin julian)

# Unified tests
time_add_test(NAME test_structural_map  TEST tests/systems/unified/test_structural_map.c  DEPS structural_map)
time_add_test(NAME test_cycle_analysis  TEST tests/systems/test_cycle_analysis.c          DEPS cycle_analysis)
time_add_test(NAME test_calendar_epoch  TEST tests/systems/unified/test_calendar_epoch.c  DEPS calendar_epoch)
time_add_test(NAME test_calendar_politics TEST tests/systems/unified/test_calendar_politics.c DEPS calendar_politics)
time_add_test(NAME test_convergence     TEST tests/systems/unified/test_convergence.c     DEPS convergence)
time_add_test(NAME test_convergence_detect TEST tests/systems/unified/test_convergence_detect.c DEPS convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_convergence_interpret TEST tests/systems/unified/test_convergence_interpret.c DEPS convergence_interpret content_i18n i18n)
time_add_test(NAME test_convergence_i18n TEST tests/systems/unified/test_convergence_i18n.c DEPS convergence_interpret content_i18n i18n)
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
time_add_test(NAME test_cosmic_fingerprint TEST tests/systems/unified/test_cosmic_fingerprint.c
    DEPS cosmic_fingerprint birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_cycle_tracker TEST tests/systems/unified/test_cycle_tracker.c
    DEPS cycle_tracker birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_exploration_progress TEST tests/systems/unified/test_exploration_progress.c
    DEPS exploration_progress)
time_add_test(NAME test_profile_lens TEST tests/systems/unified/test_profile_lens.c
    DEPS profile_lens cosmic_fingerprint birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_annual_summary TEST tests/systems/unified/test_annual_summary.c
    DEPS annual_summary birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_festival_detector TEST tests/systems/unified/test_festival_detector.c
    DEPS festival_detector korean_calendar hebrew sabbatical hijri buddhist lunar wheel_of_year thai_calendar tamil_calendar myanmar persian zoroastrian calendar_fixed)
time_add_test(NAME test_calendar_convert TEST tests/systems/unified/test_calendar_convert.c
    DEPS calendar_convert julian tzolkin haab cr_cycle chinese iching hebrew hijri persian coptic ethiopian egyptian french_republican japanese bahai korean_calendar thai_calendar tamil_calendar myanmar zoroastrian celtic_tree calendar_fixed)
time_add_test(NAME test_lunar_harmonics TEST tests/systems/unified/test_lunar_harmonics.c
    DEPS lunar_harmonics lunar julian)
time_add_test(NAME test_user_context TEST tests/systems/unified/test_user_context.c
    DEPS user_context birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler wisdom wisdom_engine fun_facts convergence_detect tzolkin_board persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_calendar_fixed TEST tests/systems/unified/test_calendar_fixed.c
    DEPS calendar_fixed egyptian coptic ethiopian zoroastrian)
time_add_test(NAME test_today_summary TEST tests/systems/unified/test_today_summary.c
    DEPS today_summary gregorian julian tzolkin dreamspell haab chinese hebrew hijri buddhist lunar panchanga nakshatra iching zodiac human_design sefirot coptic ethiopian persian japanese korean_calendar thai_calendar geo_time cosmic_time calendar_fixed)
time_add_test(NAME test_cal_vectors TEST tests/systems/unified/test_cal_vectors.c
    DEPS cal_vectors)
time_add_test(NAME test_depth_tier TEST tests/systems/unified/test_depth_tier.c
    DEPS depth_tier)
time_add_test(NAME test_brain_types TEST tests/systems/unified/test_brain_types.c
    DEPS brain_types)
time_add_test(NAME test_brain_scan TEST tests/systems/unified/test_brain_scan.c
    DEPS brain_scan brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_depth_selector TEST tests/systems/unified/test_depth_selector.c
    DEPS depth_selector)
time_add_test(NAME test_brain_score TEST tests/systems/unified/test_brain_score.c
    DEPS brain_score brain_types)
time_add_test(NAME test_brain_explain TEST tests/systems/unified/test_brain_explain.c
    DEPS brain_explain brain_types)
time_add_test(NAME test_depth_ring TEST tests/systems/unified/test_depth_ring.c
    DEPS depth_ring)
time_add_test(NAME test_depth_integration TEST tests/systems/unified/test_depth_integration.c
    DEPS today_summary depth_tier depth_ring depth_selector
    gregorian julian tzolkin dreamspell haab chinese hebrew hijri buddhist lunar
    panchanga nakshatra iching zodiac human_design sefirot coptic ethiopian persian
    japanese korean_calendar thai_calendar geo_time cosmic_time calendar_fixed
    LABELS integration)
time_add_test(NAME test_brain_narrative TEST tests/systems/unified/test_brain_narrative.c
    DEPS brain_narrative brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_brain_temporal TEST tests/systems/unified/test_brain_temporal.c
    DEPS brain_temporal brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_brain_personal TEST tests/systems/unified/test_brain_personal.c
    DEPS brain_personal brain_types birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_brain_adapt TEST tests/systems/unified/test_brain_adapt.c
    DEPS brain_adapt brain_types user_context birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_brain_depth TEST tests/systems/unified/test_brain_depth.c
    DEPS brain_depth brain_types depth_tier today_summary gregorian julian tzolkin dreamspell haab chinese hebrew hijri buddhist lunar panchanga nakshatra iching zodiac human_design sefirot coptic ethiopian persian japanese korean_calendar thai_calendar geo_time cosmic_time calendar_fixed)
time_add_test(NAME test_brain_surprise TEST tests/systems/unified/test_brain_surprise.c
    DEPS brain_surprise brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_brain_predict TEST tests/systems/unified/test_brain_predict.c
    DEPS brain_predict brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_brain_cluster TEST tests/systems/unified/test_brain_cluster.c
    DEPS brain_cluster brain_types)
time_add_test(NAME test_brain_archive TEST tests/systems/unified/test_brain_archive.c
    DEPS brain_archive brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_brain_query TEST tests/systems/unified/test_brain_query.c
    DEPS brain_query brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_cal_terms TEST tests/systems/unified/test_cal_terms.c
    DEPS cal_terms)
time_add_test(NAME test_depth_interpret TEST tests/systems/unified/test_depth_interpret.c
    DEPS depth_interpret hebrew_interpret hebrew islamic_interpret hijri buddhist_interpret hindu_interpret panchanga nakshatra iching_interpret iching kabbalah_interpret content_i18n i18n)
time_add_test(NAME test_depth_hud TEST tests/systems/unified/test_depth_hud.c
    DEPS depth_hud)
time_add_test(NAME test_brain_audio_bridge TEST tests/systems/unified/test_brain_audio_bridge.c
    DEPS brain_audio_bridge brain_types)
time_add_test(NAME test_brain_language_bridge TEST tests/systems/unified/test_brain_language_bridge.c
    DEPS brain_language_bridge brain_types)
time_add_test(NAME test_brain_visual_bridge TEST tests/systems/unified/test_brain_visual_bridge.c
    DEPS brain_visual_bridge brain_types)
time_add_test(NAME test_daily_narrative TEST tests/systems/unified/test_daily_narrative.c
    DEPS daily_narrative)
time_add_test(NAME test_personal_resonance TEST tests/systems/unified/test_personal_resonance.c
    DEPS personal_resonance)
time_add_test(NAME test_depth_temporal TEST tests/systems/unified/test_depth_temporal.c
    DEPS depth_temporal)
time_add_test(NAME test_narrative TEST tests/systems/unified/test_narrative.c
    DEPS narrative brain_language_bridge brain_types)
time_add_test(NAME test_text_lod TEST tests/systems/unified/test_text_lod.c
    DEPS text_lod depth_tier)
time_add_test(NAME test_universal_card TEST tests/systems/unified/test_universal_card.c
    DEPS universal_card sensitivity)
time_add_test(NAME test_convergence_forecast TEST tests/systems/unified/test_convergence_forecast.c
    DEPS convergence_forecast)
time_add_test(NAME test_structural_insight TEST tests/systems/unified/test_structural_insight.c
    DEPS structural_insight)
time_add_test(NAME test_aha_moments TEST tests/systems/unified/test_aha_moments.c
    DEPS aha_moments)
time_add_test(NAME test_drama_narrator TEST tests/systems/unified/test_drama_narrator.c
    DEPS drama_narrator)
time_add_test(NAME test_stored_convergences TEST tests/systems/unified/test_stored_convergences.c
    DEPS stored_convergences convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_brain_stats TEST tests/systems/unified/test_brain_stats.c
    DEPS brain_stats brain_archive brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_grand_cycle TEST tests/systems/unified/test_grand_cycle.c
    DEPS grand_cycle)
time_add_test(NAME test_time_entropy TEST tests/systems/unified/test_time_entropy.c
    DEPS time_entropy convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_system_correlation TEST tests/systems/unified/test_system_correlation.c
    DEPS system_correlation convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
set_tests_properties(test_system_correlation PROPERTIES TIMEOUT 30)
time_add_test(NAME test_red_thread TEST tests/systems/unified/test_red_thread.c
    DEPS red_thread brain_explain brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_sacred_scan TEST tests/systems/unified/test_sacred_scan.c
    DEPS sacred_scan sacred_numbers julian tzolkin iching hebrew hijri chinese)
time_add_test(NAME test_decade_heatmap TEST tests/systems/unified/test_decade_heatmap.c
    DEPS decade_heatmap brain_archive brain_types convergence_detect julian tzolkin tzolkin_board iching chinese hebrew hijri buddhist lunar persian coptic wheel_of_year bahai japanese egyptian french_republican korean_calendar thai_calendar tamil_calendar myanmar zoroastrian calendar_fixed)

# UI tests
time_add_test(NAME test_time_hud        TEST tests/ui/test_time_hud.c
    DEPS time_hud gregorian julian sidereal tzolkin zodiac planets orbit kepler)
time_add_test(NAME test_fmt             TEST tests/ui/test_fmt.c              DEPS fmt)
time_add_test(NAME test_animation       TEST tests/ui/test_animation.c        DEPS animation easing)
time_add_test(NAME test_glyph_layout    TEST tests/ui/test_glyph_layout.c    DEPS glyph_layout)
time_add_test(NAME test_card_data       TEST tests/ui/test_card_data.c        DEPS card_data)
time_add_test(NAME test_today_card     TEST tests/ui/test_today_card.c
    DEPS today_card card_data julian gregorian tzolkin haab iching chinese hebrew hijri buddhist lunar panchanga nakshatra zodiac human_design sefirot geo_time coptic ethiopian persian japanese korean_calendar thai_calendar calendar_fixed)
time_add_test(NAME test_zodiac_glyphs   TEST tests/ui/test_zodiac_glyphs.c   DEPS zodiac_glyphs)
time_add_test(NAME test_hexagram_visual TEST tests/ui/test_hexagram_visual.c  DEPS hexagram_visual iching)
time_add_test(NAME test_symbol_atlas    TEST tests/ui/test_symbol_atlas.c    DEPS symbol_atlas zodiac_glyphs)
time_add_test(NAME test_visual_transition TEST tests/ui/test_visual_transition.c
    DEPS visual_transition animation golden_layout card_layout toast_message render_layers camera_scale easing)
time_add_test(NAME test_focus_mode      TEST tests/ui/test_focus_mode.c       DEPS focus_mode card_layout render_layers camera_scale golden_layout easing)
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
time_add_test(NAME test_i18n_locales   TEST tests/ui/test_i18n_locales.c    DEPS i18n)
time_add_test(NAME test_i18n_p1        TEST tests/ui/test_i18n_p1.c         DEPS i18n)
time_add_test(NAME test_content_i18n   TEST tests/ui/test_content_i18n.c    DEPS content_i18n i18n)
time_add_test(NAME test_greeting       TEST tests/ui/test_greeting.c        DEPS greeting)
time_add_test(NAME test_share_template TEST tests/ui/test_share_template.c  DEPS share_template)
time_add_test(NAME test_seasonal_greeting TEST tests/ui/test_seasonal_greeting.c DEPS seasonal_greeting)
time_add_test(NAME test_iching_card    TEST tests/ui/test_iching_card.c     DEPS iching_card hexagram_visual iching)
time_add_test(NAME test_lunar_display  TEST tests/ui/test_lunar_display.c   DEPS lunar_display lunar julian zodiac)
time_add_test(NAME test_rtl_layout     TEST tests/ui/test_rtl_layout.c      DEPS rtl_layout)
time_add_test(NAME test_scroll_layers  TEST tests/ui/test_scroll_layers.c   DEPS scroll_layers)
time_add_test(NAME test_help_overlay  TEST tests/ui/test_help_overlay.c   DEPS help_overlay)
time_add_test(NAME test_ui_state     TEST tests/ui/test_ui_state.c       DEPS ui_state)
time_add_test(NAME test_layer_panel TEST tests/ui/test_layer_panel.c   DEPS layer_panel)
time_add_test(NAME test_command_palette TEST tests/ui/test_command_palette.c DEPS command_palette)
time_add_test(NAME test_time_control   TEST tests/ui/test_time_control.c   DEPS time_control)
time_add_test(NAME test_toast_message TEST tests/ui/test_toast_message.c  DEPS toast_message)
time_add_test(NAME test_ui_html      TEST tests/ui/test_ui_html.c       DEPS ui_html help_overlay command_palette layer_panel toast_message)
time_add_test(NAME test_timeline_data TEST tests/ui/test_timeline_data.c DEPS timeline_data gregorian julian)
time_add_test(NAME test_theme_css    TEST tests/ui/test_theme_css.c    DEPS theme_css theme golden_layout color_theory color)
time_add_test(NAME test_touch_gestures TEST tests/ui/test_touch_gestures.c DEPS touch_gestures)
time_add_test(NAME test_view_registry  TEST tests/ui/test_view_registry.c  DEPS view_registry)
time_add_test(NAME test_audio_score   TEST tests/ui/test_audio_score.c
    DEPS audio_score audio_culture audio_event audio_data frequency view_registry
         brain_scan brain_audio_bridge brain_types
         planets orbit kepler aspects convergence_detect julian tzolkin
         tzolkin_board iching chinese hebrew hijri buddhist lunar persian
         coptic wheel_of_year bahai japanese egyptian french_republican
         korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         calendar_fixed)
time_add_test(NAME test_share_moment TEST tests/ui/test_share_moment.c   DEPS share_moment)
time_add_test(NAME test_view_adapt  TEST tests/ui/test_view_adapt.c    DEPS view_adapt)
time_add_test(NAME test_a11y_score TEST tests/ui/test_a11y_score.c   DEPS a11y_score)
time_add_test(NAME test_system_theme TEST tests/ui/test_system_theme.c DEPS system_theme theme theme_css color_theory color golden_layout)
time_add_test(NAME test_timeline    TEST tests/ui/test_timeline.c    DEPS timeline animation easing LABELS unit)
time_add_test(NAME test_sensitivity TEST tests/ui/test_sensitivity.c
    DEPS sensitivity wisdom cultural_stories fun_facts)
time_add_test(NAME test_input_motion TEST tests/ui/test_input_motion.c
    DEPS input_motion spring LABELS unit)
time_add_test(NAME test_audio_event TEST tests/ui/test_audio_event.c
    DEPS audio_event planets orbit kepler aspects audio_data frequency
         convergence_detect julian tzolkin tzolkin_board iching chinese
         hebrew hijri buddhist lunar persian coptic wheel_of_year bahai
         japanese egyptian french_republican korean_calendar thai_calendar
         tamil_calendar myanmar zoroastrian calendar_fixed)
time_add_test(NAME test_audio_culture TEST tests/ui/test_audio_culture.c
    DEPS audio_culture)
time_add_test(NAME test_audio_meditation TEST tests/ui/test_audio_meditation.c
    DEPS audio_meditation golden_layout chakra)
time_add_test(NAME test_pianist_score TEST tests/ui/test_pianist_score.c
    DEPS pianist_score audio_score audio_culture audio_event audio_data frequency view_registry
         brain_scan brain_audio_bridge brain_types
         planets orbit kepler aspects convergence_detect julian tzolkin
         tzolkin_board iching chinese hebrew hijri buddhist lunar persian
         coptic wheel_of_year bahai japanese egyptian french_republican
         korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         calendar_fixed)
time_add_test(NAME test_midi_output TEST tests/ui/test_midi_output.c
    DEPS midi_output audio_score audio_culture audio_event audio_data frequency view_registry
         brain_scan brain_audio_bridge brain_types
         planets orbit kepler aspects convergence_detect julian tzolkin
         tzolkin_board iching chinese hebrew hijri buddhist lunar persian
         coptic wheel_of_year bahai japanese egyptian french_republican
         korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         calendar_fixed)
time_add_test(NAME test_audio_vis_data TEST tests/ui/test_audio_vis_data.c
    DEPS audio_vis_data)
time_add_test(NAME test_audio_a11y TEST tests/ui/test_audio_a11y.c
    DEPS audio_a11y pianist_score audio_score audio_culture audio_event audio_data frequency
         view_registry brain_scan brain_audio_bridge brain_types
         planets orbit kepler aspects convergence_detect julian tzolkin
         tzolkin_board iching chinese hebrew hijri buddhist lunar persian
         coptic wheel_of_year bahai japanese egyptian french_republican
         korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         calendar_fixed)
time_add_test(NAME test_audio_queue TEST tests/ui/test_audio_queue.c
    DEPS audio_queue)
time_add_test(NAME test_audio_pipeline TEST tests/ui/test_audio_pipeline.c
    DEPS pianist_score midi_output audio_vis_data audio_a11y audio_queue
         audio_score audio_event audio_culture audio_meditation audio_data
         frequency view_registry golden_layout chakra
         brain_scan brain_audio_bridge brain_types
         planets orbit kepler aspects convergence_detect julian tzolkin
         tzolkin_board iching chinese hebrew hijri buddhist lunar persian
         coptic wheel_of_year bahai japanese egyptian french_republican
         korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         calendar_fixed
    LABELS integration)
# ui_bridge is Emscripten-only (S1) — no native test target
time_add_test(NAME test_welcome_text TEST tests/ui/test_welcome_text.c
    DEPS welcome_text birth_profile julian tzolkin dreamspell chinese hebrew hijri buddhist lunar iching zodiac human_design planets orbit kepler)
time_add_test(NAME test_wisdom_display TEST tests/ui/test_wisdom_display.c
    DEPS wisdom_display wisdom_engine wisdom sensitivity)
time_add_test(NAME test_zoom_depth TEST tests/ui/test_zoom_depth.c
    DEPS zoom_depth depth_tier depth_ring camera_scale easing spring LABELS unit)
time_add_test(NAME test_focus_flow TEST tests/ui/test_focus_flow.c
    DEPS focus_flow camera_path click_target render_layers camera_scale easing
         spring vec_interp vec3 golden_layout LABELS unit)
time_add_test(NAME test_a11y_narrative TEST tests/ui/test_a11y_narrative.c
    DEPS a11y_narrative)
time_add_test(NAME test_text_reveal TEST tests/ui/test_text_reveal.c
    DEPS text_reveal animation easing)
time_add_test(NAME test_text_choreo TEST tests/ui/test_text_choreo.c
    DEPS text_choreo text_reveal animation easing)
time_add_test(NAME test_auto_theme TEST tests/ui/test_auto_theme.c
    DEPS auto_theme theme golden_layout color_theory color daylight solar_events)
time_add_test(NAME test_loading_sequence TEST tests/ui/test_loading_sequence.c
    DEPS loading_sequence)
time_add_test(NAME test_input_dispatch TEST tests/ui/test_input_dispatch.c
    DEPS input_dispatch)
time_add_test(NAME test_kf_track TEST tests/ui/test_kf_track.c
    DEPS kf_track animation easing LABELS unit)
time_add_test(NAME test_drama_seq TEST tests/ui/test_drama_seq.c
    DEPS drama_seq kf_track animation easing LABELS unit)
time_add_test(NAME test_card_depth TEST tests/ui/test_card_depth.c
    DEPS card_depth LABELS unit)
time_add_test(NAME test_card_selector TEST tests/ui/test_card_selector.c
    DEPS card_selector card_depth LABELS unit)
time_add_test(NAME test_card_style TEST tests/ui/test_card_style.c
    DEPS card_style theme golden_layout color_theory color LABELS unit)
time_add_test(NAME test_kin_cell TEST tests/ui/test_kin_cell.c
    DEPS kin_cell LABELS unit)
time_add_test(NAME test_kin_oracle_layout TEST tests/ui/test_kin_oracle_layout.c
    DEPS kin_oracle_layout kin_cell dreamspell tzolkin julian LABELS unit)
time_add_test(NAME test_kin_wavespell_layout TEST tests/ui/test_kin_wavespell_layout.c
    DEPS kin_wavespell_layout kin_cell dreamspell tzolkin julian LABELS unit)
time_add_test(NAME test_kin_card_layout TEST tests/ui/test_kin_card_layout.c
    DEPS kin_card_layout kin_cell dreamspell tzolkin tzolkin_board julian LABELS unit)
time_add_test(NAME test_kin_board_layout TEST tests/ui/test_kin_board_layout.c
    DEPS kin_board_layout kin_cell tzolkin_board LABELS unit)
time_add_test(NAME test_kin_wheel_layout TEST tests/ui/test_kin_wheel_layout.c
    DEPS kin_wheel_layout kin_cell tzolkin julian LABELS unit)
time_add_test(NAME test_kin_round_layout TEST tests/ui/test_kin_round_layout.c
    DEPS kin_round_layout kin_cell cr_cycle haab tzolkin julian LABELS unit)
time_add_test(NAME test_kin_castle_layout TEST tests/ui/test_kin_castle_layout.c
    DEPS kin_castle_layout kin_cell dreamspell tzolkin julian LABELS unit)
time_add_test(NAME test_kin_color_compass TEST tests/ui/test_kin_color_compass.c
    DEPS kin_color_compass kin_cell LABELS unit)
time_add_test(NAME test_kin_wavespell_journey TEST tests/ui/test_kin_wavespell_journey.c
    DEPS kin_wavespell_journey dreamspell LABELS unit)
time_add_test(NAME test_kin_earth_family TEST tests/ui/test_kin_earth_family.c
    DEPS kin_earth_family dreamspell LABELS unit)
time_add_test(NAME test_kin_pulsar_layout TEST tests/ui/test_kin_pulsar_layout.c
    DEPS kin_pulsar_layout LABELS unit)
time_add_test(NAME test_kin_oracle_map TEST tests/ui/test_kin_oracle_map.c
    DEPS kin_oracle_map kin_cell dreamspell LABELS unit)
time_add_test(NAME test_thirteen_moon TEST tests/systems/tzolkin/test_thirteen_moon.c
    DEPS thirteen_moon julian gregorian LABELS unit)
time_add_test(NAME test_kin_moon_page TEST tests/ui/test_kin_moon_page.c
    DEPS kin_moon_page thirteen_moon kin_cell tzolkin tzolkin_board julian gregorian LABELS unit)
time_add_test(NAME test_kin_heptad_row TEST tests/ui/test_kin_heptad_row.c
    DEPS kin_heptad_row thirteen_moon kin_cell tzolkin julian gregorian LABELS unit)
time_add_test(NAME test_kin_daily_chrono TEST tests/ui/test_kin_daily_chrono.c
    DEPS kin_daily_chrono kin_cell dreamspell tzolkin thirteen_moon heptad_gate julian gregorian LABELS unit)
time_add_test(NAME test_kin_moon_schematic TEST tests/ui/test_kin_moon_schematic.c
    DEPS kin_moon_schematic thirteen_moon julian gregorian LABELS unit)
time_add_test(NAME test_earth_transition TEST tests/ui/test_earth_transition.c
    DEPS earth_transition camera_path spring vec_interp vec3 animation easing LABELS unit)
time_add_test(NAME test_motion_prefs TEST tests/ui/test_motion_prefs.c
    DEPS motion_prefs LABELS unit)
time_add_test(NAME test_enter_zoom TEST tests/ui/test_enter_zoom.c
    DEPS enter_zoom camera_path spring vec_interp vec3 easing LABELS unit)
time_add_test(NAME test_birth_flight TEST tests/ui/test_birth_flight.c
    DEPS birth_flight camera_path spring vec_interp vec3 easing LABELS unit)
time_add_test(NAME test_convergence_motion TEST tests/ui/test_convergence_motion.c
    DEPS convergence_motion spring LABELS unit)
time_add_test(NAME test_transition_fx TEST tests/ui/test_transition_fx.c
    DEPS transition_fx LABELS unit)
time_add_test(NAME test_card_flight TEST tests/ui/test_card_flight.c
    DEPS card_flight spring LABELS unit)

# Platform tests
time_add_test(NAME test_storage_bridge TEST tests/platform/test_storage_bridge.c DEPS storage_bridge)

# Cross-system validation test
time_add_test(NAME test_cross_validation TEST tests/systems/test_cross_validation.c
    DEPS julian sidereal ecliptic kepler orbit planets gregorian zodiac aspects observer houses tzolkin chinese)

# ===== Contract Tests =====
time_add_test(NAME test_contract_julian    TEST tests/contracts/test_contract_julian.c    DEPS julian    LABELS contract)
time_add_test(NAME test_contract_zodiac    TEST tests/contracts/test_contract_zodiac.c    DEPS zodiac    LABELS contract)
time_add_test(NAME test_contract_orbit     TEST tests/contracts/test_contract_orbit.c     DEPS orbit kepler LABELS contract)
time_add_test(NAME test_contract_planets   TEST tests/contracts/test_contract_planets.c   DEPS planets orbit kepler LABELS contract)
time_add_test(NAME test_contract_gregorian TEST tests/contracts/test_contract_gregorian.c DEPS gregorian julian LABELS contract)
time_add_test(NAME test_contract_tzolkin   TEST tests/contracts/test_contract_tzolkin.c   DEPS tzolkin julian LABELS contract)
time_add_test(NAME test_contract_chinese   TEST tests/contracts/test_contract_chinese.c   DEPS chinese julian LABELS contract)
time_add_test(NAME test_contract_hebrew    TEST tests/contracts/test_contract_hebrew.c    DEPS hebrew julian LABELS contract)
time_add_test(NAME test_contract_hijri     TEST tests/contracts/test_contract_hijri.c     DEPS hijri julian LABELS contract)
time_add_test(NAME test_contract_camera    TEST tests/contracts/test_contract_camera.c    DEPS camera vec3 mat4 LABELS contract)

# ===== Integration Tests =====
time_add_test(NAME test_render_pipeline TEST tests/integration/test_render_pipeline.c
    DEPS camera vec3 mat4 camera_scale easing golden_layout render_layers
         star_field star_catalog star_catalog_ext constellation star_colors
         planet_pack planets orbit kepler planet_data atmo_ring
         zodiac_pack ring_geometry cusp_lines aspect_lines billboard color_palette color_theory color
         moon_pack moon_data
         orbit_trail_pack
         earth_pack earth_globe earth_atmosphere
         bodygraph_pack bodygraph
         milkyway_pack noise_shader shader_builder
         deep_sky_pack deep_sky
         card_pack card_layout
         tree_pack tree_geometry sefirot
         saturn_ring_pack
         planet_surface_pack
         lens_flare
         post_process
         weather_overlay wind_patterns storm_data
    LABELS integration)
time_add_test(NAME test_calendar_pipeline TEST tests/integration/test_calendar_pipeline.c
    DEPS julian gregorian tzolkin chinese hebrew hijri buddhist lunar
         persian coptic ethiopian egyptian japanese
         korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         bahai french_republican celtic_tree
         calendar_convert haab cr_cycle iching
         convergence_detect tzolkin_board
         wheel_of_year calendar_fixed
    LABELS integration)
time_add_test(NAME test_astronomy_pipeline TEST tests/integration/test_astronomy_pipeline.c
    DEPS julian kepler ecliptic sidereal orbit planets lunar solar_events
         zodiac aspects observer houses
    LABELS integration)
time_add_test(NAME test_calendar_roundtrip TEST tests/integration/test_calendar_roundtrip.c
    DEPS julian zoroastrian hijri ethiopian coptic thai_calendar egyptian
         tamil_calendar persian bahai hebrew myanmar french_republican japanese
         korean_calendar celtic_tree chinese tzolkin iching haab pawukon calendar_fixed
    LABELS integration)
time_add_test(NAME test_property_based TEST tests/integration/test_property_based.c
    DEPS easing vec3 mat4
    LABELS integration)
time_add_test(NAME test_shader_audit TEST tests/integration/test_shader_audit.c
    DEPS star_field star_catalog star_catalog_ext constellation star_colors
         planet_pack planets orbit kepler planet_data atmo_ring
         zodiac_pack ring_geometry cusp_lines aspect_lines billboard color_palette color_theory color vec3 mat4
         moon_pack moon_data
         orbit_trail_pack
         earth_pack earth_globe earth_atmosphere
         bodygraph_pack bodygraph
         milkyway_pack noise_shader shader_builder
         deep_sky_pack deep_sky
         card_pack card_layout
         tree_pack tree_geometry sefirot
         saturn_ring_pack
         planet_surface_pack
         lens_flare
         post_process
         weather_overlay wind_patterns storm_data
         sun_shader mesh_shader billboard_shader diffraction
         render_layers camera_scale easing golden_layout
    LABELS integration)

# ===== Performance Benchmarks =====
time_add_test(NAME bench_core TEST tests/benchmark/bench_core.c
    DEPS julian kepler mat4 vec3 orbit planets planet_data
         calendar_convert gregorian tzolkin haab cr_cycle chinese iching hebrew
         hijri persian coptic ethiopian egyptian french_republican japanese
         bahai korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         celtic_tree pawukon calendar_fixed
    LABELS benchmark)
time_add_test(NAME bench_memory TEST tests/benchmark/bench_memory.c
    DEPS julian kepler mat4 vec3 orbit planets planet_data
         calendar_convert gregorian tzolkin haab cr_cycle chinese iching hebrew
         hijri persian coptic ethiopian egyptian french_republican japanese
         bahai korean_calendar thai_calendar tamil_calendar myanmar zoroastrian
         celtic_tree pawukon calendar_fixed
    LABELS benchmark)
