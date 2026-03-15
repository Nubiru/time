# Time — Project Metrics

**Last refreshed**: 2026-03-15 (OMEGA sweep #11)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 230 |
| Header files (.h) | 230 |
| Lines of code (src/) | 71,706 |
| Lines of tests | 103,304 |
| Test files | 208 |
| Render pass files | 16 |
| Contributors | 175 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 207 |
| Test functions (RUN_TEST) | 9,935 |
| Test assertions (TEST_ASSERT) | 15,748 |
| Failures | 0 |
| CTest time | 1.02s |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (207/207) |
| TODOs in code | 1 (earth_pass.c — Earth View mode gate) |

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
| Stateful modules | 23 (gl_init, shader, mesh, main, app_state, hud, input, 16 passes) |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 13 | 13 |
| render | 56 pure + 19 stateful | 50 |
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
| earth | 13 | 13 |
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
| unified | 27 | 27 |
| zoroastrian | 2 | 2 |
| ui | 31 | 31 |
| core | 5 | 1 |

## Health (OMEGA sweep #11)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Build system sync | WARN (shared_uniforms.c not in CMakeLists — untracked WIP) |
| Purity audit | 0 NEW (4 P5 known near-pure, unchanged) |
| Dead code | 0 (new modules time_control, toast_message, command_palette — CLEAN) |
| Naked TODOs | 1 (earth_pass.c — Earth View mode gate) |
| Missing attribution | 1 (E.G. Richards — celtic_tree.c) |
| Render pipeline | COMPLETE (16/16 passes wired) |
| Style compliance | 153+ hardcoded visual constants (policy pending) |
| Test coverage | 90% file coverage (208/230) |
| Refactor candidates | earth_pass.c (init 140L, draw 121L), srp_pack 118L, wov_arrow_glyph 104L |
| Orphaned files | shared_uniforms.c/.h + test, ui_html.h (all untracked — MEGA WIP) |
