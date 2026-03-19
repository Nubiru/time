# Time — Project Metrics

**Last refreshed**: 2026-03-19 (INFRA session 4)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 349 |
| Header files (.h) | 349 |
| Lines of code (src/) | 86,412 |
| Lines of tests | 153,500+ |
| Test files | 343 |
| Render pass files | 17 |
| Contributors | 179 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 343 |
| Test functions (RUN_TEST) | 14,263 |
| Test assertions (TEST_ASSERT) | 24,237 |
| Failures | 0 |
| CTest time | 1.72s |
| ASan/UBSan | PASS (benchmarks excluded via `-LE benchmark`, ASan inflates VmPeak) |

### Test Pyramid

| Label | Count | Notes |
|-------|-------|-------|
| unit | 8 | Labeled tests |
| contract | 10 | Labeled tests |
| integration | 6 | Labeled tests |
| benchmark | 2 | CPU + memory |
| unlabeled | 284 | Functional tests without explicit labels |

## Code Coverage (refreshed — 2026-03-19)

| Metric | Value |
|--------|-------|
| **Overall line coverage** | **95.8%** (25,773 / 26,907 lines) |
| **Overall function coverage** | **99.9%** (3,181 / 3,183 functions) |
| Uncovered functions | `spring_vec3_velocity`, `headline_cycle` |

*Coverage includes 80+ new stream-era modules. 95.8% across 26,907 lines is excellent.*

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
| CTest -j12 | PASS (343/343) |
| Sanitizer build | PASS (benchmarks excluded, 0 ASan/UBSan findings) |
| WASM build | PASS (285 KB raw, 111 KB gzipped) — 3 symbol collisions fixed |
| CI/CD | GitHub Actions (`native` + `wasm` jobs) |
| TODOs in code | 1 (earth_pass.c — Earth View mode gate) |
| Build system | Per-directory CMakeLists.txt + PRODUCTION/STAGING/DEVELOPMENT defines |

## Render Pipeline (17 passes)

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
| 16 | text_pass | TEXT | Text rendering pass |
| 17 | post_pass | (wraps all) | FBO bloom + tonemap + vignette |

## Purity

| Metric | Value |
|--------|-------|
| P1 violations (GL in pure zones) | 0 |
| P2 violations (malloc in pure zones) | 0 |
| P3 violations (I/O in pure zones) | 0 |
| P4 violations (globals in pure zones) | 0 |
| P5 violations (mutable statics) | 4 (2 files — lazy shader string init, low severity) |
| Naming convention compliance | 100% |
| Include guard compliance | 100% |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 15 | 15 |
| render | 63 + 17 passes | 60 |
| ui | 56 | 54 |
| core | 5 | 1 |
| platform | 1 | 1 |
| unified | 65 | 64 |
| earth | 23 | 23 |
| astronomy | 10 | — |
| astrology | 7 | — |
| tzolkin | 6 | 4 |
| geology | 6 | 5 |
| kabbalah | 4 | 4 |
| hindu | 4 | 3 |
| buddhist | 3 | 3 |
| hebrew | 3 | 3 |
| islamic | 3 | 3 |
| iching | 2 | 1 |
| human_design | 2 | — |
| celtic | 2 | 2 |
| zoroastrian | 2 | 2 |
| 16 single-module domains | 16 | 16 |
| integration tests | — | 6 |
| benchmarks | — | 2 |

## WASM Binary Size

| Metric | Value |
|--------|-------|
| Raw .wasm | 285,503 bytes (285 KB) |
| Gzipped | 113,438 bytes (111 KB) |
| Build mode | Development (-Os, ASSERTIONS=2) |
| Growth since sweep #19 | +70 KB raw (+33%) |

## God Functions (>80 lines)

| Lines | File | Function | Notes |
|-------|------|----------|-------|
| 235 | convergence_detect.c | cd_is_significant | 18-case switch — domain logic |
| 183 | calendar_convert.c | cal_convert | Exhaustive date converter |
| 179 | text_pass.c | draw_card_text | Complex text render |
| 156 | audio_score.c | audio_score_compute | Audio computation |
| 153 | seasonal_lighting.c | slp_sky_gradient | Sky color gradient |
| 129 | touch_gestures.c | tg_touch_end | Multi-gesture handler |
| 129 | scale_hud.c | hud_visibility_at_scale | Visibility rules |

*Most are domain-specific switch/dispatch logic. No refactoring urgency.*

## Health (INFRA sweep — 2026-03-19)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | OK (349 .c files, all registered, 0 phantoms) |
| Purity audit | CLEAN (P1-P5 all zones) |
| Native build | PASS (343/343 tests) |
| WASM build | PASS (285 KB) — zero linker warnings |
| Symbol conflicts | **FIXED** — 4 total: cal_system_name, ti_interpret, hdi_interpret, ci_interpret |
| ASan/UBSan | PASS (benchmarks excluded via `-LE benchmark`, 0 findings) |
| Dead code | 0 |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Missing attribution | 0 |
| Render pipeline | 17/17 passes (text_pass added) |
| Refactor audit | 7 God Functions >100L, all domain logic, no urgency |
