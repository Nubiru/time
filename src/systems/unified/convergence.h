/* convergence.h -- Cross-Calendar Convergence Detector
 *
 * Scans a Julian Day across 11 calendar/astronomical systems for significant
 * dates (new year, new month, holy days, cycle starts, lunar phases, etc.),
 * then detects "convergence events" when 3+ systems have active signals.
 *
 * STANDALONE: all calendar math is approximated inline.
 * Only depends on math.h and string.h — no external calendar modules.
 *
 * All functions are pure: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CONVERGENCE_H
#define TIME_CONVERGENCE_H

#define CONV_MAX_SYSTEMS    16
#define CONV_MAX_SIGNALS    32

typedef enum {
    CONV_SYS_GREGORIAN = 0,
    CONV_SYS_HEBREW,
    CONV_SYS_ISLAMIC,
    CONV_SYS_CHINESE,
    CONV_SYS_TZOLKIN,
    CONV_SYS_HAAB,
    CONV_SYS_ICHING,
    CONV_SYS_BUDDHIST,
    CONV_SYS_HINDU,
    CONV_SYS_LUNAR,
    CONV_SYS_ZODIAC,
    CONV_SYS_COUNT
} conv_system_t;

typedef enum {
    CONV_SIG_NEW_YEAR = 0,
    CONV_SIG_NEW_MONTH,
    CONV_SIG_HOLY_DAY,
    CONV_SIG_CYCLE_START,
    CONV_SIG_FULL_MOON,
    CONV_SIG_NEW_MOON,
    CONV_SIG_SOLSTICE,
    CONV_SIG_SACRED_NUMBER,
    CONV_SIG_TONE_ONE,
    CONV_SIG_SABBATH,
    CONV_SIG_COUNT
} conv_signal_type_t;

typedef struct {
    conv_system_t system;
    conv_signal_type_t type;
    const char *description;
    int weight;  /* 1=minor, 2=moderate, 3=major */
} conv_signal_t;

typedef struct {
    double jd;
    conv_signal_t signals[CONV_MAX_SIGNALS];
    int signal_count;
    int systems_active;
    int total_weight;
    int is_convergence;  /* 1 if systems_active >= 3 */
} conv_scan_t;

/* Name lookups */
const char *conv_system_name(conv_system_t sys);
const char *conv_signal_type_name(conv_signal_type_t type);

/* Scan a single date for signals across all systems. */
conv_scan_t conv_scan_date(double jd);

/* Check one system for signals on a given JD.
 * Writes up to max_out signals into out. Returns signal count. */
int conv_check_system(conv_system_t sys, double jd,
                      conv_signal_t *out, int max_out);

/* Find the JD with the highest total_weight in range [jd_start, jd_end].
 * Scans day by day. Returns the JD of the best day. */
double conv_best_in_range(double jd_start, double jd_end);

/* Count convergence days (systems_active >= 3) in range. */
int conv_count_in_range(double jd_start, double jd_end);

/* Find the next convergence date after jd, scanning up to max_days.
 * Returns the JD, or 0.0 if none found. */
double conv_next_convergence(double jd, int max_days);

/* Find the previous convergence date before jd, scanning up to max_days.
 * Returns the JD, or 0.0 if none found. */
double conv_prev_convergence(double jd, int max_days);

#endif /* TIME_CONVERGENCE_H */
