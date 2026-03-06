#ifndef TIME_GREGORIAN_H
#define TIME_GREGORIAN_H

#include <stddef.h>

/* Format Gregorian date from Julian Day as "YYYY-MM-DD".
 * Writes to buf, respecting buf_size. */
void gregorian_format_date(double jd, char *buf, size_t buf_size);

/* Format time-of-day from Julian Day as "HH:MM:SS".
 * Writes to buf, respecting buf_size. */
void gregorian_format_time(double jd, char *buf, size_t buf_size);

/* Format full date+time from Julian Day as "YYYY-MM-DD HH:MM:SS".
 * Writes to buf, respecting buf_size. */
void gregorian_format_full(double jd, char *buf, size_t buf_size);

/* Day of week from Julian Day. Monday=0 ... Sunday=6. */
int gregorian_day_of_week(double jd);

/* Day name string from day index (0=Monday ... 6=Sunday).
 * Returns "?" for invalid index. */
const char *gregorian_day_name(int day);

#endif
