# Task: Snow Season Model

**Agent**: GAMMA
**Roadmap Reference**: Track 23.1 — "Agent: Snow Season Model (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure computation module modeling snow probability from latitude, elevation, and month. Provides ski season window estimation, powder likelihood scoring, and avalanche risk factor assessment. Complements ski_resorts (data) with physics-based snow modeling.

## Files to Create

- `src/systems/earth/snow_season.h`
- `src/systems/earth/snow_season.c`
- `tests/systems/earth/test_snow_season.c`

## DONE WHEN

- [ ] snow_probability returns 0-1 based on lat/elevation/month
- [ ] ski_season_window returns start/end months
- [ ] Powder likelihood scoring
- [ ] Avalanche risk factors
- [ ] Southern hemisphere handled (winter = Jun-Aug)
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- Standalone module (no compile-time dependencies)
