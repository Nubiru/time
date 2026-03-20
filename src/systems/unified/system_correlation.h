/* system_correlation.h — Cross-system co-firing analysis (Track 26, S100)
 *
 * Scans N years of convergence data to build a 21x21 correlation matrix.
 * Each cell [i][j] = frequency that systems i and j are simultaneously
 * significant, expressed as a ratio (0.0 to 1.0).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Caller provides the result buffer. */

#ifndef TIME_SYSTEM_CORRELATION_H
#define TIME_SYSTEM_CORRELATION_H

#include "convergence_detect.h"

/* Correlation matrix result */
typedef struct {
    double  matrix[CD_SYS_COUNT][CD_SYS_COUNT]; /* co-firing frequency 0.0-1.0 */
    int     solo_count[CD_SYS_COUNT];            /* how many days each system fires */
    int     days_scanned;
    int     years_scanned;
} sc_corr_result_t;

/* Top co-firing pair */
typedef struct {
    int     system_a;
    int     system_b;
    double  frequency;      /* 0.0-1.0 */
    int     co_fire_days;   /* absolute count */
} sc_corr_pair_t;

/* Compute correlation matrix by scanning N years starting from start_year.
 * Scans 365 days per year.
 * Returns number of days scanned. */
int sc_corr_compute(int start_year, int years, sc_corr_result_t *out);

/* Get the top N most correlated pairs from a computed matrix.
 * out must point to array of at least max_pairs elements.
 * Returns number of pairs found. */
int sc_corr_top_pairs(const sc_corr_result_t *result,
                      sc_corr_pair_t *out, int max_pairs);

/* Get co-firing frequency for a specific pair (order-independent). */
double sc_corr_frequency(const sc_corr_result_t *result,
                         cd_system_t a, cd_system_t b);

/* Get the number of days a specific system fires (solo_count). */
int sc_corr_solo(const sc_corr_result_t *result, cd_system_t sys);

#endif /* TIME_SYSTEM_CORRELATION_H */
