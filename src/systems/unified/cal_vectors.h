/* cal_vectors.h — Calendar Test Vector database
 * Static database of historically verified reference dates with their
 * correct representations across multiple calendar systems.
 *
 * Each vector provides a Julian Day Number and the corresponding date
 * in one or more calendar systems, verified against authoritative sources
 * (Reingold/Dershowitz "Calendrical Calculations", JPL, historical records).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Self-contained: does not depend on any other Time project headers. */

#ifndef TIME_CAL_VECTORS_H
#define TIME_CAL_VECTORS_H

/* Maximum number of calendar entries per vector */
#define CV_MAX_ENTRIES 12

/* Calendar system IDs (mirrors cal_system_t from calendar_convert.h).
 * Duplicated here so this module remains self-contained. */
#define CV_CAL_GREGORIAN         0
#define CV_CAL_TZOLKIN           1
#define CV_CAL_HAAB              2
#define CV_CAL_CHINESE           3
#define CV_CAL_ICHING            4
#define CV_CAL_HEBREW            5
#define CV_CAL_ISLAMIC           6
#define CV_CAL_PERSIAN           7
#define CV_CAL_COPTIC            8
#define CV_CAL_ETHIOPIAN         9
#define CV_CAL_EGYPTIAN         10
#define CV_CAL_FRENCH_REPUBLICAN 11
#define CV_CAL_JAPANESE         12
#define CV_CAL_BAHAI            13
#define CV_CAL_KOREAN           14
#define CV_CAL_THAI             15
#define CV_CAL_TAMIL            16
#define CV_CAL_MYANMAR          17
#define CV_CAL_ZOROASTRIAN      18
#define CV_CAL_CELTIC_TREE      19
#define CV_CAL_CALENDAR_ROUND   20

/* A single calendar date in a vector */
typedef struct {
    int system;              /* calendar system ID (0-20) */
    int year;
    int month;
    int day;
    int extra;               /* system-specific (cycle, hexagram, etc.) */
} cv_entry_t;

/* A complete test vector: one JD with dates in multiple systems */
typedef struct {
    double jd;               /* Julian Day Number */
    const char *name;        /* human-readable event name */
    const char *description; /* why this date is significant */
    int entry_count;         /* how many calendar entries */
    cv_entry_t entries[CV_MAX_ENTRIES];
} cv_vector_t;

/* Total number of test vectors in the database */
int cv_count(void);

/* Get a test vector by index (0-based).
 * Returns vector with name=NULL for invalid index. */
cv_vector_t cv_get(int index);

/* Get JD for a vector by index.
 * Returns 0.0 for invalid index. */
double cv_jd(int index);

/* Get the vector name by index.
 * Returns "Unknown" for invalid index. */
const char *cv_name(int index);

/* Find an entry within a vector for a specific calendar system.
 * Returns entry with system=-1 if not found. */
cv_entry_t cv_find_entry(cv_vector_t vec, int system);

/* Check if a vector contains an entry for a given system.
 * Returns 1 if present, 0 if absent. */
int cv_has_system(cv_vector_t vec, int system);

#endif /* TIME_CAL_VECTORS_H */
