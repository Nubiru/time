#ifndef TIME_JULIAN_H
#define TIME_JULIAN_H

/* Convert Gregorian date to Julian Day Number.
 * Month: 1-12, Day may include fractional part for time of day.
 * Gregorian calendar assumed for dates on or after 1582-10-15.
 * For Julian calendar dates, use julian_day_from_julian_cal(). */
double gregorian_to_jd(int year, int month, double day);

/* Convert Julian Day Number back to Gregorian date.
 * Returns year, month via pointers. Return value is day (with fraction). */
double jd_to_gregorian(double jd, int *year, int *month);

/* Convert Julian calendar date to JD (for dates before 1582-10-15). */
double julian_cal_to_jd(int year, int month, double day);

#endif
