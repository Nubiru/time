/* precession_detect.h — Precession Encoder Detector
 *
 * Pure investigative module testing whether ancient time systems encode
 * the axial precession cycle (~25,772 years). For each registered system,
 * tests fundamental cycle lengths x integer multipliers against the
 * precession period. Reports match quality (error percentage) and
 * generates cross-cultural precession encoding analysis.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_PRECESSION_DETECT_H
#define TIME_PRECESSION_DETECT_H

#define PI 3.14159265358979323846

/* Current best estimate of precession period (years) */
#define PRECESSION_PERIOD_YEARS 25772.0

/* Degrees per year of precession (360 / 25772) */
#define PRECESSION_RATE_DEG_PER_YEAR (360.0 / PRECESSION_PERIOD_YEARS)

/* One "age" = 1/12 of precession (zodiacal age) */
#define PRECESSION_AGE_YEARS (PRECESSION_PERIOD_YEARS / 12.0)

/* One degree of precession in years */
#define PRECESSION_YEARS_PER_DEGREE_VAL (PRECESSION_PERIOD_YEARS / 360.0)

/* Maximum registered systems to test */
#define PRECESSION_MAX_SYSTEMS 24
#define PRECESSION_MAX_MATCHES 8

/* A cycle-multiplier combination that approximates precession */
typedef struct {
    double cycle_length;       /* base cycle in years */
    int multiplier;            /* integer multiplier */
    double product;            /* cycle_length * multiplier */
    double error_years;        /* absolute error from precession */
    double error_percent;      /* error as percentage of precession */
} precession_match_t;

/* A cultural system with its precession encoding candidates */
typedef struct {
    const char *system_name;   /* "Mayan", "Hindu", "Hebrew", etc. */
    const char *culture;       /* broader culture area */
    int match_count;           /* number of matches found */
    precession_match_t matches[PRECESSION_MAX_MATCHES];
    precession_match_t best;   /* lowest error_percent match */
    double best_error_percent; /* convenience: best.error_percent */
} precession_system_t;

/* Full precession analysis report */
typedef struct {
    int system_count;          /* how many systems tested */
    precession_system_t systems[PRECESSION_MAX_SYSTEMS];
    int total_matches;         /* matches across all systems */
    const char *best_system;   /* system with lowest error */
    double best_error_percent; /* lowest error found */
} precession_report_t;

/* Test a single cycle x multiplier against precession.
 * cycle_years: base cycle length in years.
 * multiplier: integer multiplier (1 to ~1000).
 * Returns match result. */
precession_match_t precession_test(double cycle_years, int multiplier);

/* Test all multipliers (1..max_mult) for a cycle against precession.
 * Returns best match found. */
precession_match_t precession_best_match(double cycle_years, int max_mult);

/* Is this match within threshold? (error_percent < threshold) */
int precession_is_match(precession_match_t match, double threshold_percent);

/* Degrees of precession per cycle_length years. */
double precession_degrees(double cycle_years);

/* How many complete cycles of cycle_years fit in one precession? */
double precession_cycles_per_precession(double cycle_years);

/* Current precession angle from J2000 epoch.
 * years_from_j2000: positive = future, negative = past.
 * Returns angle in degrees (0-360). */
double precession_angle(double years_from_j2000);

/* Current zodiacal age (which 30-degree sector).
 * Returns 0-11. The "Age of Aquarius" question. */
int precession_current_age(double years_from_j2000);

/* Name of zodiacal age (0="Pisces" at J2000, going backward through zodiac). */
const char *precession_age_name(int age_index);

/* Get pre-registered system count. */
int precession_system_count(void);

/* Get pre-registered system by index. */
precession_system_t precession_system_get(int index);

/* Generate full report testing all registered systems.
 * threshold_percent: maximum error to count as a match (e.g., 1.0 = 1%).
 * max_mult: maximum multiplier to search.
 * Returns full report. */
precession_report_t precession_full_report(double threshold_percent, int max_mult);

/* Years per degree of precession (convenience). */
double precession_years_per_degree(void);

#endif /* TIME_PRECESSION_DETECT_H */
