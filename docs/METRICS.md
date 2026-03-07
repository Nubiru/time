# Time — Project Metrics

**Last refreshed**: 2026-03-07 (OMEGA sweep)

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 161 |
| Header files (.h) | 160 |
| Lines of C code | 37,698 |
| Test files | 159 |
| Shader files | 4 |
| Scripts | 5 |
| Contributors | 112 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites (CTest) | 116 |
| Test functions (RUN_TEST) | 6,676 |
| Test assertions (TEST_ASSERT) | 10,941 |
| Failures | 0 |
| CTest time | 0.83s |

## Build

| Target | Status |
|--------|--------|
| CMake native | PASS (zero warnings) |
| CTest -j12 | PASS (116/116) |
| Makefile native | PASS |
| Makefile WASM | 35 files pending integration |
| TODOs in code | 0 |

## Purity

| Metric | Value |
|--------|-------|
| P1 violations (GL in pure zones) | 0 |
| P2 violations (malloc in pure zones) | 0 |
| P3 violations (I/O in pure zones) | 0 |
| P4 violations (globals in pure zones) | 0 |
| Style violations (hardcoded constants) | 4 files |
| Stateful modules | 5 (gl_init, shader, mesh, main, app_state) |

## System Domains

| Domain | Modules | Tests |
|--------|---------|-------|
| math | 15 | 14 |
| render | 31 | 31 |
| astrology | 7 | 7 |
| astronomy | 10 | 10 |
| buddhist | 2 | 2 |
| celtic | 1 | 1 |
| chakra | 1 | 1 |
| chinese | 1 | 1 |
| coptic | 1 | 1 |
| earth | 11 | 11 |
| egyptian | 1 | 1 |
| ethiopian | 1 | 1 |
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
| tzolkin | 4 | 4 |
| unified | 20 | 20 |
| zoroastrian | 2 | 2 |
| ui | 24 | 24 |
| core | 3 | 1 |

## Health (OMEGA baseline)

| Check | Status |
|-------|--------|
| Git integrity | OK (dangling commits only) |
| Git tracked violations | 57 .context/.claude files tracked |
| Purity audit | CLEAN |
| Dead code | None detected |
| Naked TODOs | 0 |
| Missing attribution | 0 (fixed this sweep) |
| Test coverage | 100% (every .c has test) |
| All tests >= 18 | YES |
