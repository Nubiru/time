# Task: Historical Achievement Data

**Agent**: GAMMA
**Roadmap Reference**: Track 29.1 — "Agent: Historical Achievement Data (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data module cataloging ancient astronomers' computational achievements with their precision compared to modern values. Dramatizes the genius: Aryabhata (499 CE) computed Earth's circumference to 0.2% error with NO telescope. Hillel II (359 CE) determined the molad to 0.4 seconds/month accuracy. Maya astronomers tracked Venus cycles to 2-hour precision. Each entry carries: person, year, culture, what was computed, their value, the modern value, the error, and what tools they had available.

## READ FIRST

- `src/systems/unified/structural_map.h` — cross-system concordance patterns
- `data/contributors.json` — existing contributor entries for these astronomers

## Files to Create

- `src/systems/unified/achievement.h`
- `src/systems/unified/achievement.c`
- `tests/systems/unified/test_achievement.c`

## API

```c
#ifndef TIME_ACHIEVEMENT_H
#define TIME_ACHIEVEMENT_H

/* Achievement categories */
typedef enum {
    ACH_CAT_ORBITAL = 0,     /* Orbital period, year length */
    ACH_CAT_GEOMETRIC,       /* Earth size, distances */
    ACH_CAT_LUNAR,           /* Lunar cycle, eclipse prediction */
    ACH_CAT_PLANETARY,       /* Planet positions, cycles */
    ACH_CAT_CALENDAR,        /* Calendar accuracy, intercalation */
    ACH_CAT_MATHEMATICAL,    /* Mathematical methods, equations */
    ACH_CAT_COUNT
} achievement_category_t;

/* A single historical achievement */
typedef struct {
    int id;                        /* Unique index */
    const char *person;            /* "Aryabhata", "Hillel II", etc. */
    int year;                      /* Negative for BCE (e.g., -240 for 240 BCE) */
    const char *culture;           /* "Indian", "Jewish", "Maya", etc. */
    const char *description;       /* What was computed/discovered */
    double value_computed;         /* Their measured/computed value */
    double modern_value;           /* Modern accepted value */
    const char *unit;              /* "km", "days", "degrees", etc. */
    double absolute_error;         /* |computed - modern| */
    double relative_error_pct;     /* |(computed - modern) / modern| * 100 */
    const char *tools_available;   /* "naked eye", "gnomon", "armillary sphere", etc. */
    const char *tools_NOT_available; /* "telescope", "clock", etc. */
    achievement_category_t category;
    const char *significance;      /* Why this matters */
} achievement_t;

/* Total number of achievements. */
int achievement_count(void);

/* Get achievement by index. Returns entry with id=-1 if invalid. */
achievement_t achievement_get(int index);

/* Get achievements by culture. Fills out_indices (max out_max entries).
 * Returns count found. */
int achievement_by_culture(const char *culture, int *out_indices, int out_max);

/* Get achievements by person. Fills out_indices. Returns count found. */
int achievement_by_person(const char *person, int *out_indices, int out_max);

/* Get achievements by category. Fills out_indices. Returns count found. */
int achievement_by_category(achievement_category_t cat, int *out_indices, int out_max);

/* Error ratio: modern_value / absolute_error (how many times more precise than the error).
 * Higher = more impressive. Returns 0.0 if invalid or zero error. */
double achievement_error_ratio(int index);

/* Accuracy percentage: 100.0 - relative_error_pct. Returns 0.0 if invalid. */
double achievement_accuracy_pct(int index);

/* Category name string. Returns "?" for invalid. */
const char *achievement_category_name(achievement_category_t cat);

/* How many distinct cultures are represented? */
int achievement_culture_count(void);

/* Get distinct culture name by index. Returns "?" for invalid. */
const char *achievement_culture_get(int index);

/* How many distinct persons are represented? */
int achievement_person_count(void);

#endif /* TIME_ACHIEVEMENT_H */
```

## Achievement Data (minimum entries)

1. **Aryabhata (499 CE, Indian)**: Earth circumference 39,968 km (actual 40,075 km) — 0.27% error. Tools: gnomon, mathematical models. No telescope.
2. **Aryabhata (499 CE, Indian)**: Sidereal year 365.25858 days (actual 365.25636) — 0.00061% error (3.2 min). No clock.
3. **Aryabhata (499 CE, Indian)**: Equation of center (Kepler's equation equivalent) — 1200 years before Kepler.
4. **Eratosthenes (240 BCE, Greek)**: Earth circumference 39,375 km (actual 40,075) — 1.7% error. Tools: gnomon + geometry.
5. **Hipparchus (130 BCE, Greek)**: Tropical year 365.2467 days (actual 365.2422) — 6.5 min error. Tools: armillary sphere.
6. **Hipparchus (130 BCE, Greek)**: Precession rate ~46"/year (actual 50.3"/year) — 8.5% error. First to discover precession.
7. **Hillel II (359 CE, Jewish)**: Molad (lunar month) 29.530594 days (actual 29.530589) — 0.4 sec/month error.
8. **Al-Khwarizmi (820 CE, Arab)**: Planetary position tables used for 400+ years. Sine tables to 4 decimal places.
9. **Maya astronomers (~800 CE, Maya)**: Venus synodic cycle 583.92 days (actual 583.93) — 2 hour error per cycle.
10. **Maya astronomers (~800 CE, Maya)**: Eclipse prediction over 405 lunations (32.8 years) — accurate to hours.
11. **Ptolemy (150 CE, Greco-Egyptian)**: Obliquity of ecliptic 23.855° (actual for his era ~23.71°) — 0.15° error.
12. **Ulugh Beg (1437 CE, Timurid)**: Sidereal year 365.25636 days (actual 365.25636) — essentially exact. Tools: massive sextant.
13. **Copernicus (1543 CE, Polish)**: Sidereal year 365.25671 days — 0.3 sec error per day.
14. **Brahmagupta (628 CE, Indian)**: Lunar month 29.530582 days (actual 29.530589) — 0.6 sec error.
15. **Su Song (1088 CE, Chinese)**: Tropical year 365.2436 days — ~2 min error. Water-powered clock tower.

## DONE WHEN

- [ ] >= 15 achievement entries with verified data
- [ ] Error ratios computed correctly
- [ ] Filter by culture, person, category
- [ ] Accuracy percentage calculation
- [ ] Distinct culture and person counts
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- Standalone module (no compile-time dependencies)
- out_indices arrays provided by caller (stack allocated)
- Data sources: standard astronomical history (public domain)
