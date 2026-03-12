# Time — Project Metrics

**Last refreshed**: 2026-03-08 (OMEGA sweep #3)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 184 |
| Header files (.h) | 184 |
| Lines of C code | 55,252 |
| Test files | 174 |
| Shader files | 4 |
| Scripts | 5 |
| Contributors | 125 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 172 |
| Test functions (RUN_TEST) | 7,861 |
| Test assertions (TEST_ASSERT) | 12,744 |
| Failures | 0 |
| CTest time | 0.31s |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (172/172) |
| TODOs in code | 0 |

## Purity

| Metric | Value |
|--------|-------|
| P1 violations (GL in pure zones) | 0 |
| P2 violations (malloc in pure zones) | 0 |
| P3 violations (I/O in pure zones) | 0 |
| P4 violations (globals in pure zones) | 0 |
| Style violations (hardcoded constants) | 5 files |
| Stateful modules | 5 (gl_init, shader, mesh, main, app_state) |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 15 | 14 |
| render | 36 | 33 |
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
| ui | 24 | 24 |
| core | 3 | 1 |

## Health (OMEGA sweep #3)

| Check | Status |
|-------|--------|
| Git integrity | OK (dangling commits only) |
| Git tracked violations | 0 (fixed since sweep #2) |
| Purity audit | CLEAN |
| Dead code | None detected |
| Naked TODOs | 0 |
| Missing attribution | 0 (fixed this sweep) |
| Test coverage | 97% (3 shader modules lack tests) |
| All tests >= 18 | 13 legacy modules below minimum |
