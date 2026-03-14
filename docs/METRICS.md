# Time — Project Metrics

**Last refreshed**: 2026-03-14 (OMEGA sweep #8)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 214 |
| Header files (.h) | 214 |
| Lines of code (src/) | 65,788 |
| Lines of tests | 93,703 |
| Test files | 191 |
| Render pass files | 16 |
| Contributors | 143 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 189 |
| Test functions (RUN_TEST) | 9,122 |
| Test assertions (TEST_ASSERT) | 15,158 |
| Failures | 0 |
| CTest time | 0.90s |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (189/189) |
| TODOs in code | 0 |

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
| Stateful modules | 22 (gl_init, shader, mesh, main, app_state, hud, input, 16 passes) |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 15 | 14 |
| render | 50 | 43 |
| astrology | 7 | 7 |
| astronomy | 10 | 10 |
| aztec | 1 | 1 |
| buddhist | 2 | 2 |
| celtic | 2 | 2 |
| chakra | 1 | 1 |
| chinese | 1 | 1 |
| coptic | 1 | 1 |
| earth | 11 | 11 |
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
| bahai | 1 | 1 |
| tamil | 1 | 1 |
| thai | 1 | 1 |
| tzolkin | 6 | 6 |
| unified | 26 | 26 |
| zoroastrian | 2 | 2 |
| ui | 25 | 25 |
| core | 3 | 1 |

## Health (OMEGA sweep #8)

| Check | Status |
|-------|--------|
| Git integrity | OK |
| Purity audit | CLEAN (0 violations P1-P5) |
| Dead code | 0 (cleanup completed sweep #5) |
| Naked TODOs | 0 |
| Missing attribution | 0 |
| Render pipeline | COMPLETE (16/16 passes wired) |
