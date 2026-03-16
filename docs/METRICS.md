# Time — Project Metrics

**Last refreshed**: 2026-03-16 (OMEGA sweep #14)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 235 |
| Header files (.h) | 235 |
| Lines of code (src/) | 74,115 |
| Lines of tests | 106,709 |
| Test files | 215 |
| Render pass files | 16 |
| Contributors | 185 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 212 |
| Test functions (RUN_TEST) | 10,213 |
| Test assertions (TEST_ASSERT) | 16,181 |
| Failures | 0 |
| CTest time | 1.08s |
| ASan/UBSan | PASS (212/212, 0 findings) |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (212/212) |
| Sanitizer build | PASS (ASan + UBSan, 0 findings) |
| Coverage build | BLOCKED (lcov not installed — `sudo apt install lcov`) |
| TODOs in code | 1 (earth_pass.c — Earth View mode gate) |
| Build system | Per-directory CMakeLists.txt (refactored 2026-03-15) |

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

## Health (OMEGA sweep #14)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | OK (235 .c files, all registered) |
| Purity audit | CLEAN (P1-P4 all zones, 4 P5 known) |
| ASan/UBSan | PASS (0 memory errors, 0 undefined behavior) |
| Dead code | 0 |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Missing attribution | 0 |
| Render pipeline | COMPLETE (16/16 passes wired) |
| Refactor candidates | earth_pass.c decomposed (sweep #13 task) |
| Style debt | Policy documented in CONVENTIONS.md (domain colors exempt) |
