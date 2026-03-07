# Task: Megalithic Alignments

**Agent**: ALPHA
**Roadmap Reference**: Track 41.2 — "Agent: Megalithic Alignments (ALPHA)"
**Date**: 2026-03-07
**Status**: CLAIMED

## Goal

Pure geometry module providing megalithic site alignment data and sunrise/sunset/moonrise azimuth computation. Stores archaeological alignment data for major megalithic sites (Stonehenge, Newgrange, Callanish, Carnac, Mnajdra) and computes whether their alignments match current or upcoming celestial events. Enables "Newgrange aligns in 3 days" alerts.

## READ FIRST

- `src/systems/astronomy/solar_events.h` — se_solar_declination(), se_sunrise_hour_angle()
- `src/math/vec3.h` — basic vector math
- `src/render/color_theory.h` — ct_system_primary() for GEOLOGY colors (stone/earth)

## Files to Create

- `src/render/megalithic.h`
- `src/render/megalithic.c`
- `tests/render/test_megalithic.c`

## API

```c
#ifndef TIME_MEGALITHIC_H
#define TIME_MEGALITHIC_H

#define PI 3.14159265358979323846

#define MEG_MAX_SITES 8

/* Celestial event that a site aligns with */
typedef enum {
    MEG_EVENT_SUMMER_SOLSTICE_SUNRISE = 0,
    MEG_EVENT_SUMMER_SOLSTICE_SUNSET,
    MEG_EVENT_WINTER_SOLSTICE_SUNRISE,
    MEG_EVENT_WINTER_SOLSTICE_SUNSET,
    MEG_EVENT_EQUINOX_SUNRISE,
    MEG_EVENT_EQUINOX_SUNSET,
    MEG_EVENT_LUNAR_STANDSTILL_NORTH,
    MEG_EVENT_LUNAR_STANDSTILL_SOUTH,
    MEG_EVENT_COUNT
} meg_event_t;

/* Megalithic site data */
typedef struct {
    const char *name;       /* site name */
    const char *location;   /* country/region */
    double lat;             /* latitude in degrees */
    double lon;             /* longitude in degrees */
    double alignment_az;    /* alignment azimuth in degrees from north */
    meg_event_t event;      /* target celestial event */
    int epoch_bce;          /* approximate construction date BCE */
    double tolerance_deg;   /* angular tolerance for "aligned" check */
} meg_site_t;

/* Alignment check result */
typedef struct {
    int aligned;            /* 1 if event azimuth matches site alignment */
    double event_az;        /* computed azimuth of the event (degrees) */
    double site_az;         /* site alignment azimuth (degrees) */
    double delta_deg;       /* angular difference */
} meg_alignment_t;

/* Get number of sites. */
int meg_site_count(void);

/* Get site by index. */
meg_site_t meg_site_get(int index);

/* Get event name as string. */
const char *meg_event_name(meg_event_t event);

/* Compute sunrise azimuth for a given latitude and solar declination.
 * Returns degrees from north (0=N, 90=E, 180=S, 270=W). */
double meg_sunrise_azimuth(double lat_deg, double declination_deg);

/* Compute sunset azimuth (= 360 - sunrise azimuth). */
double meg_sunset_azimuth(double lat_deg, double declination_deg);

/* Compute moonrise azimuth at maximum lunar standstill.
 * Moon declination extremes: +/- 28.585 degrees (major standstill). */
double meg_lunar_standstill_azimuth(double lat_deg, int north);

/* Solar declination at solstice/equinox. */
double meg_solstice_declination(int summer);
double meg_equinox_declination(void);

/* Check if a site is aligned at a given Julian Day.
 * Uses solar declination from solar_events.h. */
meg_alignment_t meg_check_alignment(int site_index, double jd);

/* Check all sites, return count of currently aligned sites.
 * Writes aligned site indices to out[], returns count. */
int meg_aligned_now(double jd, int *out, int max_out);

/* Days until next alignment for a site (searches forward from jd).
 * Returns -1 if event doesn't occur within 400 days. */
int meg_days_to_alignment(int site_index, double jd);

#endif /* TIME_MEGALITHIC_H */
```

## DONE WHEN

- [ ] 5+ megalithic sites with full alignment data
- [ ] Sunrise/sunset azimuth computation correct (verified against known values)
- [ ] Lunar standstill azimuth correct
- [ ] Alignment checking works for all event types
- [ ] meg_days_to_alignment finds next solstice/equinox
- [ ] >= 25 tests covering: azimuth math, site data, alignment checks, edge cases
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- `#define PI 3.14159265358979323846` (no M_PI)
- No malloc, no globals, no side effects
- Depends on solar_events.h for se_solar_declination()
- All site data as static const
- Azimuth formula: sunrise_az = arccos(sin(dec)/cos(lat)) for flat horizon
- Obliquity of ecliptic: 23.4393 degrees (J2000 standard)
- Major lunar standstill: moon declination ±28.585° (obliquity + lunar inclination 5.145°)
