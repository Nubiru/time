#include "time_format.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* --- JD grouped thousands --- */

void fmt_jd_grouped(double jd, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    /* Format integer part with commas, then append decimal */
    long long int_part = (long long)floor(fabs(jd));
    double frac = fabs(jd) - (double)int_part;

    /* Format integer with commas */
    char digits[32];
    snprintf(digits, sizeof(digits), "%lld", int_part);
    int len = (int)strlen(digits);

    char grouped[48];
    int gi = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            grouped[gi++] = ',';
        }
        grouped[gi++] = digits[i];
    }
    grouped[gi] = '\0';

    /* Format fractional part */
    char frac_str[16];
    snprintf(frac_str, sizeof(frac_str), "%.4f", frac);
    /* frac_str = "0.XXXX", skip the leading "0" */
    const char *dot = frac_str + 1; /* ".XXXX" */

    if (jd < 0.0) {
        snprintf(buf, sz, "-%s%s", grouped, dot);
    } else {
        snprintf(buf, sz, "%s%s", grouped, dot);
    }
}

/* --- Adaptive time scale --- */

void fmt_time_adaptive(double seconds, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    double abs_s = fabs(seconds);

    if (abs_s < 60.0) {
        snprintf(buf, sz, "%.1f s", seconds);
    } else if (abs_s < 3600.0) {
        snprintf(buf, sz, "%.1f m", seconds / 60.0);
    } else if (abs_s < 86400.0) {
        snprintf(buf, sz, "%.1f h", seconds / 3600.0);
    } else if (abs_s < 365.25 * 86400.0) {
        snprintf(buf, sz, "%.1f d", seconds / 86400.0);
    } else if (abs_s < 1000.0 * 365.25 * 86400.0) {
        snprintf(buf, sz, "%.2f yr", seconds / (365.25 * 86400.0));
    } else if (abs_s < 1.0e6 * 365.25 * 86400.0) {
        snprintf(buf, sz, "%.2f kyr", seconds / (1000.0 * 365.25 * 86400.0));
    } else if (abs_s < 1.0e9 * 365.25 * 86400.0) {
        snprintf(buf, sz, "%.3f Myr", seconds / (1.0e6 * 365.25 * 86400.0));
    } else {
        snprintf(buf, sz, "%.3f Gyr", seconds / (1.0e9 * 365.25 * 86400.0));
    }
}

/* --- Planetary hours --- */

/* Chaldean order: Saturn(0), Jupiter(1), Mars(2), Sun(3), Venus(4), Mercury(5), Moon(6) */

/* Day rulers by weekday (Mon=0..Sun=6):
 * Monday=Moon, Tuesday=Mars, Wednesday=Mercury, Thursday=Jupiter,
 * Friday=Venus, Saturday=Saturn, Sunday=Sun */
static const int DAY_RULERS[7] = {
    6, /* Mon = Moon */
    2, /* Tue = Mars */
    5, /* Wed = Mercury */
    1, /* Thu = Jupiter */
    4, /* Fri = Venus */
    0, /* Sat = Saturn */
    3  /* Sun = Sun */
};

static const char *RULER_NAMES[7] = {
    "Saturn", "Jupiter", "Mars", "Sun", "Venus", "Mercury", "Moon"
};

static const char *RULER_SYMBOLS[7] = {
    "\xe2\x99\x84", /* ♄ Saturn */
    "\xe2\x99\x83", /* ♃ Jupiter */
    "\xe2\x99\x82", /* ♂ Mars */
    "\xe2\x98\x89", /* ☉ Sun */
    "\xe2\x99\x80", /* ♀ Venus */
    "\xe2\x98\xbf", /* ☿ Mercury */
    "\xe2\x98\xbd"  /* ☽ Moon */
};

tf_planetary_hour_t tf_planetary_hour(int day_of_week, double day_fraction)
{
    tf_planetary_hour_t ph;

    /* Clamp inputs */
    if (day_of_week < 0) day_of_week = 0;
    if (day_of_week > 6) day_of_week = 6;
    if (day_fraction < 0.0) day_fraction = 0.0;
    if (day_fraction >= 1.0) day_fraction = 0.999;

    /* Equinox model: sunrise at 0.25 (6am), sunset at 0.75 (6pm) */
    int day_ruler = DAY_RULERS[day_of_week];
    int hour_in_sequence;

    if (day_fraction >= 0.25 && day_fraction < 0.75) {
        /* Day hours: 12 hours from 6am to 6pm */
        double day_progress = (day_fraction - 0.25) / 0.5;
        int day_hour = (int)(day_progress * 12.0);
        if (day_hour > 11) day_hour = 11;
        ph.hour_number = day_hour + 1;
        ph.is_night = 0;
        hour_in_sequence = day_hour;
    } else {
        /* Night hours: 12 hours from 6pm to 6am */
        double night_frac;
        if (day_fraction >= 0.75) {
            night_frac = (day_fraction - 0.75) / 0.5;
        } else {
            night_frac = (day_fraction + 0.25) / 0.5;
        }
        int night_hour = (int)(night_frac * 12.0);
        if (night_hour > 11) night_hour = 11;
        ph.hour_number = night_hour + 13;
        ph.is_night = 1;
        hour_in_sequence = night_hour + 12;
    }

    /* Chaldean sequence: starting from day ruler, advance by hour_in_sequence */
    ph.planet_ruler = (day_ruler + hour_in_sequence) % 7;

    return ph;
}

const char *tf_planetary_hour_ruler_name(int planet_ruler)
{
    if (planet_ruler < 0 || planet_ruler > 6) return "?";
    return RULER_NAMES[planet_ruler];
}

const char *tf_planetary_hour_ruler_symbol(int planet_ruler)
{
    if (planet_ruler < 0 || planet_ruler > 6) return "?";
    return RULER_SYMBOLS[planet_ruler];
}

/* --- Aspect pair --- */

void fmt_aspect_pair(const char *body_a, const char *aspect_sym,
                     const char *body_b, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "%s %s %s",
             body_a ? body_a : "?",
             aspect_sym ? aspect_sym : "?",
             body_b ? body_b : "?");
}

/* --- Elapsed compact --- */

void fmt_elapsed_compact(double seconds, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    double abs_s = fabs(seconds);
    int total = (int)floor(abs_s);
    int s = total % 60;
    int m = (total / 60) % 60;
    int h = (total / 3600) % 24;
    int d = total / 86400;

    if (d > 0) {
        snprintf(buf, sz, "%dd %dh", d, h);
    } else if (h > 0) {
        snprintf(buf, sz, "%dh %dm", h, m);
    } else if (m > 0) {
        snprintf(buf, sz, "%dm %ds", m, s);
    } else {
        snprintf(buf, sz, "%ds", s);
    }
}
