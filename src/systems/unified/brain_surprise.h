/* brain_surprise.h — Anomaly detector for convergence patterns (Track 26, L3.1)
 *
 * Detects when a Julian Day has unusually high convergence activity
 * by comparing against a statistical baseline. Uses a sampling window
 * to compute mean and standard deviation of convergence significance,
 * then flags days that exceed a configurable z-score threshold.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_SURPRISE_H
#define TIME_BRAIN_SURPRISE_H

#include "brain_types.h"

/* Baseline sampling parameters */
#define BR_SURPRISE_SAMPLE_DAYS  365  /* days to sample for baseline */
#define BR_SURPRISE_SAMPLE_STEP    7  /* sample every N days for speed */

/* Surprise detection result */
typedef struct {
    double  significance;    /* today's convergence significance (0.0-1.0) */
    double  baseline_mean;   /* average significance across sample */
    double  baseline_stddev; /* standard deviation of significance */
    double  z_score;         /* (significance - mean) / stddev */
    int     is_surprise;     /* 1 if z_score >= threshold */
    char    headline[BR_HEADLINE_MAX];
} br_surprise_t;

/* Detect if a Julian Day is a convergence surprise.
 * Samples ±sample_days at sample_step intervals to build baseline,
 * then compares today's significance against it.
 * z_threshold: typically 2.0 (notable) or 3.0 (rare).
 * Returns 1 if the day is a surprise, 0 otherwise. */
int br_surprise_detect(double jd, double z_threshold,
                       br_surprise_t *out);

/* Quick check: is this JD above the 2-sigma threshold?
 * Convenience wrapper for br_surprise_detect with z=2.0. */
int br_surprise_check(double jd);

/* Compute baseline statistics for a JD range.
 * Fills mean and stddev with convergence significance stats. */
void br_surprise_baseline(double center_jd, int sample_days,
                          int sample_step,
                          double *mean, double *stddev);

#endif /* TIME_BRAIN_SURPRISE_H */
