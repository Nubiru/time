/* calendar_convert.h — Calendar Conversion Engine
 * Unified interface for converting Julian Day to any of 21 calendar systems.
 * Uses JD as a universal pivot for cross-system conversion.
 *
 * Pure functions only. No GL, no malloc, no globals. */

#ifndef TIME_CALENDAR_CONVERT_H
#define TIME_CALENDAR_CONVERT_H

#define CAL_FMT_SIZE 80
#define CAL_SYSTEM_COUNT 21

typedef enum {
    CAL_GREGORIAN = 0,
    CAL_TZOLKIN,
    CAL_HAAB,
    CAL_CHINESE,
    CAL_ICHING,
    CAL_HEBREW,
    CAL_ISLAMIC,
    CAL_PERSIAN,
    CAL_COPTIC,
    CAL_ETHIOPIAN,
    CAL_EGYPTIAN,
    CAL_FRENCH_REPUBLICAN,
    CAL_JAPANESE,
    CAL_BAHAI,
    CAL_KOREAN,
    CAL_THAI,
    CAL_TAMIL,
    CAL_MYANMAR,
    CAL_ZOROASTRIAN,
    CAL_CELTIC_TREE,
    CAL_CALENDAR_ROUND,
    CAL_COUNT
} cal_system_t;

typedef struct {
    cal_system_t system;
    int year;
    int month;
    int day;
    int extra;                    /* system-specific (cycle year, hexagram #, etc.) */
    char formatted[CAL_FMT_SIZE]; /* human-readable date string */
} cal_date_t;

/* Convert JD to a specific calendar system. */
cal_date_t cal_convert(cal_system_t sys, double jd);

/* Format a date for a specific system. Returns chars written. */
int cal_format(cal_system_t sys, double jd, char *buf, int buf_size);

/* System display name. */
const char *cal_system_name(cal_system_t sys);

/* Number of supported systems. */
int cal_system_count(void);

/* Check if system enum is valid. */
int cal_system_valid(cal_system_t sys);

/* Convert between two systems (JD as pivot). year/month/day from source system. */
cal_date_t cal_convert_between(cal_system_t from, int year, int month, int day,
                               cal_system_t to);

#endif /* TIME_CALENDAR_CONVERT_H */
