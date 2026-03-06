#ifndef TIME_TIME_FORMAT_H
#define TIME_TIME_FORMAT_H

#include <stddef.h>

/* Format JD with thousands separator: "2,451,545.0000" */
void fmt_jd_grouped(double jd, char *buf, size_t sz);

/* Adaptive time scale: picks best unit automatically.
 * 0.5 -> "0.5 s", 3600 -> "1.0 h", 86400 -> "1.0 d",
 * 3.15e7 -> "1.0 yr", 3.15e10 -> "1.0 kyr", 3.15e13 -> "1.0 Myr", etc. */
void fmt_time_adaptive(double seconds, char *buf, size_t sz);

/* Planetary hour info. Traditional Chaldean order:
 * Saturn, Jupiter, Mars, Sun, Venus, Mercury, Moon (repeating).
 * Day ruler = planet of the weekday. Hours cycle from there. */
typedef struct {
    int hour_number;     /* 1-24 (1-12 day, 13-24 night) */
    int planet_ruler;    /* 0-6: Saturn=0, Jupiter=1, Mars=2, Sun=3, Venus=4, Mercury=5, Moon=6 */
    int is_night;        /* 0=day hour, 1=night hour */
} tf_planetary_hour_t;

/* Compute planetary hour from day-of-week (0=Mon..6=Sun) and
 * day_fraction (0.0=midnight, 0.25=6am, 0.5=noon, 0.75=6pm).
 * Assumes equal day/night (equinox approximation). */
tf_planetary_hour_t tf_planetary_hour(int day_of_week, double day_fraction);

/* Name of planetary hour ruler. */
const char *tf_planetary_hour_ruler_name(int planet_ruler);

/* Symbol of planetary hour ruler. */
const char *tf_planetary_hour_ruler_symbol(int planet_ruler);

/* Format a single aspect pair: "Sun - Moon" */
void fmt_aspect_pair(const char *body_a, const char *aspect_symbol,
                     const char *body_b, char *buf, size_t sz);

/* Format elapsed time compactly: seconds -> "2d 5h", "3h 15m", "45m 30s" */
void fmt_elapsed_compact(double seconds, char *buf, size_t sz);

#endif
