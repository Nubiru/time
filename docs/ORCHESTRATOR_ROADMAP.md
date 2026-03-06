# Orchestrator Roadmap — Primary Agent Work Plan

**Owner**: Primary Claude (orchestrator)
**Last Updated**: 2026-03-06
**Philosophy**: I build the architecture and the visuals. Agents build the modules. I focus on what connects everything.

---

## Delegation Model

```
PRIMARY (me):  Architecture, visual rendering, camera systems, integration,
               wiring modules into the live build, UI/UX, the art itself.
               Macro-level direction: I define WHAT needs building in this roadmap.

AGENTS (A/B):  Autonomous 4-phase pipeline (evaluate → plan → execute → document).
               Agents read THIS roadmap, find unchecked [ ] items in their domain,
               design their own TDD task specs, implement, and report back.
               Pipeline command: /execute A  or  /execute B

AGENT A DOMAIN: Geometry, render infrastructure, vertex data, projection math
AGENT B DOMAIN: UI data, formatting, knowledge system visuals, display logic
```

---

## TRACK 1: Complete the Bridge (Phase 4 Finish + Integration)

Wire up everything we've already built but haven't connected.

### 1.1 Wire LST into HUD — Complete Phase 4
- [x] Add observer longitude to app_state (default: Greenwich 51.48°N, 0°W)
- [x] Call `lst_degrees(gst, longitude)` in update_hud
- [x] Display LST in HUD: "LST: 08h 15m 30s"
- [x] Phase 4 criterion met: 6/6 COMPLETE

### 1.2 Wire Chinese Calendar into HUD
- [x] Include `chinese.h` in main.c
- [x] Add Chinese year info to HUD: animal emoji + element + animal name + polarity
- [x] Add animal emoji from `chinese_animal_symbol()`

### 1.3 Wire Observer/Houses into HUD
- [x] Compute Ascendant from LST + obliquity + latitude
- [x] Display: "ASC ♋ Cancer 12" in HUD
- [x] Display current house system summary

### 1.4 Wire Aspects into HUD
- [x] Compute planet longitudes -> run `aspects_find_all()`
- [x] Display active aspects: "Sun ☍ Moon, Venus △ Mars"

### 1.5 Verify Full WASM Build
- [x] `make wasm` with all new modules (chinese, observer, houses, aspects, dignity, lunar, iching, easing, arc_geometry, font_atlas, fmt)
- [ ] Test in browser — all HUD data displays correctly
- [ ] No console errors, 60fps maintained

---

## TRACK 2: Multi-Scale Camera Architecture

The artistic backbone. Time at every scale.

### 2.1 Define Scale Levels
```
SCALE 0: Personal Chart       — exp(-4.6) ≈ 0.01
SCALE 1: Earth-Moon System    — exp(0) ≈ 1.0
SCALE 2: Inner Solar System   — exp(1.4) ≈ 4.0
SCALE 3: Solar System         — exp(3.4) ≈ 30.0 ← DEFAULT
SCALE 4: Solar Neighborhood   — exp(5.3) ≈ 200
SCALE 5: Milky Way            — exp(7.6) ≈ 2000
SCALE 6: Observable Universe  — exp(9.9) ≈ 20000
```

- [x] Design `scale_level_t` struct: { id, name, log_zoom_min/max/center }
- [x] Define 7 scale levels with camera distance thresholds (camera_scale.h/.c)
- [ ] Map each knowledge system to its "home" scale level
- [ ] Document in VISUAL_DESIGN.md

### 2.2 Logarithmic Zoom
- [x] Replace linear camera zoom with logarithmic distance
- [x] `cam.distance = exp(log_zoom)` — smooth across 10+ orders of magnitude
- [x] Scroll wheel adjusts log_zoom linearly
- [x] Zoom range: 0.01 (chart view) to 50000 (universe view)
- [x] Scale name displayed in HUD
- [x] 21 tests for camera_scale module

### 2.3 Scale-Dependent Rendering
- [x] Add `visible_at_scale` flags to each render pass (render_layers.h/.c)
- [x] 8 layers: Stars, Galaxy, Orbits, Planets, Zodiac Ring, Labels, Cards, HUD
- [x] Each layer has scale_min/max + base_opacity + user toggle
- [x] Fade-in/fade-out at scale boundaries (20% ramp zone)
- [x] layers_compute() returns all 8 opacities from log_zoom
- [x] 19 tests for render_layers module

### 2.4 Scale Transition Animation
- [x] Use easing functions (from agent pipeline) for smooth transitions
- [x] Camera interpolation between scale levels: 1.2 second transition
- [x] Layer crossfade during transitions (layers_compute fed by transitioning log_zoom)
- [x] Shift+0-6 jump to scale levels (Shift avoids collision with speed keys)

---

## TRACK 3: Zodiac Wheel Rendering

The first knowledge system to become VISIBLE. Bridges Phase 5 computation to Phase 7 visuals.

### 3.1 Ecliptic Ring Geometry
- [x] Use ring_geometry (from agent pipeline) to generate ring mesh
- [x] 12 colored segments on the ecliptic plane (in 3D space)
- [x] Ring positioned between Mars and Jupiter (4.2-4.8 scene units)
- [x] Segment colors from color_zodiac_sign() (from color_palette)
- [x] Ring shader: vertex-colored, alpha-blended, scale-gated by LAYER_ZODIAC_RING

### 3.2 Sign Labels on Ring
- [x] Zodiac glyph bitmaps packed into 96×12 R8 texture atlas
- [x] Textured billboard shader with per-vertex color
- [x] Place zodiac symbol at center of each 30° segment (billboarded)
- [x] Fade in/out with LAYER_ZODIAC_RING opacity

### 3.3 Planet Markers on Ring
- [x] Compute geocentric longitudes from heliocentric positions
- [x] Position small spheres at planet longitude on ring midpoint (4.5 units)
- [x] Earth index renders as Sun marker (yellow, geocentric convention)
- [x] Markers fade with ring layer opacity, scale-gated by LAYER_ZODIAC_RING

### 3.4 Aspect Lines
- [x] Draw chord lines inside the zodiac ring for active aspects
- [x] Color by aspect type via aspect_lines_generate() colors
- [x] Line opacity based on orb tightness (exact=opaque, wide=faint)
- [x] Per-vertex RGBA line shader with layer opacity multiplier

### 3.5 House Cusp Lines — Geometry DELIVERED (Agent A: cusp_lines.c, 22 tests)
- [x] Wire cusp_lines_generate() into render loop
- [x] Upload cusp line vertices to GPU, draw as GL_LINES
- [x] Brighter lines for ASC/DSC/MC/IC angular cusps (opacity 0.8 vs 0.35)
- [x] Cusp line angles from houses_whole_sign() + observer longitude + LST

### 3.8 Agent: Glyph Quad Batch Builder (Agent A)
- [ ] Pure module: takes glyph IDs + world positions + sizes → interleaved vertex data
- [ ] Combines billboard geometry + font_atlas UVs into a single draw batch
- [ ] Output: flat float arrays (position, uv, color) ready for GPU upload
- [ ] Configurable: per-glyph tint color, per-glyph scale
- [ ] Used by: 3.2 sign labels, 3.3 planet glyphs (future glyph rendering)

### 3.6 Agent: Screen Projection Utilities (Agent A) — DELIVERED
- [x] Pure module: project 3D world position to 2D screen coordinates
- [x] Inverse: unproject screen coords to world ray
- [x] Viewport-aware (width, height, aspect ratio)
- [x] 22 tests, 0 failures

### 3.7 Agent: Billboard Geometry (Agent A) — DELIVERED
- [x] Camera-facing quads for sign labels + planet glyphs
- [x] Y-locked mode, pixel-size computation, batch generation
- [x] 20 tests, 0 failures

---

## TRACK 4: Knowledge System Visual Cards

Screen-space panels that display each system's data.

### 4.1 Card Renderer Infrastructure
- [ ] Define `card_t` struct: { x, y, w, h, visible, opacity, content_type }
- [ ] Card render pass: screen-space quads over the 3D scene
- [ ] Card transition: slide-in from edge + fade
- [ ] Toggle per card: K=Kin, I=I Ching, C=Chinese, D=HD, A=Astrology

### 4.2 Tzolkin Kin Card
- [ ] Left panel: seal icon (procedural or glyph atlas)
- [ ] Seal name, tone number, kin number
- [ ] Card background color = seal family color (Red/White/Blue/Yellow)
- [ ] "Kin 207 · Blue Crystal Hand · Tone 12"

### 4.3 I Ching Hexagram Card
- [ ] 6 horizontal lines (broken/solid) stacked vertically
- [ ] Upper + lower trigram names
- [ ] King Wen number + hexagram name
- [ ] "Hexagram 42 · Yi (Increase) · Wind/Thunder"

### 4.4 Chinese Calendar Card
- [ ] Animal emoji + name
- [ ] Heavenly Stem + Earthly Branch
- [ ] Element + Polarity
- [ ] "Year of the Fire Horse · Bing-Wu · Yang"

### 4.5 Human Design Gate Card
- [ ] Sun gate + Earth gate (incarnation cross)
- [ ] Gate name + keyword
- [ ] Line number
- [ ] "Gate 17.4 · Opinions · Following"

### 4.6 Astrology Summary Card
- [ ] Mini zodiac wheel (2D, flat)
- [ ] Planet positions listed with sign + degree
- [ ] Active aspects listed
- [ ] Ascendant + house overview

### 4.7 Agent: Hexagram Visual Data (Agent B) — DELIVERED + INTEGRATED
- [x] Pure module: line patterns for all 64 hexagrams (broken/solid per line)
- [x] Trigram decomposition (upper + lower)
- [x] Nuclear hexagram derivation
- [x] ASCII art generator for hexagram display
- [x] 22 tests, Makefile integrated

### 4.8 Agent: Extended Time Formatters (Agent B) — DELIVERED + INTEGRATED
- [x] Pure module: format JD with thousands separator, adaptive time scale
- [x] Duration formatter (seconds → "1d 0h"), planetary hour formatter
- [x] Aspect pair formatter, elapsed compact formatter
- [x] 22 tests, Makefile integrated

### 4.9 Agent: Scale-Aware HUD Selector (Agent B) — DELIVERED + INTEGRATED
- [x] Pure module: determine which HUD lines to show per scale level (0-6)
- [x] Personal scale → astrology details. Solar → planets + Tzolkin. Galaxy → cosmic time
- [x] Visibility flags struct per system per scale
- [x] 22 tests, Makefile integrated

### 4.10 Agent: Tzolkin Card Data (Agent B)
- [ ] Pure module: format Tzolkin day data for card display
- [ ] Seal family color (Red/White/Blue/Yellow) from seal index
- [ ] Wavespell position (1-13), castle assignment (1-5)
- [ ] Oracle structure: guide, analog, antipode, occult, from kin number
- [ ] Used by: 4.2 Tzolkin Kin Card

### 4.11 Agent: Astrology Summary Formatter (Agent B)
- [ ] Pure module: format complete astrology snapshot for card display
- [ ] Planet positions as "Sun ♌ Leo 15.3°" style strings
- [ ] Dignity status per planet (domicile/detriment/exaltation/fall)
- [ ] Aspect summary with orb values
- [ ] Used by: 4.6 Astrology Summary Card

### 4.12 Agent: Haab Calendar (Agent B)
- [ ] Pure module: Mayan 365-day solar calendar (complements Tzolkin)
- [ ] 18 months of 20 days + 5-day Wayeb period
- [ ] Haab date from Julian Day (using GMT correlation 584283)
- [ ] Month name + day number (e.g., "3 Pop", "17 Yax")
- [ ] Used by: Tzolkin/Haab Calendar Round display

---

## TRACK 5: User Interaction

### 5.1 Date/Time Input
- [ ] DOM input field (HTML, not WebGL) — triggered by T key or clicking date in HUD
- [ ] Parse date string -> JD -> jump simulation to that moment
- [ ] "Enter date: 2024-04-08 14:30" -> solar eclipse moment
- [ ] Close input field on Enter or Escape

### 5.2 Location Input
- [ ] Latitude/longitude entry (DOM input or preset cities)
- [ ] Updates LST, Ascendant, houses in real-time
- [ ] Preset locations: Greenwich, New York, Tokyo, Cairo, Mexico City
- [ ] L key opens location picker

### 5.3 Layer Visibility Controls
- [ ] DOM overlay checkboxes or keyboard toggles per layer
- [ ] Layers: Orbits, Trails, Zodiac Ring, Labels, Aspects, Houses, Cards
- [ ] Each layer has independent opacity (0.0 - 1.0)
- [ ] Shift+number toggles individual layers

### 5.4 Time Navigation
- [ ] Arrow keys: ←/→ step 1 day, ↑/↓ step 1 month
- [ ] Shift+arrows: step 1 year / 10 years
- [ ] Scrub bar (DOM range input) for continuous time travel
- [ ] Current controls (0-5, Space, -) remain as speed controls

### 5.5 Agent: Date Parser (Agent B) — DELIVERED + INTEGRATED
- [x] Pure module: parse date strings to year/month/day/hour/min/sec
- [x] Validate ranges, ISO 8601 support
- [x] 28 tests, Makefile integrated

### 5.6 Agent: City Preset Data (Agent B) — DELIVERED + INTEGRATED
- [x] Pure module: lookup table of world cities with lat/lon
- [x] Region grouping, search by name
- [x] 22 tests, Makefile integrated

---

## TRACK 6: Infrastructure Hardening

### 6.1 Code Formatting
- [ ] Create `.clang-format` config (matches our conventions: 4-space indent, K&R braces)
- [ ] Add `make format` target: `clang-format -i src/**/*.c src/**/*.h`
- [ ] Run once on entire codebase to establish baseline

### 6.2 Header Dependency Check
- [ ] Script: `scripts/check_deps.sh` — lists all #include chains
- [ ] Detect circular includes
- [ ] Verify pure modules never include render headers
- [ ] Add `make check-deps` target

### 6.3 WASM Size Audit
- [ ] Measure current WASM binary size
- [ ] Identify largest contributors (module-by-module)
- [ ] Set budget: < 512KB for core, < 1MB with all systems
- [ ] Add `make wasm-size` target that reports binary size

### 6.4 Performance Baseline
- [ ] Measure FPS with all modules active
- [ ] Profile render loop (which draw calls are heaviest)
- [ ] Set budget: 60fps on mid-range hardware (Gabriel's i5)
- [ ] Document in METRICS.md

---

## TRACK 7: The Galaxy View (Cosmic Scale)

The artistic frontier. Zooming OUT from the solar system.

### 7.1 Star Field
- [ ] Star positions rendered as point sprites
- [ ] Star brightness mapped to point size
- [ ] Stars visible at scales 0-3, fade out when zooming into solar system
- [ ] New shader: star_field.vert/frag

### 7.5 Agent: Star Catalog Data (Agent A)
- [ ] Pure module: `src/render/star_catalog.h/.c`
- [ ] Data: ~300 brightest stars (V magnitude < 4.0) as `static const` arrays
- [ ] Per star: RA (hours), Dec (degrees), V magnitude, B-V color index, name (if named), constellation ID
- [ ] `star_to_cartesian(ra_hours, dec_deg, radius)` → vec3 (equatorial to Cartesian)
- [ ] `star_color_from_bv(bv)` → RGB (blackbody approximation: blue→white→yellow→orange→red)
- [ ] `star_size_from_mag(mag, mag_limit)` → float (inverse: brighter = bigger point)
- [ ] `star_catalog_count()`, `star_catalog_get(index)` → accessor functions
- [ ] All data compiled into binary (no file I/O, no malloc)
- [ ] Used by: 7.7 star point sprite shader

### 7.6 Agent: Constellation Lines (Agent A)
- [ ] Pure module: `src/render/constellation_lines.h/.c`
- [ ] Data: 12 zodiac constellations + ~28 major constellations (Orion, Ursa Major, Scorpius, etc.)
- [ ] Per constellation: name, IAU 3-letter abbreviation, array of (star_index_a, star_index_b) pairs
- [ ] `constellation_lines_for(id)` → array of star-index pairs + count
- [ ] `constellation_containing_longitude(ecliptic_lon)` → constellation ID
- [ ] `constellation_count()`, `constellation_name(id)`, `constellation_abbr(id)` → accessors
- [ ] Zodiac constellations are PRIORITY (connect to astrology layer)
- [ ] Star indices reference star_catalog entries by position in the catalog array
- [ ] Used by: 7.8 constellation overlay

### 7.7 Star Point Sprite Shader — Orchestrator
- [ ] New shader program: `star_vert` / `star_frag`
- [ ] Vertex attributes: position (vec3), color (vec3), size (float) — stride 28
- [ ] Vertex shader: transform position, set `gl_PointSize` from magnitude + camera distance
- [ ] Fragment shader: circular point with soft radial gradient, color from B-V
- [ ] Additive blending (`GL_SRC_ALPHA, GL_ONE`) for star glow
- [ ] Scale-gated: visible at LAYER_STARS scale range
- [ ] Sky dome approach: stars on celestial sphere at radius ~100 scene units
- [ ] Depends on: 7.5 star_catalog (Agent A)

### 7.8 Constellation Overlay — Orchestrator
- [ ] Reuse existing line shader (per-vertex RGBA) for stick figures
- [ ] Generate line vertex data from constellation_lines module
- [ ] Lines faint (alpha ~0.3), zodiac constellations slightly brighter
- [ ] Optional: constellation name labels via billboard glyph system (from 3.2)
- [ ] Toggle with 'C' key (constellations on/off)
- [ ] Scale-gated: same range as LAYER_STARS
- [ ] Depends on: 7.6 constellation_lines (Agent A) + 7.7 star shader

### 7.9 Star Label System — Orchestrator
- [ ] Named stars (~30 brightest) get billboard text labels
- [ ] Reuse glyph billboard system from Track 3.2
- [ ] Labels fade with camera distance (only visible at closer zoom)
- [ ] Toggle with 'S' key (star labels on/off)
- [ ] Depends on: 7.5 star_catalog + glyph billboard infrastructure

### 7.2 Milky Way Spiral — Geometry DELIVERED (Agent A: galaxy_geometry.c, 20 tests)
- [ ] Wire galaxy_generate() into render loop at Scale 5
- [ ] Sun's position marked: ~27,000 light-years from center
- [ ] Galactic rotation visible when time is set to millions-of-years scale
- [ ] Galactic year: ~225 million Earth years per orbit
- [ ] Use golden_spiral() from sacred geometry module

### 7.3 Cosmic Time Display
- [ ] At galaxy scale, display cosmic time metrics:
  - Age of universe: 13.8 billion years
  - Galactic year position (current: ~20th orbit)
  - Sun's age: 4.6 billion years
  - Time since last supernovae in neighborhood
- [ ] Time scale selector: seconds / days / years / millennia / megayears / gigayears

### 7.4 Scale Labels
- [ ] Show current scale label: "Solar System" / "Milky Way" / "Local Group"
- [ ] Distance indicator: "50 AU" / "100 light-years" / "2.5 million light-years"
- [ ] Smooth transition text during zoom

---

## Execution Priority Order

Work these tracks interleaved, not sequentially. Priority by impact:

```
COMPLETED:
  1.1-1.4  Track 1 (HUD wiring) — all done
  2.1-2.4  Track 2 (multi-scale camera) — all done

CURRENT SESSION:
  3.1  Ecliptic ring geometry — DONE
  3.3  Planet markers on ring — DONE
  3.4  Aspect lines — DONE
  3.5  House cusp lines — DONE
  3.2  Sign labels on ring — DONE
  1.5  Browser verify (Gabriel tests)

NEXT SESSION:
  7.5  Star catalog data (Agent A — pure module)
  7.6  Constellation lines (Agent A — pure module)
  7.7  Star point sprite shader (orchestrator — wire 7.5)
  7.8  Constellation overlay (orchestrator — wire 7.6)
  7.9  Star labels (orchestrator — reuse glyph billboards)
  1.5  Browser verify (Gabriel tests all rendering)

FOLLOWING:
  4.1  Card renderer infrastructure
  4.2-4.6  Knowledge system cards
  5.1  Date/time input
  5.2-5.4  User interaction

LONG-TERM:
  7.2  Milky Way spiral (wire galaxy_geometry)
  7.3  Cosmic time display
  6.1-6.4  Infrastructure hardening

ONGOING:
  Integrate agent modules into Makefile + WASM build
  Run /purity, /refresh after integrations
```

---

## Dependencies on Agent Pipeline

Agents self-direct from unchecked items below. This table tracks which track items
depend on pure modules (delivered or needed).

| Track Item | Needs Module | Status |
|------------|-------------|--------|
| 3.1 Ecliptic ring | arc_geometry | DELIVERED |
| 3.2 Sign labels | font_atlas, zodiac_glyphs | DELIVERED |
| 3.3 Planet glyphs | wheel_layout | DELIVERED |
| 3.4 Aspect lines | aspect_lines | DELIVERED |
| 3.6 Screen projection | projection (Agent A) | DELIVERED |
| 3.7 Billboard geometry | billboard (Agent A) | DELIVERED |
| 3.5 House cusp lines | cusp_lines (Agent A) | DELIVERED |
| 7.2 Galaxy spiral | galaxy_geometry (Agent A) | DELIVERED |
| 4.1 Card renderer | card_data | DELIVERED |
| 4.3 I Ching card | hexagram_visual (Agent B) | DELIVERED + INTEGRATED |
| 4.5 HD card | human_design | DELIVERED |
| 4.8 Time formatters | time_format (Agent B) | DELIVERED + INTEGRATED |
| 4.9 Scale HUD selector | scale_hud (Agent B) | DELIVERED + INTEGRATED |
| 5.5 Date parser | date_parse (Agent B) | DELIVERED + INTEGRATED |
| 5.6 City presets | location_presets (Agent B) | DELIVERED + INTEGRATED |
| 3.8 Glyph batch | glyph_batch (Agent A) | NEEDED |
| 4.10 Tzolkin card data | tzolkin_card (Agent B) | NEEDED |
| 4.11 Astrology formatter | astro_fmt (Agent B) | NEEDED |
| 4.12 Haab calendar | haab (Agent B) | NEEDED |
| 7.5 Star catalog | star_catalog (Agent A) | NEEDED — PRIORITY |
| 7.6 Constellation lines | constellation_lines (Agent A) | NEEDED — PRIORITY |
| 7.7 Star shader | star_catalog (7.5) | BLOCKED on 7.5 |
| 7.8 Constellation overlay | constellation_lines (7.6) + line shader | BLOCKED on 7.6 |
| 7.9 Star labels | star_catalog (7.5) + glyph billboards | BLOCKED on 7.5 |
| 2.4 Scale transitions | easing | DELIVERED |
| 7.2 Milky Way spiral | sacred_geometry | DELIVERED |
| 7.3 Cosmic time | cosmic_time | DELIVERED |

---

## Success Metrics

| Milestone | Criteria |
|-----------|----------|
| Phase 4 COMPLETE | All 6 criteria checked, LST displayed |
| Phase 5 VISUAL | Zodiac ring renders in browser with planet positions |
| Phase 6 COMPUTED | All 7 knowledge systems return correct data for any date |
| Phase 6 VISUAL | At least 4 knowledge system cards render in browser |
| Multi-scale PROOF | Camera zooms from chart view to galaxy view smoothly |
| Phase 7 INTEGRATED | All layers visible, 60fps, smooth transitions |
| Phase 8 DEPLOYED | Live on the web |

---

**This document is the primary orchestrator's work plan. It is NOT the project roadmap (that's ROADMAP.md). It is what I work on, in what order, while agents handle the module pipeline.**
