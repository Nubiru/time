# Time — Project Metrics

**Last refreshed**: 2026-03-20 (INFRA session 17)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 408 |
| Header files (.h) | 409 |
| Lines of code (src/) | 98,487 |
| Lines of tests | 174,000+ |
| Test suites (CTest) | 399 |
| Render pass files | 19 |
| Pure modules | 381+ |
| Stateful modules | 29 |
| Contributors | 179 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 399 |
| Test functions (RUN_TEST) | 16,178 |
| Test assertions (TEST_ASSERT) | 28,500+ |
| Failures | 0 |
| CTest time | 5.00s (with benchmarks) |
| E2E tests (Playwright) | 5 |
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

## Code Coverage (refreshed — 2026-03-20, session 14)

| Metric | Value |
|--------|-------|
| **Overall line coverage** | **95.9%** (30,315 / 31,615 lines) |
| **Overall function coverage** | **99.9%** (3,710 / 3,712 functions) |
| Uncovered functions | `spring_vec3_velocity`, `headline_cycle` |

*Coverage maintained at 95.9% across 31,615 lines (up from 29,616). 385 source files, 385 test suites.*

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
| brain_depth.c | 67.2% | 90/134 | New — depth tier selection logic |
| vec_interp.c | 70.9% | 61/86 | 1 uncovered fn: spring_vec3_velocity |
| timeline_data.c | 78.6% | 228/290 | Large module, many edge paths |
| text_lod.c | 78.6% | 81/103 | LOD text tier selection |
| camera_path.c | 80.5% | 91/113 | Camera interpolation paths |
| input_dispatch.c | 80.6% | 100/124 | New — keyboard dispatch |
| audio_event.c | 83.1% | 54/65 | Audio event triggers |
| bezier.c | 83.2% | 89/107 | Edge cases, degenerate inputs |
| planetary_hours.c | 84.6% | 22/26 | 4 uncovered lines |
| brain_surprise.c | 84.6% | 33/39 | New — surprise detection |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (367/367) |
| Sanitizer build | PASS (benchmarks excluded, 0 ASan/UBSan findings) |
| WASM build | PASS (306 KB raw, 121 KB gzipped) — ring_pass + render wiring |
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
| Raw .wasm | 329,626 bytes (330 KB) |
| Gzipped | 129,390 bytes (129 KB) |
| Build mode | Development (-Os, ASSERTIONS=2) |
| Growth since sweep #19 | +70 KB raw (+33%) |

## God Functions (>80 lines)

| Lines | File | Function | Notes |
|-------|------|----------|-------|
| 240 | convergence_detect.c | cd_is_significant | 18-case switch — domain logic |
| 183 | calendar_convert.c | cal_convert | Exhaustive date converter |
| 179 | text_pass.c | draw_card_text | Complex text render |
| 153 | seasonal_lighting.c | slp_sky_gradient | Sky color gradient |
| 129 | scale_hud.c | hud_visibility_at_scale | Visibility rules |
| 105 | kin_social.c | ks_match | Kin matching logic |
| 98 | timeline_data.c | format_label | Label formatting |
| 97 | content_curator.c | cc_digest | Content digest |
| 97 | horizon_geometry.c | hzg_pack | Horizon mesh packing |
| 95 | number_scanner.c | number_scan | Number pattern detection |
| 87 | command_palette.c | cmd_palette_search | Command search |
| 84 | zoroastrian_interpret.c | zi_interpret | Calendar interpretation |
| 83 | lens_flare.c | lf_sun_config | Lens flare configuration |

*13 functions >80L (5 over 100L). All domain-specific logic. No refactoring urgency.*

## Health (INFRA sweep — 2026-03-20, session 17)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | OK (408 .c files, 0 orphans) |
| Purity audit | CLEAN (P1-P5 all zones, session 11) |
| Native build | PASS (399/399 tests) — use -j4 for clean builds (race at -j12 with 380+ OBJECT libs) |
| WASM build | PASS (330 KB raw, 129 KB gzipped) — zero linker warnings |
| E2E tests | PASS (5/5 Playwright) |
| Coverage | 95.9% lines (30,315/31,615), 99.9% functions (3,710/3,712) |
| Git hooks | pre-commit (build safety) + commit-msg (stream domain) — both active |
| Dead code | 0 |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Render pipeline | 19/19 passes |
| AUDIO core wiring | Committed by INFRA on behalf of AUDIO (view-aware audio + M mute) |
