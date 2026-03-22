/* today_summary.h — Unified "Today in All Systems" snapshot.
 *
 * Pure module: computes a structured summary of a Julian Day across
 * 20 calendar/time systems. Each entry provides a formatted date string,
 * significance level, and descriptive note. Convergence scoring detects
 * cross-system alignment.
 *
 * No GL, no malloc, no globals, no side effects.
 *
 * Sources: Each calendar system module (gregorian, tzolkin, chinese,
 * hebrew, hijri, buddhist, panchanga, iching, zodiac, human_design,
 * sefirot, coptic, ethiopian, persian, japanese, korean, thai, geo_time,
 * cosmic_time). */

#ifndef TIME_TODAY_SUMMARY_H
#define TIME_TODAY_SUMMARY_H

#define TS_MAX_SYSTEMS 38
#define TS_DATE_MAX 64
#define TS_NOTE_MAX 128

/* Known time system IDs */
typedef enum {
    TS_SYS_GREGORIAN = 0,
    TS_SYS_TZOLKIN,
    TS_SYS_HAAB,
    TS_SYS_CHINESE,
    TS_SYS_HEBREW,
    TS_SYS_ISLAMIC,
    TS_SYS_BUDDHIST,
    TS_SYS_HINDU,
    TS_SYS_ICHING,
    TS_SYS_ASTROLOGY,
    TS_SYS_HUMAN_DESIGN,
    TS_SYS_KABBALAH,
    TS_SYS_COPTIC,
    TS_SYS_ETHIOPIAN,
    TS_SYS_PERSIAN,
    TS_SYS_JAPANESE,
    TS_SYS_KOREAN,
    TS_SYS_THAI,
    TS_SYS_GEOLOGICAL,
    TS_SYS_COSMIC,
    TS_SYS_EARTH,
    TS_SYS_ASTRONOMY,
    TS_SYS_TAROT,
    TS_SYS_NUMEROLOGY,
    TS_SYS_CHAKRA,
    TS_SYS_ZOROASTRIAN,
    TS_SYS_BALINESE,
    TS_SYS_FRENCH_REPUBLICAN,
    TS_SYS_AZTEC,
    TS_SYS_EGYPTIAN,
    TS_SYS_CELTIC,
    TS_SYS_LAO,
    TS_SYS_MYANMAR,
    TS_SYS_BAHAI,
    TS_SYS_TAMIL,
    TS_SYS_COUNT
} ts_system_t;

/* A single system's contribution to today */
typedef struct {
    ts_system_t system;
    char date_str[TS_DATE_MAX];       /* "Kin 207 Blue Crystal Hand" */
    char note[TS_NOTE_MAX];           /* "Portal Day" or "" */
    int significance;                  /* 0=normal, 1=notable, 2=major, 3=extraordinary */
    int active;                        /* 1 = data available, 0 = no data */
} ts_entry_t;

/* Complete today summary */
typedef struct {
    double jd;                         /* the date this was computed for */
    ts_entry_t entries[TS_MAX_SYSTEMS];
    int entry_count;
    int convergence_score;             /* 0-10: how many systems align today */
    char headline[TS_NOTE_MAX];        /* "5 systems converge" or best description */
} ts_summary_t;

/* Compute full summary for a Julian Day. */
ts_summary_t ts_compute(double jd);

/* Get a specific system's entry. Returns entry with active=0 if not found. */
ts_entry_t ts_get_system(const ts_summary_t *summary, ts_system_t system);

/* Count entries with significance >= threshold. */
int ts_significant_count(const ts_summary_t *summary, int min_significance);

/* Format as multi-line text (all systems, one per line). */
int ts_format_all(const ts_summary_t *summary, char *buf, int buf_size);

/* Format headline + top 5 significant entries as compact text. */
int ts_format_brief(const ts_summary_t *summary, char *buf, int buf_size);

/* System name string. */
const char *ts_system_name(ts_system_t system);

/* Total system count. */
int ts_system_count(void);

#endif /* TIME_TODAY_SUMMARY_H */
