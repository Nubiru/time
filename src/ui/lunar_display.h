#ifndef TIME_LUNAR_DISPLAY_H
#define TIME_LUNAR_DISPLAY_H

#include <stddef.h>

/* Lunar display data — all strings ready for rendering. */
typedef struct {
    char phase_line[48];      /* "🌕 Full Moon" */
    char illumination[16];    /* "98.3%" */
    char age_line[32];        /* "Day 14.7 of 29.5" */
    char moon_sign[32];       /* "Moon in ♌ Leo" */
    char next_new[32];        /* "New Moon in 12.3d" */
    char next_full[32];       /* "Full Moon in 2.1d" */
    char summary[64];         /* "🌕 Full Moon 98% · ♌ Leo" */
} lunar_display_t;

/* Compute all display strings from JD.
 * Uses lunar.h internally for phase computation. */
lunar_display_t lunar_display(double jd);

/* Format illumination as percentage string: "98.3%" */
void lunar_fmt_illumination(double illumination, char *buf, size_t sz);

/* Format moon age: "Day 14.7 of 29.5" */
void lunar_fmt_age(double age_days, double synodic_month, char *buf, size_t sz);

/* Format moon sign: "Moon in ♌ Leo" */
void lunar_fmt_sign(int sign_index, char *buf, size_t sz);

/* Format days until event: "New Moon in 12.3d" or "Full Moon in 2.1d" */
void lunar_fmt_countdown(const char *event_name, double days_until,
                         char *buf, size_t sz);

/* Illumination bar: visual bar of moon fullness.
 * e.g. "####...." for ~50%. Writes to buf. width = number of chars. */
void lunar_fmt_bar(double illumination, char *buf, size_t sz, int width);

#endif
