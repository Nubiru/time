#ifndef TIME_DATE_PARSE_H
#define TIME_DATE_PARSE_H

#include <stddef.h>

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int valid;     /* 1 if parse succeeded, 0 if failed */
} parsed_date_t;

/* Parse date string to components.
 * Supported formats:
 *   "2024-04-08"           -> date only (time = 00:00:00)
 *   "2024-04-08 14:30"     -> date + hour:minute
 *   "2024-04-08 14:30:00"  -> date + hour:minute:second
 *   "-4712-01-01"          -> negative years (BCE)
 * Returns parsed_date_t with valid=0 on failure. */
parsed_date_t date_parse(const char *str);

/* Parse string directly to JD. Convenience wrapper.
 * Returns -1.0 on parse failure. */
double date_parse_to_jd(const char *str);

/* Format JD to ISO date string: "YYYY-MM-DD". */
void date_format_iso(double jd, char *buf, size_t sz);

/* Format JD to full datetime string: "YYYY-MM-DD HH:MM:SS". */
void date_format_full(double jd, char *buf, size_t sz);

/* Validate date components (range checking). */
int date_validate(int year, int month, int day, int hour, int minute, int second);

/* Days in a given month (handles leap years). */
int date_days_in_month(int year, int month);

/* Is this a leap year? (Gregorian rules) */
int date_is_leap_year(int year);

#endif
