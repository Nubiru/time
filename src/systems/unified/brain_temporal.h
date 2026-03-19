/* brain_temporal.h — Temporal context for brain insights (Track 26, L1.3)
 *
 * Answers "when did this convergence last happen?" and "when will it
 * happen again?" by scanning nearby Julian Days for similar patterns.
 *
 * Uses convergence_detect to sample convergence strength at regular
 * intervals, finding dates with similar or higher event counts.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_TEMPORAL_H
#define TIME_BRAIN_TEMPORAL_H

#include "brain_types.h"

/* Maximum scan range (days) in each direction */
#define BR_TEMPORAL_MAX_RANGE 36525  /* ~100 years */

/* Default scan range */
#define BR_TEMPORAL_DEFAULT_RANGE 18263  /* ~50 years */

/* Scan step size (days) — checks every N days for efficiency */
#define BR_TEMPORAL_STEP 1

/* Result of a temporal scan */
typedef struct {
    double  last_jd;          /* JD of most recent similar pattern (0 if none) */
    double  next_jd;          /* JD of next similar pattern (0 if none) */
    double  recurrence_days;  /* average days between occurrences (0 if < 2 found) */
    int     occurrences;      /* how many similar events found in scan range */
    int     scan_range;       /* how many days were scanned in each direction */
} br_temporal_t;

/* Find when a convergence of at least min_strength last occurred
 * before jd, scanning backwards up to range_days.
 * Returns JD of last occurrence, or 0.0 if none found. */
double br_temporal_last(double jd, double min_strength, int range_days);

/* Find when a convergence of at least min_strength next occurs
 * after jd, scanning forwards up to range_days.
 * Returns JD of next occurrence, or 0.0 if none found. */
double br_temporal_next(double jd, double min_strength, int range_days);

/* Estimate recurrence period for a given convergence strength.
 * Scans ±range_days around jd, counts occurrences, returns
 * average days between them. Returns 0.0 if fewer than 2 found. */
double br_temporal_recurrence(double jd, double min_strength, int range_days);

/* Full temporal analysis for current convergence strength.
 * Convenience function that calls last/next/recurrence. */
br_temporal_t br_temporal_analyze(double jd, double min_strength,
                                  int range_days);

#endif /* TIME_BRAIN_TEMPORAL_H */
