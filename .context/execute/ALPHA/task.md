# Task: Decan Star Data

**Agent**: ALPHA
**Roadmap Reference**: Track 43.2 — "Agent: Decan Star Data (ALPHA)"
**Date**: 2026-03-07
**Status**: CLAIMED

## Goal

Pure data module providing the 36 Egyptian decans as star groups with ecliptic longitude ranges, principal star identifications, and rising-time computation. Enables "which decan rules the current hour of the night" (ancient Egyptian star clock) and cross-references to zodiac signs (3 decans per sign). Foundational data for Track 43.3 (Egyptian Display) and Track 44 (Tarot decan cards).

## READ FIRST

- `src/render/star_catalog.h` — star_entry_t, star naming pattern
- `src/math/sidereal.h` — lst_degrees(), gmst_degrees() for sidereal time
- `src/systems/astrology/zodiac.h` — zodiac_sign(), zodiac_decan() for cross-reference

## Files to Create

- `src/render/decan_stars.h`
- `src/render/decan_stars.c`
- `tests/render/test_decan_stars.c`

## API

```c
#ifndef TIME_DECAN_STARS_H
#define TIME_DECAN_STARS_H

#define DECAN_COUNT 36
#define DECAN_DEGREES 10.0  /* each decan spans 10 degrees of ecliptic */

/* Element associated with each decan (via zodiac triplicity) */
typedef enum {
    DECAN_ELEM_FIRE = 0,
    DECAN_ELEM_EARTH,
    DECAN_ELEM_AIR,
    DECAN_ELEM_WATER,
    DECAN_ELEM_COUNT
} decan_element_t;

/* Decan data */
typedef struct {
    int index;                  /* 0-35 */
    const char *egyptian_name;  /* traditional Egyptian name */
    const char *principal_star; /* modern star identification (brightest in group) */
    double ecl_lon_start;       /* ecliptic longitude start (degrees, 0=vernal equinox) */
    double ecl_lon_end;         /* ecliptic longitude end (degrees) */
    int zodiac_sign;            /* 0=Aries..11=Pisces */
    int decan_in_sign;          /* 1, 2, or 3 within the sign */
    decan_element_t element;    /* element from zodiac triplicity */
    const char *description;    /* brief description of star group */
} decan_t;

/* Night hour data for the decan star clock */
typedef struct {
    int decan_index;            /* which decan is rising */
    double hour_angle;          /* hour angle of the decan's principal star */
    int is_visible;             /* 1 if above horizon */
} decan_hour_t;

/* Get total number of decans (always 36). */
int decan_count(void);

/* Get decan by index (0-35). Returns sentinel with index=-1 if invalid. */
decan_t decan_get(int index);

/* Get decan for a given ecliptic longitude (0-360 degrees).
 * Returns decan index (0-35). */
int decan_for_ecliptic_longitude(double ecl_lon_deg);

/* Get the 3 decans for a zodiac sign (0=Aries..11=Pisces).
 * Writes up to 3 indices to out[]. Returns count written (3, or 0 if invalid). */
int decan_for_sign(int sign, int *out, int max_out);

/* Decan element name as string. */
const char *decan_element_name(decan_element_t elem);

/* Compute which decan is currently rising at observer location.
 * lst_deg: local sidereal time in degrees (0-360).
 * lat_deg: observer latitude in degrees.
 * Returns decan index (0-35). */
int decan_rising_now(double lst_deg, double lat_deg);

/* Get the decan star clock: which decan rules each of the 12 night hours.
 * lst_deg: local sidereal time in degrees at sunset.
 * lat_deg: observer latitude.
 * Writes up to max_out decan_hour_t entries to out[].
 * Returns count written (up to 12). */
int decan_night_hours(double lst_deg, double lat_deg, decan_hour_t *out, int max_out);

/* Get the "ruler" decan for a given night hour (0-11) at a given LST.
 * Returns decan index, or -1 if invalid. */
int decan_for_night_hour(double lst_deg, double lat_deg, int hour);

/* Principal star ecliptic longitude for a decan (midpoint of range). */
double decan_star_longitude(int index);

#endif /* TIME_DECAN_STARS_H */
```

## DONE WHEN

- [ ] 36 decans with Egyptian names, principal stars, ecliptic longitude ranges
- [ ] `decan_for_ecliptic_longitude()` maps any longitude to correct decan
- [ ] `decan_for_sign()` returns 3 decans per zodiac sign
- [ ] `decan_rising_now()` computes rising decan from LST + latitude
- [ ] `decan_night_hours()` fills 12-hour star clock array
- [ ] `decan_for_night_hour()` returns ruler for specific night hour
- [ ] >= 30 tests covering: data integrity, longitude mapping, sign mapping, rising computation, night hours, edge cases
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- `#define PI 3.14159265358979323846` (no M_PI)
- No malloc, no globals, no side effects
- Standalone module (no external dependencies — sidereal/zodiac referenced for design but not linked)
- All decan data as static const arrays
- Egyptian decan names from Neugebauer & Parker "Egyptian Astronomical Texts" tradition
- Principal stars mapped to modern identifications where scholarly consensus exists
- Ecliptic longitude ranges: decan 0 = 0-10 deg (Aries 1st decan), decan 35 = 350-360 deg (Pisces 3rd decan)
