# Time — Project Metrics

**Last refreshed**: 2026-03-16 (OMEGA sweep #19)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 241 |
| Header files (.h) | 241 |
| Lines of code (src/) | 77,014 |
| Lines of tests | 112,199 |
| Test files | 230 |
| Render pass files | 16 |
| Contributors | 189 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 233 |
| Test functions (RUN_TEST) | 10,775 |
| Test assertions (TEST_ASSERT) | 17,420 |
| Failures | 0 |
| CTest time | 0.88s |
| ASan/UBSan | PASS (233/233, 0 findings) |

## Code Coverage (baseline — 2026-03-16)

| Metric | Value |
|--------|-------|
| **Overall line coverage** | **96.5%** (18,325 / 18,986 lines) |
| **Overall function coverage** | **100.0%** (2,364 / 2,364 functions) |

### Foundational Math Modules

| Module | Coverage | Lines |
|--------|----------|-------|
| julian.c | 100.0% | 40/40 |
| kepler.c | 100.0% | 19/19 |
| mat4.c | 100.0% | 87/87 |
| vec3.c | 100.0% | 22/22 |
| sidereal.c | 100.0% | 49/49 |
| ecliptic.c | 100.0% | 21/21 |
| arc_geometry.c | 100.0% | 104/104 |
| wheel_layout.c | 100.0% | 43/43 |
| color.c | 97.0% | 96/99 |
| projection.c | 95.6% | 109/114 |
| easing.c | 93.6% | 73/78 |
| sacred_geometry.c | 91.1% | 82/90 |
| bezier.c | 83.2% | 89/107 |

### Bottom 10 Modules (blind spots)

| Module | Coverage | Lines | Notes |
|--------|----------|-------|-------|
| bezier.c | 83.2% | 89/107 | Uncovered: edge cases, degenerate inputs |
| planetary_hours.c | 84.6% | 22/26 | 4 uncovered lines |
| tzolkin_board.c | 87.0% | 60/69 | 9 uncovered lines |
| megalithic.c | 87.0% | 87/100 | 13 uncovered lines |
| numerology.c | 87.3% | 69/79 | 10 uncovered lines |
| rtl_layout.c | 88.0% | 132/150 | 18 uncovered lines |
| fun_facts.c | 88.5% | 100/113 | 13 uncovered lines |
| star_catalog.c | 88.5% | 54/61 | 7 uncovered lines |
| earth_atmosphere.c | 88.7% | 133/150 | 17 uncovered lines |
| seasons.c | 88.8% | 127/143 | 16 uncovered lines |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (233/233) |
| Sanitizer build | PASS (ASan + UBSan, 0 findings) |
| Coverage build | PASS (96.5% line, 100% function — `make coverage`) |
| WASM build | PASS (209 KB raw, 83 KB gzipped) |
| CI/CD | GitHub Actions (`native` + `wasm` jobs) |
| TODOs in code | 1 (earth_pass.c — Earth View mode gate) |
| Build system | Per-directory CMakeLists.txt + PRODUCTION/STAGING/DEVELOPMENT defines |

## Render Pipeline (16 passes)

| # | Pass | Layer | Type |
|---|------|-------|------|
| 1 | star_pass | STARS | Static star field |
| 2 | constellation_pass | STARS | IAU boundary lines |
| 3 | deep_sky_pass | STARS | Messier DSO billboards |
| 4 | milkyway_pass | GALAXY | Galaxy band mesh |
| 5 | diffraction_pass | STARS | Star diffraction spikes |
| 6 | orbit_trail_pass | ORBITS | Keplerian orbit ellipses |
| 7 | planet_pass | PLANETS | Sun + 8 planet sprites + trails |
| 8 | saturn_pass | PLANETS | Saturn ring system |
| 9 | moon_pass | PLANETS | 9 major moons + trails |
| 10 | zodiac_pass | ZODIAC_RING | Ring + markers + aspects + cusps + glyphs |
| 11 | earth_pass | PLANETS | Globe + coastlines + terminator + atmo |
| 12 | bodygraph_pass | CARDS | HD centers + channels + gates |
| 13 | hexagram_pass | CARDS | I Ching hexagram lines |
| 14 | tree_pass | CARDS | Kabbalah Sefirot + paths + pillars |
| 15 | card_pass | CARDS | Info card backgrounds + borders |
| 16 | post_pass | (wraps all) | FBO bloom + tonemap + vignette |

## Purity

| Metric | Value |
|--------|-------|
| P1 violations (GL in pure zones) | 0 |
| P2 violations (malloc in pure zones) | 0 |
| P3 violations (I/O in pure zones) | 0 |
| P4 violations (globals in pure zones) | 0 |
| P5 violations (mutable statics) | 4 (2 files — lazy shader string init, low severity) |
| Stateful modules | 24 (gl_init, shader, mesh, main, app_state, hud, input, ui_bridge, 16 passes) |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 13 | 13 |
| render | 57 pure + 20 stateful | 51 |
| astrology | 7 | 7 |
| astronomy | 10 | 10 |
| aztec | 1 | 1 |
| bahai | 1 | 1 |
| balinese | 1 | 1 |
| buddhist | 2 | 2 |
| celtic | 2 | 2 |
| chakra | 1 | 1 |
| chinese | 1 | 1 |
| coptic | 1 | 1 |
| earth | 15 | 15 |
| egyptian | 1 | 1 |
| ethiopian | 1 | 1 |
| french_republican | 1 | 1 |
| geology | 6 | 6 |
| gregorian | 1 | 1 |
| hebrew | 2 | 2 |
| hindu | 3 | 3 |
| human_design | 2 | 2 |
| iching | 1 | 1 |
| islamic | 2 | 2 |
| japanese | 1 | 1 |
| kabbalah | 3 | 3 |
| korean | 1 | 1 |
| myanmar | 1 | 1 |
| numerology | 1 | 1 |
| persian | 1 | 1 |
| tamil | 1 | 1 |
| tarot | 1 | 1 |
| thai | 1 | 1 |
| tzolkin | 6 | 6 |
| unified | 28 | 28 |
| zoroastrian | 2 | 2 |
| ui | 33 | 33 |
| core | 5 | 1 |

## WASM Binary Size

| Metric | Value |
|--------|-------|
| Raw .wasm | 214,492 bytes (209 KB) |
| Gzipped | 84,713 bytes (83 KB) |
| Build mode | Development (-Os, ASSERTIONS=2) |

## Health (OMEGA sweep #19)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | OK (241 .c files, all registered) |
| Purity audit | CLEAN (P1-P6 all zones) |
| Native build | PASS (233/233 tests) |
| WASM build | PASS (209 KB) |
| CI/CD | GREEN (in_progress on latest push) |
| Dead code | 0 |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Missing attribution | 0 |
| Render pipeline | COMPLETE (16/16 passes wired) |
| Refactor audit | ALL CLOSED — convergence.c already dispatched (50L max), knowledge_graph.c BFS already decomposed (37L max) |
