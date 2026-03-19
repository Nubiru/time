/* depth_temporal.h — Temporal exploration for depth layers
 *
 * Given a knowledge system and its current cycle value, computes:
 * - When does this state recur? (next occurrence)
 * - What was the state N years ago?
 * - How long is the full cycle?
 *
 * Enables the "time travel" dimension of depth — not just what IS today,
 * but when was it before and when will it come again.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Known cycle lengths for each knowledge system. */

#ifndef TIME_DEPTH_TEMPORAL_H
#define TIME_DEPTH_TEMPORAL_H

#include "today_summary.h"  /* ts_system_t, TS_SYS_COUNT */

/* System cycle info */
typedef struct {
    int system_id;              /* ts_system_t value, or -1 if invalid */
    const char *cycle_name;     /* "Tzolkin", "Hexagram Cycle", etc. */
    double cycle_days;          /* Length of one full cycle in days */
    double cycle_years;         /* Approximate cycle in years (for display) */
    const char *description;    /* "260 sacred days", "64-day hexagram rotation", etc. */
} dt_cycle_t;

/* Recurrence result */
typedef struct {
    double next_jd;             /* JD of next recurrence (0 if unknown) */
    double prev_jd;             /* JD of previous recurrence (0 if unknown) */
    double days_until;          /* Days until next recurrence */
    double days_since;          /* Days since last occurrence */
    int occurrences_per_year;   /* Approximate occurrences per year */
} dt_recurrence_t;

/* Temporal snapshot — state at a different time */
typedef struct {
    int years_offset;           /* How many years ago/forward */
    char state[128];            /* "Kin 207" or "Hexagram 23" etc. */
    int has_data;               /* 1 if computed, 0 if system unsupported */
} dt_snapshot_t;

/* Returns cycle data for a ts_system_t (0-19).
 * Invalid system_id -> system_id=-1, cycle_name="?". */
dt_cycle_t dt_cycle_info(int system_id);

/* Computes next and previous recurrence based on cycle length.
 * If cycle_days == 0 (GEOLOGICAL, COSMIC): all zeros.
 * If current_jd <= 0: all zeros. */
dt_recurrence_t dt_recurrence(int system_id, double current_jd);

/* Computes approximate state at (current_jd + years_offset * 365.2425).
 * has_data = 1 for valid systems, 0 for invalid. */
dt_snapshot_t dt_snapshot(int system_id, double current_jd, int years_offset);

/* Returns 20 (TS_SYS_COUNT). */
int dt_cycle_count(void);

#endif /* TIME_DEPTH_TEMPORAL_H */
