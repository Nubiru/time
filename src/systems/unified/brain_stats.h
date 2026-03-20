/* brain_stats.h — Statistical convergence analysis (Track 26)
 *
 * Statistical queries on convergence data:
 *   - Percentile ranking ("How rare is today?")
 *   - Year score comparison ("Which year is most aligned?")
 *   - Signature search ("Last time these systems aligned?")
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Uses brain_archive for year-scanning. */

#ifndef TIME_BRAIN_STATS_H
#define TIME_BRAIN_STATS_H

#include "brain_archive.h"

/* Maximum years in a comparison */
#define BR_STATS_MAX_YEARS 16

/* Percentile result for a single day */
typedef struct {
    double  jd;
    int     system_count;       /* this day's convergence count */
    int     rank;               /* 1 = highest convergence */
    int     total_days;         /* how many days in the comparison set */
    double  percentile;         /* 0.0-100.0 (top N%) */
} br_stats_percentile_t;

/* Year score for comparison */
typedef struct {
    int     year;
    int     total_convergence;  /* sum of system_count across all days */
    int     notable_days;       /* days with 3+ systems */
    int     rare_days;          /* days with 5+ systems */
    int     max_systems;        /* highest single-day count */
} br_stats_year_score_t;

/* Multi-year comparison result */
typedef struct {
    br_stats_year_score_t  years[BR_STATS_MAX_YEARS];
    int                    count;
    int                    best_year_index;    /* highest total_convergence */
    int                    worst_year_index;   /* lowest total_convergence */
} br_stats_year_compare_t;

/* Signature match result */
typedef struct {
    double  jd;                 /* when the match occurred */
    int     matched_systems;    /* how many of the target systems matched */
    int     total_systems;      /* total systems active on that day */
} br_stats_signature_match_t;

/* Compute percentile ranking of a JD within a year.
 * Scans all 365 days of the year containing jd,
 * ranks today's convergence against them.
 * out must be valid. Returns 1 on success. */
int br_stats_percentile(double jd, br_stats_percentile_t *out);

/* Compute convergence score for a single year (Jan 1 - Dec 31).
 * Returns 1 on success. */
int br_stats_year_score(int year, br_stats_year_score_t *out);

/* Compare multiple years (start_year to start_year + count - 1).
 * count must be <= BR_STATS_MAX_YEARS.
 * Returns number of years compared. */
int br_stats_year_compare(int start_year, int count,
                          br_stats_year_compare_t *out);

/* Search backward from jd for the last day when a set of systems
 * all converged simultaneously. systems[] lists cd_system_t IDs.
 * Scans up to max_days backward.
 * Returns JD of match, or 0.0 if not found. */
double br_stats_signature_last(double jd, const int *systems,
                               int system_count, int max_days);

/* Search forward from jd for the next matching signature.
 * Same parameters as signature_last but scans forward.
 * Returns JD of match, or 0.0 if not found. */
double br_stats_signature_next(double jd, const int *systems,
                               int system_count, int max_days);

#endif /* TIME_BRAIN_STATS_H */
