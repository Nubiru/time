/* calendar_epoch.h -- How one religion's festival reshaped global timekeeping.
 *
 * Pure module covering:
 * - Easter computation (Computus, Anonymous Gregorian algorithm / Meeus)
 * - Julian/Gregorian calendar drift (accumulated discrepancy)
 * - Dionysius Exiguus's 525 CE epoch error (AD/BC miscalculation)
 * - Historical epoch facts as static const data
 *
 * The Gregorian calendar exists because Easter needed to stay near the
 * spring equinox. The entire modern civil calendar is a side effect
 * of one religion's festival computation.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_CALENDAR_EPOCH_H
#define TIME_CALENDAR_EPOCH_H

/* Dionysius Exiguus placed Jesus's birth at 1 AD.
 * Historical consensus: born ~4-7 BCE. These are the error bounds. */
#define CE_EPOCH_ERROR_YEARS_MIN 4
#define CE_EPOCH_ERROR_YEARS_MAX 7

/* Calendar epoch system */
typedef enum {
    CE_SYSTEM_JULIAN = 0,
    CE_SYSTEM_GREGORIAN,
    CE_SYSTEM_COUNT
} ce_system_t;

/* Easter date result */
typedef struct {
    int month;   /* 3=March, 4=April */
    int day;     /* day of month */
    int year;    /* CE year */
} ce_easter_t;

/* Calendar epoch fact */
typedef struct {
    int year;              /* year CE (negative for BCE) */
    const char *title;     /* short name */
    const char *who;       /* key figure */
    const char *what;      /* what happened */
    const char *impact;    /* how it changed timekeeping */
} ce_epoch_fact_t;

/* Julian/Gregorian drift */
typedef struct {
    int drift_days;        /* accumulated drift in days */
    int year;              /* year for which drift is computed */
    const char *note;      /* explanation */
} ce_drift_t;

/* Easter computation (Computus — Anonymous Gregorian algorithm, Meeus).
 * Valid for Gregorian calendar years (1583+). */
ce_easter_t ce_easter_date(int year);

/* Easter as Julian Day Number.
 * Returns JD at noon on Easter Sunday. */
double ce_easter_jd(int year);

/* Dionysius Exiguus epoch offset.
 * Historical Jesus born ~4-7 BCE, but calendar starts at 1 AD.
 * Returns the estimated error range. */
int ce_epoch_offset_min(void);
int ce_epoch_offset_max(void);

/* Julian-Gregorian drift: how many days has the Julian calendar
 * drifted from the Gregorian by a given year? */
ce_drift_t ce_julian_drift(int year);

/* Number of historical epoch facts. */
int ce_epoch_fact_count(void);

/* Get epoch fact by index (chronological order).
 * Returns empty fact for invalid index. */
ce_epoch_fact_t ce_epoch_fact_get(int index);

/* Calendar system name. Returns "?" for invalid. */
const char *ce_system_name(ce_system_t system);

/* Is this year a leap year in the Julian calendar?
 * Julian rule: divisible by 4. */
int ce_is_julian_leap(int year);

/* Is this year a leap year in the Gregorian calendar?
 * Gregorian rule: divisible by 4, except centuries not divisible by 400. */
int ce_is_gregorian_leap(int year);

/* Days between Julian and Gregorian dates for a given year.
 * Positive = Gregorian is ahead. Equivalent to ce_julian_drift().drift_days. */
int ce_gregorian_julian_diff(int year);

#endif /* TIME_CALENDAR_EPOCH_H */
