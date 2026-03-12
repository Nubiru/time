# Time — Project Metrics

**Last refreshed**: 2026-03-12 (OMEGA sweep #4)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 185 |
| Header files (.h) | 185 |
| Lines of source (.c) | 44,192 |
| Lines of headers (.h) | 12,027 |
| Lines of tests | 81,465 |
| Test files | 175 |
| Shader files | 4 |
| Scripts | 5 |
| Contributors | 140 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 174 |
| Test functions (RUN_TEST) | 7,926 |
| Test assertions (TEST_ASSERT) | 12,853 |
| Failures | 0 |
| CTest time | 0.31s |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (174/174) |
| TODOs in code | 0 |

## Purity

| Metric | Value |
|--------|-------|
| P1 violations (GL in pure zones) | 0 |
| P2 violations (malloc in pure zones) | 0 |
| P3 violations (I/O in pure zones) | 0 |
| P4 violations (globals in pure zones) | 0 |
| Style violations (hardcoded constants) | 5 files |
| Stateful modules | 10 (gl_init, shader, mesh, main, app_state, hud, input, 3 passes) |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 15 | 14 |
| render | 43 | 40 |
| astrology | 7 | 7 |
| astronomy | 10 | 10 |
| aztec | 1 | 1 |
| buddhist | 2 | 2 |
| celtic | 1 | 1 |
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
| kabbalah | 3 | 3 |
| myanmar | 1 | 1 |
| numerology | 1 | 1 |
| thai | 1 | 1 |
| tzolkin | 5 | 5 |
| unified | 21 | 21 |
| zoroastrian | 2 | 2 |
| ui | 25 | 25 |
| core | 3 | 1 |

## Health (OMEGA sweep #4)

| Check | Status |
|-------|--------|
| Git integrity | OK (clean working tree) |
| Git tracked violations | 0 |
| Purity audit | CLEAN (0 violations P1-P5) |
| Dead code | 10 items (5 genuinely unused functions) |
| Naked TODOs | 0 |
| Missing attribution | 0 |
| Test coverage | 97.4% (3 shader modules lack tests) |
| All tests >= 18 | 13 legacy modules below minimum |
