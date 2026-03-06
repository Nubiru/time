/* prayer_times.h — Compute 5 daily Islamic prayer times from solar position.
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Uses hour angle formula with solar declination from solar_events module. */

#ifndef TIME_PRAYER_TIMES_H
#define TIME_PRAYER_TIMES_H

typedef enum {
    PRAYER_METHOD_MWL = 0,     /* Muslim World League: Fajr 18, Isha 17 */
    PRAYER_METHOD_ISNA,        /* ISNA: Fajr 15, Isha 15 */
    PRAYER_METHOD_EGYPT,       /* Egyptian: Fajr 19.5, Isha 17.5 */
    PRAYER_METHOD_MAKKAH,      /* Umm al-Qura: Fajr 18.5, Isha = Maghrib+90min */
    PRAYER_METHOD_KARACHI,     /* Karachi: Fajr 18, Isha 18 */
    PRAYER_METHOD_COUNT
} prayer_method_t;

typedef enum {
    ASR_SHAFII = 0,   /* Shadow = 1x object + noon shadow */
    ASR_HANAFI         /* Shadow = 2x object + noon shadow */
} asr_jurisprudence_t;

typedef struct {
    double fajr;      /* JD of Fajr prayer */
    double sunrise;   /* JD of sunrise (end of Fajr window) */
    double dhuhr;     /* JD of Dhuhr prayer */
    double asr;       /* JD of Asr prayer */
    double maghrib;   /* JD of Maghrib prayer */
    double isha;      /* JD of Isha prayer */
} prayer_times_t;

typedef struct {
    const char *name;
    double jd;
} next_prayer_t;

/* Compute all 6 prayer times for a given JD (at noon UTC), location, method, and Asr school. */
prayer_times_t prayer_times_compute(double jd_noon, double lat, double lon,
                                     prayer_method_t method, asr_jurisprudence_t asr);

/* Return human-readable name of a calculation method. */
const char *prayer_method_name(prayer_method_t method);

/* Return the Fajr twilight angle (degrees below horizon) for a method. */
double prayer_method_fajr_angle(prayer_method_t method);

/* Return the Isha twilight angle (degrees below horizon) for a method.
 * Returns 0.0 for Makkah method (uses fixed 90 minutes instead). */
double prayer_method_isha_angle(prayer_method_t method);

/* Given today's prayer times and the current JD, return the next upcoming prayer. */
next_prayer_t prayer_next(prayer_times_t times, double current_jd);

#endif
