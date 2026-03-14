/* festival_detector.h — Cross-System Festival Detector
 * Aggregates festival detection from 10 calendar systems into a
 * unified result. Given a Julian Day, returns ALL active festivals
 * across Korean, Hebrew, Islamic, Buddhist, Celtic, Persian,
 * Thai, Tamil, Myanmar, and Zoroastrian calendars.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_FESTIVAL_DETECTOR_H
#define TIME_FESTIVAL_DETECTOR_H

#define FEST_MAX_ACTIVE 16

typedef enum {
    FEST_SYS_KOREAN = 0,
    FEST_SYS_HEBREW,
    FEST_SYS_ISLAMIC,
    FEST_SYS_BUDDHIST,
    FEST_SYS_CELTIC,
    FEST_SYS_PERSIAN,
    FEST_SYS_THAI,
    FEST_SYS_TAMIL,
    FEST_SYS_MYANMAR,
    FEST_SYS_ZOROASTRIAN,
    FEST_SYS_COUNT
} fest_system_t;

typedef enum {
    FEST_MAJOR = 0,
    FEST_MINOR = 1,
    FEST_ASTRO = 2
} fest_priority_t;

typedef struct {
    fest_system_t system;
    fest_priority_t priority;
    const char *name;
    const char *description;
} fest_entry_t;

typedef struct {
    fest_entry_t entries[FEST_MAX_ACTIVE];
    int count;
} fest_result_t;

/* Detect all active festivals at a given Julian Day. */
fest_result_t festival_detect(double jd);

/* Count festivals for a specific system in a result. */
int festival_count_for_system(fest_result_t result, fest_system_t sys);

/* System name string. */
const char *festival_system_name(fest_system_t sys);

/* Total number of systems checked. */
int festival_system_count(void);

/* Check if any festival is active at JD (returns count). */
int festival_any_active(double jd);

/* Get the highest-priority festival from a result (or NULL if none). */
const fest_entry_t *festival_highest_priority(const fest_result_t *result);

/* Count total defined festivals across all systems. */
int festival_total_defined(void);

#endif /* TIME_FESTIVAL_DETECTOR_H */
