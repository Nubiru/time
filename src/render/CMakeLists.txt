# src/render/CMakeLists.txt — Render data + passes

set(RENDER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.c
    ${CMAKE_CURRENT_SOURCE_DIR}/camera_scale.c
    ${CMAKE_CURRENT_SOURCE_DIR}/render_layers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/color_palette.c
    ${CMAKE_CURRENT_SOURCE_DIR}/color_theory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ring_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/concentric_ring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ring_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aspect_lines.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cusp_lines.c
    ${CMAKE_CURRENT_SOURCE_DIR}/galaxy_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/billboard.c
    ${CMAKE_CURRENT_SOURCE_DIR}/star_catalog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/star_catalog_ext.c
    ${CMAKE_CURRENT_SOURCE_DIR}/star_colors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constellation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/atmo_ring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/deep_sky.c
    ${CMAKE_CURRENT_SOURCE_DIR}/font_atlas.c
    ${CMAKE_CURRENT_SOURCE_DIR}/font_bitmap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glyph_batch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf_text.c
    ${CMAKE_CURRENT_SOURCE_DIR}/decan_stars.c
    ${CMAKE_CURRENT_SOURCE_DIR}/megalithic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tarot_visual.c
    ${CMAKE_CURRENT_SOURCE_DIR}/star_field.c
    ${CMAKE_CURRENT_SOURCE_DIR}/planet_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/planet_surface_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/orbit_trail_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/saturn_ring_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zodiac_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bodygraph_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/card_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/moon_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/diffraction.c
    ${CMAKE_CURRENT_SOURCE_DIR}/deep_sky_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/milkyway_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stardust_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sky_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_globe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_atmosphere.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tree_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/eclipse_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/catalog_ingest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/text_render.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hexagram_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh_shader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/noise_shader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sun_shader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constellation_bounds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constellation_label.c
    ${CMAKE_CURRENT_SOURCE_DIR}/diamond_room_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glyph_card_pack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/horizon_geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/horizon_camera.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sky_dome.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_view_frame.c
    ${CMAKE_CURRENT_SOURCE_DIR}/weather_overlay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/post_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/shared_uniforms.c
    ${CMAKE_CURRENT_SOURCE_DIR}/seasonal_lighting.c
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_builder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/billboard_shader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_constants.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lens_flare.c
    ${CMAKE_CURRENT_SOURCE_DIR}/click_target.c
    ${CMAKE_CURRENT_SOURCE_DIR}/camera_motion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/scale_spring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/camera_path.c
    ${CMAKE_CURRENT_SOURCE_DIR}/render_lod.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system_rings.c
    ${CMAKE_CURRENT_SOURCE_DIR}/convergence_visual.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pass_schedule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/view_state.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ring_today.c
    ${CMAKE_CURRENT_SOURCE_DIR}/birth_sky.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sdf_glyph.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/star_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/planet_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/zodiac_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/diffraction_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/constellation_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/saturn_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/milkyway_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/moon_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/earth_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/deep_sky_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/bodygraph_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/hexagram_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/tree_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/orbit_trail_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/ring_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/convergence_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/natal_chart_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/tree_of_life_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/bagua_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/gates_mandala_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/stardust_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/diamond_room_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/card_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/text_pass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/passes/post_pass.c
    PARENT_SCOPE
)

if(NOT EMSCRIPTEN)
    add_library(time_camera              OBJECT camera.c)
    add_library(time_camera_scale        OBJECT camera_scale.c)
    add_library(time_render_layers       OBJECT render_layers.c)
    add_library(time_color_palette       OBJECT color_palette.c)
    add_library(time_color_theory        OBJECT color_theory.c)
    add_library(time_ring_geometry       OBJECT ring_geometry.c)
    add_library(time_concentric_ring     OBJECT concentric_ring.c)
    add_library(time_ring_data           OBJECT ring_data.c)
    add_library(time_aspect_lines        OBJECT aspect_lines.c)
    add_library(time_cusp_lines          OBJECT cusp_lines.c)
    add_library(time_galaxy_geometry     OBJECT galaxy_geometry.c)
    add_library(time_billboard           OBJECT billboard.c)
    add_library(time_star_catalog        OBJECT star_catalog.c)
    add_library(time_star_catalog_ext    OBJECT star_catalog_ext.c)
    add_library(time_star_colors         OBJECT star_colors.c)
    add_library(time_constellation       OBJECT constellation.c)
    add_library(time_atmo_ring           OBJECT atmo_ring.c)
    add_library(time_deep_sky            OBJECT deep_sky.c)
    add_library(time_deep_sky_pack       OBJECT deep_sky_pack.c)
    add_library(time_font_atlas          OBJECT font_atlas.c)
    add_library(time_font_bitmap         OBJECT font_bitmap.c)
    add_library(time_glyph_batch         OBJECT glyph_batch.c)
    add_library(time_msdf_text           OBJECT msdf_text.c)
    add_library(time_decan_stars         OBJECT decan_stars.c)
    add_library(time_megalithic          OBJECT megalithic.c)
    add_library(time_tarot_visual        OBJECT tarot_visual.c)
    add_library(time_bodygraph_pack      OBJECT bodygraph_pack.c)
    add_library(time_card_pack           OBJECT card_pack.c)
    add_library(time_catalog_ingest      OBJECT catalog_ingest.c)
    add_library(time_diffraction         OBJECT diffraction.c)
    add_library(time_earth_atmosphere    OBJECT earth_atmosphere.c)
    add_library(time_earth_globe         OBJECT earth_globe.c)
    add_library(time_earth_pack          OBJECT earth_pack.c)
    add_library(time_eclipse_geometry    OBJECT eclipse_geometry.c)
    add_library(time_tree_pack           OBJECT tree_pack.c)
    add_library(time_hexagram_geometry   OBJECT hexagram_geometry.c)
    add_library(time_milkyway_pack       OBJECT milkyway_pack.c)
    add_library(time_stardust_pack      OBJECT stardust_pack.c)
    add_library(time_moon_pack           OBJECT moon_pack.c)
    add_library(time_orbit_trail_pack    OBJECT orbit_trail_pack.c)
    add_library(time_planet_pack         OBJECT planet_pack.c)
    add_library(time_planet_surface_pack OBJECT planet_surface_pack.c)
    add_library(time_saturn_ring_pack    OBJECT saturn_ring_pack.c)
    add_library(time_sky_pack           OBJECT sky_pack.c)
    add_library(time_star_field          OBJECT star_field.c)
    add_library(time_text_render         OBJECT text_render.c)
    add_library(time_zodiac_pack         OBJECT zodiac_pack.c)
    add_library(time_mesh_shader         OBJECT mesh_shader.c)
    add_library(time_noise_shader        OBJECT noise_shader.c)
    add_library(time_sun_shader          OBJECT sun_shader.c)
    add_library(time_constellation_bounds OBJECT constellation_bounds.c)
    add_library(time_constellation_label OBJECT constellation_label.c)
    add_library(time_diamond_room_pack  OBJECT diamond_room_pack.c)
    add_library(time_glyph_card_pack   OBJECT glyph_card_pack.c)
    add_library(time_horizon_geometry    OBJECT horizon_geometry.c)
    add_library(time_horizon_camera     OBJECT horizon_camera.c)
    add_library(time_sky_dome            OBJECT sky_dome.c)
    add_library(time_earth_view_frame   OBJECT earth_view_frame.c)
    add_library(time_weather_overlay     OBJECT weather_overlay.c)
    add_library(time_post_process        OBJECT post_process.c)
    add_library(time_shared_uniforms     OBJECT shared_uniforms.c)
    add_library(time_seasonal_lighting   OBJECT seasonal_lighting.c)
    add_library(time_shader_builder      OBJECT shader_builder.c)
    add_library(time_billboard_shader    OBJECT billboard_shader.c)
    add_library(time_shader_constants    OBJECT shader_constants.c)
    add_library(time_lens_flare          OBJECT lens_flare.c)
    add_library(time_click_target        OBJECT click_target.c)
    add_library(time_camera_motion       OBJECT camera_motion.c)
    add_library(time_scale_spring        OBJECT scale_spring.c)
    add_library(time_camera_path         OBJECT camera_path.c)
    add_library(time_render_lod          OBJECT render_lod.c)
    add_library(time_system_rings        OBJECT system_rings.c)
    add_library(time_convergence_visual  OBJECT convergence_visual.c)
    add_library(time_pass_schedule       OBJECT pass_schedule.c)
    add_library(time_view_state          OBJECT view_state.c)
    add_library(time_ring_today          OBJECT ring_today.c)
    add_library(time_birth_sky           OBJECT birth_sky.c)
    add_library(time_sdf_glyph          OBJECT sdf_glyph.c)
endif()
