#ifndef TIME_FMT_H
#define TIME_FMT_H

typedef struct {
    char buf[64];
} fmt_buf_t;

/* Angle formatting */
fmt_buf_t fmt_degrees_dms(double degrees);
fmt_buf_t fmt_degrees_decimal(double degrees, int precision);
fmt_buf_t fmt_hours_hms(double hours);
fmt_buf_t fmt_right_ascension(double degrees);
fmt_buf_t fmt_declination(double degrees);

/* Number formatting */
fmt_buf_t fmt_padded_int(int value, int width);
fmt_buf_t fmt_signed_float(double value, int precision);
fmt_buf_t fmt_ordinal(int n);

/* Date/time formatting */
fmt_buf_t fmt_julian_day(double jd, int precision);
fmt_buf_t fmt_iso_date(int year, int month, int day);
fmt_buf_t fmt_time_24h(int hour, int minute, int second);

/* Duration formatting */
fmt_buf_t fmt_duration_hms(double seconds);
fmt_buf_t fmt_duration_days(double days);

/* Unicode symbol helpers */
fmt_buf_t fmt_zodiac_position(double ecliptic_lon);

#endif
