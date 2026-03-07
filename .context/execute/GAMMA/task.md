# Task: Calendar Reform History

**Agent**: GAMMA
**Roadmap Reference**: Track 29.2 — "Agent: Calendar Reform History (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data module cataloging calendar systems and reform proposals throughout history. Documents the 360-day natural year, the 13-moon calendar (Arguelles), the Gregorian reform (1582), the World Calendar movement (1920s), and the concept of seasonal hours. Provides computational functions for seasonal hour length at any latitude/JD. Shows the "disorder of time" — why our current calendar has months of unequal length.

## READ FIRST

- `src/systems/gregorian/gregorian.h` — existing Gregorian module
- `src/systems/astronomy/solar_events.h` — sunrise/sunset for seasonal hours

## Files to Create

- `src/systems/unified/calendar_reform.h`
- `src/systems/unified/calendar_reform.c`
- `tests/systems/unified/test_calendar_reform.c`

## API

```c
#ifndef TIME_CALENDAR_REFORM_H
#define TIME_CALENDAR_REFORM_H

#define PI 3.14159265358979323846

/* Calendar system types */
typedef enum {
    CAL_TYPE_LUNAR = 0,       /* Pure lunar */
    CAL_TYPE_SOLAR,           /* Pure solar */
    CAL_TYPE_LUNISOLAR,       /* Lunisolar hybrid */
    CAL_TYPE_PROPOSED,        /* Reform proposal (never adopted) */
    CAL_TYPE_ADOPTED,         /* Reform that was implemented */
    CAL_TYPE_COUNT
} calendar_type_t;

/* A historical calendar system or reform proposal */
typedef struct {
    int id;
    const char *name;              /* "Gregorian", "13 Moon", "World Calendar", etc. */
    int year;                      /* Year proposed/adopted (negative=BCE) */
    const char *originator;        /* Person/body who proposed it */
    const char *culture;           /* Culture/civilization */
    calendar_type_t type;
    int days_per_year;             /* Base days (360, 364, 365, etc.) */
    int months;                    /* Number of months */
    int intercalary_days;          /* Extra days (epagomenal, Day Out of Time, etc.) */
    const char *month_structure;   /* "12x30", "13x28+1", "12 unequal", etc. */
    const char *description;       /* Full description */
    const char *problem_solved;    /* What calendar problem this addresses */
    const char *weakness;          /* Known flaw or criticism */
} calendar_system_t;

/* Get total calendar systems cataloged. */
int calendar_system_count(void);

/* Get system by index. Returns entry with id=-1 if invalid. */
calendar_system_t calendar_system_get(int index);

/* Get systems by type. Fills out_indices. Returns count found. */
int calendar_systems_by_type(calendar_type_t type, int *out_indices, int out_max);

/* Seasonal hour length in minutes.
 * In seasonal/temporal hours, day and night each have 12 hours.
 * Day hours expand in summer, shrink in winter.
 * lat_deg: latitude (-90 to 90).
 * day_of_year: 1-365.
 * is_day_hour: 1 for day hour, 0 for night hour.
 * Returns length in minutes (60.0 at equinox everywhere). */
double seasonal_hour_length(double lat_deg, int day_of_year, int is_day_hour);

/* Day length in hours at given latitude and day of year.
 * Uses solar declination approximation.
 * Returns 0-24 (handles polar day/night). */
double calendar_day_length(double lat_deg, int day_of_year);

/* Night length in hours. Complement of day length. */
double calendar_night_length(double lat_deg, int day_of_year);

/* Calendar type name string. */
const char *calendar_type_name(calendar_type_t type);

/* How many distinct cultures are represented? */
int calendar_culture_count(void);

/* Get distinct culture by index. */
const char *calendar_culture_get(int index);

/* Get the Gregorian month lengths (static data). Returns days for month 1-12. */
int gregorian_month_days(int month, int is_leap);

/* Year length comparison: return exact year length for a system index (in days, as double). */
double calendar_year_length(int index);

#endif /* TIME_CALENDAR_REFORM_H */
```

## Calendar Systems Data (minimum 10 entries)

1. **Egyptian Civil Calendar** (~3000 BCE): 12x30 + 5 epagomenal = 365 days. No leap year. Drifted 1 day per 4 years. First solar calendar.
2. **Babylonian Calendar** (~2000 BCE): Lunisolar, 12 months of 29-30 days. Intercalary month added when needed. King's decree.
3. **Roman Republican Calendar** (~713 BCE): 10 months originally (304 days), later 12 months. Pontifex controlled intercalation — political tool.
4. **Julian Calendar** (46 BCE, Julius Caesar): 365.25 days (leap year every 4 years). Reformed by Sosigenes of Alexandria. 11.5 min/year drift.
5. **Gregorian Calendar** (1582, Pope Gregory XIII): 365.2425 days. Skip 3 leap years per 400. 10 days deleted (Oct 4→15, 1582). 26 sec/year drift.
6. **French Republican Calendar** (1793): 12x30 + 5/6 complementary days. Decimal time: 10 hours/day, 100 min/hour. Abolished 1805.
7. **13 Moon Calendar** (1992, Jose Arguelles): 13x28 + 1 Day Out of Time = 365. Perfect symmetry. Every month starts on the same day. Dreamspell system.
8. **World Calendar** (1930, Elisabeth Achelis): 4 quarters of 91 days. Every quarter: 31+30+30. 1 extra "Worldsday" (Dec 31). League of Nations nearly adopted.
9. **International Fixed Calendar** (1902, Moses Cotsworth): 13x28 + 1 Year Day = 365. 13th month "Sol" between June and July. Used by Kodak internally until 1989.
10. **Seasonal Hours** (ancient, widespread): 12 hours of daylight + 12 hours of night. Hour length varies by season and latitude. Used in ancient Rome, Japan (until 1873), Jewish law.

## DONE WHEN

- [ ] >= 10 calendar systems with descriptions
- [ ] seasonal_hour_length computation (latitude + day of year)
- [ ] Day/night length calculation
- [ ] Filter by type
- [ ] Culture listing
- [ ] Year length comparison
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- `#define PI 3.14159265358979323846` (no M_PI)
- No malloc, no globals, no side effects
- Standalone module (no compile-time dependencies)
- `#include <math.h>` for sin(), cos(), acos() — link with -lm
