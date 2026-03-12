#include "fmt.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static const char *ZODIAC_SYMBOLS[] = {
    "\u2648", "\u2649", "\u264A", "\u264B",
    "\u264C", "\u264D", "\u264E", "\u264F",
    "\u2650", "\u2651", "\u2652", "\u2653"
};

/* --- Angle formatting --- */

fmt_buf_t fmt_degrees_dms(double degrees) {
    fmt_buf_t r;
    double abs_deg = fabs(degrees);
    int d = (int)floor(abs_deg);
    double rem = (abs_deg - d) * 60.0;
    int m = (int)floor(rem);
    double s = (rem - m) * 60.0;
    if (degrees < 0.0) {
        snprintf(r.buf, sizeof(r.buf), "-%d\xc2\xb0 %02d' %04.1f\"", d, m, s);
    } else {
        snprintf(r.buf, sizeof(r.buf), "%d\xc2\xb0 %02d' %04.1f\"", d, m, s);
    }
    return r;
}

fmt_buf_t fmt_hours_hms(double hours) {
    fmt_buf_t r;
    double abs_h = fabs(hours);
    int h = (int)floor(abs_h);
    double rem = (abs_h - h) * 60.0;
    int m = (int)floor(rem);
    int s = (int)floor((rem - m) * 60.0);
    snprintf(r.buf, sizeof(r.buf), "%02dh %02dm %02ds", h, m, s);
    return r;
}

fmt_buf_t fmt_right_ascension(double degrees) {
    double hours = degrees / 15.0;
    return fmt_hours_hms(hours);
}

fmt_buf_t fmt_declination(double degrees) {
    fmt_buf_t r;
    double abs_deg = fabs(degrees);
    int d = (int)floor(abs_deg);
    double rem = (abs_deg - d) * 60.0;
    int m = (int)floor(rem);
    int s = (int)floor((rem - m) * 60.0);
    char sign = degrees >= 0.0 ? '+' : '-';
    snprintf(r.buf, sizeof(r.buf), "%c%d\xc2\xb0 %02d' %02d\"", sign, d, m, s);
    return r;
}

/* --- Number formatting --- */

fmt_buf_t fmt_padded_int(int value, int width) {
    fmt_buf_t r;
    snprintf(r.buf, sizeof(r.buf), "%0*d", width, value);
    return r;
}

fmt_buf_t fmt_signed_float(double value, int precision) {
    fmt_buf_t r;
    snprintf(r.buf, sizeof(r.buf), "%+.*f", precision, value);
    return r;
}

fmt_buf_t fmt_ordinal(int n) {
    fmt_buf_t r;
    const char *suffix;
    int abs_n = n < 0 ? -n : n;
    int last_two = abs_n % 100;
    int last_one = abs_n % 10;

    if (last_two >= 11 && last_two <= 13) {
        suffix = "th";
    } else if (last_one == 1) {
        suffix = "st";
    } else if (last_one == 2) {
        suffix = "nd";
    } else if (last_one == 3) {
        suffix = "rd";
    } else {
        suffix = "th";
    }

    snprintf(r.buf, sizeof(r.buf), "%d%s", n, suffix);
    return r;
}

/* --- Date/time formatting --- */

fmt_buf_t fmt_julian_day(double jd, int precision) {
    fmt_buf_t r;
    snprintf(r.buf, sizeof(r.buf), "JD %.*f", precision, jd);
    return r;
}

fmt_buf_t fmt_iso_date(int year, int month, int day) {
    fmt_buf_t r;
    snprintf(r.buf, sizeof(r.buf), "%04d-%02d-%02d", year, month, day);
    return r;
}

fmt_buf_t fmt_time_24h(int hour, int minute, int second) {
    fmt_buf_t r;
    snprintf(r.buf, sizeof(r.buf), "%02d:%02d:%02d", hour, minute, second);
    return r;
}

/* --- Duration formatting --- */

fmt_buf_t fmt_duration_hms(double seconds) {
    fmt_buf_t r;
    double abs_s = fabs(seconds);
    int total = (int)floor(abs_s);
    int h = total / 3600;
    int m = (total % 3600) / 60;
    int s = total % 60;
    snprintf(r.buf, sizeof(r.buf), "%dh %02dm %02ds", h, m, s);
    return r;
}

fmt_buf_t fmt_duration_days(double days) {
    fmt_buf_t r;
    double abs_d = fabs(days);
    if (abs_d >= 365.25) {
        snprintf(r.buf, sizeof(r.buf), "%.2f years", days / 365.25);
    } else {
        snprintf(r.buf, sizeof(r.buf), "%.2f days", days);
    }
    return r;
}

/* --- Unicode symbol helpers --- */

fmt_buf_t fmt_zodiac_position(double ecliptic_lon) {
    fmt_buf_t r;
    double lon = fmod(ecliptic_lon, 360.0);
    if (lon < 0.0) lon += 360.0;

    int sign = (int)(lon / 30.0) % 12;
    double deg_in_sign = fmod(lon, 30.0);
    int deg = (int)deg_in_sign;
    int min = (int)((deg_in_sign - deg) * 60.0);

    snprintf(r.buf, sizeof(r.buf), "%d\xc2\xb0 %s %d'", deg, ZODIAC_SYMBOLS[sign], min);
    return r;
}
