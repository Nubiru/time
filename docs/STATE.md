# Time — State Inventory

**Last Updated**: 2026-03-07 (OMEGA sweep)

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
 src/math/* (15 modules)        src/core/main.c (orchestrator)
 src/systems/* (99 modules)     src/core/app_state.c (init)
 src/render/* (most, 31)        src/render/gl_init.c
 src/ui/* (24 modules)          src/render/mesh.c
                                src/render/shader.c
```

---

## State Container

**One struct holds ALL mutable state**: `app_state_t` in `src/core/app_state.h`

**Only one global variable in the entire program**: `g_state` in main.c

---

## Pure Modules by Domain (155 total)

### Math (15 modules)
vec3, mat4, julian, kepler, sidereal, ecliptic, easing, arc_geometry, bezier, color, projection, sacred_geometry, wheel_layout, date_parse (core)

### Render (26 pure + 5 stateful)
Pure: aspect_lines, atmo_ring, billboard, camera, camera_scale, card_pack, catalog_ingest, color_palette, color_theory, constellation, cusp_lines, decan_stars, deep_sky, earth_atmosphere, earth_globe, font_atlas, galaxy_geometry, glyph_batch, hexagram_geometry, megalithic, moon_pack, planet_pack, render_layers, ring_data, ring_geometry, star_catalog, star_catalog_ext, star_colors, star_field, tarot_visual, text_render, zodiac_pack

### Astronomy (10)
orbit, planets, lunar, precession, solar_events, cosmic_time, moon_data, moon_nodes, planet_data, retrograde

### Astrology (7)
zodiac, aspects, observer, houses, dignity, planetary_hours, wheel_layout

### Earth (11)
tidal, tide_predict, surf_spots, daylight, seasons, snow_season, ski_resorts, solar_radiation, storm_data, wind_patterns, biorhythm

### Geology (6)
geo_time, radiometric, thermo, climate_history, fossil_milestones, tectonic

### Unified (20)
cycle_analysis, structural_map, convergence, frequency, phase_space, sacred_numbers, codon_hexagram, number_scanner, calendar_epoch, calendar_politics, calendar_reform, precession_detect, earth_drama, knowledge_graph, achievement, audio_data, fun_facts, ratio_analysis, wisdom, wisdom_engine

### Calendars (20)
gregorian, tzolkin, haab, cr_cycle, tzolkin_board, iching, chinese, human_design, bodygraph, hebrew, sabbatical, hijri, prayer_times, buddhist, kalpa, coptic, ethiopian, egyptian, numerology, panchanga, nakshatra, yuga, wheel_of_year, chakra, sefirot, four_worlds, tree_geometry, myanmar, thai_calendar, cosmic_duality, zoroastrian

### UI (24)
time_hud, fmt, card_data, card_layout, astro_fmt, astro_summary, zodiac_glyphs, hexagram_visual, glyph_layout, scale_hud, system_scale_map, time_format, animation, location_presets, hd_card, iching_card, chinese_fmt, lunar_display, tzolkin_card, accessibility, rtl_layout, i18n, scroll_layers, golden_layout, theme

---

## Stateful Modules (imperative shell — 5 total)

| Module | Side effects | Guarded by |
|--------|-------------|-----------|
| main.c | Owns g_state, runs loop, registers callbacks | `#ifdef __EMSCRIPTEN__` |
| app_state.c | Creates initial state (no GL) | pure initializer |
| gl_init.c | Creates WebGL2 context, sets viewport | `#ifdef __EMSCRIPTEN__` |
| shader.c | Creates/deletes GL shader objects | `#ifdef __EMSCRIPTEN__` |
| mesh.c | Allocates VAO/VBO/EBO, uploads to GPU | `#ifdef __EMSCRIPTEN__` |

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
