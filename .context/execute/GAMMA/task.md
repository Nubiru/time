# Task: Precession Encoder Detector

**Agent**: GAMMA
**Roadmap Reference**: Track 26.4 — "Agent: Precession Encoder Detector (Agent A)"
**Date**: 2026-03-07
**Status**: CLAIMED

## Goal

Pure investigative module testing whether ancient time systems encode the precession cycle (~25,772 years). Registers known cycles from multiple cultures, tests each against precession via integer multiples, and generates a report ranking systems by encoding accuracy. The question: did ancient cultures independently discover precession?

## READ FIRST

- `src/systems/unified/number_scanner.h` — related cycle scanning (already delivered)
- `src/systems/unified/structural_map.h` — cross-system mappings (already delivered)

## Files to Create

- `src/systems/unified/precession_detect.h`
- `src/systems/unified/precession_detect.c`
- `tests/systems/unified/test_precession_detect.c`

## API

```c
#ifndef TIME_PRECESSION_DETECT_H
#define TIME_PRECESSION_DETECT_H

#define PRECESSION_CYCLE_YEARS 25772.0
#define PRECESSION_MAX_CYCLES 32
#define PRECESSION_MAX_MATCHES 16

/* A cultural time cycle to test against precession */
typedef struct {
    const char *name;        /* cycle name */
    const char *culture;     /* originating culture */
    double period_years;     /* cycle length in years */
    int known_multiplier;    /* known integer multiple (0 = search) */
} pd_cycle_t;

/* Result of testing one cycle against precession */
typedef struct {
    const char *cycle_name;
    const char *culture;
    double period_years;
    int multiplier;          /* how many cycles to approximate precession */
    double product_years;    /* period * multiplier */
    double error_percent;    /* abs(product - precession) / precession * 100 */
    int rank;                /* 1 = best match, higher = worse */
} pd_match_t;

/* Full precession report */
typedef struct {
    pd_match_t matches[PRECESSION_MAX_MATCHES];
    int match_count;
    double best_error_percent;
    const char *best_culture;
} pd_report_t;

/* Get number of registered cycles. */
int pd_cycle_count(void);

/* Get cycle by index. */
pd_cycle_t pd_cycle_get(int index);

/* Test a single cycle against precession.
 * If multiplier > 0, test that specific multiple.
 * If multiplier == 0, find best integer multiple. */
pd_match_t pd_test(double period_years, int multiplier,
                    const char *name, const char *culture);

/* Find best integer multiplier for a period to match precession.
 * Searches multipliers 1-1000. */
int pd_best_multiplier(double period_years);

/* Error percentage: abs(product - precession) / precession * 100 */
double pd_error(double product_years);

/* Generate full report: test all registered cycles, rank by accuracy. */
pd_report_t pd_report(void);

/* Get match by rank (1-based). Returns empty match if out of range. */
pd_match_t pd_report_rank(const pd_report_t *report, int rank);

/* Count how many cultures have encodings within a threshold (e.g., 1.0%). */
int pd_cultures_within(const pd_report_t *report, double max_error_percent);

/* Degrees of precession per year (360 / 25772). */
double pd_degrees_per_year(void);

/* Years per degree of precession (25772 / 360 ≈ 71.59). */
double pd_years_per_degree(void);

#endif /* TIME_PRECESSION_DETECT_H */
```

## Known Precession Encodings (test data)

| Culture | Cycle | Period (years) | Multiplier | Product | Error % |
|---------|-------|---------------|------------|---------|---------|
| Mayan | Long Count Great Cycle | 5,125.36 | 5 | 25,626.8 | 0.56% |
| Hindu | Precessional Age (Yuga subdivision) | 2,160 | 12 | 25,920 | 0.57% |
| Hebrew | Jubilee × months | 50 × 12 = 600 | 43 | 25,800 | 0.11% |
| Egyptian | Sothic cycle | 1,461 | 18 | 26,298 | 2.04% |
| Babylonian | Saros (eclipse) | 18.03 | 1,430 | 25,783 | 0.04% |
| Chinese | 60-year cycle | 60 | 429 | 25,740 | 0.12% |
| Greek | Great Year (Plato) | 25,920 | 1 | 25,920 | 0.57% |
| Buddhist | Kalpa subdivision | 4,320 | 6 | 25,920 | 0.57% |
| Islamic | 30-year Hijri cycle | 30 | 859 | 25,770 | 0.01% |

## DONE WHEN

- [ ] Registered cycle table with 9+ cultural cycles
- [ ] pd_test computes multiplier × period and error vs precession
- [ ] pd_best_multiplier searches for optimal integer multiple
- [ ] pd_report generates ranked report of all cycles
- [ ] Known encodings verified (Mayan 0.56%, Hebrew 0.11%, etc.)
- [ ] pd_cultures_within counts matches under threshold
- [ ] Precession constants correct (25,772 years, ~71.59 years/degree)
- [ ] >= 40 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- `#include <math.h>` for fabs, round
- `#include <string.h>` for strcmp
- Standalone module (no compile-time deps)
- All cycle data as `static const` arrays
