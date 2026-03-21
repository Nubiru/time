# Time — Project Metrics

**Last refreshed**: 2026-03-21 (INFRA session 32)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 469 |
| Header files (.h) | 472 |
| Lines of code (src/) | 155,000+ |
| Lines of tests | 200,000+ |
| Test suites (CTest) | 496 |
| Render pass files | 19 |
| Pure modules | 399+ |
| Stateful modules | 29 |
| Contributors | 186 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 496 |
| Test functions (RUN_TEST) | 17,500+ |
| Test assertions (TEST_ASSERT) | 29,891+ |
| Failures | 0 |
| CTest time | 1.43s (with -j4, benchmarks included) |
| E2E tests (Playwright) | 15 (5 basic + 10 visual) |
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

## Code Coverage (refreshed — 2026-03-20, session 27)

| Metric | Value |
|--------|-------|
| **Overall line coverage** | **95.9%** (34,295 / 35,751 lines) |
| **Overall function coverage** | **99.97%** (4,071 / 4,072 functions) |
| Uncovered functions | `spring_vec3_velocity` (vec_interp.c — advanced physics helper) |

*Coverage maintained at 95.9% across 35,751 lines. 431 source files, 460 test suites. 2 coverage gaps closed this session (headline_cycle, sr_neutral_color).*

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
| CTest -j4 | PASS (496/496, 1.19s) |
| Sanitizer build | PASS (benchmarks excluded, 0 ASan/UBSan findings) |
| WASM build | PASS (363 KB raw, ~143 KB gzipped) — brain visual bridge wired |
| CI/CD | GitHub Actions (`native` + `wasm` jobs) |
| TODOs in code | 1 (earth_pass.c — Earth View mode gate) |
| Build system | Per-directory CMakeLists.txt + PRODUCTION/STAGING/DEVELOPMENT defines |

## Render Pipeline (19 passes)

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
| 17 | ring_pass | RINGS | Concentric knowledge system rings |
| 18 | convergence_pass | CONVERGENCE | Cross-system convergence visuals |
| 19 | post_pass | (wraps all) | FBO bloom + tonemap + vignette |

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
| Raw .wasm | 373,690 bytes (365 KB) |
| Gzipped | ~143 KB |
| Build mode | Development (-Os, ASSERTIONS=2) |
| Release .wasm | 311,718 bytes (304 KB) — with full LTO |
| Growth since session 18 | +44 KB raw (all wiring + 20 Kin Maya layouts + focus-mode audio) |

## God Functions (>80 lines)

| Lines | File | Function | Notes |
|-------|------|----------|-------|
| 241 | convergence_detect.c | cd_is_significant | 18-case switch — domain logic |
| 273 | main.c | main_loop | Central render loop — well-sectioned, AUDIO added focus-mode timbres |
| 189 | text_pass.c | draw_card_text | Complex text render |
| 142 | text_pass.c | text_pass_draw | GL text rendering |
| 130 | hud.c | hud_update | HUD overlay update |
| 105 | weather_overlay.c | wov_arrow_glyph | Wind arrow glyph geometry |
| 103 | card_pass.c | card_pass_draw | GL card rendering |
| 102 | brain_personal.c | br_personal_scan | Personal convergence analysis |
| 99 | timeline_data.c | format_label | Label formatting |
| 99 | post_pass.c | post_pass_init | FBO + shader setup |
| 99 | planet_pass.c | planet_pass_init | Planet texture + shader setup |
| 94 | ring_pass.c | ring_pass_draw | GL ring rendering |
| 92 | astronomy_interpret.c | ai_phase_count (block) | Phase data array |
| 90 | geology_interpret.c | gli_supercontinent_count (block) | Data array |
| 89 | numerology_interpret.c | ni_number_count (block) | Data array |

*15 functions >80L (8 over 100L). main_loop grew from wiring (brain+motion+audio). GL passes are naturally large. No refactoring urgency.*

## Health (INFRA sweep — 2026-03-21, session 32)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | OK (469 .c files, 0 orphans) |
| Purity audit | CLEAN (P1-P5 all zones, session 23) |
| Native build | PASS (496/496 tests, 1.19s) — use -j4 for clean builds |
| WASM build | PASS (365 KB raw, ~143 KB gzipped) |
| E2E tests | PASS (5/5 basic + 7/10 visual, 2 skips, 1 SwiftShader) |
| Coverage | 95.9% lines (34,295/35,751), 99.97% functions (4,071/4,072) |
| Git hooks | pre-commit (domain safety, blocks .context/.claude) — active |
| Dead code | 0 |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Render pipeline | 19/19 passes |
| Brain pipeline | WIRED — headline + convergence_strength + brain_visual per-day cache (session 24+26) |
| Motion wiring | ALL 9 motion modules in core — MOTION fully wired |
| Interpret locale | 32 systems have interpret_locale — LANGUAGE i18n fully deployed |
| Content encoding | CLEAN — encoding fix (session 22→23, commit 67079b7) |
| Header deps | CLEAN — 941 files, 0 cycles, 0 purity violations, 0 missing, max depth 7 |
| ASan/UBSan | PASS (461/461, zero findings, session 28) |
| Shader budget | All 19 passes under 8KB (largest: sun_shader 4.5KB) |
