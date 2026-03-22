# Time — Project Metrics

**Last refreshed**: 2026-03-22 (INFRA session 74)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 505 |
| Header files (.h) | 508 |
| Lines of code (src/) | 117,835 |
| Lines of tests | 212,000+ |
| Test suites (CTest) | 528 |
| Render pass files | 23 |
| Knowledge systems | 34 |
| Pure modules | 450+ |
| Stateful modules | 32 |
| Contributors | 283 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 528 |
| Test functions (RUN_TEST) | 19,986 |
| Test assertions (TEST_ASSERT) | 34,308 |
| Failures | 0 |
| CTest time | ~0.9s (with -j12) |
| E2E tests (Playwright) | 17 (5 basic + 12 visual) |
| ASan/UBSan | PASS (benchmarks excluded via `-LE benchmark`, ASan inflates VmPeak) |

### Test Pyramid

| Label | Count | Notes |
|-------|-------|-------|
| unit | 8 | Labeled tests |
| contract | 10 | Labeled tests |
| integration | 6 | Labeled tests |
| benchmark | 2 | CPU + memory |
| unlabeled | 284 | Functional tests without explicit labels |
| e2e | 5 | Playwright (WASM load, canvas, help, pause) |

## Code Coverage (refreshed — 2026-03-21, session 33)

| Metric | Value |
|--------|-------|
| **Overall line coverage** | **96.0%** (35,581 / 37,053 lines) |
| **Overall function coverage** | **99.98%** (4,233 / 4,234 functions) |
| Uncovered functions | `spring_vec3_velocity` (vec_interp.c — advanced physics helper) |

*Coverage improved to 96.0% across 37,053 lines (+1,302 since session 27). 469 source files, 496 test suites. All new Kin Maya, Synchronotron, and Telektonon modules fully covered.*

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
| brain_depth.c | 68.9% | 111/161 | Depth tier selection logic |
| vec_interp.c | 70.9% | 61/86 | 1 uncovered fn: spring_vec3_velocity |
| zoroastrian_interpret.c | 75.8% | 91/120 | New — locale expansion added paths |
| timeline_data.c | 78.6% | 228/290 | Large module, many edge paths |
| text_lod.c | 78.6% | 81/103 | LOD text tier selection |
| camera_path.c | 80.5% | 91/113 | Camera interpolation paths |
| input_dispatch.c | 80.6% | 100/124 | Keyboard dispatch |
| audio_event.c | 83.1% | 54/65 | Audio event triggers |
| bezier.c | 83.2% | 89/107 | Edge cases, degenerate inputs |
| brain_narrative.c | 83.2% | 119/143 | New — narrative composition |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (528/528, ~1s) |
| Sanitizer build | PASS (benchmarks excluded, 0 ASan/UBSan findings) |
| WASM build | PASS (737 KB Release — 34 systems, 23 passes, god rays + DOF) |
| CI/CD | GitHub Actions (`native` + `wasm` jobs) |
| TODOs in code | 1 (earth_pass.c — Earth View mode gate) |
| Build system | Per-directory CMakeLists.txt + PRODUCTION/STAGING/DEVELOPMENT defines |

## Render Pipeline (23 passes)

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
| 12 | bodygraph_pass | FOCUS_HD | HD 9 centers + channels + gates |
| 13 | hexagram_pass | FOCUS_ICHING | I Ching hexagram lines |
| 14 | tree_pass | FOCUS_KABBALAH | Kabbalah Sefirot + paths + pillars |
| 15 | natal_chart_pass | FOCUS_ASTRO | Zodiac arcs + house cusps + planet positions + aspects |
| 16 | tree_of_life_pass | FOCUS_KABBALAH | 10 Sefirot circles + 22 colored paths |
| 17 | bagua_pass | FOCUS_ICHING | 8 trigrams circle |
| 18 | gates_mandala_pass | FOCUS_KABBALAH | 22 Hebrew letters + 231 golden lines |
| 19 | card_pass | CARDS | Info card backgrounds + borders |
| 20 | text_pass | TEXT | Text rendering (3D labels + 2D MSDF cards) |
| 21 | ring_pass | RINGS | Concentric knowledge system rings |
| 22 | convergence_pass | CONVERGENCE | Cross-system convergence visuals |
| 23 | post_pass | (wraps all) | FBO bloom + tonemap + vignette |

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

| Domain | Modules | Notes |
|--------|---------|-------|
| math | 16 | All pure |
| render | 72 + 19 passes | 69 pure + 3 infra + 19 stateful passes |
| ui | 72 | 70 pure + 2 stateful |
| core | 5 | 1 pure (date_parse) + 4 stateful |
| platform | 1 | Pure |
| unified | 69 | All pure (brain_*, depth_*, convergence, etc.) |
| earth | 23 | All pure |
| astronomy | 11 | All pure |
| astrology | 8 | All pure |
| tzolkin | 7 | All pure |
| geology | 7 | All pure |
| kabbalah | 4 | All pure |
| hindu | 4 | All pure |
| 20 other calendar systems | 54 | All pure (2-3 modules each) |
| integration tests | — | 7 tests |
| benchmarks | — | 2 tests |
| e2e | — | 5 tests (Playwright) |

## WASM Binary Size

| Metric | Value |
|--------|-------|
| Raw .wasm | 754,340 bytes (737 KB) |
| Build mode | Release (-Os) |
| MSDF font atlas | JetBrains Mono 512×512 RGB (~250 KB in binary) |
| Sans atlas | Excluded (MSDF_SANS_ENABLED not set) |
| Gzipped | ~300 KB |
| Delta since session 60 | +74 KB (god rays, DOF, wisdom, signature search, onboarding) |

## God Functions (>100 lines)

| Lines | File | Function | Notes |
|-------|------|----------|-------|
| 306 | main.c | main_loop | Central render loop — well-sectioned |
| 254 | audio_score.c | audio_score_compute | Complex scoring logic |
| 170 | card_pass.c | draw_oracle_cross | Oracle cross + wavespell quads |
| 168 | input.c | on_key_down | Key handler switch |
| 154 | card_pass.c | draw_iching_overlay | I Ching visual overlay |
| 145 | text_pass.c | text_pass_draw | Planet labels + card text dispatch |
| 126 | card_pass.c | card_pass_draw | Card layout + border rendering |
| 125 | text_pass.c | draw_iching_text | I Ching text overlay |
| 122 | text_pass.c | draw_card_text | System card text layout |
| 101 | text_pass.c | text_pass_init | Shader + atlas initialization |

*10 functions >100L. text_pass.c grew from wiring 19 daily systems + focus modes. GL passes are naturally large. No refactoring urgency.*

## Large Files (>800 lines, non-data)

| Lines | File | Notes |
|-------|------|-------|
| 1073 | text_pass.c | Candidate: split cards/labels/overlays |
| 756 | card_pass.c | Render pass with multiple overlays |

## Health (INFRA sweep — 2026-03-22, session 74)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | OK (505 .c files, 0 orphans) |
| Purity audit | CLEAN (P1-P5 all zones) |
| Native build | PASS (528/528 tests, ~1s) — use -j12 |
| WASM build | PASS (737 KB Release — god rays + DOF post-processing) |
| E2E tests | PASS (5/5 basic + 12 visual) |
| Coverage | 96.0% lines, 99.98% functions |
| Git hooks | pre-commit (domain safety, blocks .context/.claude) — active |
| Dead code | 0 |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Render pipeline | 23/23 passes (4 new geometric: natal_chart, tree_of_life, bagua, gates_mandala) |
| Theme audit | CLEAN — 23/23 passes audited. Cosmos-scope uses theme_cosmos_constant(). UI-scope uses theme_get(). Zero leakage. |
| Canvas resize | WIRED — ui_on_resize KEEPALIVE, debounced JS listener (session 59) |
| Audio envelope | WIRED — setTargetAtTime, attack/release envelope, volume 0.12 (session 59) |
| Brain pipeline | WIRED — headline + convergence_strength + brain_visual per-day cache |
| Motion wiring | ALL 9 motion modules in core — MOTION fully wired |
| Interpret locale | 32 systems have interpret_locale — LANGUAGE i18n fully deployed |
| Daily cards | 19/19 wired (11 original + Earth + Coptic/Ethiopian/Japanese/Korean/Persian/Thai + Haab + Kabbalah) |
| Content encoding | CLEAN |
| ASan/UBSan | PASS (benchmarks excluded, zero findings) |
| Shader budget | All 23 passes under 8KB |
