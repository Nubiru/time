/* precession_detect.h -- Precession Encoder Detector
 *
 * Pure investigative module testing whether ancient time systems encode
 * the axial precession cycle (~25,772 years). Registers known cycles
 * from multiple cultures, tests each against precession via integer
 * multiples, and generates a report ranking systems by encoding accuracy.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

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

/* Years per degree of precession (25772 / 360). */
double pd_years_per_degree(void);

#endif /* TIME_PRECESSION_DETECT_H */
