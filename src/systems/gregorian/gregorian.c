#include "gregorian.h"
#include "../../math/julian.h"
#include <stdio.h>
#include <math.h>

static const char *DAY_NAMES[] = {
    "Monday", "Tuesday", "Wednesday", "Thursday",
    "Friday", "Saturday", "Sunday"
};

void gregorian_format_date(double jd, char *buf, size_t buf_size) {
    int year, month;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)day_frac;
    snprintf(buf, buf_size, "%04d-%02d-%02d", year, month, day);
}

void gregorian_format_time(double jd, char *buf, size_t buf_size) {
    /* JD noon = .0 fraction. Extract time from (jd + 0.5) fractional part. */
    double shifted = jd + 0.5;
    double frac = shifted - floor(shifted);
    double total_seconds = frac * 86400.0;
    int seconds_int = (int)(total_seconds + 0.5); /* round to nearest second */
    int h = seconds_int / 3600;
    int m = (seconds_int % 3600) / 60;
    int s = seconds_int % 60;
    /* Clamp in case rounding pushes to 24:00:00 */
    if (h >= 24) { h = 0; m = 0; s = 0; }
    snprintf(buf, buf_size, "%02d:%02d:%02d", h, m, s);
}

void gregorian_format_full(double jd, char *buf, size_t buf_size) {
    char date[16];
    char time[16];
    gregorian_format_date(jd, date, sizeof(date));
    gregorian_format_time(jd, time, sizeof(time));
    snprintf(buf, buf_size, "%s %s", date, time);
}

int gregorian_day_of_week(double jd) {
    /* JD 0.0 was a Monday. (jd + 0.5) gives the calendar day integer.
     * JD 2451545.0 (J2000.0, 2000-01-01 noon) -> floor(jd+0.5) = 2451545
     * 2451545 % 7 = 0 -> need offset so Saturday = 5 (Monday=0).
     * 2000-01-01 was Saturday. 2451545 % 7 = 350220*7 + 5 = 5. Check: 2451545/7=350220.71..
     * Actually: 2451545 % 7 = 2451545 - 350220*7 = 2451545 - 2451540 = 5. That gives 5.
     * We need Saturday=5 in Monday=0 scheme. So offset = 0 works if base is Monday. */
    long day_num = (long)floor(jd + 0.5);
    int dow = (int)(day_num % 7);
    if (dow < 0) dow += 7;
    return dow;
}

const char *gregorian_day_name(int day) {
    if (day < 0 || day > 6) return "?";
    return DAY_NAMES[day];
}
