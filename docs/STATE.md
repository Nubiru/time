# Time — State Inventory

**Last Updated**: 2026-03-20 (INFRA session 10 — full inventory sync)

This document tracks what is **pure** (stateless) and what is **stateful** in the codebase. The goal: maximize pure code, minimize and isolate mutable state.

---

## Architecture Pattern: Functional Core, Imperative Shell

```
 PURE CORE (stateless)          IMPERATIVE SHELL (stateful)
 ─────────────────────          ───────────────────────────
 Input → Output                 Mutates state, talks to GPU
 No side effects                GL calls, input handling
 Fully testable                 Tested visually / integration
 ─────────────────────          ───────────────────────────
 src/math/* (16 modules)        src/core/main.c (orchestrator)
 src/systems/* (189 modules)   src/core/app_state.c (init)
 src/render/* (most, 69)       src/core/hud.c (DOM overlay)
 src/ui/* (70 pure modules)    src/core/input.c (callbacks)
 src/platform/* (1 module)     src/render/gl_init.c
                                src/render/mesh.c
                                src/render/shader.c
                                src/render/passes/* (19 passes)
                                src/ui/audio_engine.c
                                src/ui/ui_bridge.c
```

---

## State Container

**One struct holds ALL mutable state**: `app_state_t` in `src/core/app_state.h`

**Only one global variable in the entire program**: `g_state` in main.c

---

## Pure Modules by Domain (347 total)

### Math (16 modules)
vec3, mat4, julian, kepler, sidereal, ecliptic, easing, arc_geometry, bezier, color, projection, sacred_geometry, wheel_layout, spring, vec_interp, equatorial_horizontal

### Render (69 pure + 22 stateful)
Pure: aspect_lines, atmo_ring, billboard, billboard_shader, bodygraph_pack, camera, camera_motion, camera_path, camera_scale, card_pack, catalog_ingest, click_target, color_palette, color_theory, concentric_ring, constellation, constellation_bounds, convergence_visual, cusp_lines, decan_stars, deep_sky, deep_sky_pack, diffraction, earth_atmosphere, earth_globe, earth_pack, earth_view_frame, eclipse_geometry, font_atlas, font_bitmap, galaxy_geometry, glyph_batch, hexagram_geometry, horizon_camera, horizon_geometry, lens_flare, megalithic, mesh_shader, milkyway_pack, moon_pack, noise_shader, orbit_trail_pack, pass_schedule, planet_pack, planet_surface_pack, post_process, render_frame (header-only), render_layers, render_lod, ring_data, ring_geometry, saturn_ring_pack, scale_spring, seasonal_lighting, shader_builder, shader_constants, shared_uniforms, sky_dome, star_catalog, star_catalog_ext, star_colors, star_field, sun_shader, system_rings, tarot_visual, text_render, tree_pack, view_state, weather_overlay, zodiac_pack
Stateful infra: gl_init, shader, mesh (3) + 19 render passes

### Astronomy (11)
orbit, planets, lunar, precession, solar_events, cosmic_time, moon_data, moon_nodes, planet_data, retrograde, astro_interpret

### Astrology (8)
zodiac, aspects, observer, houses, dignity, planetary_hours, wheel_layout, astrology_interpret

### Earth (23)
tidal, tide_predict, surf_spots, daylight, seasons, snow_season, ski_resorts, solar_radiation, storm_data, wind_patterns, biorhythm, interest_profile, location_alerts, local_events, privacy_framework, user_prefs, event_filter, neighbor_roulette, personal_space, content_curator, onboarding, persona_io, persona_persist

### Geology (7)
geo_time, radiometric, thermo, climate_history, fossil_milestones, tectonic, geology_interpret

### Unified (69)
cycle_analysis, structural_map, convergence, convergence_detect, frequency, phase_space, sacred_numbers, codon_hexagram, number_scanner, calendar_epoch, calendar_politics, calendar_reform, precession_detect, earth_drama, knowledge_graph, achievement, audio_data, birth_profile, cultural_stories, fun_facts, lunar_harmonics, ratio_analysis, time_philosophy, today_summary, wisdom, wisdom_engine, festival_detector, calendar_convert, user_context, aha_moments, brain_adapt, brain_audio_bridge, brain_cluster, brain_depth, brain_explain, brain_language_bridge, brain_narrative, brain_personal, brain_predict, brain_query, brain_scan, brain_score, brain_surprise, brain_temporal, brain_types, brain_visual_bridge, cal_terms, cal_vectors, calendar_fixed, convergence_forecast, convergence_interpret, cosmic_fingerprint, cycle_tracker, daily_narrative, depth_hud, depth_interpret, depth_ring, depth_selector, depth_temporal, depth_tier, drama_narrator, exploration_progress, narrative, personal_resonance, profile_lens, structural_insight, text_lod, universal_card, annual_summary

### Calendars & Knowledge Systems (71 modules across 32 systems)
aztec (2), bahai (2), balinese (2), buddhist (3), celtic (3), chakra (2), chinese (2), coptic (2), egyptian (2), ethiopian (2), french_republican (2), gregorian (2), hebrew (3), hindu (4), human_design (3), iching (2), islamic (3), japanese (2), kabbalah (4), korean (2), myanmar (2), numerology (2), persian (2), tamil (2), tarot (2), thai (2), tzolkin (7), zoroastrian (3)

### UI (70 pure + 2 stateful)
Pure: time_hud, fmt, card_data, card_layout, astro_fmt, astro_summary, zodiac_glyphs, hexagram_visual, glyph_layout, scale_hud, system_scale_map, time_format, animation, command_palette, layer_panel, location_presets, hd_card, help_overlay, iching_card, chinese_fmt, lunar_display, time_control, toast_message, tzolkin_card, ui_html, ui_state, accessibility, rtl_layout, i18n, scroll_layers, golden_layout, theme, timeline_data, a11y_score, audio_a11y, audio_culture, audio_event, audio_meditation, audio_queue, audio_score, audio_vis_data, focus_flow, input_motion, midi_output, pianist_score, sensitivity, share_moment, symbol_atlas, system_theme, theme_css, timeline, touch_gestures, view_adapt, view_registry, welcome_text, wisdom_display, zoom_depth, a11y_narrative, auto_theme, drama_seq, focus_mode, greeting, input_dispatch, kf_track, loading_sequence, seasonal_greeting, share_template, text_choreo, text_reveal, visual_transition
Stateful: audio_engine (EM_JS Web Audio), ui_bridge (EM_ASM DOM injection)

### Platform (1)
storage_bridge_native

---

## Stateful Modules (imperative shell — 29 total)

| Module | Side effects | Guarded by |
|--------|-------------|-----------|
| main.c | Owns g_state, runs loop, registers callbacks | `#ifdef __EMSCRIPTEN__` |
| app_state.c | Creates initial state (no GL) | pure initializer |
| hud.c | Pushes time data to HTML DOM via EM_ASM | `#ifdef __EMSCRIPTEN__` |
| input.c | Registers input callbacks, mutates g_state | `#ifdef __EMSCRIPTEN__` |
| gl_init.c | Creates WebGL2 context, sets viewport | `#ifdef __EMSCRIPTEN__` |
| shader.c | Creates/deletes GL shader objects | `#ifdef __EMSCRIPTEN__` |
| mesh.c | Allocates VAO/VBO/EBO, uploads to GPU | `#ifdef __EMSCRIPTEN__` |
| audio_engine.c | EM_JS for Web Audio API, static muted/init state | `#ifdef __EMSCRIPTEN__` |
| ui_bridge.c | EM_ASM DOM injection, static state | `#ifdef __EMSCRIPTEN__` |
| star_pass.c | Star field render pass | `#ifdef __EMSCRIPTEN__` |
| constellation_pass.c | IAU boundary lines | `#ifdef __EMSCRIPTEN__` |
| deep_sky_pass.c | Messier DSO billboards | `#ifdef __EMSCRIPTEN__` |
| milkyway_pass.c | Galaxy band mesh | `#ifdef __EMSCRIPTEN__` |
| diffraction_pass.c | Star diffraction spikes | `#ifdef __EMSCRIPTEN__` |
| orbit_trail_pass.c | Keplerian orbit ellipses | `#ifdef __EMSCRIPTEN__` |
| planet_pass.c | Sun + 8 planet sprites | `#ifdef __EMSCRIPTEN__` |
| saturn_pass.c | Saturn ring system | `#ifdef __EMSCRIPTEN__` |
| moon_pass.c | 9 major moons | `#ifdef __EMSCRIPTEN__` |
| zodiac_pass.c | Ring + markers + aspects + cusps + glyphs | `#ifdef __EMSCRIPTEN__` |
| earth_pass.c | Globe + coastlines + terminator | `#ifdef __EMSCRIPTEN__` |
| bodygraph_pass.c | HD centers + channels + gates | `#ifdef __EMSCRIPTEN__` |
| hexagram_pass.c | I Ching hexagram lines | `#ifdef __EMSCRIPTEN__` |
| tree_pass.c | Kabbalah Sefirot + paths | `#ifdef __EMSCRIPTEN__` |
| card_pass.c | Info card backgrounds + borders | `#ifdef __EMSCRIPTEN__` |
| text_pass.c | Text rendering pass | `#ifdef __EMSCRIPTEN__` |
| ring_pass.c | Concentric knowledge system rings | `#ifdef __EMSCRIPTEN__` |
| convergence_pass.c | Cross-system convergence visuals | `#ifdef __EMSCRIPTEN__` |
| post_pass.c | FBO bloom + tonemap + vignette | `#ifdef __EMSCRIPTEN__` |

**Core pure utilities** (live in src/core/ but are stateless): date_parse

**Rule**: All GL code behind `#ifdef __EMSCRIPTEN__` guards. Native build never sees it.

---

## Dependency Rules (enforced)

```
PURE code must NEVER:
  - Include GL headers (P1)
  - Call malloc/calloc/realloc/free (P2)
  - Use printf/fprintf/puts (P3)
  - Reference app_state_t or g_state (P4)
  - Modify any global or have side effects (P5)

STATEFUL code:
  - Must be behind #ifdef __EMSCRIPTEN__ if it uses GL
  - Must only mutate g_state (not random globals)
  - Must use pure functions for computation
```

---

## How to Verify Purity

```bash
# No GL headers in pure code:
grep -r "GLES3\|glCreate\|glBind" src/math/ src/systems/
# Should return nothing

# No globals in pure code:
grep -r "g_state\|app_state" src/math/ src/systems/
# Should return nothing

# Full audit: run OMEGA sweep
/execute OMEGA
```
