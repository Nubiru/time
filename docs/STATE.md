# Time — State Inventory

**Last Updated**: 2026-03-12 (OMEGA sweep #4)

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
 src/math/* (13 modules)        src/core/main.c (orchestrator)
 src/systems/* (93 modules)     src/core/app_state.c (init)
 src/render/* (most, 42)        src/core/hud.c (DOM overlay)
 src/ui/* (25 modules)          src/core/input.c (callbacks)
                                src/render/gl_init.c
                                src/render/mesh.c
                                src/render/shader.c
                                src/render/passes/* (3 passes)
```

---

## State Container

**One struct holds ALL mutable state**: `app_state_t` in `src/core/app_state.h`

**Only one global variable in the entire program**: `g_state` in main.c

---

## Pure Modules by Domain (179 total)

### Math (13 modules)
vec3, mat4, julian, kepler, sidereal, ecliptic, easing, arc_geometry, bezier, color, projection, sacred_geometry, wheel_layout

### Render (43 pure + 6 stateful)
Pure: aspect_lines, atmo_ring, billboard, bodygraph_pack, camera, camera_scale, card_pack, catalog_ingest, color_palette, color_theory, constellation, constellation_bounds, cusp_lines, decan_stars, deep_sky, deep_sky_pack, diffraction, earth_atmosphere, earth_globe, earth_pack, eclipse_geometry, font_atlas, galaxy_geometry, glyph_batch, hexagram_geometry, horizon_geometry, megalithic, mesh_shader, milkyway_pack, moon_pack, noise_shader, orbit_trail_pack, planet_pack, post_process, render_frame (header-only), render_layers, ring_data, ring_geometry, saturn_ring_pack, star_catalog, star_catalog_ext, star_colors, star_field, sun_shader, tarot_visual, text_render, tree_pack, weather_overlay, zodiac_pack

### Astronomy (10)
orbit, planets, lunar, precession, solar_events, cosmic_time, moon_data, moon_nodes, planet_data, retrograde

### Astrology (7)
zodiac, aspects, observer, houses, dignity, planetary_hours, wheel_layout

### Earth (11)
tidal, tide_predict, surf_spots, daylight, seasons, snow_season, ski_resorts, solar_radiation, storm_data, wind_patterns, biorhythm

### Geology (6)
geo_time, radiometric, thermo, climate_history, fossil_milestones, tectonic

### Unified (22)
cycle_analysis, structural_map, convergence, frequency, phase_space, sacred_numbers, codon_hexagram, number_scanner, calendar_epoch, calendar_politics, calendar_reform, precession_detect, earth_drama, knowledge_graph, achievement, audio_data, cultural_stories, fun_facts, ratio_analysis, wisdom, wisdom_engine, festival_detector

### Calendars (41)
aztec, bahai, gregorian, tzolkin, haab, cr_cycle, tzolkin_board, dreamspell, iching, chinese, human_design, bodygraph, hebrew, sabbatical, hijri, prayer_times, buddhist, kalpa, coptic, ethiopian, egyptian, french_republican, numerology, panchanga, nakshatra, yuga, wheel_of_year, celtic_tree, chakra, sefirot, four_worlds, tree_geometry, myanmar, thai_calendar, cosmic_duality, zoroastrian, japanese, persian, tarot, tamil_calendar, korean_calendar

### UI (25)
time_hud, fmt, card_data, card_layout, astro_fmt, astro_summary, zodiac_glyphs, hexagram_visual, glyph_layout, scale_hud, system_scale_map, time_format, animation, location_presets, hd_card, iching_card, chinese_fmt, lunar_display, tzolkin_card, accessibility, rtl_layout, i18n, scroll_layers, golden_layout, theme

---

## Stateful Modules (imperative shell — 10 total)

| Module | Side effects | Guarded by |
|--------|-------------|-----------|
| main.c | Owns g_state, runs loop, registers callbacks | `#ifdef __EMSCRIPTEN__` |
| app_state.c | Creates initial state (no GL) | pure initializer |
| hud.c | Pushes time data to HTML DOM via EM_ASM | `#ifdef __EMSCRIPTEN__` |
| input.c | Registers input callbacks, mutates g_state | `#ifdef __EMSCRIPTEN__` |
| gl_init.c | Creates WebGL2 context, sets viewport | `#ifdef __EMSCRIPTEN__` |
| shader.c | Creates/deletes GL shader objects | `#ifdef __EMSCRIPTEN__` |
| mesh.c | Allocates VAO/VBO/EBO, uploads to GPU | `#ifdef __EMSCRIPTEN__` |
| star_pass.c | Star field render pass (GL resources) | `#ifdef __EMSCRIPTEN__` |
| planet_pass.c | Planet/Sun render pass (GL resources) | `#ifdef __EMSCRIPTEN__` |
| zodiac_pass.c | Zodiac ring render pass (GL resources) | `#ifdef __EMSCRIPTEN__` |

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
