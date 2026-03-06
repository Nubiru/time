# Task: Mountain Resort Database

**Agent**: GAMMA
**Roadmap Reference**: Track 23.2 — "Agent: Mountain Resort Database (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data module providing ~50 world ski/mountain resorts with geographic coordinates, elevation data, and seasonal quality scoring. Mirrors surf_spots pattern for mountain sports. Enables Track 23.3 (snow conditions display).

## Files to Create

- `src/systems/earth/ski_resorts.h`
- `src/systems/earth/ski_resorts.c`
- `tests/systems/earth/test_ski_resorts.c`

## API

```c
#ifndef TIME_SKI_RESORTS_H
#define TIME_SKI_RESORTS_H

#include <stdbool.h>

#define SKI_RESORT_COUNT 50

typedef struct {
    int id;
    const char *name;
    const char *country;
    double lat;
    double lon;
    int base_elev_m;
    int summit_elev_m;
    int season_start;     /* month 1-12 */
    int season_end;       /* month 1-12 */
    const char *description;
} ski_resort_t;

ski_resort_t ski_resort_get(int index);
int ski_resort_count(void);
int ski_resort_nearest(double lat, double lon);
double ski_resort_snow_quality(int resort_index, int month);
const char *ski_resort_name(int index);
int ski_resort_vertical(int index);
int ski_resorts_in_region(double lat_min, double lat_max,
                          double lon_min, double lon_max,
                          int *out_indices, int out_max);
bool ski_resort_in_season(int index, int month);

#endif
```

## DONE WHEN

- [ ] 50 ski resorts with correct coordinates and elevations
- [ ] ski_resort_nearest uses Haversine
- [ ] snow_quality returns sensible 0-1 scores based on month vs season
- [ ] in_season handles southern hemisphere wrap-around (e.g., Jun-Oct)
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- All data as static const arrays
- Standalone module (no compile-time dependencies)
- Data sources: Wikipedia ski resort lists (public domain)
