# Orchestrator Roadmap — Primary Agent Work Plan

**Owner**: Primary Claude (orchestrator)
**Last Updated**: 2026-03-08
**Philosophy**: I build the architecture and the visuals. Agents build the modules. I focus on what connects everything.

---

## ADR-006 PIVOT: Art-First Rendering (2026-03-08)

**ALL rendering work now targets cinematic visual quality.** The data engine is massive (147+ deliveries, 172 tests) but visuals are prototype-level. Priority shifts from "build more invisible modules" to "make what exists beautiful."

**Key changes:**
- Render pass architecture (ADR-005): GL code in `src/render/passes/`, wiring is 4-step recipe
- Procedural sun shader: `noise_shader.h/.c` + `sun_shader.h/.c` replace flat yellow sphere
- OMEGA now wires delivered packs (see OMEGA priority.md section 11)
- DELTA now researches shader techniques alongside book extraction
- ALPHA builds shader data packs (noise, atmosphere, procedural surfaces)

**Immediate MEGA priorities:** Sun shader, Earth View mode, planet surface shaders, bloom/glow

---

## Delegation Model

```
MEGA (me):     Architecture, shader authorship, the art itself, cinematic rendering.
               Macro-level direction: I define WHAT needs building in this roadmap.
               I write shaders. I make it beautiful.

OMEGA:         Wiring delivered packs into render passes, git commits, codebase health.
               The finisher. Mechanical maintenance following recipes.

ALPHA:         Shader data packs, procedural noise, atmosphere models, GPU-ready vertex data.
               /execute ALPHA

BETA:          UI data, formatting, knowledge system visuals, display logic.
               /execute BETA

GAMMA:         Sacred calendars, world calendar algorithms, book extraction (code tasks).
               /execute GAMMA

DELTA:         Research mining: PDF books + shader technique research (Shadertoy, papers).
               No C code. /execute DELTA
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

### 3.8 Agent: Glyph Quad Batch Builder (Agent A) — DELIVERED (ALPHA: glyph_batch, 33 tests)
- [x] Pure module: takes glyph IDs + world positions + sizes → interleaved vertex data
- [x] Combines billboard geometry + font_atlas UVs into a single draw batch
- [x] Output: flat float arrays (position, uv, color) ready for GPU upload
- [x] Configurable: per-glyph tint color, per-glyph scale
- [x] Used by: 3.2 sign labels, 3.3 planet glyphs (future glyph rendering)

### 3.6 Agent: Screen Projection Utilities (Agent A) — DELIVERED
- [x] Pure module: project 3D world position to 2D screen coordinates
- [x] Inverse: unproject screen coords to world ray
- [x] Viewport-aware (width, height, aspect ratio)
- [x] 22 tests, 0 failures

### 3.7 Agent: Billboard Geometry (Agent A) — DELIVERED
- [x] Camera-facing quads for sign labels + planet glyphs
- [x] Y-locked mode, pixel-size computation, batch generation
- [x] 20 tests, 0 failures

### 3.9 Agent: Zodiac Render Pack (Agent A) — DELIVERED
- [x] Pure module: `src/render/zodiac_pack.h/.c`
- [x] Unified vertex packer: ALL zodiac wheel elements into batched draw arrays
- [x] Ring segments: 12 colored arcs from ring_geometry + arc_geometry → triangle strip vertices
- [x] Degree tick marks: 360 small radial lines as GL_LINES vertex data
- [x] House cusp lines: cusp_lines_generate() → packed line vertices with angular cusps brighter
- [x] Aspect lines: aspect_lines_generate() → packed line vertices with orb-based alpha
- [x] Sign glyph quads: glyph_batch positions for 12 zodiac glyphs on ring
- [x] Planet marker quads: glyph_batch positions for planet markers at geocentric longitudes
- [x] Zodiac ring shader source as static const strings (GLSL ES 3.00)
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: ring_geometry, arc_geometry, cusp_lines, aspect_lines, glyph_batch, billboard

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

### 4.10 Agent: Tzolkin Card Data (Agent B) — DELIVERED (BETA: tzolkin_card, 58 tests)
- [x] Pure module: format Tzolkin day data for card display
- [x] Seal family color (Red/White/Blue/Yellow) from seal index
- [x] Wavespell position (1-13), castle assignment (1-5)
- [x] Oracle structure: guide, analog, antipode, occult, from kin number
- [x] Used by: 4.2 Tzolkin Kin Card

### 4.11 Agent: Astrology Summary Formatter (Agent B) — DELIVERED (GAMMA: astro_summary, 37 tests)
- [x] Pure module: format complete astrology snapshot for card display
- [x] Planet positions as "Sun ♌ Leo 15.3°" style strings
- [x] Dignity status per planet (domicile/detriment/exaltation/fall)
- [x] Aspect summary with orb values
- [x] Used by: 4.6 Astrology Summary Card

### 4.12 Agent: Haab Calendar (Agent B) — DELIVERED (BETA: haab, 41 tests)
- [x] Pure module: Mayan 365-day solar calendar (complements Tzolkin)
- [x] 18 months of 20 days + 5-day Wayeb period
- [x] Haab date from Julian Day (using GMT correlation 584283)
- [x] Month name + day number (e.g., "3 Pop", "17 Yax")
- [x] Used by: Tzolkin/Haab Calendar Round display

### 4.13 Agent: Card Quad Pack (Agent A) — DELIVERED
- [x] Pure module: `src/render/card_pack.h/.c`
- [x] Pack card_layout screen-space positions into quad vertex arrays for batched rendering
- [x] Per card: 4 vertices (pos + uv + color) forming a screen-space quad
- [x] Background quads, border lines, section dividers for each card type
- [x] `card_pack(const card_layout_t *layout, float viewport_w, float viewport_h, float *out)` → vertex count
- [x] Card shader source (GLSL ES 3.00) — screen-space quad with rounded corners + transparency
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: card_layout (4.1)

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

### 7.5 Agent: Star Catalog Data (Agent A) — DELIVERED (ALPHA: 195 stars, 27 tests)
- [x] Pure module: `src/render/star_catalog.h/.c`
- [x] Data: 195 brightest stars (V magnitude < 4.5) as `static const` arrays
- [x] Per star: RA (hours), Dec (degrees), V magnitude, B-V color index, name (if named), constellation ID
- [x] `star_to_cartesian(ra_hours, dec_deg, radius)` → vec3 (equatorial to Cartesian)
- [x] `star_color_from_bv(bv)` → RGB (blackbody approximation: blue→white→yellow→orange→red)
- [x] `star_size_from_mag(mag, mag_limit)` → float (inverse: brighter = bigger point)
- [x] `star_catalog_count()`, `star_catalog_get(index)` → accessor functions
- [x] All data compiled into binary (no file I/O, no malloc)
- [x] Used by: 7.7 star point sprite shader

### 7.6 Agent: Constellation Lines (Agent A) — DELIVERED (ALPHA: 40 constellations, 37 tests)
- [x] Pure module: `src/render/constellation.h/.c`
- [x] Data: 12 zodiac constellations + 28 major constellations (40 total)
- [x] Per constellation: name, IAU 3-letter abbreviation, array of (star_index_a, star_index_b) pairs
- [x] `constellation_lines_for(id)` → array of star-index pairs + count
- [x] `constellation_containing_longitude(ecliptic_lon)` → constellation ID
- [x] `constellation_count()`, `constellation_name(id)`, `constellation_abbr(id)` → accessors
- [x] Zodiac constellations are PRIORITY (connect to astrology layer)
- [x] Star indices reference star_catalog entries by position in the catalog array
- [x] Used by: 7.8 constellation overlay

### 7.7a Agent: Star Field GPU Data (Agent A) — DELIVERED (ALPHA: star_field, 53 tests)
- [x] Pure module: `src/render/star_field.h/.c`
- [x] Pack star_catalog (195) + star_catalog_ext (400) into interleaved vertex arrays
- [x] Per star: position(vec3) + color(vec3) + size(float) = stride 28 bytes
- [x] Constellation line packing: star index pairs to 3D line vertices
- [x] Tier offsets for LOD: tier 0 (brightest ~21) through tier 2 (500+ stars)
- [x] Star + constellation shader source as static const strings (GLSL ES 3.00)
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: 7.5 star_catalog, 9.1 star_catalog_ext, 9.2 star_colors, 7.6 constellation

### 7.7b Star Rendering — Orchestrator
- [ ] Compile star/constellation shaders from star_field shader source strings
- [ ] Upload packed vertex data from star_field_pack() to GPU VBOs
- [ ] Draw stars as GL_POINTS with additive blending (GL_SRC_ALPHA, GL_ONE)
- [ ] gl_PointSize perspective-scaled by camera distance
- [ ] Sky dome: stars on celestial sphere at radius ~100 scene units
- [ ] Scale-gated: visible at LAYER_STARS scale range
- [ ] Depends on: 7.7a star_field (Agent A)

### 7.8 Constellation Overlay — Orchestrator
- [ ] Draw constellation lines from star_field constellation_lines_pack() vertex data
- [ ] Lines faint (alpha ~0.25), zodiac constellations brighter (~0.4)
- [ ] Optional: constellation name labels via billboard glyph system (from 3.2)
- [ ] Toggle with 'C' key (constellations on/off)
- [ ] Scale-gated: same range as LAYER_STARS
- [ ] Depends on: 7.7a star_field data + 7.7b star rendering

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

## TRACK 8: True-Color Planetary Rendering

Gas giants and rocky planets with scientifically accurate colors, atmospheric effects, and real physical data.

### 8.1 Agent: Planetary Physical Data (Agent B) — DELIVERED (GAMMA: planet_data, 39 tests)
- [x] Pure module: `src/systems/astronomy/planet_data.h/.c`
- [x] Per-planet: equatorial radius (km), mass, albedo, axial tilt, rotation period
- [x] Surface/atmosphere color profiles from NASA Planetary Fact Sheet (RGB triplets)
- [x] Jupiter: banded atmosphere (orange/white/brown), Great Red Spot latitude
- [x] Saturn: pale gold, ring system inner/outer radii + opacity
- [x] Mars: rusty iron oxide, polar cap albedo variation
- [x] Earth: ocean blue, cloud white, land green/brown
- [x] Gas giant color gradients: latitude-dependent banding data
- [x] Data source: NASA Planetary Fact Sheet + JPL Horizons
- [x] Book needed: **"Fundamentals of Planetary Science"** (Lissauer & de Pater) — still valuable for future refinement

### 8.2 Agent: Planet Atmosphere Shader Data (Agent A) — DELIVERED (ALPHA: atmo_ring, 42 tests)
- [x] Pure module: geometry for atmospheric limb glow — `src/render/atmo_ring.h/.c` (42 tests)
- [x] Rayleigh scattering approximation: blue scatter at edges
- [x] Per-planet atmosphere thickness ratio (relative to radius)
- [x] Ring geometry for Saturn + Uranus + Neptune (inner/outer radius, Cassini gap, opacity)
- [x] Output: vertex data for atmosphere shells + ring quads + Fresnel limb brightening

### 8.6 Agent: Planet Render Pack (Agent A) — DELIVERED
- [x] Pure module: `src/render/planet_pack.h/.c`
- [x] Pack planet positions + colors + sizes into interleaved vertex arrays
- [x] Per planet: position(vec3) + color(vec3) + radius(float) + atmo_thickness(float) = stride 32 bytes
- [x] Orbit trail packing: per-planet polyline vertices (N samples around orbit)
- [x] `planet_pack(double jd, float *out, int max_planets)` → vertex count
- [x] `orbit_trail_pack(int planet_idx, double jd, float *out, int max_verts)` → vertex count
- [x] Planet billboard shader source as static const strings (GLSL ES 3.00)
- [x] Orbit trail line shader source (per-vertex color + alpha fade by trail age)
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: planet_data (8.1), planetary_pos, atmo_ring (8.2), orbit_trail

### 8.3 Planet Surface Shader — Orchestrator
- [ ] Enhanced planet shader: per-planet color from planet_data module
- [ ] Latitude-dependent banding for gas giants (procedural)
- [ ] Atmospheric limb glow (Fresnel-like edge brightening)
- [ ] Saturn ring rendering (textured ring plane, shadow on planet)
- [ ] Toggle: T(rue) color mode vs simple color mode

### 8.4 Agent: Moon Data (Agent B) — DELIVERED (BETA: moon_data, 33 tests)
- [x] Pure module: 9 major moons — `src/systems/astronomy/moon_data.h/.c` (33 tests)
- [x] Earth: Moon (orbital elements, Keplerian position, scene scaling)
- [x] Mars: Phobos, Deimos
- [x] Jupiter: Io, Europa, Ganymede, Callisto (Galilean moons)
- [x] Saturn: Titan, Enceladus
- [x] Moon positions from Keplerian elements + solve_kepler()
- [x] Data source: NASA/JPL Planetary Satellite Physical Parameters

### 8.7 Agent: Moon Render Pack (Agent A) — DELIVERED
- [x] Pure module: `src/render/moon_pack.h/.c`
- [x] Pack 9 major moon positions + colors + sizes into vertex arrays
- [x] Moon orbit trail vertices (short trails to show orbital motion)
- [x] Moon billboard shader source (GLSL ES 3.00)
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: moon_data (8.4)

### 8.5 Moon Rendering — Orchestrator
- [ ] Render major moons as small spheres at computed positions
- [ ] Moon trails (optional, same VertexID technique)
- [ ] Visible at Scale 1-2 (Earth-Moon to Inner Solar System)
- [ ] Galilean moon dance visible when time accelerated

---

## TRACK 9: Deep Star Field (Planetarium Quality)

From 200 stars to 9000+. Real sky as seen from Earth.

### 9.1 Agent: Extended Star Catalog (Agent A) — DELIVERED (agent-ALPHA, 54 tests)
- [x] Pure module: `src/render/star_catalog_extended.h/.c`
- [ ] Data: Yale Bright Star Catalogue (BSC5) — 9110 stars to V=6.5
- [ ] Source: FREE open data (VizieR / Yale archives)
- [ ] Per star: RA, Dec, V mag, B-V color, spectral type, proper name (if any)
- [ ] Split into brightness tiers for LOD (Level of Detail):
  - Tier 0: V < 1.5 (21 stars — always visible, named, labeled)
  - Tier 1: V < 3.0 (~170 stars — visible at medium zoom)
  - Tier 2: V < 4.5 (~1000 stars — visible at wide view)
  - Tier 3: V < 6.5 (~9000 stars — full sky, close zoom only)
- [ ] Book needed: **Yale Bright Star Catalogue (BSC5)** — FREE data download

### 9.2 Agent: Star Spectral Classification Colors (Agent A) — DELIVERED (ALPHA: star_colors, 34 tests)
- [x] Pure module: `src/render/star_colors.h/.c` (34 tests)
- [x] B-V to RGB via Planck blackbody physics (Ballesteros formula)
- [x] Spectral types O through M with color temperature mapping
- [x] Verified against known star colors

### 9.5 Agent: Star Diffraction Geometry (Agent A) — DELIVERED
- [x] Pure module: `src/render/diffraction.h/.c`
- [x] Procedural cross/spike geometry for brightest stars (magnitude < 1.5)
- [x] Per star: 4 or 6 spike lines radiating from center, length proportional to brightness
- [x] Spike vertex data: position(vec3) + color(vec4) + alpha fade along spike length
- [x] `diffraction_pack(const float *star_positions, const float *magnitudes, int count, float *out)` → vertex count
- [x] Diffraction spike shader source (GLSL ES 3.00)
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: star_field (7.7a)

### 9.3 Star LOD Shader System — Orchestrator
- [ ] Tiered rendering: draw brightest tier always, add tiers as zoom increases
- [ ] Point size varies with magnitude AND camera distance
- [ ] Star twinkle: subtle animated brightness variation (atmospheric scintillation)
- [ ] Star diffraction spikes on brightest stars (procedural cross pattern in fragment shader)
- [ ] Bloom/glow pass for magnitude < 1.5 stars

### 9.4 Proper Motion Animation — Future
- [ ] Stars move! Proper motion data from Hipparcos/Gaia catalogs
- [ ] Visible only at extreme time scales (thousands of years)
- [ ] Constellation shapes deform over millennia
- [ ] Data source: Hipparcos catalog (FREE) or Gaia DR3

---

## TRACK 10: Nebulae & Deep Sky Objects

The gas clouds, the stellar nurseries, the remnants.

### 10.1 Agent: Deep Sky Object Catalog (Agent B) — DELIVERED (BETA: deep_sky, 35 tests)
- [x] Pure module: `src/render/deep_sky.h/.c` (35 tests)
- [x] Messier objects + NGC objects with RA, Dec, type, angular size, brightness
- [x] Type classification: emission/planetary nebula, open/globular cluster, galaxy
- [x] Visual color hints per type

### 10.2 Agent: Deep Sky Render Pack (Agent A) — DELIVERED
- [x] Pure module: `src/render/deep_sky_pack.h/.c`
- [x] Pack deep_sky catalog into billboard vertex arrays for nebula/cluster rendering
- [x] Per object: position(vec3) + size(float) + color(vec4) + type(int) — billboard quads
- [x] Type-based color assignment: emission=red/pink, reflection=blue, planetary=teal, cluster=white
- [x] `deep_sky_pack(float sphere_radius, const float cam_right[3], const float cam_up[3], float *out)` → vertex count
- [x] Nebula billboard shader source (GLSL ES 3.00) — gaussian blob with radial falloff
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: deep_sky (10.1)

### 10.3 Nebula Shader — Orchestrator
- [ ] Procedural nebula rendering from catalog positions + angular sizes
- [ ] Soft gaussian blobs with type-dependent coloring
- [ ] H-II regions: warm red/pink (hydrogen alpha emission)
- [ ] Reflection nebulae: blue scattered light
- [ ] Planetary nebulae: green/teal (O-III emission)
- [ ] Opacity from surface brightness, scale-gated to LAYER_STARS range
- [ ] NOT photographic textures — procedural from data (consistent with project philosophy)

### 7.10 Agent: Milky Way Render Pack (Agent A) — DELIVERED
- [x] Pure module: `src/render/milkyway_pack.h/.c`
- [x] Pack galaxy_geometry spiral arm data into vertex arrays for band rendering
- [x] Galactic plane mesh: textured strip along galactic equator with opacity variation
- [x] Dark lane data: opacity reduction zones for the Great Rift
- [x] `milkyway_pack(float sphere_radius, float *out, int max_verts)` → vertex count
- [x] Milky Way shader source (GLSL ES 3.00) — procedural glow with dark lanes
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: galaxy_geometry (7.2)

### 10.3 Milky Way Band — Orchestrator
- [ ] Dense star field along galactic plane
- [ ] Procedural glow following galactic coordinates
- [ ] Dark lanes (Rift) as opacity reduction zones
- [ ] Visible at Scales 2-4, dominant at Scale 3
- [ ] Combines with individual star rendering for depth

---

## TRACK 11: Advanced Knowledge System Visuals

Beyond cards — immersive visual representations.

### 11.1 Agent: Hexagram Visual Geometry (Agent A) — DELIVERED
- [x] Pure module: `src/render/hexagram_geometry.h/.c`
- [x] 6-line hexagram as vertex data: solid lines (yang) and broken lines (yin) as GL_LINES
- [x] `hexagram_pack(int hexagram_number, float *out, float width, float height)` → vertex count
- [x] Per line: position(vec2) + color(vec4) — changing lines highlighted with distinct color
- [x] Trigram decomposition: upper/lower split with visual gap
- [x] Nuclear hexagram derivation: lines 2-3-4, 3-4-5 rendered alongside
- [x] All 64 hexagrams renderable; King Wen number as input
- [x] Hexagram line shader source as static const strings (GLSL ES 3.00)
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: iching module (hexagram_lines data)

### 11.4 Agent: Bodygraph Render Pack (Agent A) — DELIVERED (ALPHA: bodygraph_pack, 47 tests)
- [x] Pure module: `src/render/bodygraph_pack.h/.c`
- [x] Pack bodygraph 9 centers as colored quads/circles (defined vs undefined coloring)
- [x] Pack 36 channels as line segments with activation coloring
- [x] Pack 64 gate positions as small markers on channels
- [x] `bgp_pack_centers()`, `bgp_pack_channels()`, `bgp_pack_gates()` → vertex counts
- [x] Bodygraph shader source (GLSL ES 3.00) — colored shapes with glow for defined centers
- [x] No GL calls — produces float arrays ready for glBufferData
- [x] Depends on: bodygraph (11.2)

### 11.2 Agent: Human Design Bodygraph Data (Agent A) — DELIVERED (ALPHA: bodygraph, 44 tests)
- [x] Pure module: bodygraph geometry (9 centers, 36 channels, 64 gates) — **44 tests**
- [x] Center positions as 2D coordinates (head, ajna, throat, G, heart, spleen, sacral, root, solar plexus)
- [x] Channel line segments connecting centers
- [x] Gate positions on channels (for planetary activation highlighting)
- [x] Defined vs undefined centers from planetary gate activations

### 11.3 Tzolkin Grid Visual — Orchestrator
- [ ] 13x20 grid (260 kin) rendered as colored cells
- [ ] Current kin highlighted with glow
- [ ] Wavespell highlighted (13-day wave)
- [ ] Castle coloring (Red/White/Blue/Yellow/Green)
- [ ] Harmonic families visible as columns

### 11.4 Natal Chart Wheel — Orchestrator
- [ ] Traditional circular chart with 12 houses
- [ ] Planet glyphs at their zodiacal positions
- [ ] Aspect lines drawn between planets
- [ ] House cusp markers
- [ ] Rotates with Ascendant at 9 o'clock position (Western convention)

---

## TRACK 12: Data Pipeline Automation

### 12.1 Agent: Catalog Ingestion Framework (Agent A) — DELIVERED (agent-ALPHA, 60 tests)
- [x] Pure module: parse CSV/TSV star catalog format into C struct arrays
- [ ] Validate data ranges (RA 0-24h, Dec -90 to +90, mag reasonable)
- [ ] Generate `static const` C arrays from parsed data (code generation)
- [ ] Used by: converting BSC5 CSV → star_catalog_extended.c at build time

### 12.2 Research Digest Automation
- [ ] Agents extract algorithms from PDFs → write .context/research/digested/ files
- [ ] Each digest: source, page range, algorithm pseudocode, C implementation plan
- [ ] Digest feeds directly into task.md API design section
- [ ] Attribution automatically added to data/contributors.json

---

## TRACK 13: Geological Deep Time

Earth's 4.6-billion-year history as a time system. Visible at Scale 5-6.

### 13.1 Agent: Geological Time Scale Data (Agent B) — DELIVERED (BETA: geo_time, 46 tests)
- [x] Pure module: `src/systems/geology/geo_time.h/.c` — **46 tests**
- [x] International Chronostratigraphic Chart data (ICS 2024)
- [x] Eons: Hadean, Archean, Proterozoic, Phanerozoic (start dates in Ma)
- [x] Eras: Paleozoic, Mesozoic, Cenozoic (with sub-eras)
- [x] Periods: Cambrian through Quaternary (with epochs)
- [x] Per entry: name, start_ma (millions of years ago), end_ma, color (ICS standard)
- [x] `geo_eon_at(double years_ago)` → eon data
- [x] `geo_period_at(double years_ago)` → period data
- [x] `geo_epoch_at(double years_ago)` → epoch data
- [x] Data source: GitHub `TobbeTripitaka/strat2file` (JSON/CSV from ICS, FREE open data)

### 13.2 Agent: Radiometric Dating Constants (Agent A) — DELIVERED (ALPHA: radiometric, 61 tests)
- [x] Pure module: isotope half-lives and decay chains
- [x] U-238→Pb-206 (4.47 Ga), U-235→Pb-207 (704 Ma), K-40→Ar-40 (1.25 Ga)
- [x] C-14→N-14 (5730 yr), Rb-87→Sr-87 (48.8 Ga)
- [x] `decay_fraction(half_life, elapsed_time)` → remaining fraction
- [x] `age_from_ratio(parent, daughter, half_life)` → age estimate
- [x] Visual: decay curve data for shader animation

### 13.3 Geological Timeline — Orchestrator
- [ ] Deep time display at Scale 5-6: "Cenozoic Era · Quaternary · Holocene"
- [ ] Color-coded timeline bar (ICS standard colors)
- [ ] Major events annotated: Great Oxidation, Cambrian Explosion, K-Pg extinction
- [ ] Integrates with cosmic_time module at largest scales

---

## TRACK 14: Hebrew Calendar (Lunisolar)

The oldest continuously used calendar. 19-year Metonic cycle, leap months, sabbatical rhythm.

### 14.1 Agent: Hebrew Calendar Algorithms (Agent B) — DELIVERED (GAMMA: hebrew, 49 tests)
- [x] Pure module: `src/systems/hebrew/hebrew.h/.c` (49 tests)
- [x] Hebrew year from Julian Day (Calendrical Calculations algorithm)
- [x] 12 or 13 months per year (Adar I/Adar II leap month)
- [x] 19-year Metonic cycle: years 3, 6, 8, 11, 14, 17, 19 are leap
- [x] Molad interval: 29d 12h 793 chalakim (1 chelek = 1/1080 hour)
- [x] 4 postponement rules (dechiyyot): Lo ADU, Molad Zaken, GaTRaD, BeTUTaKPaT
- [x] Month lengths: deficient (353), regular (354), abundant (355) + leap variants
- [x] Round-trip JD↔Hebrew verified across multiple eras
- [x] Data source: Calendrical Calculations + Russinoff paper

### 14.2 Agent: Sabbatical & Jubilee Cycles (Agent B) — DELIVERED (BETA: sabbatical, 38 tests)
- [x] Pure module: 7-year Sabbatical (Shemitah) and 50-year Jubilee cycles
- [x] `sabbatical_year(int hebrew_year)` → position in 7-year cycle (1-7)
- [x] `jubilee_year(int hebrew_year)` → position in 50-year cycle
- [x] `is_shemitah(int hebrew_year)` → bool
- [x] `next_jubilee(int hebrew_year)` → year of next Jubilee
- [x] Holiday calculations: Rosh Hashanah, Yom Kippur, Sukkot, Passover, Shavuot

### 14.3 Hebrew Calendar Display — Orchestrator
- [ ] HUD integration: "5786 · Tishrei 15 · Sukkot"
- [ ] Sabbatical cycle indicator (year X of 7)
- [ ] Toggle: H key (Hebrew calendar on/off)

---

## TRACK 15: Islamic Calendar (Pure Lunar)

354-355 day year. No intercalation. The calendar drifts through all seasons over 33 years.

### 15.1 Agent: Hijri Calendar Algorithms (Agent B) — DELIVERED
- [x] Pure module: `src/systems/islamic/hijri.h/.c` (51 tests, GAMMA)
- [x] Hijri date from Julian Day (tabular Islamic calendar algorithm)
- [x] 12 months alternating 30/29 days, plus leap day in month 12
- [x] 30-year cycle with 11 leap years, round-trip JD conversion verified
- [x] All 12 month names (Muharram through Dhul Hijjah)

### 15.2 Agent: Prayer Time Calculator (Agent B) — DELIVERED (GAMMA: prayer_times, 38 tests)
- [x] Pure module: `src/systems/islamic/prayer_times.h/.c`
- [x] 5 daily prayer times from solar position (reuse solar_events + observer modules)
- [x] Fajr (dawn, sun depression 18°/15°), Dhuhr (midday), Asr (afternoon, shadow ratio), Maghrib (sunset), Isha (night, 17°/15°)
- [x] `prayer_times(double jd, double lat, double lon, int method)` → prayer_times_t
- [x] Multiple calculation methods (MWL, ISNA, Egypt, Umm al-Qura, Karachi, Tehran)
- [x] Shadow ratio for Asr: Hanafi (shadow = 2× object) vs Shafi'i (shadow = 1× object)
- [x] Data source: praytimes.org/calculation (FREE pseudocode) + Mohamoud 2017 paper (FREE PDF)
- [x] Depends on: solar_events, observer modules (ALREADY DELIVERED)

### 15.3 Islamic Calendar Display — Orchestrator
- [ ] HUD integration: "1447 · Ramadan 15"
- [ ] Prayer time overlay (next prayer countdown)
- [ ] Toggle: I key (Islamic calendar on/off)

---

## TRACK 16: Buddhist Time

Mind-scale time. From Uposatha lunar days to kalpas spanning billions of years.

### 16.1 Agent: Buddhist Calendar Data (Agent B) — DELIVERED (GAMMA: buddhist, 45 tests)
- [x] Pure module: `src/systems/buddhist/buddhist.h/.c` — **45 tests**
- [x] Buddhist Era: BE = CE + 543 (Theravada convention)
- [x] Uposatha observance days from lunar phase (new moon, full moon, quarter moons)
- [x] Vassa (Rains Retreat), 12 Pali lunar months, Vesak full moon computation
- [x] `buddhist_era()`, `buddhist_uposatha()`, `buddhist_next_uposatha()`, `buddhist_month()`, `buddhist_vesak_jd()`
- [x] Depends on: lunar module (DELIVERED)

### 16.2 Agent: Kalpa & Cosmic Cycle Data (Agent B) — DELIVERED (BETA: kalpa, 49 tests)
- [x] Pure module: `src/systems/buddhist/kalpa.h/.c` — **49 tests**
- [x] Maha-kalpa hierarchy: 4 phases (formation, existence, destruction, emptiness)
- [x] 20 antarakalpas per kalpa with lifespan cycle data
- [x] `kalpa_position(cosmic_age)`, `kalpa_from_geological(ma)`, `kalpa_from_cosmic_age()`
- [x] `kalpa_antarakalpa_info()` — lifespan cycle within existence phase
- [x] Maps to geological time and cosmic age

### 16.3 Buddhist Time Display — Orchestrator
- [ ] HUD at personal scale: "BE 2569 · Uposatha (Full Moon)"
- [ ] Deep time scale: "Current Kalpa · Existence Phase"
- [ ] Toggle: B key (Buddhist time on/off)

---

## TRACK 17: Hindu/Vedic Time

The most mathematically elaborate time system ever devised. Panchanga (5 limbs) + Yuga cycles.

### 17.1 Agent: Panchanga Calculator (Agent B) — DELIVERED (BETA: panchanga, 42 tests)
- [x] Pure module: `src/systems/hindu/panchanga.h/.c`
- [x] 5 limbs of Hindu time from Julian Day + sidereal longitude:
- [x] Tithi: 30 lunar days per synodic month (Sun-Moon elongation / 12°)
- [x] Vara: 7 weekdays (shared with Gregorian)
- [x] Nakshatra: 27 lunar mansions (Moon's sidereal longitude / 13.333°)
- [x] Yoga: 27 yogas (Sun + Moon sidereal longitude / 13.333°)
- [x] Karana: 60 karanas per month (half-tithis)
- [x] `panchanga_from_jd(double jd, double sun_lon, double moon_lon)` → panchanga_t
- [x] Data source: Calendrical Calculations (ACQUIRED) + Surya Siddhanta (FREE on archive.org) + Narasimha Rao (FREE PDF)
- [x] Depends on: lunar module, sidereal module (ALREADY DELIVERED)

### 17.2 Agent: Nakshatra Star Positions (Agent A) — DELIVERED (ALPHA: nakshatra, 53 tests)
- [x] Pure module: `src/systems/hindu/nakshatra.h/.c`
- [x] 27 Nakshatras with ecliptic longitude boundaries (each 13°20' = 800 arcmin)
- [x] Per Nakshatra: name (Sanskrit), ruling planet, deity, symbol, quality
- [x] Junction stars (yogatara): specific star from star_catalog that defines each Nakshatra
- [x] Known yogataras: Ashwini=beta Ari, Krittika=Alcyone, Rohini=Aldebaran, Ardra=Betelgeuse, Magha=Regulus, Chitra=Spica, Swati=Arcturus, Jyeshtha=Antares, Revati=zeta Psc
- [x] `nakshatra_from_longitude(double sidereal_lon)` → nakshatra_t
- [x] `nakshatra_star_index(int nakshatra_id)` → star_catalog index
- [x] 4 padas (quarters) per Nakshatra: 27 × 4 = 108 (sacred number)
- [x] Navamsa mapping: pada → zodiac sign (108 ÷ 12 = 9 per sign)
- [x] Ayanamsa: Lahiri (Spica at 0° Libra sidereal), currently ~24°
- [x] Data source: Surya Siddhanta (FREE on archive.org) + Wikipedia Nakshatra list + Hipparcos cross-ref
- [x] Depends on: star_catalog (DELIVERED), sidereal module

### 17.3 Agent: Yuga Cycle Data (Agent B) — DELIVERED (GAMMA: yuga, 34 tests)
- [x] Pure module: `src/systems/hindu/yuga.h/.c`
- [x] 4 Yugas: Satya (1,728,000 yr), Treta (1,296,000 yr), Dvapara (864,000 yr), Kali (432,000 yr)
- [x] Total Maha-Yuga: 4,320,000 years (ratio 4:3:2:1)
- [x] 1000 Maha-Yugas = 1 Kalpa = 1 Day of Brahma = 4.32 billion years
- [x] `yuga_position(double years_since_epoch)` → yuga_t (current yuga + position)
- [x] Current: Kali Yuga, started 3102 BCE (traditional Hindu date)
- [x] Connection to geological time: 1 Kalpa = 4.32 Ga ≈ Earth's age (4.54 Ga) — independent derivation!
- [x] Data source: Surya Siddhanta Ch. 12 (FREE on archive.org)

### 17.4 Hindu Calendar Display — Orchestrator
- [ ] HUD: "Chaitra · Shukla Tritiya · Ashvini Nakshatra"
- [ ] Yuga indicator at deep time scales
- [ ] Nakshatra overlay on star field (27 wedges from Moon's perspective)
- [ ] Toggle: V key (Vedic time on/off)

---

## TRACK 18: Kabbalah — Tree of Life

Mathematical-geometric framework. 10 Sefirot, 22 paths, 4 Worlds. Deep structural parallel to Human Design.

### 18.1 Agent: Sefirot Data (Agent B) — DELIVERED (GAMMA: sefirot, 65 tests)
- [x] Pure module: `src/systems/kabbalah/sefirot.h/.c`
- [x] 10 Sefirot: Keter, Chokmah, Binah, Chesed, Gevurah, Tiferet, Netzach, Hod, Yesod, Malkuth
- [x] Per Sefirah: name (Hebrew), meaning, planet association, color, position_x, position_y
- [x] Da'at (hidden 11th) as optional
- [x] 3 Pillars: Severity (left), Mercy (right), Balance (middle)
- [x] 3 Triads: Intellectual (1-3), Moral (4-6), Material (7-9) + Malkuth
- [x] Planet mapping: Keter=Neptune/Pluto, Chokmah=Uranus, Binah=Saturn, Chesed=Jupiter, Gevurah=Mars, Tiferet=Sun, Netzach=Venus, Hod=Mercury, Yesod=Moon, Malkuth=Earth
- [x] `sefirah_get(int index)` → sefirah_t
- [x] `sefirah_for_planet(int planet_index)` → sefirah_t (bridge to astrology layer!)

### 18.2 Agent: Tree of Life Geometry (Agent A) — DELIVERED (ALPHA: tree_geometry, 63 tests)
- [x] Pure module: `src/systems/kabbalah/tree_geometry.h/.c`
- [x] 10 Sefirot positions as 2D coordinates (traditional Tree layout, 3 pillars)
- [x] 22 paths connecting Sefirot (Golden Dawn canonical connections)
- [x] Per path: from_sefirah, to_sefirah, hebrew_letter, tarot_major_arcana, zodiac/planet/element
- [x] 3 mother letters (Aleph=Air/Fool, Mem=Water/Hanged Man, Shin=Fire/Judgement) → 3 horizontal paths
- [x] 7 double letters (Bet=Mercury/Magician, Gimel=Moon/Priestess, Dalet=Venus/Empress, Kaf=Jupiter/Wheel, Pe=Mars/Tower, Resh=Sun/Sun, Tav=Saturn/World) → 7 vertical paths
- [x] 12 simple letters → 12 zodiac signs → 12 diagonal paths
- [x] 231 Gates: C(22,2) = 231 two-letter combinations (Sefer Yetzirah)
- [x] `tree_path_get(int index)` → tree_path_t
- [x] `tree_path_for_zodiac(int sign)` → path index
- [x] `tree_path_for_planet(int planet)` → path index
- [x] `tree_geometry_generate(float width, float height)` → vertex data for rendering
- [x] Structural comparison data: Sefirot ↔ HD centers mapping (topological reduction, not isomorphism)
- [x] Data source: Sefer Yetzirah (FREE on archive.org) + Crowley "777" (FREE online) for correspondence tables

### 18.3 Agent: Four Worlds Layer (Agent B) — DELIVERED (GAMMA: four_worlds, 64 tests)
- [x] Pure module: 4 Worlds of Kabbalah as nested scale
- [x] Atzilut (Emanation/Archetypal) → cosmic scale
- [x] Beriah (Creation/Intellectual) → galactic scale
- [x] Yetzirah (Formation/Emotional) → solar system scale
- [x] Assiah (Action/Physical) → personal scale
- [x] Maps to camera Scale Levels! (6→Atzilut, 4-5→Beriah, 2-3→Yetzirah, 0-1→Assiah)
- [x] `world_at_scale(int scale_level)` → kabbalistic_world_t

### 18.4 Tree of Life Rendering — Orchestrator
- [ ] Traditional Tree layout with planetary activations from current sky
- [ ] Lit Sefirot when their planet is in domicile/exaltation
- [ ] Path glow when zodiac sign associated with path has planets
- [ ] Structural overlay mode: show HD bodygraph beside Tree for comparison
- [ ] Toggle: Q key (Kabbalah on/off)

---

## TRACK 19: Unified Sacred Cycles

The mathematical structure beneath ALL time systems. Cross-system convergence and pattern detection.

### 19.1 Agent: Cross-Calendar Convergence Detector (Agent B) — DELIVERED (GAMMA: convergence)
- [x] Pure module: `src/systems/unified/convergence.h/.c`
- [x] Input: date as JD → compute ALL calendar dates simultaneously
- [x] Detect "convergence events": when 3+ systems have significant dates
- [x] Example: Islamic Friday + Jewish Shabbat eve + Mayan tone 7 = triple resonance
- [x] Example: Full moon Uposatha + Tzolkin 1-Dragon + Chinese first day = quadruple alignment
- [x] `convergence_scan(double jd)` → convergence_t (list of aligned systems + significance)
- [ ] Significance scoring: more systems aligned = higher score
- [ ] Depends on: ALL calendar modules

### 19.2 Agent: Cycle Nesting Analysis (Agent A) — DELIVERED (ALPHA: cycle_analysis, 54 tests)
- [x] Pure module: `src/systems/unified/cycle_analysis.h/.c`
- [x] Mathematical comparison of cycle lengths across systems
- [x] LCM (Least Common Multiple) of cycles: when do they ALL reset?
- [x] Tzolkin 260 × Haab 365 = Calendar Round 18,980 days (52 years)
- [x] Metonic cycle 19 years × Islamic 30-year cycle = 570 years
- [x] Nakshatra 27 × Hexagram 64 = 1728 (= Satya Yuga in thousands)
- [x] `cycle_lcm(int *cycle_lengths, int count)` → long long
- [x] `cycle_resonance(double jd, int system_a, int system_b)` → resonance_t
- [x] Number theory: GCD relationships between sacred numbers (7, 12, 13, 19, 22, 27, 64, 260)

### 19.3 Agent: Structural Pattern Map (Agent B) — DELIVERED
- [x] Pure module: structural parallels data
- [x] Kabbalah 10 Sefirot ↔ Human Design 9 Centers (mapping table)
- [x] Kabbalah 22 paths ↔ 22 Major Arcana ↔ 22 Hebrew letters
- [x] I Ching 64 hexagrams ↔ Human Design 64 gates ↔ 64 codons (DNA)
- [x] Zodiac 12 signs ↔ 12 Hebrew simple letters ↔ 12 Chinese branches
- [x] 7 classical planets ↔ 7 Hebrew double letters ↔ 7 chakras ↔ 7 days
- [x] `structural_map(int system_a, int system_b)` → mapping table
- [x] `symbol_bridge(int system, int index)` → equivalent symbols in other systems

### 19.4 Unified Display — Orchestrator
- [ ] "Synchronicity view": when convergences happen, highlight all aligned systems
- [ ] Mandala mode: radial display of all active cycles as nested rings
- [ ] "Today in all systems" summary card
- [ ] Toggle: U key (Unified view on/off)

---

## TRACK 20: Geochronology & Thermochronology

The physical record of time written in rock. Radiometric clocks, cooling histories, tectonic rhythms.

### 20.1 Agent: Thermochronology Data (Agent A) — DELIVERED (ALPHA: thermo, 67 tests)
- [x] Pure module: `src/systems/geology/thermo.h/.c`
- [x] Earth thermal history: core temperature vs age curve
- [x] Major thermal events: Late Heavy Bombardment, core formation, mantle convection onset
- [x] Fission track / (U-Th)/He system parameters for dating
- [x] Closure temperatures for common minerals (zircon, apatite, muscovite)
- [x] `thermal_age_at(double years_ago)` → earth_thermal_t (temperature, state)
- [x] `cooling_curve(mineral_type, double *temps, double *ages, int n)` → fill arrays

### 20.2 Agent: Tectonic Event Timeline (Agent A) — DELIVERED (ALPHA: tectonic, 46 tests)
- [x] Pure module: `src/systems/geology/tectonic.h/.c`
- [x] Supercontinent cycle: Vaalbara → Kenorland → Columbia → Rodinia → Pannotia → Pangaea → present
- [x] Per event: name, start_ma, end_ma, description
- [x] Major tectonic events: rift openings, mountain building, ocean closings
- [x] `supercontinent_at(double ma)` → supercontinent_t
- [x] `tectonic_events_between(double start_ma, double end_ma)` → event array

### 20.3 Deep Earth Display — Orchestrator
- [ ] At Scale 6: geological timeline with color bars
- [ ] Supercontinent animation (extremely accelerated time)
- [ ] Radiometric decay visualization (parent → daughter isotope)
- [ ] Integration with Track 13 (geological time scale provides the framework)

### 20.4 Agent: Climate History Data (Agent B) — DELIVERED (GAMMA: climate_history, 34 tests)
- [x] Pure module: `src/systems/geology/climate_history.h/.c`
- [x] Major climate events as static const data:
- [x] Snowball Earth episodes (Sturtian 717 Ma, Marinoan 645 Ma) — global glaciation
- [x] Great Oxidation Event (2.4 Ga) — atmosphere becomes toxic to anaerobic life
- [x] PETM (56 Ma) — +5-8°C in millennia, ocean acidification, mass migration
- [x] Pleistocene ice ages — Milankovitch cycles driving 100 kyr glacial/interglacial rhythm
- [x] Holocene optimum (6000 BCE) — warmest post-glacial period, agriculture begins
- [x] Little Ice Age (1300-1850 CE) — Thames froze, crop failures, political upheaval
- [x] Per event: name, start_ma, end_ma, temp_anomaly_C, CO2_ppm, description
- [x] `climate_event_count()`, `climate_event_at(double ma)`, `climate_event_by_index(int index)`
- [x] Data sources: NASA paleoclimate (FREE), ice core records, Geologic Time Scale 2020

### 20.5 Agent: Fossil Record Milestones (Agent B) — DELIVERED (GAMMA: fossil_milestones, 39 tests)
- [x] Pure module: `src/systems/geology/fossil_milestones.h/.c`
- [x] Key appearances in Earth's story:
- [x] First life: stromatolites (3.5 Ga), possibly earlier (3.8 Ga)
- [x] First photosynthesis (2.7 Ga) — cyanobacteria begin oxygenating atmosphere
- [x] First eukaryotes (2.1 Ga) — complex cells with nuclei
- [x] Cambrian explosion (541 Ma) — almost all animal phyla appear in ~20 Myr
- [x] First land plants (470 Ma) — colonization of continents begins
- [x] First forests (385 Ma) — Archaeopteris, carbon burial begins
- [x] First tetrapods (375 Ma) — Tiktaalik, fish walk onto land
- [x] First mammals (225 Ma) — tiny nocturnal creatures in dinosaur shadow
- [x] K-Pg impact (66 Ma) — dinosaurs end, mammals inherit Earth
- [x] First primates (55 Ma), first hominids (7 Ma), Homo sapiens (0.3 Ma)
- [x] Per milestone: name, ma, what_appeared, what_changed, significance
- [x] `fossil_milestone_count()`, `fossil_milestone_get(int index)`, `fossil_milestone_at(double ma)`

---

## TRACK 21: Tidal Systems — The Moon's Clock on Earth

Tides are lunar time made visible. Computed from first principles, no APIs needed.

### 21.1 Agent: Tidal Harmonic Constants (Agent A) — DELIVERED (ALPHA: tidal, 63 tests)
- [x] Pure module: `src/systems/earth/tidal.h/.c`
- [x] Major tidal constituents: M2 (principal lunar, 12.42h), S2 (principal solar, 12.00h), N2 (lunar elliptic), K1 (luni-solar diurnal), O1 (principal lunar diurnal)
- [x] `tidal_force(double moon_dist, double moon_declination, double sun_dist, double sun_declination, double lat)` → tidal_force_t
- [x] `tidal_height_approx(double jd, double lat, double lon)` → approximate tidal height (relative)
- [x] Spring tide detection: Moon + Sun aligned (new/full moon → bigger tides)
- [x] Neap tide detection: Moon + Sun 90° apart (quarter moon → smaller tides)
- [x] King tide: spring tide + lunar perigee + perihelion alignment
- [x] `next_spring_tide(double jd)` → double jd
- [x] `next_king_tide(double jd)` → double jd
- [x] Depends on: lunar module (DELIVERED), planets module (DELIVERED), observer module (DELIVERED)

### 21.2 Agent: Surf Spot Database (Agent B) — DELIVERED (GAMMA: surf_spots, 65 tests)
- [x] Pure module: `src/systems/earth/surf_spots.h/.c`
- [x] ~50 world-class surf spots with lat/lon + beach orientation + best swell direction
- [x] Per spot: name, lat, lon, beach_facing_deg, best_tide (low/mid/high), best_swell_dir, best_season
- [x] `surf_spot_get(int index)` → surf_spot_t
- [x] `surf_spot_count()` → int
- [x] `surf_spot_nearest(double lat, double lon)` → index
- [x] `surf_spot_tide_quality(int spot, double tidal_height)` → quality score (0-1)
- [x] Classic spots: Pipeline, Teahupo'o, Jeffreys Bay, Uluwatu, Nazaré, Mavericks, Bells Beach, etc.

### 21.3 Agent: Tidal Prediction Engine (Agent B) — DELIVERED (BETA: tide_predict, 41 tests)
- [x] Pure module: `src/systems/earth/tide_predict.h/.c`
- [x] Predict tidal state for a location and time from lunar/solar positions
- [x] `tide_state(double jd, double lat, double lon)` → tide_state_t
- [x] `best_surf_windows(double jd_start, double jd_end, int spot_index)` → array of (jd, quality_score)
- [x] Combine tidal phase + spring/neap cycle + time of day + spot preferences
- [x] Depends on: tidal module (21.1), surf_spots module (21.2), lunar, solar_events

### 21.4 Tidal Display — Orchestrator
- [ ] Tide chart overlay: 24h wave curve showing predicted tide at selected location
- [ ] Moon phase indicator linked to spring/neap cycle
- [ ] King tide alert when conditions align
- [ ] Surf quality indicator per saved spot
- [ ] Part of Earth View mode

---

## TRACK 22: Atmospheric Time — Wind, Storms, Seasons

Weather as time patterns. Computed from solar heating + Earth rotation + seasonal cycles.

### 22.1 Agent: Solar Radiation Model (Agent A) — DELIVERED (ALPHA: solar_radiation, 40 tests)
- [x] Pure module: `src/systems/earth/solar_radiation.h/.c`
- [x] Solar irradiance at latitude/longitude/date (top-of-atmosphere)
- [x] `solar_irradiance(double jd, double lat)` → watts per m² (accounting for declination + atmosphere)
- [x] Day length from latitude + solar declination (we already compute this!)
- [x] Seasonal temperature approximation: f(latitude, day_of_year, elevation)
- [x] Freezing line altitude: elevation where temperature ≈ 0°C for given lat/date
- [x] `freezing_elevation(double lat, int day_of_year)` → meters
- [x] Depends on: solar_events (DELIVERED), ecliptic (DELIVERED)

### 22.2 Agent: Coriolis & Wind Pattern Data (Agent A) — DELIVERED (ALPHA: wind_patterns, 43 tests)
- [x] Pure module: `src/systems/earth/wind_patterns.h/.c`
- [x] Coriolis effect: deflection from Earth rotation at latitude
- [x] `coriolis_parameter(double lat)` → f = 2Ω sin(lat)
- [x] General circulation cells: Hadley (0-30°), Ferrel (30-60°), Polar (60-90°)
- [x] Prevailing wind direction at latitude: trades, westerlies, easterlies
- [x] `prevailing_wind(double lat)` → wind_pattern_t {direction_deg, strength_relative}
- [x] ITCZ (Intertropical Convergence Zone) latitude as f(month) — shifts with seasons
- [x] `itcz_latitude(int month)` → degrees

### 22.3 Agent: Storm Season Data (Agent B) — DELIVERED (GAMMA: storm_data, 68 tests)
- [x] Pure module: `src/systems/earth/storm_data.h/.c`
- [x] Hurricane/cyclone season windows per ocean basin
- [x] Atlantic: June 1 - Nov 30. East Pacific: May 15 - Nov 30. West Pacific: year-round peak Jul-Nov
- [x] Monsoon season data: Indian (Jun-Sep), East Asian (Jun-Aug), Australian (Dec-Mar)
- [x] `storm_season(double lat, double lon, int month)` → storm_risk_t {cyclone_risk, monsoon_active}
- [x] Swell generation zones: where ocean swells originate by season
- [x] `swell_source_direction(double lat, double lon, int month)` → dominant swell direction

### 22.4 Weather Pattern Display — Orchestrator
- [ ] Earth globe view: general circulation arrows
- [ ] Seasonal animation: ITCZ migration, trade wind shifts
- [ ] Storm season highlighting by ocean basin
- [ ] Swell propagation lines (for surfer context)

---

## TRACK 23: Snow & Mountain Systems

Elevation-dependent time. When and where snow falls, persists, or melts.

### 23.1 Agent: Snow Season Model (Agent B) — DELIVERED (GAMMA: snow_season, 36 tests)
- [x] Pure module: `src/systems/earth/snow_season.h/.c`
- [x] Snow probability from latitude + elevation + month
- [x] `snow_probability(double lat, double elevation_m, int month)` → 0.0-1.0
- [x] Ski season window: months where snow probability > 0.5 at resort elevation
- [x] `ski_season_window(double lat, double elevation_m)` → {start_month, end_month}
- [x] Powder likelihood: temperature + recent precipitation proxy
- [x] Avalanche risk factors: rapid temperature swing + new snow + wind loading
- [x] `avalanche_risk_factors(double lat, double elevation_m, int month)` → risk_t

### 23.2 Agent: Mountain Resort Database (Agent B) — DELIVERED (GAMMA: ski_resorts, 47 tests)
- [x] Pure module: `src/systems/earth/ski_resorts.h/.c`
- [x] ~50 world ski resorts with lat/lon + base/summit elevation
- [x] Per resort: name, lat, lon, base_elev_m, summit_elev_m, best_months
- [x] `ski_resort_get(int index)` → ski_resort_t
- [x] `ski_resort_nearest(double lat, double lon)` → index
- [x] `ski_resort_snow_quality(int resort, int month)` → quality score (0-1)
- [x] Classic resorts: Chamonix, Whistler, Niseko, Verbier, Jackson Hole, Portillo, etc.

### 23.3 Snow Conditions Display — Orchestrator
- [ ] Elevation color map: snow line visible on Earth globe
- [ ] Seasonal snow animation: watch snow advance/retreat through the year
- [ ] Resort quality calendar: month-by-month rating per resort

---

## TRACK 24: Earth View Mode

The paradigm shift: looking DOWN at Earth instead of OUT to space.

### 24.1 Agent: Earth Globe Geometry (Agent A) — DELIVERED (ALPHA: earth_globe, 38 tests)
- [x] Pure module: `src/render/earth_globe.h/.c`
- [x] Sphere mesh: UV-sphere with configurable resolution (lat/lon grid)
- [x] `earth_globe_generate(int lat_segments, int lon_segments)` → vertex data
- [x] Vertex positions on unit sphere with lat/lon texture coordinates
- [x] Day/night terminator line from solar position (we ALREADY compute this!)
- [x] `earth_terminator(double solar_ra, double solar_dec)` → great circle vertices
- [x] Coastline data: simplified world coastline as line segments (static const)
- [x] `earth_coastline_segments()` → line vertex data

### 24.2 Agent: Earth Atmosphere Rendering Data (Agent A) — DELIVERED (ALPHA: earth_atmosphere, 45 tests)
- [x] Pure module: atmosphere shell geometry
- [x] Rayleigh scattering approximation data (blue sky at edge, dark at top)
- [x] `atmosphere_color(double altitude_km, double sun_angle)` → RGB
- [x] Atmosphere shell vertex data (slightly larger sphere around Earth)
- [x] Cloud band latitude data (ITCZ, mid-latitude storm tracks)

### 24.3 Agent: Earth Render Pack (Agent A)
- [ ] Pure module: `src/render/earth_pack.h/.c`
- [ ] Pack earth_globe mesh into interleaved vertex arrays (pos + normal + texcoord)
- [ ] Per vertex: position(vec3) + normal(vec3) + uv(vec2) = stride 32 bytes
- [ ] Coastline line packing: earth_coastline_segments() → GL_LINES vertex arrays
- [ ] Terminator line packing: earth_terminator() → great circle GL_LINE_STRIP vertices
- [ ] Atmosphere shell packing: slightly larger sphere for limb glow
- [ ] Earth + atmosphere + coastline shader source as static const strings (GLSL ES 3.00)
- [ ] Earth fragment shader: day/night coloring based on terminator + sun direction uniform
- [ ] No GL calls — produces float arrays ready for glBufferData
- [ ] Depends on: earth_globe (24.1), earth_atmosphere (24.2)

### 24.4 Earth View Rendering — Orchestrator
- [ ] Two modes accessible via view toggle:
  - [ ] **Space View** (existing): Solar system → galaxy → universe (Shift+0-6)
  - [ ] **Earth View** (new): Globe → continent → region → local (E key toggles)
- [ ] Earth globe with day/night terminator
- [ ] Coastline overlay
- [ ] Atmosphere glow at limb
- [ ] Scale levels within Earth View:
  - [ ] E0: Full globe (weather patterns, terminator, storm zones)
  - [ ] E1: Hemisphere (regional weather, swell direction)
  - [ ] E2: Regional (tide predictions, surf spots, ski resorts)
  - [ ] E3: Local (specific spot data, prayer times, sunrise/sunset)
- [ ] All existing systems still active in Earth View (Tzolkin, I Ching, etc. overlay as cards)

---

## TRACK 25: Earth-Specific Time Displays

Information layers that only make sense when looking at Earth.

### 25.1 Agent: Sunrise/Sunset Worldwide (Agent B) — DELIVERED (agent-BETA)
- [x] Pure module: `src/systems/earth/daylight.h/.c`
- [ ] Global daylight map: for any JD, compute sunlit vs dark for all lat/lon
- [ ] `daylight_at(double jd, double lat, double lon)` → {is_day, sun_elevation, minutes_to_sunrise, minutes_to_sunset}
- [ ] Golden hour / blue hour computation (photographer's time)
- [ ] `golden_hour(double jd, double lat, double lon)` → {start_jd, end_jd} for each
- [ ] Polar day/night detection (midnight sun / polar night at high latitudes)
- [ ] Depends on: solar_events (DELIVERED), observer (DELIVERED)

### 25.2 Agent: Seasonal Cycle Visualizer Data (Agent B) — DELIVERED (agent-GAMMA)
- [x] Pure module: `src/systems/earth/seasons.h/.c`
- [ ] Solstice/equinox dates (already computed by solar_events!)
- [ ] Growing season: frost-free days at latitude
- [ ] `growing_season(double lat)` → {start_day_of_year, end_day_of_year}
- [ ] Seasonal hemisphere comparison (when it's winter in north, it's summer in south)
- [ ] `season_at(double lat, int day_of_year)` → season name + progress (0-1)

### 25.3 Unified Earth Display — Orchestrator
- [ ] Earth View HUD: location name, local time, prayer times, tide state, daylight remaining
- [ ] "Best time to..." suggestions: surf (tide + swell), ski (snow + daylight), photograph (golden hour)
- [ ] All sacred calendars still compute and display — they're location-independent
- [ ] Connection: prayer times (Track 15.2) appear naturally in Earth View at the user's location

---

## TRACK 26: The Correlation Engine — Pattern Discovery

The heart of Time's investigative power. Finds mathematical relationships ACROSS systems that humans haven't noticed or have only intuited. Not a search engine — a discovery engine.

### 26.1 Agent: Sacred Number Registry (Agent B) — DELIVERED (BETA: sacred_numbers, 44 tests)
- [x] Pure module: `src/systems/unified/sacred_numbers.h/.c`
- [x] Catalog of numbers that appear across multiple traditions:
- [x] 7 (days, planets, chakras, Hebrew doubles, musical notes)
- [x] 12 (zodiac, months, tribes, disciples, Chinese branches, Hebrew simples)
- [x] 13 (Tzolkin tones, lunar months per year, Mayan Baktun count)
- [x] 19 (Metonic cycle, Quran chapters, Bahai calendar)
- [x] 22 (Hebrew letters, Kabbalah paths, Tarot Major Arcana)
- [x] 27 (Nakshatras, lunar mansion systems worldwide)
- [x] 64 (I Ching hexagrams, HD gates, DNA codons, chess squares)
- [x] 72 (years per degree of precession, names of God in Kabbalah, Confucian disciples)
- [x] 108 (Nakshatra padas, Buddhist beads, Hindu beads, 12×9, 27×4)
- [x] 260 (Tzolkin days, human gestation days approximate)
- [x] 432 (Kali Yuga/1000, precession/60, day_seconds/200)
- [x] Per number: value, systems_present[], description, scale_appearances[]
- [x] `sacred_number_get(int value)` → sacred_number_t (all known appearances)
- [x] `sacred_number_systems(int value)` → list of systems that use this number

### 26.2 Agent: Ratio Analyzer (Agent A) — DELIVERED (ALPHA: ratio_analysis, 47 tests)
- [x] Pure module: `src/systems/unified/ratio_analysis.h/.c`
- [x] Input: two cycle lengths → compute their ratio
- [x] Flag when ratio ≈ simple fraction (within tolerance): 2/1, 3/2, 4/3, 5/4, etc.
- [x] Flag when ratio ≈ phi (1.618...), e (2.718...), pi (3.14159...)
- [x] GCD and LCM computation for large integers (cycle lengths in days)
- [x] `ratio_analyze(double cycle_a, double cycle_b)` → ratio_result_t {ratio, nearest_fraction, error, is_harmonic}
- [x] `cycle_lcm(long a, long b)` → long (when do two cycles realign?)
- [x] `harmonic_series(double fundamental, int n, double *harmonics)` → fill harmonic overtones
- [x] Scale factor detection: is number_a = number_b × 10^n?
- [x] Example: Maha-Yuga (4,320,000) = Kali Yuga (432,000) × 10 = Kalpa (4,320,000,000) / 1000

### 26.3 Agent: Cross-System Number Scanner (Agent B) — DELIVERED (agent-GAMMA)
- [x] Pure module: `src/systems/unified/number_scanner.h/.c`
- [ ] Input: a number (e.g., 432) → scan ALL registered cycle lengths for appearances
- [ ] Check: is this number a factor of any known cycle?
- [ ] Check: is this number a multiple of any sacred number?
- [ ] Check: does this number appear at different scales (×10, ×100, ×1000)?
- [ ] `number_scan(int target)` → scan_result_t {matches[], match_count, scale_appearances[]}
- [ ] `factor_scan(long cycle_length)` → all sacred numbers that divide it
- [ ] `coincidence_score(int number)` → how many independent systems reference this number (0-N)
- [ ] The "surprise" metric: higher score = less likely to be coincidence

### 26.4 Agent: Precession Encoder Detector (Agent A) — DELIVERED (agent-GAMMA)
- [x] Pure module: `src/systems/unified/precession_detect.h/.c`
- [ ] Hypothesis: many ancient systems encode the precession cycle (~25,772 years)
- [ ] Test each system's fundamental cycles against precession:
  - [ ] Mayan: 5 × 5,125.36 = 25,627 (within 0.6% of precession)
  - [ ] Hindu: 12 × 2,160 = 25,920 (within 0.6% of precession)
  - [ ] Hebrew: Jubilee (50) × 12 months × 43 = 25,800 (within 0.1%!)
  - [ ] Egyptian: Sothic (1,461 × 365.25 / 20.74...) — more complex
- [ ] `precession_test(double cycle_length, int multiplier)` → precession_match_t {product, error_percent, systems_using}
- [ ] Generate "precession encoding report" for all registered systems
- [ ] This is the investigative module: it asks "did ancient culture X know about precession?"

### 26.5 Correlation Dashboard — Orchestrator
- [ ] "Investigation Mode" toggle (/ key): shows correlation findings
- [ ] Real-time: as date changes, show which number patterns are active
- [ ] "Today's correlations": scan current date across all systems, highlight alignments
- [ ] Surprise alerts: when an unusually high number of systems align
- [ ] Number web visualization: sacred numbers as nodes, systems as edges
- [ ] Precession report: which cultures encoded it, how accurately, at what scale
- [ ] User exploration: click a number → see all its appearances across traditions

---

## TRACK 27: Harmonic Resonance — What Claude Would Add

These are systems I believe are missing. Time is vibration. Every cycle is a frequency. The artwork should not just display time — it should reveal that **all of reality oscillates**.

### 27.1 Agent: Frequency Mapper (Agent A) — DELIVERED (agent-GAMMA)
- [x] Pure module: `src/systems/unified/frequency.h/.c`
- [ ] Convert ANY time cycle to a frequency: f = 1 / period
- [ ] Convert ANY frequency to audible range via octave transposition: f × 2^n
- [ ] Planetary orbital frequencies (Earth year → C# at ~136.1 Hz, "Om" frequency)
- [ ] Lunar month → ~210 Hz (approximately G#)
- [ ] Day → ~194.18 Hz (approximately G)
- [ ] `cycle_to_frequency(double period_seconds)` → double hz
- [ ] `frequency_to_audible(double hz)` → double (transposed to 20-20000 Hz range)
- [ ] `frequency_to_note(double hz)` → note_t {name, octave, cents_off}
- [ ] `planetary_frequency(int planet_index)` → double hz (orbital period as frequency)
- [ ] Musical interval detection between planet pairs: ratio → interval name
- [ ] The "Music of the Spheres" is computable — Kepler was right, just premature

### 27.2 Agent: Biological Clocks (Agent B) — DELIVERED (GAMMA: biorhythm, 45 tests)
- [x] Pure module: `src/systems/earth/biorhythm.h/.c`
- [x] Circadian rhythm: ~24h (Earth rotation → encoded in every living cell)
- [x] Heartbeat: ~72 bpm average = 4,320 beats/hour = 103,680 beats/day
- [x] Note: 72 years = 1° of precession. 72 bpm × 60 min = 4,320 beats/hour. 432 again.
- [x] Breath: ~15 per minute = 21,600 per day = 25,920 per 28.8 hours (precession number!)
- [x] Lunar cycle: menstrual cycle ~29.5 days ≈ synodic month (29.530589 days)
- [x] `biorhythm_heartbeats(double hours)` → total beats
- [x] `biorhythm_breaths(double hours)` → total breaths
- [x] `biorhythm_match(double bio_rate, double astro_cycle)` → correlation
- [x] Human body as astronomical clock: the numbers match because we evolved under this sky

### 27.3 Agent: Phase Space Mapper (Agent A) — DELIVERED (agent-ALPHA, 59 tests)
- [x] Pure module: `src/systems/unified/phase_space.h/.c`
- [ ] Represent the current moment as a point in N-dimensional cycle space
- [ ] Each dimension = one time system's position in its cycle (0.0-1.0)
- [ ] Dimension 0: Tzolkin (position in 260-day cycle)
- [ ] Dimension 1: Lunar phase (0.0-1.0)
- [ ] Dimension 2: Zodiac position (0°-360° of ecliptic)
- [ ] Dimension 3: Hebrew calendar (position in 19-year Metonic)
- [ ] Dimension 4: Hijri (position in 30-year cycle)
- [ ] etc. for all registered systems
- [ ] `phase_point(double jd, int *system_ids, int count)` → float[] (N-dimensional position)
- [ ] `phase_distance(float *point_a, float *point_b, int dims)` → float (distance in phase space)
- [ ] `phase_nearest_return(double jd, float *target, int dims)` → double jd (when does the point return closest to target?)
- [ ] This answers: "When was the last time ALL these cycles were in this configuration?" and "When will it happen again?"

### 27.4 Agent: DNA-Hexagram Structural Map (Agent B) — DELIVERED (GAMMA: codon_hexagram, 67 tests)
- [x] Pure module: `src/systems/unified/codon_hexagram.h/.c`
- [x] 64 DNA codons ↔ 64 I Ching hexagrams (Schönberger mapping)
- [x] Both are 6-position binary systems: codon = 3 base pairs × 2 states, hexagram = 6 lines × 2 states
- [x] Codon table: 4 bases (A, C, G, U/T) × 4 × 4 = 64 codons → 20 amino acids + 3 stop
- [x] Map each codon to King Wen hexagram sequence
- [x] `codon_to_hexagram(int codon_index)` → int hexagram_number
- [x] `hexagram_to_amino_acid(int hexagram)` → const char* (amino acid name)
- [x] This is not mysticism — it's combinatorics. Both systems exhaustively enumerate 2^6 = 64 states.
- [x] The question Time asks: is the ORDERING meaningful, or just the COUNT?

### 27.5 Harmonic Display — Orchestrator
- [ ] "Resonance Mode" toggle: visual + (future) sonic representation
- [ ] Planetary frequencies as musical intervals (visualized as wave interference patterns)
- [ ] Phase space trajectory: animated dot moving through projected 2D/3D slice of N-dimensional space
- [ ] "Heartbeat of the cosmos": overlay human biological rhythms with astronomical ones
- [ ] Show where 432 appears at every scale simultaneously

---

## TRACK 28: The Living Library — Knowledge Graph

Every algorithm we implement traces back to a human mind. The contributors aren't just names — they're nodes in a network of knowledge transmission across millennia.

### 28.1 Agent: Knowledge Graph Data (Agent B) — DELIVERED (agent-GAMMA)
- [x] Pure module: `src/systems/unified/knowledge_graph.h/.c`
- [ ] Extend contributors.json into a relational structure
- [ ] Per contributor: name, era, culture, systems_contributed[], influenced_by[], influenced[]
- [ ] Transmission chains: Surya Siddhanta (India) → Arabic translation → European astronomy
- [ ] Meeus (Belgium) used → JPL (USA) used → our orbit.c
- [ ] Arguelles (Mexico/USA) synthesized → Mayan → Tibetan → Galactic → our tzolkin.c
- [ ] `knowledge_path(int system_a, int system_b)` → chain of contributors connecting two systems
- [ ] `contributor_influence(int contributor_id)` → which systems they enabled
- [ ] This makes visible: human knowledge is ONE river with many tributaries

### 28.2 Knowledge Graph Display — Orchestrator
- [ ] Network visualization: contributors as nodes, influence as edges
- [ ] Time-animated: show knowledge spreading across centuries and continents
- [ ] Highlight living chains: "You are using an algorithm that traveled from India (500 CE) through Baghdad (800 CE) through Toledo (1100 CE) through Meeus (1991) to this pixel"

---

## TRACK 29: The Dramatization Engine — Showing, Not Telling

Time must dramatize mathematical achievements. Show what Aryabhata computed with NO telescope in 499 CE. Show what Hillel II derived from pure observation in 359 CE. Show the absence of instruments and the presence of genius. Let people FEEL the weight of these accomplishments. The title "Expounding the Mathematical Seed" (Bhaskara I, 629 CE) captures the philosophy: every civilization that planted the seed of mathematics and watered it with observation grew the same tree.

### 29.1 Agent: Historical Achievement Data (Agent B) — DELIVERED (GAMMA: achievement, 62 tests)
- [x] Pure module: `src/systems/unified/achievement.h/.c`
- [x] Per achievement: person, year, culture, value_computed, modern_value, error_margin, tools_available
- [x] Aryabhata (499 CE): Earth circumference 39,968 km (actual: 40,075) — 0.2% error, NO telescope
- [x] Aryabhata: sidereal year 365.25858 days (actual: 365.25636) — 3 minute error
- [x] Aryabhata: solved the equation of center (Kepler's equation) 1200 years before Kepler
- [x] Hillel II (359 CE): molad 29.530594 days (actual: 29.530589) — 0.4 seconds/month error
- [x] Al-Khwarizmi (820 CE): planetary position algorithms still used today
- [x] Maya: Venus cycle 583.92 days (actual: 583.93) — 2 hours error per 584 days
- [x] `achievement_count()`, `achievement_get(int index)`, `achievement_by_culture(const char *culture)`
- [x] `achievement_error_ratio(int index)` — how close to modern value
- [x] The drama: timeline showing WHEN, with WHAT tools, achieving WHAT precision

### 29.2 Agent: Calendar Reform History (Agent B) — DELIVERED (GAMMA: calendar_reform, 66 tests)
- [x] Pure module: `src/systems/unified/calendar_reform.h/.c`
- [x] The 360-day natural year: 12 months x 30 days — the feminine lunar calendar
- [x] The 13-moon calendar: 13 x 28 = 364 + 1 Day Out of Time — Arguelles's proposal
- [x] The Gregorian reform (1582): Pope Gregory XIII, 10 days deleted, power consolidation
- [x] The 1920s World Calendar movement: League of Nations attempted reform, failed
- [x] The "disorder of time": months of unequal length (28/29/30/31), bissextile year, timezone chaos
- [x] What we COULD have: no time zones (one Earth time), equal months, seasonal hours
- [x] Seasonal hours: 12 hours of daylight, 12 of night — each hour's LENGTH changes with season
- [x] `calendar_system_count()`, `calendar_system_get(int index)` — historical proposals
- [x] `seasonal_hour_length(double jd, double latitude)` — what "1 hour" means in seasonal time
- [x] Books needed: Calendar reform history, League of Nations records, Arguelles 13 Moon

### 29.3 Agent: Earth History Dramatization (Agent B) — DELIVERED (BETA: earth_drama, 62 tests)
- [x] Pure module: `src/systems/unified/earth_drama.h/.c`
- [x] Per geological era: what LIVED, what DIED, what changed
- [x] Climate extremes as data: Snowball Earth (720-635 Ma), PETM (56 Ma, +5-8°C), ice ages
- [x] The thin thread: habitable zone width, atmosphere composition changes over time
- [x] Atmosphere evolution: reducing → oxidizing (Great Oxidation Event 2.4 Ga) → ozone → modern
- [x] Fossil milestones: first life (3.8 Ga), photosynthesis (2.7 Ga), Cambrian explosion (541 Ma), land plants (470 Ma), tetrapods (375 Ma), mammals (225 Ma), primates (55 Ma), humans (0.3 Ma)
- [x] Human arrival ratio: 300,000 years out of 4,600,000,000 — dramatize the scale
- [x] `earth_milestone_count()`, `earth_milestone_get(int index)` — what appeared and when
- [x] `earth_climate_at(double ma)` — temperature anomaly, CO2 level, ice coverage
- [x] Uses: geo_time module for geological framework, extinction events as anchors

### 29.4 Dramatization Display — Orchestrator
- [ ] Achievement timeline: scrollable through millennia
- [ ] Per achievement: animated reveal showing tools available (stylus, sand, stars — NO telescope)
- [ ] Side-by-side: ancient value vs modern value, error highlighted
- [ ] Calendar reform timeline: what humanity chose vs what was possible
- [ ] Interactive toggle: Gregorian world vs 13-Moon world vs seasonal hours
- [ ] Seasonal hour clock: hours expand/contract with daylight at user's latitude
- [ ] Earth history scrubber: scroll through 4.6 Ga with visual era transitions

### 29.5 Agent: The Patriarchal Decision (Agent B) — DELIVERED (GAMMA: calendar_politics, 66+ tests)
- [x] Pure module: `src/systems/unified/calendar_politics.h/.c`
- [x] Key decision points where power chose ORDER over NATURE:
- [x] Council of Nicaea (325 CE): standardized Easter computation, 7-day week mandated empire-wide
- [x] Gregorian Reform (1582): Pope Gregory XIII deleted 10 days, Protestant nations refused for centuries
- [x] Railroad time (1883): standardized time zones for commerce, not for nature
- [x] International Date Line (1884): where one day becomes another — political negotiation
- [x] Constantine's Sabbath shift (321 CE): rest day moved from Saturday to Sunday — political
- [x] `calendar_decision_count()`, `calendar_decision_get(int index)`
- [x] Per decision: who, when, what changed, what was lost, who benefited
- [x] Arguelles's Law of Time: "Time is not money. Time is art."

### 29.6 Agent: Jesus & The Calendar (Agent B) — DELIVERED (BETA: calendar_epoch, 59 tests)
- [x] Pure module: `src/systems/unified/calendar_epoch.h/.c`
- [x] The Julian/Gregorian split: Before Christ / Anno Domini as civilizational time epoch
- [x] The missing years: Jesus born 4-7 BCE, but calendar starts at 1 AD — Dionysius Exiguus's error (525 CE)
- [x] Easter computation: the Computus algorithm (Meeus) — both Gregorian and Julian
- [x] `ce_easter_date(int year)` — Computus algorithm, verified against 12 known Easter dates
- [x] `ce_epoch_offset_min/max()` — the 4-7 year error range
- [x] `ce_julian_drift()` — accumulated Julian-Gregorian drift by year
- [x] How one religion's festival computation reshaped global timekeeping

---

## TRACK 30: Presentation Quality — Rendering, Typography, Interaction

The artwork must be BEAUTIFUL. Sub-pixel rendering. Smooth animations. Scroll-driven exploration. Perspective transitions. This is not a dashboard — it is art that reveals knowledge through interaction.

### 30.1 Agent: Typography & Text Rendering (Agent A) — DELIVERED (agent-ALPHA, 93 tests)
- [x] Pure module: `src/render/text_render.h/.c`
- [ ] SDF (Signed Distance Field) based text rendering for resolution independence
- [ ] Multi-size text: titles, labels, annotations, data values
- [ ] Unicode coverage: Hebrew, Arabic, Devanagari, Chinese, Mayan glyphs
- [ ] Text animation parameters: fade, slide, scale, typewriter
- [ ] Books needed: GPU text rendering, SDF techniques, font atlas generation

### 30.2 Agent: Scroll & Layer Transition Data (Agent B) — DELIVERED
- [x] Pure module: `src/ui/scroll_layers.h/.c`
- [x] Layer transition definitions: which layers appear at which scroll/zoom position
- [x] Parallax depth values for each information layer
- [x] Scroll-to-zoom mapping: continuous scroll to logarithmic scale change
- [x] Perspective shift: how view rotates as user navigates deeper
- [x] `scroll_layer_opacity(int layer, float scroll_position)` — 0-1
- [x] `scroll_perspective(float scroll_position)` — camera transform

### 30.3 High-Quality Rendering — Orchestrator
- [ ] Anti-aliased lines (MSAA or shader-based falloff)
- [ ] Bloom/glow for stars and bright objects (post-process shader)
- [ ] Smooth camera transitions between all 7+ scale levels
- [ ] HDR-like tone mapping for star brightness range
- [ ] Books needed: Real-Time Rendering, GPU Gems, WebGL2 advanced techniques

### 30.4 Interaction Design — Orchestrator
- [ ] Scroll-driven exploration (wheel/touch = navigate time scales)
- [ ] Click-to-focus: click any object -> smooth zoom + info panel
- [ ] Drag-to-orbit: intuitive camera control
- [ ] Timeline scrubber: drag through time with visual feedback
- [ ] Mobile touch: pinch-zoom, swipe-orbit, tap-select
- [ ] Keyboard shortcuts overlay (? key)

---

## Execution Priority Order

Work these tracks interleaved, not sequentially. Priority by impact:

```
COMPLETED:
  1.1-1.4  Track 1 (HUD wiring) — all done
  2.1-2.4  Track 2 (multi-scale camera) — all done
  3.1-3.8  Track 3 (zodiac wheel) — all rendering done + glyph batch
  4.7-4.12 Track 4 agent modules — all 6 delivered
  7.5-7.6  Track 7 data modules — star catalog (195) + constellations (40) delivered
  8.1      Planet data — delivered (39 tests, GAMMA)
  8.2      Atmosphere & ring geometry — delivered (42 tests, ALPHA)
  8.4      Moon data (9 moons) — delivered (33 tests, BETA)
  9.2      Star spectral colors — delivered (34 tests, ALPHA)
  10.1     Deep sky catalog (Messier+NGC) — delivered (35 tests, BETA)
  11.2     Bodygraph geometry — delivered (44 tests, ALPHA)
  13.1     Geological time scale — delivered (46 tests, BETA)
  13.2     Radiometric dating — delivered (61 tests, ALPHA)
  14.1     Hebrew calendar — delivered (49 tests, GAMMA)
  15.1     Hijri calendar — delivered (51 tests, GAMMA)
  15.2     Prayer times — delivered (38 tests, GAMMA)
  16.1     Buddhist calendar — delivered (45 tests, GAMMA)
  16.2     Kalpa cosmic cycles — delivered (49 tests, BETA)
  17.1     Panchanga — delivered (42 tests, BETA)
  17.2     Nakshatra — delivered (53 tests, ALPHA)
  14.2     Sabbatical cycles — delivered (38 tests, BETA)
  17.3     Yuga cycles — delivered (34 tests, GAMMA)
  18.1     Sefirot data — delivered (65 tests, GAMMA)
  18.2     Tree geometry — delivered (63 tests, ALPHA)
  18.3     Four Worlds — delivered (64 tests, GAMMA)
  19.2     Cycle analysis — delivered (54 tests, ALPHA)
  20.1     Thermochronology — delivered (67 tests, ALPHA)
  20.2     Tectonic timeline — delivered (46 tests, ALPHA)
  20.4     Climate history — delivered (34 tests, GAMMA)
  20.5     Fossil milestones — delivered (39 tests, GAMMA)
  21.1     Tidal harmonics — delivered (63 tests, ALPHA)
  21.2     Surf spots — delivered (65 tests, GAMMA)
  22.1     Solar radiation — delivered (40 tests, ALPHA)
  22.2     Wind patterns — delivered (43 tests, ALPHA)
  22.3     Storm data — delivered (68 tests, GAMMA)
  23.1     Snow season — delivered (36 tests, GAMMA)
  23.2     Ski resorts — delivered (47 tests, GAMMA)
  24.1     Earth globe — delivered (38 tests, ALPHA)
  24.2     Earth atmosphere — delivered (45 tests, ALPHA)
  26.1     Sacred numbers — delivered (44 tests, BETA)
  26.2     Ratio analysis — delivered (47 tests, ALPHA)
  27.2     Biological clocks — delivered (45 tests, GAMMA)
  27.4     Codon hexagram — delivered (67 tests, GAMMA)
  29.1     Achievement — delivered (62 tests, GAMMA)
  29.2     Calendar reform — delivered (66 tests, GAMMA)
  9.1      Extended star catalog (400 stars) — delivered (54 tests, ALPHA)
  12.1     Catalog ingestion — delivered (60 tests, ALPHA)
  25.1     Daylight worldwide — delivered (BETA)
  25.2     Seasons — delivered (GAMMA)
  26.3     Number scanner — delivered (GAMMA)
  26.4     Precession detect — delivered (GAMMA)
  27.1     Frequency mapper — delivered (GAMMA)
  27.3     Phase space — delivered (59 tests, ALPHA)
  28.1     Knowledge graph — delivered (GAMMA)
  30.1     Text render — delivered (93 tests, ALPHA)
  31.1     Tzolkin board — delivered (BETA)
  31.2     Calendar Round — delivered (33 tests, BETA)
  21.3     Tide predict — delivered (41 tests, BETA)
  29.3     Earth drama — delivered (62 tests, BETA)
  29.5     Calendar politics — delivered (GAMMA)
  29.6     Calendar epoch — delivered (59 tests, BETA)
  41.2     Megalithic alignments — delivered (45 tests, ALPHA)

ROUND 4 DELIVERED (2026-03-12):
  ALPHA: Horizon Geometry (24.5) — 55 tests, COMMITTED
  BETA:  Time Philosophy (35.2) — 82 tests + Birth Profile (36.2) — 66 tests, COMMITTED
  GAMMA: Korean Calendar (53.2) — 123 tests, COMMITTED
  OMEGA: 3 passes wired (constellation, saturn, post_process) + dead code + zodiac decomp + brand fix

ROUND 5 QUEUED (2026-03-12):
  ALPHA: Orbit Trail Pack (8.7) — Kepler ellipses with gradient fade
  BETA:  Convergence Detector (19.1) — cross-system cycle alignment scanner
  GAMMA: Coptic Calendar (40.1) — oldest continuous calendar
  OMEGA: Wire 3 more passes (milkyway, moon, earth) + horizon wiring

═══════════════════════════════════════════════════════════════
  COMPRESSION CYCLE 3 (2026-03-12)
  160+ data modules. 7 render passes (was 4 last cycle).
  OMEGA wiring 3 more → 10 passes. 8 packs still awaiting passes.
  RENDERING GAP CLOSING. UI INTERACTION NEXT.
═══════════════════════════════════════════════════════════════

SHORT-TERM — OMEGA WIRING QUEUE (priority order):
  ✅ constellation_bounds → constellation_pass (ROUND 4 — DONE)
  ✅ saturn_ring_pack → saturn_pass (ROUND 4 — DONE)
  ✅ post_process → post_pass (ROUND 4 — DONE)
  ✅ Dead function cleanup (ROUND 4 — DONE, 5 removed + 2 static)
  ✅ Zodiac decomposition (ROUND 4 — DONE, init→3 + draw→5 helpers)
  ✅ Brand color fix (ROUND 4 — DONE, solar gold)
  → milkyway_pack → milkyway_pass (ROUND 5)
  → moon_pack → moon_pass (ROUND 5)
  → earth_pack → earth_pass (ROUND 5)
  → horizon_geometry → horizon_pass (ROUND 5 — bonus if time)
  ✓ bodygraph_pack → bodygraph_pass (Human Design view)
  ✓ hexagram_geometry → hexagram_pass (I Ching view)
  ✓ tree_geometry → tree_pass (Kabbalah Tree of Life view)
  ✓ deep_sky_pack → deep_sky_pass (Messier/NGC objects)
  ✓ card_pack → card_pass (knowledge info cards)

SHORT-TERM — ALPHA RENDER DATA QUEUE:
  1. Orbit Trail Pack (8.7) — Kepler orbit lines with gradient fade (ROUND 5)
  2. Planet Surface Shader Pack (8.3) — procedural per-planet surfaces
  3. Lens Flare Pack (30.3b) — sun/bright star screen-space flare
  4. Atmosphere Glow Pack (24.3) — atmospheric scattering for Earth View
  5. Click Target Pack (5.1) — hit-test geometry for interactive objects

SHORT-TERM — BETA DATA QUEUE:
  1. Convergence Detector (19.1) — cross-system alignment scanner (ROUND 5)
  2. Structural Map (19.3) — structural patterns across knowledge systems
  3. Dreamspell Oracle (31.3) — interactive Tzolkin game oracle
  4. Card Render Pack (4.x) — knowledge system info card vertex data
  5. Wisdom Engine (32.1) — contextual quote/insight correlator

SHORT-TERM — GAMMA CALENDAR QUEUE:
  1. Coptic Calendar (40.1) — oldest continuous calendar (ROUND 5)
  2. Ethiopian Calendar (40.2) — follows from Coptic, shared structure
  3. Zoroastrian Calendar (39.1) — first dualist calendar
  4. Egyptian Calendar (43.1) — Ma'at and the Sothic cycle
  5. Balinese Pawukon (42.2) — 210-day cycle, Southeast Asian system
  6. Wheel of the Year (41.1) — Celtic 8 festivals + solar position
  7. Myanmar Calendar (42.1) — Southeast Asian lunar system

MID-TERM → PROMOTING TO SHORT-TERM:
  UI Interaction (was MID-TERM, now decomposable — 7 passes wired):
  → Click-to-focus: ALPHA builds click_target_pack → OMEGA wires input handler
  → Keyboard overlay: BETA builds help_overlay data → OMEGA wires as HUD element
  → Timeline scrubber: BETA builds timeline_data → ALPHA builds scrubber geometry

  View Modes (data packs exist, passes needed):
  → Earth View (E key): earth_pass (OMEGA R5) + horizon_pass + daylight data
  → Human Design (D key): bodygraph_pass (OMEGA R6) + gate activation overlay
  → I Ching (I key): hexagram_pass (OMEGA R6) + changing lines animation
  → Kabbalah (K key): tree_pass (OMEGA R6) + Sefirot glow shader

MID-TERM:
  orbit_trail_pass  — OMEGA wires after ALPHA orbit_trail_pack (ROUND 5 delivery)
  planet_surface_pass — OMEGA wires after ALPHA planet_surface (ROUND 6)
  lens_flare_pass   — OMEGA wires after ALPHA lens_flare (ROUND 7)
  Correlation mode (/ key): convergence_detect + alignment dashboard UI
  Card renderer: card_pack (BETA) → card_pass (OMEGA) → cards visible

LONG-TERM → COMPRESSED TO MID-TERM:
  5.2-5.4  Scroll + scrubber: input handler decomposition (ALPHA geometry + OMEGA wiring)
  6.1-6.4  Infrastructure: performance profiling, error recovery, memory guard
  34.1     Community backend: server decision deferred (localStorage first)
  45.1     Audio layer: Web Audio data module (BETA) after frequency mapper

LONG-TERM (TRUE FUTURE):
  47.4     User identity (localStorage + onboarding + profile editor)
  48.4     Extrovert layer display
  49.3     Personal space display
  34.3     Community Kin discovery (needs 34.1 backend)

OMEGA (permanent, rotating):
  Health sweeps + pass wiring + red flag cleanup
  See .context/execute/OMEGA/priority.md
```

ONGOING (AGENT SELF-MAINTAINED + OMEGA OVERSEES):
  Writers (ALPHA-GAMMA) Phase 5 Maintainer handles per-task cleanup.
  OMEGA handles project-wide health (rotating checklist).
  MEGA reviews reports, does NOT run operational commands.

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
| 3.8 Glyph batch | glyph_batch (Agent A) | DELIVERED — 33 tests |
| 3.9 Zodiac render pack | zodiac_pack (ALPHA) | NEEDED — depends on ring/arc/cusp/aspect geometry |
| 4.10 Tzolkin card data | tzolkin_card (Agent B) | DELIVERED (BETA) |
| 4.11 Astrology formatter | astro_summary (Agent B) | DELIVERED (GAMMA) |
| 4.12 Haab calendar | haab (Agent B) | DELIVERED (BETA) |
| 7.5 Star catalog | star_catalog (Agent A) | DELIVERED (ALPHA — 195 stars) |
| 7.6 Constellation lines | constellation (Agent A) | DELIVERED (ALPHA — 40 constellations) |
| 7.7a Star field pack | star_field (ALPHA) | DELIVERED — 53 tests |
| 7.7b Star rendering | star_field (7.7a) | UNBLOCKED — orchestrator |
| 7.8 Constellation overlay | star_field (7.7a) | UNBLOCKED — orchestrator |
| 7.9 Star labels | star_catalog + glyph billboards | UNBLOCKED — orchestrator |
| 8.1 Planet data | planet_data (Agent B) | DELIVERED (GAMMA — 39 tests) |
| 8.2 Atmosphere shader data | atmo_ring (Agent A) | DELIVERED — 42 tests |
| 8.4 Moon data | moon_data (Agent B) | DELIVERED — 33 tests |
| 8.6 Planet render pack | planet_pack (ALPHA) | NEEDED — depends on planet_data |
| 9.1 Extended star catalog | star_catalog_extended (ALPHA) | DELIVERED — 54 tests |
| 9.2 Spectral colors | star_colors (ALPHA) | DELIVERED — 34 tests |
| 10.1 Deep sky catalog | deep_sky (BETA) | DELIVERED — 35 tests |
| 11.1 Hexagram geometry | hexagram_geometry (ALPHA) | NEEDED |
| 11.2 Bodygraph data | bodygraph (ALPHA) | DELIVERED — 44 tests |
| 12.1 Catalog ingestion | catalog_ingest (ALPHA) | DELIVERED — 60 tests |
| 13.1 Geological time | geo_time (BETA) | DELIVERED — 46 tests |
| 13.2 Radiometric dating | radiometric (Agent A) | DELIVERED — 61 tests |
| 14.1 Hebrew calendar | hebrew (GAMMA) | DELIVERED — 49 tests |
| 14.2 Sabbatical cycles | sabbatical (BETA) | DELIVERED — 38 tests |
| 15.1 Hijri calendar | hijri (GAMMA) | DELIVERED — 51 tests |
| 15.2 Prayer times | prayer_times (GAMMA) | DELIVERED — 38 tests |
| 16.1 Buddhist calendar | buddhist (BETA) | DELIVERED — 45 tests |
| 16.2 Kalpa cycles | kalpa (BETA) | DELIVERED — 49 tests |
| 17.1 Panchanga | panchanga (BETA) | DELIVERED — 42 tests |
| 17.2 Nakshatra positions | nakshatra (ALPHA) | DELIVERED — 53 tests |
| 17.3 Yuga cycles | yuga (GAMMA) | DELIVERED — 34 tests |
| 18.1 Sefirot data | sefirot (GAMMA) | DELIVERED — 65 tests |
| 18.2 Tree geometry | tree_geometry (ALPHA) | DELIVERED — 63 tests |
| 18.3 Four Worlds | four_worlds (GAMMA) | DELIVERED — 64 tests |
| 19.1 Convergence detector | convergence (Agent B) | NEEDED — depends on all calendars |
| 19.2 Cycle analysis | cycle_analysis (ALPHA) | DELIVERED — 54 tests |
| 19.3 Structural patterns | structural_map (Agent B) | NEEDED |
| 20.1 Thermochronology | thermo (ALPHA) | DELIVERED — 67 tests |
| 20.2 Tectonic timeline | tectonic (ALPHA) | DELIVERED — 46 tests |
| 21.1 Tidal harmonics | tidal (ALPHA) | DELIVERED — 63 tests |
| 21.2 Surf spots | surf_spots (GAMMA) | DELIVERED — 65 tests |
| 21.3 Tide prediction | tide_predict (BETA) | DELIVERED — 41 tests |
| 22.1 Solar radiation | solar_radiation (ALPHA) | DELIVERED — 40 tests |
| 22.2 Wind patterns | wind_patterns (Agent A) | DELIVERED — 43 tests |
| 22.3 Storm data | storm_data (GAMMA) | DELIVERED — 68 tests |
| 23.1 Snow model | snow_season (GAMMA) | DELIVERED — 36 tests |
| 23.2 Ski resorts | ski_resorts (GAMMA) | DELIVERED — 47 tests |
| 24.1 Earth globe | earth_globe (ALPHA) | DELIVERED — 38 tests |
| 24.2 Atmosphere render | earth_atmosphere (ALPHA) | DELIVERED — 45 tests |
| 24.3 Earth render pack | earth_pack (ALPHA) | NEEDED — depends on earth_globe, earth_atmosphere |
| 25.1 Daylight worldwide | daylight (BETA) | DELIVERED |
| 25.2 Seasonal cycles | seasons (GAMMA) | DELIVERED |
| 2.4 Scale transitions | easing | DELIVERED |
| 7.2 Milky Way spiral | sacred_geometry | DELIVERED |
| 7.3 Cosmic time | cosmic_time | DELIVERED |
| 47.1 Interest profile | interest_profile (BETA) | NEEDED |
| 47.2 Privacy framework | privacy_framework (BETA) | NEEDED |
| 47.3 User preferences | user_prefs (BETA) | NEEDED — depends on 47.1 |
| 48.1 Local events | local_events (BETA) | NEEDED — depends on 47.1 |
| 48.2 Event filter | event_filter (BETA) | NEEDED — depends on 47.1, 48.1 |
| 48.3 Neighbor roulette | neighbor_roulette (BETA) | NEEDED — depends on 47.1, 47.2 |
| 49.1 Personal space | personal_space (BETA) | NEEDED — depends on 47.1, 47.3 |
| 49.2 Content curator | content_curator (BETA) | NEEDED — depends on 47.1, 48.1, 32.1, 33.1 |

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
| Sacred Calendars | Hebrew + Islamic + Buddhist + Hindu compute correctly for any date |
| Kabbalah VISUAL | Tree of Life renders with planetary activations |
| Deep Time | Geological timeline + Yuga + Kalpa display at cosmic scale |
| Unity View | Cross-system convergence detection works for 10+ systems |
| User Identity | Interest profile + privacy framework + preferences stored locally |
| Extrovert Layer | Event feed filtered by interests, astronomical events auto-generated, roulette opt-in |
| Introvert Layer | Personal space with tracked cycles, cosmic fingerprint, curated "Today for you" digest |
| Privacy SOLID | No tracking, no third-party analytics, user can export/delete all data at any time |
| Phase 8 DEPLOYED | Live on the web |

---

---

## TRACK 31: Deep Tzolkin — The Dreamspell Game

The Tzolkin is not just a calendar — it's a game, a meditation, a social structure. Gabriel wants FULL implementation: the 260-day board, the 52-year flower, wavespell mechanics, oracle computation, castle navigation. Arguelles's vision made interactive.

### 31.1 Agent: Tzolkin 260-Day Board Data (Agent B) — DELIVERED (agent-BETA)
- [x] Pure module: `src/systems/tzolkin/tzolkin_board.h/.c`
- [ ] Full 260-kin grid as structured data: 13 tones × 20 seals
- [ ] Per kin: tone, seal, color (Red/White/Blue/Yellow), wavespell (1-20), castle (1-5)
- [ ] Harmonic family groupings (columns of 4 kin with same tone)
- [ ] Chromatic groupings (5 kin sequences)
- [ ] Galactic activation portals (52 green squares on the Tzolkin grid)
- [ ] `tzolkin_board_kin(int tone, int seal)` → kin_t with full metadata
- [ ] `tzolkin_board_column(int col)` → 20 kins in that harmonic column
- [ ] `tzolkin_board_row(int row)` → 13 kins in that wavespell row
- [ ] `is_galactic_portal(int kin)` → bool (the 52 portal days)
- [ ] Data source: Arguelles Dreamspell (ACQUIRED)

### 31.2 Agent: 52-Year Calendar Round (Agent B) — DELIVERED (agent-BETA, 33 tests)
- [x] Pure module: `src/systems/tzolkin/calendar_round.h/.c`
- [ ] The "flower" pattern: 52 Haab years × 365 = 73 Tzolkin cycles × 260 = 18,980 days
- [ ] 4 year-bearer logic: only 4 of 20 seals can carry a year (depends on correlation)
- [ ] Calendar Round date: Tzolkin day + Haab day uniquely identifies day within 52-year cycle
- [ ] `calendar_round_from_jd(double jd)` → {tzolkin_kin, haab_date, year_bearer, position_in_52}
- [ ] `calendar_round_next_occurrence(int kin, int haab_month, int haab_day)` → double jd
- [ ] `calendar_round_flower_position(double jd)` → angle in 52-year flower (0-360°)
- [ ] Visual data: 52 petals, each petal = 1 solar year, colored by year-bearer
- [ ] Depends on: tzolkin (DELIVERED), haab (DELIVERED)

### 31.3 Agent: Dreamspell Oracle Engine (Agent B) — DELIVERED
- [x] Pure module: `src/systems/tzolkin/dreamspell.h/.c`
- [x] Full oracle computation for any kin: guide, analog, antipode, occult, hidden
- [x] Wavespell mechanics: 13-day wave purpose, action, power for each position
- [x] Castle narrative: 5 castles × 4 wavespells = 20 wavespells × 13 days = 260
- [x] Earth family (4 kin per family, 5 families): Cardinal, Core, Signal, Gateway, Polar
- [x] Color family cycling: Red (initiate) → White (refine) → Blue (transform) → Yellow (ripen)
- [x] `dreamspell_oracle(int kin)` → oracle_t {guide, analog, antipode, occult, hidden}
- [x] `dreamspell_wavespell(int kin)` → wavespell_t {number, purpose, action, power}
- [x] `dreamspell_castle(int kin)` → castle_t {number, name, color, theme}
- [x] `dreamspell_earth_family(int seal)` → family_t {name, members[4]}
- [x] Data source: Dreamspell kit (ACQUIRED), Law of Time website (FREE)

### 31.4 Tzolkin Visual Board — Orchestrator
- [ ] Interactive 13×20 grid with full color coding
- [ ] Click any cell → show oracle, wavespell, castle context
- [ ] Current kin pulsing glow, user's birth kin marked
- [ ] Galactic portal days highlighted (the 52 special kins)
- [ ] 52-year flower visualization: radial petal pattern showing Calendar Round position
- [ ] Wavespell journey: 13-day arc with position indicator
- [ ] Toggle: M key (Maya full board on/off)

---

## TRACK 32: The Wisdom Engine — Voices Across Time

Plato, Aristotle, Lao Tzu, Rumi, Aryabhata, Pythagoras — their insights correlated with active knowledge systems. Not random quotes. ALGORITHMIC wisdom: what appears depends on who you are (birth kin), what you're looking at (navigation state), and what the sky is doing (current alignments).

### 32.1 Agent: Wisdom Quote Database (Agent B) — DELIVERED (BETA: wisdom, 125 quotes, 13 cultures)
- [x] Pure module: `src/systems/unified/wisdom.h/.c`
- [x] ~125 curated quotes from great minds across 13 civilizations
- [ ] Per quote: text, author, year, culture, tags[] (astronomy, time, cycles, unity, mathematics, nature, consciousness)
- [ ] Tags map to knowledge systems: astronomy quotes surface near zodiac view, unity quotes near convergence events
- [ ] `wisdom_quote_count()`, `wisdom_quote_get(int index)` → wisdom_t
- [ ] `wisdom_by_tag(const char *tag, int *results, int max)` → count
- [ ] `wisdom_by_culture(const char *culture, int *results, int max)` → count
- [ ] Source categories:
  - [ ] Greek: Plato (time is a moving image of eternity), Aristotle (nature does nothing in vain), Pythagoras (all is number), Heraclitus (everything flows)
  - [ ] Eastern: Lao Tzu (the Tao that can be told), Confucius, Zhuangzi, Nagarjuna
  - [ ] Islamic: Rumi, Al-Khwarizmi, Ibn Arabi, Omar Khayyam
  - [ ] Indian: Aryabhata, Bhaskara, Patanjali, Upanishads
  - [ ] Mayan: Popol Vuh excerpts, Chilam Balam, Arguelles
  - [ ] Hebrew: Ecclesiastes (a time for everything), Sefer Yetzirah, Zohar
  - [ ] Modern: Einstein (the distinction between past present future is an illusion), Sagan, Feynman

### 32.2 Agent: Wisdom Correlation Engine (Agent B) — DELIVERED (BETA: wisdom_engine)
- [x] Pure module: `src/systems/unified/wisdom_engine.h/.c`
- [x] Input: current state (active systems, user kin, navigation context, convergence events)
- [x] Output: ranked list of contextually relevant quotes
- [ ] Correlation rules:
  - [ ] User viewing zodiac → surface astronomy/astrology quotes
  - [ ] User's birth kin seal → surface quotes from matching culture/theme
  - [ ] Convergence event active → surface unity/harmony quotes
  - [ ] Deep time scale → surface cosmic perspective quotes
  - [ ] Earth view → surface nature/ecology quotes
  - [ ] Calendar reform active → surface time/power/freedom quotes
- [ ] `wisdom_select(wisdom_context_t *ctx, int *results, int max)` → count
- [ ] `wisdom_context_t`: { active_systems[], user_kin, scale_level, convergence_score, view_mode }
- [ ] Rotation: don't show same quote twice in a session
- [ ] Subtle, non-intrusive: appears as a quiet text overlay, not a popup

### 32.3 Wisdom Display — Orchestrator
- [ ] Subtle text overlay: quote fades in at bottom or corner of screen
- [ ] Context-triggered: appears when conditions align (not random, not constant)
- [ ] Attribution: "— Plato, Timaeus (360 BCE)"
- [ ] Frequency: max 1 quote per 5 minutes of active use (never annoying)
- [ ] User can dismiss or pin favorites
- [ ] Toggle: W key (wisdom on/off)

---

## TRACK 33: Fun Facts & Contextual Discovery

The system notices things and shares them. Cross-references between systems, location-based discoveries, behavioral observations. "FUN FACT: it's humid in X village near you but in X city people are complaining about heat." This is Time being ALIVE — not just displaying data, but noticing patterns and sharing insights.

### 33.1 Agent: Fun Fact Generator (Agent B) — DELIVERED
- [x] Pure module: `src/systems/unified/fun_facts.h/.c`
- [x] Categories:
  - [x] **Cross-system**: "Your Tzolkin seal (Dragon) corresponds to I Ching hexagram 1 (Creative Force) — both represent INITIATION"
  - [x] **Historical**: "On this day in 1582, Pope Gregory XIII deleted 10 days from the calendar"
  - [x] **Astronomical**: "Venus is currently in the same zodiac sign as your birth chart — a Venus return"
  - [x] **Mathematical**: "The Hebrew molad and Hindu tithi agree to 0.5 seconds per month — different continents, same moon"
  - [x] **Geological**: "The last time CO2 was this high (420 ppm), sea levels were 20m higher — the Pliocene, 3 million years ago"
- [x] Per fact: text, category, trigger_condition (what state activates this fact), priority
- [x] `fun_fact_evaluate(app_context_t *ctx, int *results, int max)` → matching facts
- [x] `fun_fact_count()`, `fun_fact_get(int index)` → fun_fact_t
- [x] Trigger conditions: date match, system alignment, user navigation, birth chart aspect

### 33.2 Agent: Location-Aware Alerts (Agent B)
- [ ] Pure module: `src/systems/earth/location_alerts.h/.c`
- [ ] Alerts computed from existing modules (NO external APIs):
  - [ ] Surf alert: "King tide at Pipeline in 3 days" → from tidal prediction (Track 21)
  - [ ] Snow alert: "Peak powder probability at Niseko this month" → from snow model (Track 23)
  - [ ] Golden hour: "Golden hour at your location starts in 47 minutes" → from solar_events
  - [ ] Rain/humidity: computed from atmospheric model (Track 22) — seasonal probability, not live weather
  - [ ] Prayer time: "Maghrib in 12 minutes" → from prayer_times (Track 15)
- [ ] `location_alerts_evaluate(double jd, double lat, double lon, alert_t *results, int max)` → count
- [ ] Per alert: type, message, urgency (info/suggestion/alert), time_until_event
- [ ] All computed from first principles — this is TIME's philosophy: no APIs, only math
- [ ] Depends on: tidal (21), snow (23), solar_events, prayer_times (15), atmospheric (22)

### 33.3 Agent: User Context Tracker (Agent B)
- [ ] Pure module: `src/systems/unified/user_context.h/.c`
- [ ] Track user behavior to personalize facts (local only, no server):
  - [ ] Birth date + birth location (entered once, stored in localStorage via JS bridge)
  - [ ] Birth kin (Tzolkin), birth hexagram (I Ching), birth gate (HD) — all computed
  - [ ] Navigation history: which scales visited, which systems toggled, time spent per view
  - [ ] Current session patterns: is user exploring deep time or personal chart?
- [ ] `user_context_update(user_context_t *ctx, user_action_t action)` — pure state update
- [ ] `user_interests(user_context_t *ctx)` → interest_profile_t (which systems they engage with most)
- [ ] Privacy: ALL data stays local. No telemetry. No tracking. The code serves the USER.

### 33.4 Fun Facts Display — Orchestrator
- [ ] Non-intrusive notification area (bottom-right or top bar)
- [ ] Facts fade in/out, auto-dismiss after 10 seconds
- [ ] Tap/click to expand for more detail
- [ ] "More like this" option seeds future fact selection
- [ ] Max 1 fact per 3 minutes (never overwhelming)
- [ ] Toggle: F key (fun facts on/off)

---

## TRACK 34: Community & Kin Discovery

Users can optionally share their location and Kin. Other users with the same or complementary Kin nearby become visible. This generates culture: strangers discovering shared Dreamspell identities, forming kin groups, meeting in person. The app becomes a social fabric woven from sacred time.

### 34.1 Architecture: Privacy-First Social Layer
- [ ] Design decision: this is the ONLY track that requires a server component
- [ ] Minimal backend: location + kin + online status (nothing else)
- [ ] User opts IN explicitly (toggle, default OFF)
- [ ] Data: { user_id (anonymous hash), lat, lon, kin_number, last_seen, visible }
- [ ] No chat, no profiles, no messages — just PRESENCE on a shared map
- [ ] When you see another dot, you know: "a Yellow Cosmic Sun is 2km from me right now"
- [ ] Kin groups: same wavespell, same seal, same tone — these are the Dreamspell's social units
- [ ] The app doesn't facilitate conversation — it facilitates AWARENESS

### 34.2 Agent: Kin Compatibility Data (Agent B)
- [ ] Pure module: `src/systems/tzolkin/kin_social.h/.c`
- [ ] Kin relationships from Dreamspell: analog, antipode, occult, guide
- [ ] Same wavespell = "wavespell family" (13 members)
- [ ] Same seal = "seal tribe" (13 members across 13 tones)
- [ ] Same color = "color family" (5 seals × 13 tones = 65 members)
- [ ] Earth family grouping: which seals form a working group
- [ ] `kin_relationship(int kin_a, int kin_b)` → relationship_t {type, description}
- [ ] `kin_tribe_members(int seal)` → array of 13 kin numbers
- [ ] `kin_wavespell_members(int wavespell)` → array of 13 kin numbers
- [ ] The social meaning: "your analog kin is your best friend, your antipode is your teacher"

### 34.3 Community Display — Orchestrator
- [ ] Earth View: anonymous dots on globe showing other active users (when both opt in)
- [ ] Dot color = seal color (Red/White/Blue/Yellow)
- [ ] Tap dot → see kin number + relationship to your kin
- [ ] "3 people in your wavespell are within 10km" notification
- [ ] Kin tribe gathering: when 4+ same-seal users are nearby, highlight it
- [ ] This is the ONLY feature that touches networking — everything else is pure computation
- [ ] Server: minimal API, open-source, self-hostable. Consider WebRTC for P2P as future option.

---

## TRACK 35: Cultural Mythology & Philosophy Layer

Time is not just math — it's STORY. Each time system emerged from a civilization with myths, philosophy, and wisdom that shaped how they understood time. This track brings that cultural depth into the experience: Norse cosmology, Roman calendar politics, Indian golden age, Chinese dynastic wisdom, Buddhist philosophy of existence, Irish mystery traditions, Greek philosophy. Not a textbook — a living encounter with how humanity has thought about time across every culture.

### 35.1 Agent: Cultural Story Database (Agent B) — DELIVERED
- [x] Pure module: `src/systems/unified/cultural_stories.h/.c`
- [x] Per culture: origin myth, time philosophy, key figures, golden age period
- [x] Categories:
  - [x] **Norse**: Yggdrasil (World Tree as time structure), Ragnarok (cyclical destruction), Norns (weavers of fate/time), Viking navigation by stars
  - [x] **Greek**: Chronos vs Kairos (two kinds of time), Plato's Timaeus (time as moving image of eternity), Pythagorean harmony
  - [x] **Roman**: Calendar politics (Caesar's reform, month naming: July/August = ego), Senate time control
  - [x] **Indian**: Vedic golden age, Aryabhata's genius, dance of Shiva as cosmic rhythm, dharma as natural law
  - [x] **Chinese**: Mandate of Heaven tied to calendar accuracy, I Ching as cosmic computer, dynastic astronomy
  - [x] **Buddhist**: Why monks live as they do — Uposatha observance, impermanence as time philosophy, mindfulness = present-moment time
  - [x] **Celtic/Irish**: Sidhe (fairy mounds as time portals), Samhain/Beltane (fire festivals marking time), Newgrange (5000-year-old solstice marker predating Stonehenge)
  - [x] **Egyptian**: Ma'at (cosmic order), Sothic cycle (Sirius rising = Nile flood = year), Book of the Dead as time journey
  - [x] **Mayan**: Popol Vuh creation myth, Hero Twins, Long Count as cosmological clock
- [x] Per story: text, culture, era, related_system (which Track it connects to), mood_tags[]
- [x] `cultural_story_count()`, `cultural_story_get(int index)`, `cultural_story_by_culture(const char *culture)`
- [x] `cultural_story_for_system(int system_id)` → stories that contextualize that time system

### 35.2 Agent: Philosophy of Time Database (Agent B)
- [ ] Pure module: `src/systems/unified/time_philosophy.h/.c`
- [ ] Great thinkers on the nature of time:
  - [ ] Plato: "Time is the moving image of eternity" (Timaeus 37d)
  - [ ] Aristotle: "Time is the measure of motion" (Physics IV.11)
  - [ ] Augustine: "What then is time? If no one asks me, I know; if I wish to explain, I do not know"
  - [ ] Nagarjuna: "Time is dependent on events; without events, time does not exist"
  - [ ] Einstein: "The distinction between past, present, and future is only a stubbornly persistent illusion"
  - [ ] Heidegger: Being-toward-death as fundamental temporality
  - [ ] Buddhist: Impermanence (anicca) — all phenomena arise and pass away
  - [ ] Arguelles: "Time is not money. Time is art."
- [ ] Per entry: quote, author, year, tradition, related_tracks[]
- [ ] These feed into Track 32 (Wisdom Engine) as high-priority contextual material

### 35.3 Cultural Story Display — Orchestrator
- [ ] When a user engages with a time system, offer cultural context on demand
- [ ] "Why this system exists" panel: the civilization, the philosophy, the story
- [ ] Example: viewing Hebrew calendar → "The Metonic cycle was standardized by Hillel II in 359 CE, during the crisis of the Roman Empire's pressure on Jewish autonomy. The calendar became the last act of the Sanhedrin."
- [ ] Not forced — available when the user is curious. Depth on demand.
- [ ] Toggle: G key (cultural stories on/off)

---

## TRACK 36: The Welcome Journey — Onboarding as Art

First contact with Time should itself be an experience of time. No boring forms. The user enters through an animated, beautiful discovery of WHO they are in every time system. Birth date and location are the keys that unlock everything.

### 36.1 Onboarding Flow Design — Orchestrator
- [ ] Step 1: "When were you born?" — elegant date picker (day, month, year)
  - [ ] Instantly computes: Tzolkin Kin, I Ching hexagram, Chinese animal, Hebrew year, HD gate
  - [ ] Visual reveal: each system's result appears as an animated card
- [ ] Step 2: "Where were you born?" — interactive globe spin
  - [ ] Start: full Earth globe, slowly rotating
  - [ ] User clicks continent → smooth zoom animation → continent fills view
  - [ ] Geography becomes visible: coastlines, mountains, rivers
  - [ ] Clicks region → closer zoom → cities appear
  - [ ] Clicks city or area → pin drops → "This is where your story begins"
  - [ ] Computes: Ascendant, houses, local sidereal time at birth, birth Nakshatra
- [ ] Step 3: "Where are you now?" — same globe interaction, separate from birth location
  - [ ] Enables: prayer times, golden hour, tide predictions, local astronomy
  - [ ] "You don't need to be precise — we just want to know your sky"
- [ ] Step 4 (OPTIONAL): "What interests you?" — visual icon grid, not text checkboxes
  - [ ] Icons for: Astrology, Mayan/Tzolkin, I Ching, Chinese, Human Design, Sacred Geometry, Deep Time, Earth/Nature
  - [ ] Skip entirely if the user wants — no pressure
  - [ ] Seeds the Wisdom Engine (Track 32) and Fun Facts (Track 33) with preferences
- [ ] All steps skippable. Minimal data needed: birth date alone unlocks 80% of the experience.
- [ ] Data stored in browser localStorage — NO server, NO accounts, NO tracking

### 36.2 Agent: Birth Profile Computer (Agent B)
- [ ] Pure module: `src/systems/unified/birth_profile.h/.c`
- [ ] Input: birth date + optional birth location
- [ ] Output: complete profile across ALL time systems
- [ ] `birth_profile_compute(int year, int month, int day, double lat, double lon)` → birth_profile_t
- [ ] birth_profile_t contains:
  - [ ] Tzolkin kin number, seal, tone, wavespell, castle, oracle
  - [ ] I Ching hexagram, changing lines
  - [ ] Chinese animal, element, stem-branch
  - [ ] Hebrew date, month, sabbatical position
  - [ ] Buddhist era, Vesak proximity
  - [ ] Hijri date
  - [ ] HD incarnation cross (Sun gate, Earth gate, Nodal gates)
  - [ ] Ascendant, Sun sign, Moon sign (if birth time known)
  - [ ] Geological era humor: "You were born 4.54 billion years after Earth formed"
- [ ] Depends on: ALL calendar/system modules

### 36.3 Personal 3D Space — Future Vision
- [ ] "Diamond Lego Room" concept: user creates a miniature 3D personal space
- [ ] Small geometric room assembled from modular diamond/crystal shapes
- [ ] Room reflects user's profile: colors from Kin seal, shapes from hexagram, proportions from HD type
- [ ] Future: room evolves as user explores more of the app
- [ ] Implementation: procedural geometry from user profile data — pure C, rendered in WebGL
- [ ] This is a LONG-TERM artistic vision — capture now, build when foundation is solid

---

## TRACK 37: Unified Style System — Phi Everywhere

**The Golden Ratio governs EVERY visual value.** No magic numbers. No hardcoded pixels. Every font size, spacing, timing, opacity, corner radius, border width, shadow, and icon size derives from phi applied to a small set of base units.

**MANDATORY FOR ALL AGENTS**: Every new module that outputs visual data (sizes, colors, timing, opacity) MUST use `theme.h` and `golden_layout.h`. Hardcoded visual constants are a build-rejection offense. See 37.4 for retrofit of existing modules.

### 37.1 Theme Module — COMPLETE (MEGA: theme.h, 46 tests)
- [x] Pure module: `src/ui/theme.h/.c`
- [x] `theme_t` struct: 14 semantic colors + typography + spacing + timing + opacity + derived metrics
- [x] Two built-in themes: Cosmos (dark), Dawn (light)
- [x] Brand colors: Solar gold {1.0, 0.85, 0.55} + Celestial teal {0.2, 0.75, 0.8}
- [x] User preferences: font_scale, opacity_scale, animation_speed, custom accent
- [x] System-aware colors: accent shifts to knowledge system personality when focused
- [x] Theme interpolation: smooth blend between themes via `theme_lerp()`
- [x] CSS export: `theme_to_css()` generates CSS custom properties for DOM HUD

### 37.2 Golden Layout Module — COMPLETE (MEGA: golden_layout.h, 45 tests)
- [x] Pure module: `src/ui/golden_layout.h/.c`
- [x] Typography, spacing, timing, opacity — all phi-derived
- [x] Screen layout, card dimensions, ring proportions — all phi

### 37.3 Color Theory Module — COMPLETE (MEGA: color_theory.h, 40 tests)
- [x] Pure module: `src/render/color_theory.h/.c`
- [x] sRGB/linear, golden angle palettes, color psychology, 13 system palettes, WCAG AA

### 37.4 MEGA: Style Retrofit — Hardcoded Value Elimination
**Audited 2026-03-06: 248 hardcoded visual constants across 8 files.**
This is MEGA work (crosses module boundaries). NOT agent work.

**Priority 1 — Colors** (183 violations):
- [ ] `color_palette.c` (57): Replace legacy zodiac/planet/element palettes with ct_system_primary/secondary/accent
- [ ] `deep_sky.c` (121): Derive Messier/DSO type colors from color_theory system palettes
- [ ] `aspect_lines.c` (5): Use ct_system_accent() for aspect colors

**Priority 2 — Opacity/Fade** (24 violations):
- [ ] `render_layers.c` (8): Scale layer opacities from gl_opacity_cascade()
- [ ] `system_scale_map.c` (8+): Derive fade ramp and system opacities from golden_layout
- [ ] `card_layout.c` (7): Derive screen-relative spacing from gl_spacing_scale()

**Priority 3 — Physics-driven** (39 violations, partially acceptable):
- [ ] `atmo_ring.c` (24): Blend ct_system_accent() with physical wavelength data
- [ ] `earth_atmosphere.c` (15): Sky/sunset colors — physics-driven but could reference theme for artistic control

**Acceptable / No change needed**:
- `star_colors.c`: Temperature-to-RGB (pure physics, correct as-is)
- Camera/geometry files: No visual constants

- [ ] Verify: no visual regression after each file, `make test` passes throughout

### 37.5 Agent: CSS Custom Property Integration (BETA)
- [ ] Wire `theme_to_css()` into DOM via JS interop
- [ ] `web/style.css` uses `var(--bg-space)`, `var(--text-primary)`, etc.
- [ ] Theme toggle (Shift+T = Cosmos/Dawn), prefs to localStorage

### 37.6 Agent: Knowledge System Theme Extensions (BETA)
- [ ] Per-system card tinting via `theme_system_surface()`
- [ ] Focus mode: accent shifts to system personality color

### 37.7 Brand Identity & Logo — Orchestrator (MEGA) — COMPLETE
- [x] Design TIME logo: The Phi Spiral — golden spiral with φ growth factor
- [x] Logo as procedural GLSL: `shaders/logo.frag` (no image assets)
- [x] Brand spec: `docs/BRAND_IDENTITY.md` (colors, logo construction, voice, typography direction)
- [x] Loading screen animation: spiral draws itself over φ² seconds (2.618s), breathing glow after
- [x] Preview: `web/logo-preview.html` (self-contained WebGL2, open via make serve)
- [ ] Favicon generation: capture shader output at 32x32 + 16x16
- [ ] Wordmark: "TIME" with golden-proportioned letter spacing (deferred until SDF fonts)

### 37.8 Agent: Chakra Knowledge System (BETA) — DELIVERED
- [x] Pure module: `src/systems/chakra/chakra.h/.c`
- [x] 7 chakras: root (red) → crown (violet), Sanskrit names, frequencies, mantras
- [x] Yoga/breathing practice references (informational, never prescriptive)

---

## TRACK 39: Zoroastrian Time — The First Dualist Calendar

The oldest monotheistic religion and the first dualist cosmology (good vs evil mapped to time). Zoroastrianism directly influenced Judaism, Christianity, and Islam. The Bundahishn describes a 12,000-year cosmic cycle from creation through mixture to final separation — a cosmological drama mapped onto calendar time. We have "The Bundahishn: The Zoroastrian Book of Creation" in the library.

### 39.1 Agent: Zoroastrian Calendar Data (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/zoroastrian/zoroastrian.h/.c`
- [x] 12 months of 30 days + 5 Gatha days = 365-day year (no leap year in traditional form)
- [x] Month names (Farvardin through Esfand), day names (Hormozd through Aneran)
- [x] Seasonal festivals: Nowruz (spring equinox), Yalda (winter solstice), 6 Gahanbars (seasonal feasts)
- [x] Zoroastrian Era: from traditional date of Zoroaster or from Yazdgerdi epoch (632 CE)
- [x] `zoroastrian_month()`, `zoroastrian_day_name()`, `zoroastrian_festival()`

### 39.2 Agent: Cosmic Duality Timeline (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/zoroastrian/cosmic_duality.h/.c`
- [x] 12,000-year cosmic cycle from the Bundahishn:
  - [x] Creation (Bundahishn) — Ahura Mazda creates the world in ideal form
  - [x] Mixture (Gumezishn) — current era, good and evil intertwined
  - [x] Separation (Wizarishn/Frashokereti) — final renovation, evil destroyed
- [x] Map 12,000-year cycle to geological timescale for cross-system perspective
- [x] Amesha Spentas (7 divine emanations) as calendar guardians — parallel to Sefirot
- [x] `duality_era()`, `duality_progress()`, `amesha_spenta_for_day()`

### 39.3 Zoroastrian Display — Orchestrator
- [ ] Current Zoroastrian date in HUD
- [ ] Cosmic duality timeline visualization at deep-time scale
- [ ] Cross-reference: Amesha Spentas ↔ Sefirot ↔ Chakras (Track 19 unified)

---

## TRACK 40: Coptic & Ethiopian Time — Africa's Living Calendars

Two ancient African calendar systems still in daily use. The Coptic calendar descends directly from ancient Egypt (the oldest solar calendar on Earth), Christianized under Roman persecution. The Ethiopian calendar is 7-8 years behind the Gregorian — Ethiopia literally lives in a different year. Both use 13 months. Neither is currently represented in Time.

### 40.1 Agent: Coptic Calendar (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/coptic/coptic.h/.c`
- [x] 13 months: 12 months of 30 days + Nasie (5 or 6 epagomenal days)
- [x] Coptic Era: Anno Martyrum from Diocletian persecution (284 CE)
- [x] Month names (Thout through Nasie), Coptic feast days
- [x] Leap year rule: every 4th year (same as Julian)
- [x] Connection to ancient Egyptian civil calendar (direct descendant)
- [x] `coptic_from_jd()`, `coptic_month_name()`, `coptic_year()`

### 40.2 Agent: Ethiopian Calendar (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/ethiopian/ethiopian.h/.c`
- [x] 13 months of sunshine: 12 × 30 + Pagume (5-6 days)
- [x] Ethiopian Era: ~7-8 years behind Gregorian (Incarnation era differs from Dionysius)
- [x] Ge'ez month names (Meskerem through Pagume)
- [x] Enkutatash (New Year, September 11/12), Timkat, Meskel
- [x] `ethiopian_from_jd()`, `ethiopian_month_name()`, `ethiopian_year()`

### 40.3 Coptic & Ethiopian Display — Orchestrator
- [ ] Current dates in both calendars in HUD
- [ ] Visual: 13-month wheel (unique among all calendar systems)
- [ ] Cross-reference: Coptic months → Egyptian months → Decan star groups (Track 43)

---

## TRACK 41: Celtic/Pagan Wheel of the Year

The 8-fold year is the oldest European time system — solstices, equinoxes, and the four cross-quarter days between them. Stonehenge (3000 BCE), Newgrange (3200 BCE), Callanish align to these moments. The fire festivals (Samhain, Beltane, Imbolc, Lughnasadh) divide the year by nature's rhythms, not by political decree. We have Irish mythology books in the library.

### 41.1 Agent: Wheel of the Year Data (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/celtic/wheel_of_year.h/.c`
- [x] 8 festivals with solar-computed dates:
  - [x] **Samhain** (Oct 31 / Nov 1) — Celtic New Year, veil between worlds thinnest
  - [x] **Yule** (winter solstice) — rebirth of the sun
  - [x] **Imbolc** (Feb 1-2) — first stirrings of spring, Brigid's festival
  - [x] **Ostara** (spring equinox) — balance of light and dark
  - [x] **Beltane** (May 1) — fire festival, summer begins
  - [x] **Litha** (summer solstice) — peak of light
  - [x] **Lughnasadh** (Aug 1) — first harvest, games of Lugh
  - [x] **Mabon** (autumn equinox) — second harvest, gratitude
- [x] Cross-quarter dates computed from midpoint between solstice/equinox (astronomical, not fixed)
- [x] Per festival: name, Celtic name, solar longitude, seasonal meaning, element, direction
- [x] `wheel_current_festival()`, `wheel_next_festival()`, `wheel_days_until()`

### 41.2 Agent: Megalithic Alignments (ALPHA) — DELIVERED (ALPHA: megalithic, 45 tests)
- [x] Pure module: `src/render/megalithic.h/.c`
- [x] 5 major sites with alignment data:
  - [x] **Newgrange** (Ireland, 3200 BCE): winter solstice sunrise, tolerance 5.0°
  - [x] **Stonehenge** (England, 3000 BCE): summer solstice sunrise, tolerance 2.0°
  - [x] **Callanish** (Scotland, 2900 BCE): lunar standstill south, tolerance 3.0°
  - [x] **Carnac** (France, 4500 BCE): summer solstice sunrise, tolerance 2.5°
  - [x] **Mnajdra** (Malta, 3600 BCE): equinox sunrise, tolerance 1.5°
- [x] Per site: lat/lon, azimuth of alignment, target event, epoch
- [x] Compute sunrise/sunset azimuth from solar position code → verify alignment
- [x] `meg_site_count()`, `meg_check_alignment()`, `meg_aligned_now()`, `meg_days_to_alignment()`

### 41.3 Celtic Display — Orchestrator
- [ ] Wheel of the Year as 8-spoke visual overlay (Earth View)
- [ ] Current position on the wheel, days until next festival
- [ ] Megalithic alignment alerts: "Newgrange aligns in 3 days" (winter solstice approaching)

---

## TRACK 42: Southeast Asian Calendars — Burma, Thai, Lao

Living calendar systems used daily by hundreds of millions of people. The Myanmar (Burmese) calendar has its own unique lunisolar intercalation algorithm — we literally have the algorithm document in our library. Thai and Lao calendars use Buddhist Era dating with modified solar reckoning. We have "Myanmar Calendar Algorithm", "Old Muslim Calendars of SE Asia", and Theravada Traditions references.

### 42.1 Agent: Myanmar Calendar (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/myanmar/myanmar.h/.c`
- [x] Unique lunisolar system with Metonic-like intercalation but different from any other tradition
- [x] Myanmar months (Tagu through Tabaung), 29/30-day alternation
- [x] Watat (intercalary month) and Waso (double month) rules
- [x] Myanmar Era (Thekkarit/Sakkaraj from 638 CE)
- [x] Thingyan (water festival, Myanmar New Year) date computation
- [x] Reference: Myanmar Calendar Algorithm document in library
- [x] `myanmar_from_jd()`, `myanmar_month_name()`, `myanmar_is_watat_year()`

### 42.2 Agent: Thai Solar Calendar (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/thai/thai_calendar.h/.c`
- [x] Buddhist Era (BE = CE + 543) — Thailand's official calendar
- [x] Modified Gregorian structure with BE year numbering
- [x] Thai month names, traditional Thai New Year (Songkran, April 13-15)
- [x] Royal Thai calendar specifics (year starts January 1 since 1941, previously April 1)
- [x] `thai_year()`, `thai_month_name()`, `thai_is_songkran()`

### 42.3 Southeast Asian Display — Orchestrator
- [ ] Current dates in Myanmar and Thai calendars in HUD
- [ ] Water festival countdown (Thingyan/Songkran — both cultures share the tradition)
- [ ] Cross-reference: Buddhist Era (Track 16) ↔ Thai BE ↔ Myanmar calendar

---

## TRACK 43: Egyptian Time — Ma'at and the Sothic Cycle

Ancient Egypt created the first 365-day solar calendar, 3000 years before Caesar reformed Rome's calendar. The Sothic cycle — 1,461 years for the heliacal rising of Sirius to drift through the entire civil calendar and return — is one of the most elegant astronomical observations in human history. The 36 decans (star groups whose rising marked the night hours) are the ancestors of the zodiac. Referenced in multiple books across our library.

### 43.1 Agent: Egyptian Calendar Data (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/egyptian/egyptian.h/.c`
- [x] Civil calendar: 12 months × 30 days + 5 epagomenal days = 365 (no leap year)
- [x] Three seasons: Akhet (inundation), Peret (emergence), Shemu (harvest)
- [x] Month names (Thoth through Mesore + 5 epagomenal days)
- [x] Sothic cycle: 1,461 civil years = 1,460 Julian years (Sirius return period)
- [x] Current position in Sothic cycle from any Julian Day
- [x] Relationship to Coptic calendar (Track 40) — direct descendant
- [x] `egyptian_from_jd()`, `egyptian_season()`, `sothic_cycle_position()`

### 43.2 Agent: Decan Star Data (ALPHA) — DELIVERED (ALPHA: decan_stars)
- [x] Pure module: `src/render/decan_stars.h/.c`
- [x] 36 decans as star groups: name, principal star, ecliptic longitude range (10° each)
- [x] Decan rising times: which decan rises at each hour of the night (ancient star clock)
- [x] Compute current rising decan from sidereal time + observer latitude
- [x] Map decans to modern star identifications (where known)
- [x] Connection: 36 decans → 12 zodiac signs (3 decans per sign) → astrology (Track 3)
- [x] `decan_count()`, `decan_rising_now()`, `decan_for_ecliptic_longitude()`

### 43.3 Egyptian Display — Orchestrator
- [ ] Egyptian date with season name in HUD
- [ ] Sothic cycle progress bar (1,461-year visualization)
- [ ] Decan star clock: which decan rules the current hour of the night
- [ ] Cross-reference: Decans → Zodiac signs → Tarot decans (Track 44)

---

## TRACK 44: Tarot as Time System — 22 Paths Through the Year

The 22 Major Arcana map to the 22 Hebrew letters, 22 paths on the Tree of Life (Track 18), and 22 divisions of the zodiac/planets/elements. The 36 minor arcana number cards (2-10 of each suit) map to the 36 zodiac decans (Track 43). The court cards map to zodiac quadrants. This is a time system hiding inside a divination tool — Crowley's Book of Thoth and Aleister Crowley's 777 (both in our library) make the correspondences explicit.

### 44.1 Agent: Tarot-Calendar Mapping (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/tarot/tarot.h/.c`
- [x] 22 Major Arcana → Hebrew letter + zodiac sign/planet/element (Golden Dawn attributions)
  - [x] 12 zodiac Majors (Emperor=Aries through Star=Aquarius)
  - [x] 7 planetary Majors (Magician=Mercury through World=Saturn)
  - [x] 3 elemental Majors (Fool=Air, Hanged Man=Water, Judgment=Fire/Spirit)
- [x] 36 number cards (2-10 per suit) → 36 zodiac decans (10° each)
- [x] 16 court cards → zodiac quadrant rulerships
- [x] 4 suits (Wands/Cups/Swords/Disks) → elements (Fire/Water/Air/Earth)
- [x] 10 Sephirotic mapping: Ace=Kether through 10=Malkuth per suit
- [x] For any date: which Major Arcana rules (via zodiac sign) + which decan card rules
- [x] `tarot_major_for_sign()`, `tarot_decan_card()`, `tarot_for_date()`, `tarot_path_on_tree()`

### 44.2 Agent: Tarot Visual Data (ALPHA) — DELIVERED (ALPHA: tarot_visual)
- [x] Pure module: `src/render/tarot_visual.h/.c`
- [x] Per card: symbolic color scheme (4 Golden Dawn color scales), geometric motifs
- [x] Thoth deck symbolism (Crowley/Harris): geometric and color data for future rendering
- [x] Card frame proportions: golden ratio rectangle (1:1.618)
- [x] Tree of Life path positions for each Major Arcanum (reuse Tree geometry from 18.2)
- [x] `tv_major_get()`, `tv_card_color()`, `tv_card_frame()`, `tv_tree_position()`

### 44.3 Tarot Display — Orchestrator
- [ ] Current ruling Major Arcanum based on zodiac position of Sun
- [ ] Current decan card based on exact solar longitude
- [ ] Tree of Life overlay: highlight active path (the "card of the day" on the Tree)
- [ ] Cross-reference: Tarot → Hebrew letters (Track 14/18) → Zodiac (Track 3) → Decans (Track 43)

---

## TRACK 45: Audio Layer — Music of the Spheres

Pythagoras heard it. Kepler calculated it. We can generate it. Track 27.1 already has the frequency mapper that transposes planetary orbital periods into audible range. This track extends that pure data into actual audio output — hear the solar system as a living chord. The planets ARE music; we just need to make it audible.

### 45.1 Agent: Web Audio Data (BETA) — DELIVERED
- [x] Pure module: `src/systems/unified/audio_data.h/.c`
- [x] Frequency tables: planetary orbital periods transposed to audible range (reference: frequency.h)
- [x] Musical intervals between planets: which pairs form consonances (octave, fifth, fourth, third)
- [x] Harmonic series for each planet: fundamental + overtones based on orbital eccentricity
- [x] Moon phase modulation: lunar cycle as amplitude envelope
- [x] Aspect sonification: conjunction = unison, opposition = octave, trine = fifth, square = fourth
- [x] `audio_planet_freq()`, `audio_interval()`, `audio_aspect_harmony()`, `audio_moon_envelope()`

### 45.2 Audio Engine — Orchestrator
- [ ] WebAudio API synthesis via Emscripten JS interop
- [ ] Real-time chord: all visible planets as simultaneous tones
- [ ] Aspect events as musical transitions (conjunction crescendo, opposition bass drop)
- [ ] Volume/mute per planet, master volume control
- [ ] Key: M to toggle audio, Shift+M for meditation mode

### 45.3 Meditation Mode — Orchestrator
- [ ] Sustained drone: current planetary chord, slowly evolving
- [ ] Breathing rhythm: amplitude pulsing at phi-timed intervals (inhale φ seconds, exhale φ² seconds)
- [ ] Visual dimming: non-essential layers fade, only the sky and the sound remain
- [ ] Time flows at 1x — this is real-time sky, real-time sound, real-time presence

---

## TRACK 46: Accessibility & Internationalization

Time belongs to all of humanity. That means screen readers, RTL languages, high contrast, and translation-ready strings. This is not an afterthought — it is a moral obligation. Every human who encounters Time should be able to experience it regardless of ability or language.

### 46.1 Agent: Screen Reader Data (BETA) — DELIVERED
- [x] Pure module: `src/ui/accessibility.h/.c`
- [x] ARIA label generation for all visual elements
- [x] Text descriptions of visual state: "Sun in Aries at 15 degrees, Moon in Cancer at 22 degrees"
- [x] Announce time system changes, scale transitions, festival alerts
- [x] `aria_label_for_hud()`, `aria_describe_sky()`, `aria_announce_event()`

### 46.2 Agent: RTL Layout Support (BETA) — DELIVERED
- [x] Pure module: `src/ui/rtl_layout.h/.c`
- [x] Detect RTL context for Arabic, Hebrew, Persian, Urdu text
- [x] Layout mirroring rules: which UI elements flip, which don't (sky doesn't flip)
- [x] Bidirectional text handling for mixed LTR/RTL content (e.g., "Moon in القمر")
- [x] `rtl_is_rtl_script()`, `rtl_mirror_layout()`, `rtl_bidi_reorder()`

### 46.3 Agent: Translation Keys (BETA) — DELIVERED
- [x] Pure module: `src/ui/i18n.h/.c`
- [x] All user-facing strings as translation keys (not hardcoded English)
- [x] Default English strings embedded, extensible to other languages
- [x] Pluralization rules (1 day / 2 days / 5 days differ across languages)
- [x] Calendar system names, month names, festival names — all keyed
- [x] `i18n_get()`, `i18n_set_locale()`, `i18n_plural()`

### 46.4 High Contrast Mode — Orchestrator
- [ ] Alternative theme with WCAG AAA contrast ratios (7:1 minimum)
- [ ] Patterns in addition to colors for color-blind users (hatching, dotting on chart elements)
- [ ] Keyboard-only navigation: Tab through all interactive elements
- [ ] Reduced motion mode: disable animations for vestibular sensitivity
- [ ] Key: Shift+H for high contrast toggle

---

## Execution Priority Order

Work these tracks interleaved, not sequentially. Priority by impact:

```
COMPLETED:
  1.1-1.4  Track 1 (HUD wiring) — all done
  2.1-2.4  Track 2 (multi-scale camera) — all done
  3.1-3.8  Track 3 (zodiac wheel) — all rendering done + glyph batch
  4.7-4.12 Track 4 agent modules — all 6 delivered
  7.5-7.6  Track 7 data modules — star catalog (195) + constellations (40) delivered
  8.1      Planet data — delivered (39 tests, GAMMA)
  8.2      Atmosphere & ring geometry — delivered (42 tests, ALPHA)
  8.4      Moon data (9 moons) — delivered (33 tests, BETA)
  9.2      Star spectral colors — delivered (34 tests, ALPHA)
  10.1     Deep sky catalog (Messier+NGC) — delivered (35 tests, BETA)
  11.2     Bodygraph geometry — delivered (44 tests, ALPHA)
  13.1     Geological time scale — delivered (46 tests, BETA)
  13.2     Radiometric dating — delivered (61 tests, ALPHA)
  14.1     Hebrew calendar — delivered (49 tests, GAMMA)
  15.1     Hijri calendar — delivered (51 tests, GAMMA)
  15.2     Prayer times — delivered (38 tests, GAMMA)
  16.1     Buddhist calendar — delivered (45 tests, GAMMA)
  16.2     Kalpa cosmic cycles — delivered (49 tests, BETA)
  17.1     Panchanga — delivered (42 tests, BETA)
  17.2     Nakshatra — delivered (53 tests, ALPHA)
  14.2     Sabbatical cycles — delivered (38 tests, BETA)
  17.3     Yuga cycles — delivered (34 tests, GAMMA)
  18.1     Sefirot data — delivered (65 tests, GAMMA)
  18.2     Tree geometry — delivered (63 tests, ALPHA)
  18.3     Four Worlds — delivered (64 tests, GAMMA)
  19.2     Cycle analysis — delivered (54 tests, ALPHA)
  20.1     Thermochronology — delivered (67 tests, ALPHA)
  20.2     Tectonic timeline — delivered (46 tests, ALPHA)
  20.4     Climate history — delivered (34 tests, GAMMA)
  20.5     Fossil milestones — delivered (39 tests, GAMMA)
  21.1     Tidal harmonics — delivered (63 tests, ALPHA)
  21.2     Surf spots — delivered (65 tests, GAMMA)
  22.1     Solar radiation — delivered (40 tests, ALPHA)
  22.2     Wind patterns — delivered (43 tests, ALPHA)
  22.3     Storm data — delivered (68 tests, GAMMA)
  23.1     Snow season — delivered (36 tests, GAMMA)
  23.2     Ski resorts — delivered (47 tests, GAMMA)
  24.1     Earth globe — delivered (38 tests, ALPHA)
  24.2     Earth atmosphere — delivered (45 tests, ALPHA)
  26.1     Sacred numbers — delivered (44 tests, BETA)
  26.2     Ratio analysis — delivered (47 tests, ALPHA)
  27.2     Biological clocks — delivered (45 tests, GAMMA)
  27.4     Codon hexagram — delivered (67 tests, GAMMA)
  29.1     Achievement — delivered (62 tests, GAMMA)
  29.2     Calendar reform — delivered (66 tests, GAMMA)
  9.1      Extended star catalog (400 stars) — delivered (54 tests, ALPHA)
  12.1     Catalog ingestion — delivered (60 tests, ALPHA)
  25.1     Daylight worldwide — delivered (BETA)
  25.2     Seasons — delivered (GAMMA)
  26.3     Number scanner — delivered (GAMMA)
  26.4     Precession detect — delivered (GAMMA)
  27.1     Frequency mapper — delivered (GAMMA)
  27.3     Phase space — delivered (59 tests, ALPHA)
  28.1     Knowledge graph — delivered (GAMMA)
  30.1     Text render — delivered (93 tests, ALPHA)
  31.1     Tzolkin board — delivered (BETA)
  31.2     Calendar Round — delivered (33 tests, BETA)
  21.3     Tide predict — delivered (41 tests, BETA)
  29.3     Earth drama — delivered (62 tests, BETA)
  29.5     Calendar politics — delivered (GAMMA)
  29.6     Calendar epoch — delivered (59 tests, BETA)
  41.2     Megalithic alignments — delivered (45 tests, ALPHA)
  43.2     Decan star data — delivered (ALPHA)
  44.2     Tarot visual data — delivered (ALPHA)
  7.7a     Star field GPU pack — delivered (53 tests, ALPHA)

AGENTS NEXT ROUND (PRIORITY ORDER — 10+ tasks per agent):
  ALPHA (geometry/render — VERTEX PACKING PIPELINE, 12 tasks):
    DELIVERED: 3.9 zodiac_pack, 8.6 planet_pack, 11.1 hexagram_geometry,
              4.13 card_pack, 8.7 moon_pack, 7.7a star_field
    IN PROGRESS: 9.5 Star Diffraction Geometry
    REMAINING:
    10.2  Deep Sky Render Pack — nebula/cluster billboards from deep_sky catalog
    11.4  Bodygraph Render Pack — Human Design 9 centers + 36 channels as vertex arrays
    7.10  Milky Way Render Pack — galaxy band geometry from galaxy_geometry
    24.3  Earth Render Pack — globe mesh + coastlines + terminator as vertex arrays
    NEW:
    18.4  Kabbalah Tree Render Pack — Tree of Life 10 nodes + 22 paths as vertex arrays
    7.11  Constellation Boundary Pack — 88 IAU constellation polygon outlines
    9.6   Eclipse Geometry — solar/lunar eclipse umbra/penumbra geometry
    8.8   Ring Geometry Pack — Saturn rings + zodiac rings as reusable ring mesh
    24.5  Horizon Line Geometry — great circle at observer location for Earth View
    30.3  HUD Layout Pack — screen-space overlay quads for all HUD elements
    22.4  Weather Overlay Geometry — wind arrows + pressure contours as vertex data
    25.3  Seasonal Lighting Pack — sky gradient color ramps per season/latitude
  BETA (data/knowledge — UI + experience + earth social, 14 remaining):
    DELIVERED: 30.2, 31.3, 32.1, 32.2, 33.1, 45.1, 46.1-46.3, 39.1, 39.2,
              40.1, 40.2, 41.1, 42.1
    IN PROGRESS: 35.1 Cultural Story Database
    REMAINING:
    19.3  Structural Pattern Map
    33.2  Location-Aware Alerts
    33.3  User Context Tracker
    34.2  Kin Social Compatibility
    35.2  Philosophy of Time Database
    36.2  Birth Profile Computer
    47.1  Interest Profile Data (Earth social foundation)
    47.2  Privacy Framework (data classification + consent)
    47.3  User Preferences Store (serialization + defaults)
    48.1  Local Event Data (event struct + astronomical auto-gen)
    48.2  Event Filter Engine (multi-criteria + presets)
    48.3  Neighbor Roulette Data (matching + anonymization)
    49.1  Personal Space Data (tracked cycles + cosmic fingerprint)
    49.2  Content Curator (relevance scoring + "Today for you")
  GAMMA (sacred calendars + cross-system, 12 tasks):
    DELIVERED: 42.2, 43.1, + 5 book extractions
    IN PROGRESS: 44.1 Tarot-Calendar Mapping
    REMAINING:
    NEW TRACKS (world calendars not yet covered):
    50.1  Persian Calendar — Solar Hijri (Jalali), spring equinox epoch, 100M+ users
    50.2  Baha'i Calendar — Badí' 19x19 + intercalary days, Naw-Ruz epoch
    51.1  Japanese Calendar — imperial era system (Reiwa/Heisei), rokuyō divination days
    51.2  Aztec Calendar — xiuhpohualli (365) + tonalpohualli (260), New Fire ceremony
    52.1  French Republican Calendar — 12x30 + complementary days, decimal time
    52.2  Tamil Sidereal Calendar — solar sidereal months, 60-year Jovian cycle
    53.1  Celtic Tree Calendar — 13 lunar months, Ogham tree correspondences
    53.2  Korean Traditional Calendar — Dangun epoch, sexagenary cycle adaptation
    54.1  Cross-System Festival Detector — given JD + location, all active festivals
    54.2  Lunar Phase Harmonics — precise phase for all lunar calendars
    54.3  Calendar Conversion Engine — convert any date between all 20+ calendar systems
    + book extraction continues between code tasks

ORCHESTRATOR (MEGA wires):
  7.7-7.9    Star shaders + constellation overlay + labels
  8.3, 8.5   Planet rendering + moon rendering
  11.3       Tzolkin grid visual (expanded by Track 31 board data)
  21.4-25.3  Earth view wiring
  31.4       Tzolkin board interactive display
  32.3       Wisdom overlay display
  33.4       Fun facts display
  34.3       Community/Kin discovery display
  39.3       Zoroastrian display
  40.3       Coptic & Ethiopian display
  41.3       Celtic Wheel display + megalithic alerts
  42.3       Southeast Asian display
  43.3       Egyptian display + Sothic cycle + decan clock
  44.3       Tarot display + Tree path overlay
  45.2-45.3  Audio engine + meditation mode
  46.4       High contrast mode + keyboard nav
  47.4       User identity (localStorage + onboarding + profile editor)
  48.4       Extrovert layer display (event feed + map pins + roulette card)
  49.3       Personal space display (dashboard + curated feed + learning map)

LONG-TERM:
  4.1-4.6    Card renderer + knowledge cards
  5.1-5.4    User interaction
  6.1-6.4    Infrastructure hardening
  13.3, 20.3 Deep time + sacred system orchestrator items
  26.5       Correlation dashboard
  27.5       Harmonic display
  28.2       Knowledge graph display
  29.4       Dramatization display
  30.2-30.4  Presentation quality (scroll layers, rendering, interaction)
  34.1, 34.3 Community backend + display (requires server decision)
  35.3       Cultural story display
  36.1       Welcome journey (onboarding flow)
  36.3       Personal 3D space (diamond lego room — artistic vision)

DELTA (research mining — permanent):
  Systematic extraction from 187-book library
  See .context/research/MINING_QUEUE.md for priority queue
  Produces digests that unblock agent coding tasks

OMEGA (codebase health — permanent, rotating):
  1. Git health (fsck, tracked file audit, conflict markers)
  2. Build system sync (CMakeLists.txt + Makefile vs actual files)
  3. Purity audit (P1-P5 on all pure modules)
  4. Refactor scan (God Files, God Functions — report to MEGA)
  5. Style compliance (no hardcoded visual constants)
  6. Dead code scan (unused functions, orphan files)
  7. Metrics update (METRICS.md, STATE.md)
  8. System diagram coverage (undocumented domains)
  9. Attribution audit (contributors.json completeness)
  10. Test quality (modules below 18-test minimum)
  See .context/execute/OMEGA/priority.md for full spec

ONGOING (AGENT SELF-MAINTAINED + OMEGA OVERSEES):
  Writers (ALPHA-GAMMA) Phase 5 Maintainer handles per-task:
  - CMakeLists.txt + Makefile registration for new modules
  - Purity audit on created files (P1-P5)
  - Test count + regression verification
  - Attribution in data/contributors.json
  OMEGA handles project-wide health (rotating checklist)
  MEGA reviews reports, does NOT run operational commands
```

---

## TRACK 38: Knowledge Mining Pipeline — Systematic Book Extraction

**The library is useless if unread.** 187 books acquired, only 15 digested. This track creates a permanent extraction pipeline.

### 38.1 Mining Queue (MEGA) — COMPLETE
- [x] `.context/research/MINING_QUEUE.md` — prioritized extraction queue
- [x] P0-P3 priority levels, extraction output format defined
- [x] Integrated into orchestrator workflow

### 38.2 DELTA Orchestrator Slot — Research Mining
- [ ] Define DELTA domain in BOOTSTRAP.md: "Research extraction only"
- [ ] DELTA loop: pick top unextracted book → survey → extract per chapter → digest → update manifest → update contributors → loop
- [ ] DELTA does NOT write C code — only produces digests + attribution
- [ ] DELTA reports: extracted algorithms, data tables, formulas, cultural context
- [ ] Exit when: mining queue empty or 3 books fully extracted per session

### 38.3 Agent Research Integration
- [ ] Orchestrator Phase 1 (EVALUATE): check manifest.json for relevant books before coding
- [ ] If digest exists → agent reads it during Phase 3 (EXECUTE)
- [ ] If PDF exists but no digest → agent extracts before coding (or escalates to DELTA)
- [ ] If no PDF → agent writes Knowledge Gap to report.md → MEGA adds to books.md

### 38.4 Mining Metrics
- [ ] Track: books acquired vs books digested vs algorithms extracted vs code modules produced
- [ ] Goal: 100% of P0-P1 books digested before their dependent tracks are attempted

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
| 3.8 Glyph batch | glyph_batch (Agent A) | DELIVERED — 33 tests |
| 3.9 Zodiac render pack | zodiac_pack (ALPHA) | NEEDED — depends on ring/arc/cusp/aspect geometry |
| 4.10 Tzolkin card data | tzolkin_card (Agent B) | DELIVERED (BETA) |
| 4.11 Astrology formatter | astro_summary (Agent B) | DELIVERED (GAMMA) |
| 4.12 Haab calendar | haab (Agent B) | DELIVERED (BETA) |
| 7.5 Star catalog | star_catalog (Agent A) | DELIVERED (ALPHA — 195 stars) |
| 7.6 Constellation lines | constellation (Agent A) | DELIVERED (ALPHA — 40 constellations) |
| 7.7a Star field pack | star_field (ALPHA) | DELIVERED — 53 tests |
| 7.7b Star rendering | star_field (7.7a) | UNBLOCKED — orchestrator |
| 7.8 Constellation overlay | star_field (7.7a) | UNBLOCKED — orchestrator |
| 7.9 Star labels | star_catalog + glyph billboards | UNBLOCKED — orchestrator |
| 8.1 Planet data | planet_data (Agent B) | DELIVERED (GAMMA — 39 tests) |
| 8.2 Atmosphere shader data | atmo_ring (Agent A) | DELIVERED — 42 tests |
| 8.4 Moon data | moon_data (Agent B) | DELIVERED — 33 tests |
| 8.6 Planet render pack | planet_pack (ALPHA) | NEEDED — depends on planet_data |
| 9.1 Extended star catalog | star_catalog_extended (ALPHA) | DELIVERED — 54 tests |
| 9.2 Spectral colors | star_colors (ALPHA) | DELIVERED — 34 tests |
| 10.1 Deep sky catalog | deep_sky (BETA) | DELIVERED — 35 tests |
| 11.1 Hexagram geometry | hexagram_geometry (ALPHA) | NEEDED |
| 11.2 Bodygraph data | bodygraph (ALPHA) | DELIVERED — 44 tests |
| 12.1 Catalog ingestion | catalog_ingest (ALPHA) | DELIVERED — 60 tests |
| 13.1 Geological time | geo_time (BETA) | DELIVERED — 46 tests |
| 13.2 Radiometric dating | radiometric (Agent A) | DELIVERED — 61 tests |
| 14.1 Hebrew calendar | hebrew (GAMMA) | DELIVERED — 49 tests |
| 14.2 Sabbatical cycles | sabbatical (BETA) | DELIVERED — 38 tests |
| 15.1 Hijri calendar | hijri (GAMMA) | DELIVERED — 51 tests |
| 15.2 Prayer times | prayer_times (GAMMA) | DELIVERED — 38 tests |
| 16.1 Buddhist calendar | buddhist (BETA) | DELIVERED — 45 tests |
| 16.2 Kalpa cycles | kalpa (BETA) | DELIVERED — 49 tests |
| 17.1 Panchanga | panchanga (BETA) | DELIVERED — 42 tests |
| 17.2 Nakshatra positions | nakshatra (ALPHA) | DELIVERED — 53 tests |
| 17.3 Yuga cycles | yuga (GAMMA) | DELIVERED — 34 tests |
| 18.1 Sefirot data | sefirot (GAMMA) | DELIVERED — 65 tests |
| 18.2 Tree geometry | tree_geometry (ALPHA) | DELIVERED — 63 tests |
| 18.3 Four Worlds | four_worlds (GAMMA) | DELIVERED — 64 tests |
| 19.1 Convergence detector | convergence (Agent B) | NEEDED — depends on all calendars |
| 19.2 Cycle analysis | cycle_analysis (ALPHA) | DELIVERED — 54 tests |
| 19.3 Structural patterns | structural_map (Agent B) | NEEDED |
| 20.1 Thermochronology | thermo (ALPHA) | DELIVERED — 67 tests |
| 20.2 Tectonic timeline | tectonic (ALPHA) | DELIVERED — 46 tests |
| 20.4 Climate history | climate_history (GAMMA) | DELIVERED — 34 tests |
| 20.5 Fossil milestones | fossil_milestones (GAMMA) | DELIVERED — 39 tests |
| 21.1 Tidal harmonics | tidal (ALPHA) | DELIVERED — 63 tests |
| 21.2 Surf spots | surf_spots (GAMMA) | DELIVERED — 65 tests |
| 21.3 Tide prediction | tide_predict (BETA) | DELIVERED — 41 tests |
| 22.1 Solar radiation | solar_radiation (ALPHA) | DELIVERED — 40 tests |
| 22.2 Wind patterns | wind_patterns (Agent A) | DELIVERED — 43 tests |
| 22.3 Storm data | storm_data (GAMMA) | DELIVERED — 68 tests |
| 23.1 Snow model | snow_season (GAMMA) | DELIVERED — 36 tests |
| 23.2 Ski resorts | ski_resorts (GAMMA) | DELIVERED — 47 tests |
| 24.1 Earth globe | earth_globe (ALPHA) | DELIVERED — 38 tests |
| 24.2 Atmosphere render | earth_atmosphere (ALPHA) | DELIVERED — 45 tests |
| 24.3 Earth render pack | earth_pack (ALPHA) | DELIVERED — 48 tests |
| 25.1 Daylight worldwide | daylight (BETA) | DELIVERED |
| 25.2 Seasonal cycles | seasons (GAMMA) | DELIVERED |
| 29.1 Achievements | achievement (GAMMA) | DELIVERED — 62 tests |
| 29.2 Calendar reform | calendar_reform (GAMMA) | DELIVERED — 66 tests |
| 29.3 Earth drama | earth_drama (BETA) | DELIVERED — 62 tests |
| 29.5 Patriarchal decision | calendar_politics (GAMMA) | DELIVERED |
| 29.6 Jesus & calendar | calendar_epoch (BETA) | DELIVERED — 59 tests |
| 30.3a Post-process pipeline | post_process (ALPHA) | DELIVERED |
| 31.1 Tzolkin board | tzolkin_board (BETA) | DELIVERED |
| 31.2 Calendar Round | calendar_round (BETA) | DELIVERED — 33 tests |
| 31.3 Dreamspell oracle | dreamspell (Agent B) | NEEDED |
| 32.1 Wisdom quotes | wisdom (Agent B) | DELIVERED |
| 32.2 Wisdom engine | wisdom_engine (Agent B) | NEEDED |
| 33.1 Fun facts | fun_facts (Agent B) | DELIVERED |
| 33.2 Location alerts | location_alerts (BETA) | DELIVERED |
| 33.3 User context | user_context (Agent B) | NEEDED |
| 34.2 Kin social | kin_social (BETA) | DELIVERED |
| 7.11 Constellation bounds | constellation_bounds (ALPHA) | DELIVERED — 65 tests |
| 8.8 Saturn ring pack | saturn_ring_pack (ALPHA) | DELIVERED — 61 tests |
| 51.2 Aztec calendar | aztec (GAMMA) | DELIVERED |
| 52.1 French Republican | french_republican (GAMMA) | DELIVERED — 116 tests |
| 52.2 Tamil calendar | tamil_calendar (GAMMA) | DELIVERED — 95 tests |
| 2.4 Scale transitions | easing | DELIVERED |
| 7.2 Milky Way spiral | sacred_geometry | DELIVERED |
| 7.3 Cosmic time | cosmic_time | DELIVERED |
| 39.1 Zoroastrian calendar | zoroastrian (GAMMA) | NEEDED |
| 39.2 Cosmic duality | cosmic_duality (GAMMA) | NEEDED — depends on 39.1 |
| 40.1 Coptic calendar | coptic (GAMMA) | NEEDED |
| 40.2 Ethiopian calendar | ethiopian (GAMMA) | NEEDED |
| 41.1 Wheel of the Year | wheel_of_year (GAMMA) | NEEDED — depends on solar position |
| 41.2 Megalithic alignments | megalithic (ALPHA) | DELIVERED — 45 tests |
| 42.1 Myanmar calendar | myanmar (GAMMA) | NEEDED — has algorithm document |
| 42.2 Thai solar calendar | thai_calendar (GAMMA) | NEEDED — depends on 16.1 Buddhist |
| 43.1 Egyptian calendar | egyptian (GAMMA) | NEEDED |
| 43.2 Decan star data | decan_stars (ALPHA) | DELIVERED |
| 44.1 Tarot-calendar map | tarot (GAMMA) | NEEDED — depends on 18.1 Sefirot |
| 44.2 Tarot visual data | tarot_visual (ALPHA) | DELIVERED |
| 45.1 Web Audio data | audio_data (BETA) | NEEDED — depends on 27.1 frequency mapper |
| 46.1 Screen reader data | accessibility (BETA) | NEEDED |
| 46.2 RTL layout | rtl_layout (BETA) | NEEDED |
| 46.3 Translation keys | i18n (BETA) | NEEDED |
| 47.1 Interest profile | interest_profile (BETA) | NEEDED |
| 47.2 Privacy framework | privacy_framework (BETA) | NEEDED |
| 47.3 User preferences | user_prefs (BETA) | NEEDED — depends on 47.1 |
| 48.1 Local event data | local_events (BETA) | NEEDED — depends on 47.1 |
| 48.2 Event filter | event_filter (BETA) | NEEDED — depends on 47.1, 48.1 |
| 48.3 Neighbor roulette | neighbor_roulette (BETA) | NEEDED — depends on 47.1, 47.2 |
| 49.1 Personal space | personal_space (BETA) | NEEDED — depends on 47.1, 47.3 |
| 49.2 Content curator | content_curator (BETA) | NEEDED — depends on 47.1, 48.1, 48.2 |
| 3.9 Zodiac render pack | zodiac_pack (ALPHA) | NEEDED — depends on ring_geometry, arc_geometry, cusp_lines, aspect_lines |
| 8.6 Planet render pack | planet_pack (ALPHA) | NEEDED — depends on planet_data, planetary_pos, atmo_ring |
| 11.1 Hexagram geometry | hexagram_geometry (ALPHA) | NEEDED — depends on iching |
| 24.3 Earth render pack | earth_pack (ALPHA) | NEEDED — depends on earth_globe, earth_atmosphere |

---

## TRACK 47: Earth Social Layer — User Identity & Privacy

Time's Earth View gains a social dimension. Visitors can define who they are, what interests them, and how much they share. This is the FOUNDATION layer — Tracks 48 and 49 build on top of it. **Privacy is non-negotiable.** We collect only what users explicitly provide, store locally first, sync optionally, and NEVER sell or leak. The user owns their data.

### 47.1 Agent: Interest Profile Data (BETA)
- [ ] Pure module: `src/systems/earth/interest_profile.h/.c`
- [ ] Interest taxonomy: nature (hiking, surfing, stargazing), culture (art, music, food), social (meetups, sports, games), spiritual (meditation, ceremony, pilgrimage), knowledge (lectures, workshops, museums)
- [ ] Profile struct: interests[] (weighted 0.0-1.0), location_radius_km, activity_level (introvert/extrovert slider), visibility (anonymous/pseudonym/public)
- [ ] Interest matching score: compare two profiles, return compatibility 0.0-1.0
- [ ] `interest_profile_create()`, `interest_match_score()`, `interest_filter_events()`
- [ ] Interest categories map to existing Time systems: surfing → tides (Track 21), stargazing → star field (Track 7), meditation → audio (Track 45), hiking → weather (Track 22)

### 47.2 Agent: Privacy Framework (BETA)
- [ ] Pure module: `src/systems/earth/privacy_framework.h/.c`
- [ ] Data classification: PUBLIC (timezone, language), PRIVATE (interests, activity), SENSITIVE (location, profile), FORBIDDEN (real name unless volunteered, contacts, browsing)
- [ ] Consent levels: NONE (anonymous visitor), BASIC (timezone + interests), FULL (location + profile + social)
- [ ] Data retention policy: session-only by default, opt-in local storage, opt-in cloud sync
- [ ] Privacy score: quantify how much a user has shared (0-100) — displayed to the user
- [ ] Data export: user can download everything we store about them (GDPR-style)
- [ ] Data deletion: user can nuke their profile at any time, no questions
- [ ] `privacy_classify()`, `privacy_consent_level()`, `privacy_score()`, `privacy_can_access()`
- [ ] Principles: no tracking pixels, no third-party analytics, no ad networks, no data sales. EVER.

### 47.3 Agent: User Preferences Store (BETA)
- [ ] Pure module: `src/systems/earth/user_prefs.h/.c`
- [ ] Preference categories: visual (theme, layers visible, default view), temporal (home timezone, preferred calendar systems), social (extrovert/introvert slider, notification frequency, roulette opt-in), location (home coordinates, exploration radius)
- [ ] Preference serialization: struct to JSON for localStorage
- [ ] Preference defaults: sensible defaults for first-time visitors (anonymous, introvert, local timezone)
- [ ] `user_prefs_default()`, `user_prefs_serialize()`, `user_prefs_deserialize()`, `user_prefs_merge()`

### 47.4 User Identity — Orchestrator
- [ ] LocalStorage integration via Emscripten JS interop
- [ ] First-visit flow: gentle onboarding ("What fascinates you?") — not a signup wall
- [ ] Profile editor: toggle interests, set radius, adjust introvert/extrovert slider
- [ ] Privacy dashboard: see what's stored, download, delete
- [ ] Key: Shift+P for profile/preferences panel

---

## TRACK 48: Earth Extrovert Layer — Local Events & Social Discovery

The EXTROVERT sublayer of Earth View. When a visitor opts in, they can see what's happening around them — trail meetups, restaurant openings, poker nights, art shows, astronomical events, surf contests. All filtered by their interests. They never see what they don't want. This layer also includes "meet a neighbor" — a roulette that shows a random nearby visitor's profile (anonymous by default). **This is NOT a social network.** It's a window into the living world around you, filtered through the lens of Time's knowledge systems.

### 48.1 Agent: Local Event Data (BETA)
- [ ] Pure module: `src/systems/earth/local_events.h/.c`
- [ ] Event struct: type, title, description, lat/lon, radius_km, start_jd, end_jd, interest_tags[], source
- [ ] Event types: NATURE (hike, surf, stargaze), CULTURE (show, exhibit, food), SOCIAL (meetup, game, sport), ALERT (accident, weather, closure), ASTRONOMICAL (eclipse, conjunction, meteor shower from existing data)
- [ ] Event filtering: by distance, by interest match, by time window, by type
- [ ] Event scoring: relevance = interest_match * distance_decay * recency
- [ ] Astronomical events auto-generated from existing modules: conjunctions (astrology), eclipses (astronomy), meteor showers (star catalog), solstices (seasons)
- [ ] `event_create()`, `event_filter()`, `event_score()`, `event_sort_by_relevance()`
- [ ] NOTE: External event data (restaurants, meetups) requires future API integration — for now, astronomical events are auto-generated, others are placeholder structures

### 48.2 Agent: Event Filter Engine (BETA)
- [ ] Pure module: `src/systems/earth/event_filter.h/.c`
- [ ] Multi-criteria filtering: interest tags (weighted AND/OR), distance (linear decay or hard cutoff), time window (next hour / today / this week / this month), event type whitelist/blacklist
- [ ] Smart defaults: "Show me what I'd care about within 50km this week"
- [ ] Filter presets: "Nature lover", "Night owl astronomer", "Cultural explorer", "Social butterfly"
- [ ] Anti-spam: rate-limit how many events surface per time period
- [ ] `filter_create()`, `filter_apply()`, `filter_preset()`, `filter_count()`

### 48.3 Agent: Neighbor Roulette Data (BETA)
- [ ] Pure module: `src/systems/earth/neighbor_roulette.h/.c`
- [ ] Profile card struct: pseudonym (auto-generated from Tzolkin kin or zodiac sign), interests (top 3), kin (Dreamspell day sign), zodiac sun sign, activity level, distance_km (fuzzy — "nearby" / "in your city" / "in your region")
- [ ] Matching algorithm: weighted interest overlap + complementary interests
- [ ] Privacy-first: NEVER reveal exact location. Minimum granularity = city/region. Pseudonym default.
- [ ] Opt-in only: user must explicitly enable roulette. Off by default.
- [ ] Cooldown: one spin per day (or configurable). No obsessive swiping.
- [ ] `roulette_spin()`, `roulette_match_score()`, `roulette_anonymize_profile()`

### 48.4 Extrovert Layer Display — Orchestrator
- [ ] Event feed: scrollable list overlaid on Earth View, filtered by interests
- [ ] Event map pins: geolocated events visible on the globe at Earth View zoom level
- [ ] Astronomical event highlights: auto-generated from existing systems (eclipses, conjunctions glow on globe)
- [ ] Neighbor roulette card: single anonymous profile card with spin animation
- [ ] Notification badges: subtle pulse on Earth View when relevant events are near
- [ ] Toggle: Shift+E for extrovert layer on/off. Sublayer controls in profile panel.

---

## TRACK 49: Earth Introvert Layer — Personal Space & Content Curation

The INTROVERT sublayer of Earth View. This is the private, inward-facing dimension — your "lego room" in Time. It reflects YOUR relationship with time: which systems resonate, which cycles you track, what you're learning. No social pressure. No feeds. Just you and the cosmos. The introvert layer receives information FROM the user to BUILD their personalized experience — Time learns what matters to you and curates accordingly.

### 49.1 Agent: Personal Space Data (BETA)
- [ ] Pure module: `src/systems/earth/personal_space.h/.c`
- [ ] Personal space struct: favorite_systems[] (which calendar/time systems the user tracks), tracked_cycles[] (specific cycles they watch — e.g., "my Saturn return", "next solar eclipse at my location"), saved_moments[] (bookmarked dates with personal notes), learning_path[] (which knowledge cards they've explored)
- [ ] Character archetype: auto-computed from birth data (Dreamspell kin, natal chart, hexagram) — the "cosmic fingerprint"
- [ ] Space theme: visual customization of their personal view (colors, background, arrangement)
- [ ] `personal_space_create()`, `personal_space_add_cycle()`, `personal_space_bookmark()`, `personal_space_archetype()`

### 49.2 Agent: Content Curator (BETA)
- [ ] Pure module: `src/systems/earth/content_curator.h/.c`
- [ ] Curation algorithm: surfaces wisdom quotes (Track 32), fun facts (Track 33), cultural stories (Track 35), and knowledge cards (Track 4) based on user profile + current moment
- [ ] Relevance scoring: system_affinity * temporal_relevance * novelty (don't repeat)
- [ ] Learning progression: track which systems the user has explored, suggest deeper dives
- [ ] "Today for you" digest: curated selection of 3-5 insights personalized to this user, this moment, this location
- [ ] `curator_score()`, `curator_suggest()`, `curator_digest()`, `curator_track_exploration()`

### 49.3 Personal Space Display — Orchestrator
- [ ] Personal dashboard: "My Time" view showing tracked cycles, upcoming personal events, cosmic fingerprint
- [ ] Saved moments timeline: visual timeline of bookmarked dates with notes
- [ ] Curated feed: "Today for you" — 3-5 personalized insights, not a stream
- [ ] Learning map: visual map of which knowledge systems you've explored (glowing nodes = visited, dim = unexplored)
- [ ] 3D personal space: the "lego room" — a customizable 3D environment that reflects your interests (long-term artistic vision, not MVP)
- [ ] Key: Shift+I for introvert layer toggle. Shift+T for "Today for you" digest.

---

## TRACK 50: Persian & Baha'i Calendars

### 50.1 Agent: Persian Calendar (GAMMA) — DELIVERED
- [x] Pure module: `src/systems/persian/persian.h/.c`
- [x] Solar Hijri (Jalali) calendar: spring equinox epoch (Nowruz), 31/30/29-day months
- [x] Algorithmic equinox computation (Borkowski algorithm or astronomical)
- [x] Month names (Farvardin-Esfand), weekday names, leap year rules
- [x] Nowruz + Yalda Night + Chaharshanbe Suri festival detection
- [x] `persian_from_jd()`, `persian_to_jd()`, `persian_month_name()`, `persian_is_nowruz()`

### 50.2 Agent: Baha'i Calendar (GAMMA) — DELIVERED (GAMMA: bahai, 111 tests)
- [x] Pure module: `src/systems/bahai/bahai.h/.c`
- [x] Badí' calendar: 19 months × 19 days + Ayyám-i-Há intercalary period
- [x] Naw-Rúz (spring equinox) epoch, twin calendar reform (2015 sunset-based)
- [x] Kull-i-Shay' (361-year) and Váhid (19-year) cycles
- [x] Holy day detection (Ridván, Naw-Rúz, etc.)
- [x] `bahai_from_jd()`, `bahai_to_jd()`, `bahai_month_name()`, `bahai_holy_day()`

---

## TRACK 51: Japanese & Aztec Calendars

### 51.1 Agent: Japanese Calendar (GAMMA)
- [ ] Pure module: `src/systems/japanese/japanese.h/.c`
- [ ] Imperial era system: Meiji(1868), Taishō(1912), Shōwa(1926), Heisei(1989), Reiwa(2019)
- [ ] Rokuyō (六曜) divination days: Sensho, Tomobiki, Senbu, Butsumetsu, Taian, Shakko
- [ ] Traditional 24 seasonal divisions (sekki) from solar longitude
- [ ] Zodiac year animals (same 12 as Chinese but Japanese pronunciation)
- [ ] `japanese_era()`, `japanese_rokuyou()`, `japanese_sekki()`, `japanese_zodiac_year()`

### 51.2 Agent: Aztec Calendar (GAMMA)
- [ ] Pure module: `src/systems/aztec/aztec.h/.c`
- [ ] Tonalpohualli (260-day sacred): 20 day signs × 13 numbers (parallel to Tzolkin)
- [ ] Xiuhpohualli (365-day solar): 18 months × 20 days + 5 nemontemi
- [ ] Calendar Round: 52-year cycle (same structure as Mayan but Aztec names)
- [ ] New Fire ceremony (Xiuhmolpilli) every 52 years
- [ ] `aztec_tonalpohualli()`, `aztec_xiuhpohualli()`, `aztec_calendar_round()`, `aztec_new_fire()`

---

## TRACK 52: French Republican & Tamil Calendars

### 52.1 Agent: French Republican Calendar (GAMMA)
- [ ] Pure module: `src/systems/french_republican/french_republican.h/.c`
- [ ] 12 months × 30 days + 5-6 complementary days (jours complémentaires)
- [ ] Month names (Vendémiaire-Fructidor), day names (natural objects, not saints)
- [ ] Decimal time: 10 hours × 100 minutes × 100 seconds
- [ ] Epoch: September 22, 1792 (autumn equinox, French Republic founding)
- [ ] `fr_from_jd()`, `fr_to_jd()`, `fr_decimal_time()`, `fr_month_name()`, `fr_day_name()`

### 52.2 Agent: Tamil Sidereal Calendar (GAMMA)
- [ ] Pure module: `src/systems/tamil/tamil_calendar.h/.c`
- [ ] Solar sidereal months (Chittirai-Panguni), 60-year Jovian cycle (Prabhava-Akshaya)
- [ ] Tamil New Year (Puthandu, mid-April when Sun enters Aries sidereal)
- [ ] Panchangam integration (Tamil version of Panchanga)
- [ ] Festival detection: Pongal, Deepavali, Tamil New Year
- [ ] `tamil_from_jd()`, `tamil_to_jd()`, `tamil_jovian_year()`, `tamil_festival()`

---

## TRACK 53: Celtic & Korean Calendars

### 53.1 Agent: Celtic Tree Calendar (GAMMA)
- [ ] Pure module: `src/systems/celtic/celtic_tree.h/.c`
- [ ] 13 lunar months mapped to Ogham trees (Birch, Rowan, Ash... Elder)
- [ ] Each month = ~28 days, + 1 intercalary day (23 Dec, "Day of the Yew")
- [ ] Tree-zodiac correspondences (Robert Graves "White Goddess" system)
- [ ] Cross-reference with wheel_of_year festivals
- [ ] `celtic_tree_month()`, `celtic_ogham_name()`, `celtic_tree_meaning()`, `celtic_lunar_phase()`

### 53.2 Agent: Korean Traditional Calendar (GAMMA)
- [ ] Pure module: `src/systems/korean/korean_calendar.h/.c`
- [ ] Dangun epoch (2333 BCE = year 1), sexagenary cycle (Heavenly Stems + Earthly Branches)
- [ ] Korean month names, Chuseok + Seollal festival detection
- [ ] Adaptation of Chinese lunisolar calendar with Korean terminology
- [ ] `korean_from_jd()`, `korean_to_jd()`, `korean_dangun_year()`, `korean_festival()`

---

## TRACK 54: Cross-System Utilities

### 54.1 Agent: Cross-System Festival Detector (GAMMA)
- [ ] Pure module: `src/systems/unified/festival_detector.h/.c`
- [ ] Given JD + observer location, return ALL active festivals across ALL calendar systems
- [ ] Festival priority: major (national/religious holiday) > minor (cultural event) > astronomical (solstice, eclipse)
- [ ] Output: array of festival_t structs with system, name, significance, start/end JD
- [ ] Depends on: all calendar modules' festival detection functions
- [ ] `festival_detect_all()`, `festival_filter_by_system()`, `festival_count()`

### 54.2 Agent: Lunar Phase Harmonics (GAMMA)
- [ ] Pure module: `src/systems/unified/lunar_harmonics.h/.c`
- [ ] Precise lunar phase computation (synodic month = 29.530588853 days)
- [ ] Phase names: new, waxing crescent, first quarter, waxing gibbous, full, waning gibbous, last quarter, waning crescent
- [ ] Integration with all lunar calendars: Hebrew month starts, Islamic month starts, Chinese month starts, Buddhist Uposatha
- [ ] Lunar ecliptic longitude for nakshatra/mansion calculations
- [ ] `lunar_phase()`, `lunar_illumination()`, `lunar_age()`, `lunar_next_phase()`

### 54.3 Agent: Calendar Conversion Engine (GAMMA)
- [ ] Pure module: `src/systems/unified/calendar_convert.h/.c`
- [ ] Convert any date between ALL 20+ calendar systems via Julian Day as pivot
- [ ] Calendar enum for all supported systems
- [ ] Formatted output string per calendar system
- [ ] `cal_convert(from_system, from_date, to_system)`, `cal_format()`, `cal_system_count()`
- [ ] Depends on: all calendar *_from_jd and *_to_jd functions

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
| Sacred Calendars | Hebrew + Islamic + Buddhist + Hindu compute correctly for any date |
| Kabbalah VISUAL | Tree of Life renders with planetary activations |
| Deep Time | Geological timeline + Yuga + Kalpa display at cosmic scale |
| Unity View | Cross-system convergence detection works for 10+ systems |
| Tzolkin DEEP | Full 260-day board + 52-year flower + Dreamspell oracle interactive |
| Wisdom ALIVE | Context-sensitive quotes from 200+ great minds across civilizations |
| Fun Facts | Cross-system insights + location-based alerts triggered by algorithms |
| Community MVP | Kin discovery: see other users' kin nearby (opt-in, anonymous) |
| World Calendars | ALL 20+ calendar systems compute correctly for any date |
| Celtic Wheel | 8 festivals with solar-computed dates + megalithic alignment alerts |
| Tarot-Time | Current Major Arcanum + decan card displayed, mapped to Tree of Life |
| Music of Spheres | Planetary chord audible in real-time, aspect sonification working |
| Accessible | Screen reader labels, RTL support, translation keys, high contrast mode |
| User Identity | Interest profile + privacy framework + preferences stored locally |
| Extrovert Layer | Event feed filtered by interests, astronomical events auto-generated, roulette opt-in |
| Introvert Layer | Personal space with tracked cycles, cosmic fingerprint, curated "Today for you" digest |
| Privacy SOLID | No tracking, no third-party analytics, user can export/delete all data at any time |
| Phase 8 DEPLOYED | Live on the web |

---

**This document is the primary orchestrator's work plan. It is NOT the project roadmap (that's ROADMAP.md). It is what I work on, in what order, while agents handle the module pipeline.**
