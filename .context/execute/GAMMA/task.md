# Task: Surf Spot Database

**Agent**: GAMMA
**Roadmap Reference**: Track 21.2 — "Agent: Surf Spot Database (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data module providing ~50 world-class surf spots with geographic coordinates, beach orientation, swell preferences, and tide/season data. Enables Track 21.3 (tidal surf display) by providing the location database for real-time surf condition assessment.

## READ FIRST

- `src/systems/astronomy/lunar.h` — tidal influence reference (for tide_quality concept)

## Files to Create

- `src/systems/earth/surf_spots.h`
- `src/systems/earth/surf_spots.c`
- `tests/systems/earth/test_surf_spots.c`

## API

```c
#ifndef TIME_SURF_SPOTS_H
#define TIME_SURF_SPOTS_H

#include <stdbool.h>

#define SURF_SPOT_COUNT 50

typedef enum {
    TIDE_LOW = 0,
    TIDE_MID,
    TIDE_HIGH,
    TIDE_ALL
} tide_pref_t;

typedef enum {
    SEASON_WINTER = 0,
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_ALL
} season_pref_t;

typedef enum {
    WAVE_REEF = 0,
    WAVE_BEACH,
    WAVE_POINT,
    WAVE_RIVER_MOUTH
} wave_type_t;

typedef struct {
    int id;
    const char *name;
    const char *country;
    double lat;
    double lon;
    double beach_facing_deg;
    double best_swell_dir_deg;
    tide_pref_t best_tide;
    season_pref_t best_season;
    wave_type_t wave_type;
    const char *description;
} surf_spot_t;

surf_spot_t surf_spot_get(int index);
int surf_spot_count(void);
int surf_spot_nearest(double lat, double lon);
double surf_spot_tide_quality(int spot_index, double tidal_height);
double surf_spot_swell_quality(int spot_index, double swell_dir_deg);
const char *surf_spot_name(int index);
int surf_spots_by_type(wave_type_t type, int *out_indices, int out_max);
int surf_spots_in_region(double lat_min, double lat_max,
                         double lon_min, double lon_max,
                         int *out_indices, int out_max);
const char *surf_tide_name(tide_pref_t tide);
const char *surf_season_name(season_pref_t season);
const char *surf_wave_type_name(wave_type_t type);

#endif /* TIME_SURF_SPOTS_H */
```

## DONE WHEN

- [ ] 50 surf spots with correct coordinates, beach facing, swell direction
- [ ] surf_spot_nearest uses Haversine or simplified distance
- [ ] tide_quality returns sensible 0-1 scores based on spot preference
- [ ] swell_quality returns sensible 0-1 scores based on angular difference
- [ ] spots_by_type filters correctly
- [ ] spots_in_region returns correct bounding box results
- [ ] Edge cases: out-of-range index, antipodal nearest, 360/0 degree wrapping
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- All data as static const arrays
- Standalone module (no compile-time dependencies)
- Distance: simplified spherical (Haversine or law-of-cosines)
- Data sources: Wikipedia surf spot lists (public domain), general surfing knowledge
