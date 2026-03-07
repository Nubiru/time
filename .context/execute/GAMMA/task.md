# Task: Seasonal Cycle Visualizer Data

**Agent**: GAMMA
**Roadmap Reference**: Track 25.2 — "Agent: Seasonal Cycle Visualizer Data (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data+computation module for seasonal cycles at any latitude. Provides season identification, growing season estimation, hemisphere comparison, and seasonal progress tracking. Standalone — uses solar declination approximation internally.

## Files to Create

- `src/systems/earth/seasons.h`
- `src/systems/earth/seasons.c`
- `tests/systems/earth/test_seasons.c`

## DONE WHEN

- [ ] season_at returns correct season + progress for both hemispheres
- [ ] Growing season (frost-free days) by latitude
- [ ] Solstice/equinox day-of-year identification
- [ ] Hemisphere comparison
- [ ] >= 30 tests, all pass, zero warnings
- [ ] Purity: no malloc, no globals, no side effects

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- `#define PI 3.14159265358979323846`
- No malloc, no globals, no side effects
- Standalone module
