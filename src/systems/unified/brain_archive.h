/* brain_archive.h — Convergence archive scanner (Track 26)
 *
 * Scans date ranges for convergence patterns across all knowledge systems.
 * Pre-computes convergence data for a year, finds notable days, returns
 * statistics. Used for the "Historical Convergence Database" feature.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Caller provides all buffers. */

#ifndef TIME_BRAIN_ARCHIVE_H
#define TIME_BRAIN_ARCHIVE_H

#include "brain_types.h"
#include "convergence_detect.h"

/* Maximum days in a single scan (1 year + margin) */
#define BR_ARCHIVE_MAX_DAYS 400

/* Maximum notable results */
#define BR_ARCHIVE_MAX_NOTABLE 64

/* Summary of convergence data for a single day */
typedef struct {
    double          jd;
    int             system_count;       /* how many systems are significant */
    cd_strength_t   strength;           /* overall convergence strength tier */
    double          significance;       /* 0.0-1.0 */
    int             top_systems[8];     /* up to 8 most significant system IDs */
    int             top_system_count;
} br_archive_day_t;

/* Result of an archive scan */
typedef struct {
    br_archive_day_t  days[BR_ARCHIVE_MAX_DAYS];
    int               count;
    double            start_jd;
    int               days_scanned;
    int               notable_count;    /* days with 3+ systems */
    int               max_systems;      /* highest system_count found */
    double            best_jd;          /* JD of the day with max_systems */
} br_archive_result_t;

/* Scan a date range for convergence data.
 * Fills out->days[] with one entry per day from start_jd.
 * days must be <= BR_ARCHIVE_MAX_DAYS.
 * Returns number of days scanned. */
int br_archive_scan(double start_jd, int days, br_archive_result_t *out);

/* Filter an archive for notable days (system_count >= min_systems).
 * Copies matching days to out[], up to max_out entries.
 * Returns number of notable days found. */
int br_archive_notable(const br_archive_result_t *archive,
                       int min_systems,
                       br_archive_day_t *out, int max_out);

/* Get the day with the highest convergence in the archive.
 * Returns pointer to the day within archive->days[], or NULL if empty. */
const br_archive_day_t *br_archive_best(const br_archive_result_t *archive);

/* Count days in the archive with system_count >= threshold. */
int br_archive_count_above(const br_archive_result_t *archive,
                           int min_systems);

/* Compute year statistics: how many days have 2+, 3+, 4+, 5+ systems.
 * Fills counts[0..4] for thresholds 2,3,4,5,6. */
void br_archive_year_stats(const br_archive_result_t *archive,
                           int counts[5]);

#endif /* TIME_BRAIN_ARCHIVE_H */
