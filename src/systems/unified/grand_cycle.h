/* grand_cycle.h — Grand Cycle period calculator (Track 26, S99)
 *
 * Computes LCM of calendar cycle lengths to find the Grand Cycle:
 * how long until the exact same configuration recurs.
 *
 * Cycle lengths (days):
 *   Tzolkin: 260, Haab: 365, Calendar Round: 18980 (LCM of 260,365)
 *   I Ching: 64 hexagrams (as day cycle)
 *   Chinese sexagenary: 60 years (21915 days)
 *   Hebrew Metonic: 6940 days (19 years)
 *   Saros (eclipse): 6585 days
 *   Rokuyo: 6, French Decade: 10
 *
 * Pure functions: no GL, no malloc, no globals. */

#ifndef TIME_GRAND_CYCLE_H
#define TIME_GRAND_CYCLE_H

#include <stdint.h>

/* Maximum cycles in a computation */
#define GC_MAX_CYCLES 16

/* A cycle definition */
typedef struct {
    const char *name;
    int64_t     period_days;    /* cycle length in days (or x100 for fractional) */
    int         scale;          /* 1 = days, 100 = hundredths of a day */
} gc_cycle_t;

/* Grand cycle result */
typedef struct {
    int64_t     lcm_days;       /* LCM in days */
    double      lcm_years;      /* LCM / 365.25 */
    int         cycle_count;    /* how many cycles included */
    const char *description;    /* human-readable context */
} gc_result_t;

/* GCD of two positive integers. */
int64_t gc_gcd(int64_t a, int64_t b);

/* LCM of two positive integers. */
int64_t gc_lcm(int64_t a, int64_t b);

/* LCM of an array of cycle periods. Returns LCM in days. */
int64_t gc_lcm_array(const int64_t *periods, int count);

/* Get the built-in cycle definitions. Returns cycle count. */
int gc_get_cycles(gc_cycle_t *out, int max);

/* Compute the Grand Cycle from all built-in cycles.
 * Fills result with LCM and description. */
int gc_compute(gc_result_t *out);

/* Compute Grand Cycle from a subset of cycles.
 * cycles[] is array of gc_cycle_t, count is how many. */
int gc_compute_subset(const gc_cycle_t *cycles, int count, gc_result_t *out);

#endif /* TIME_GRAND_CYCLE_H */
