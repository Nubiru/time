/* convergence_detect.h -- Real Cross-System Convergence Detector (Track 19.1)
 *
 * Unlike convergence.h (standalone, inline approximations), this module
 * calls the REAL calendar modules (tzolkin, hebrew, hijri, iching, chinese,
 * panchanga) for accuracy.
 *
 * Detects when multiple time/calendar systems are simultaneously at
 * "significant moments" (new year, new month, sacred days, lunar phases).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CONVERGENCE_DETECT_H
#define TIME_CONVERGENCE_DETECT_H

#define CD_MAX_CONVERGENCES 32
#define CD_MAX_SYSTEMS 16

typedef enum {
    CD_SYS_ASTRONOMY = 0,   /* lunar phases, solstice/equinox */
    CD_SYS_ASTROLOGY,       /* sign ingress, major aspects */
    CD_SYS_TZOLKIN,         /* kin 1, portal days, tone 1 */
    CD_SYS_ICHING,          /* hexagram 1/2, solstice hexagrams */
    CD_SYS_CHINESE,         /* new year proximity, stem-branch 1 */
    CD_SYS_HEBREW,          /* new month (day 1), Rosh Hashanah month */
    CD_SYS_ISLAMIC,         /* new month (day 1), Ramadan */
    CD_SYS_HINDU,           /* tithi transitions, sacred tithis */
    CD_SYS_BUDDHIST,        /* Uposatha (quarter moon days), Vesak */
    CD_SYS_PERSIAN,         /* Nowruz, month boundaries */
    CD_SYS_COPTIC,          /* Coptic new year, month boundaries */
    CD_SYS_CELTIC,          /* 8 Sabbats (solstice/equinox/cross-quarter) */
    CD_SYS_BAHAI,           /* Naw-Ruz, 19-day Feast boundaries */
    CD_SYS_COUNT
} cd_system_t;

typedef enum {
    CD_STRENGTH_MINOR = 1,   /* 2 systems align */
    CD_STRENGTH_NOTABLE = 2, /* 3 systems align */
    CD_STRENGTH_MAJOR = 3,   /* 4+ systems align */
    CD_STRENGTH_RARE = 4     /* 5+ systems, very rare */
} cd_strength_t;

typedef struct {
    cd_system_t systems[CD_MAX_SYSTEMS];
    int system_count;
    cd_strength_t strength;
    const char *description;    /* static string describing the convergence */
    double significance;        /* 0.0-1.0 rarity score */
} cd_event_t;

typedef struct {
    cd_event_t events[CD_MAX_CONVERGENCES];
    int count;
} cd_result_t;

/* Scan a Julian Day for convergences across all systems. */
cd_result_t cd_scan(double jd);

/* Check if a specific pair of systems converge at JD.
 * Returns 1 if both significant. */
int cd_check_pair(cd_system_t a, cd_system_t b, double jd);

/* Get overall significance score for a date (0.0-1.0). */
double cd_significance(double jd);

/* Find next convergence of given minimum strength, scanning up to max_days
 * forward. Returns JD of convergence, or 0.0 if none found. */
double cd_find_next(double start_jd, cd_strength_t min_strength, int max_days);

/* System name string. Returns "?" for invalid. */
const char *cd_system_name(cd_system_t sys);

/* Number of convergence system types (always CD_SYS_COUNT). */
int cd_type_count(void);

#endif /* TIME_CONVERGENCE_DETECT_H */
