#ifndef TIME_TIME_HUD_H
#define TIME_TIME_HUD_H

#include <stddef.h>

/* All displayable time strings, computed purely from JD + observer longitude. */
typedef struct {
    char gregorian_date[16]; /* "2000-01-01" */
    char gregorian_time[16]; /* "12:00:00" */
    char day_name[16];       /* "Saturday" */
    char jd_str[24];         /* "2451545.0000" */
    char gmst_str[16];       /* "18h 41m 50s" */
    char gast_str[16];       /* "18h 41m 49s" */
    char lst_str[16];        /* "20h 53m 44s" */
    char tzolkin_str[48];    /* "7 Blue Hand (Kin 137)" */
    char sun_sign_str[32];   /* "Capricorn 10.3d" */
    char speed_str[24];      /* "1x", "PAUSED", "-1x" */
} time_hud_t;

/* Compute all HUD strings from simulation state.
 * jd: current Julian Day
 * observer_lon: degrees east (positive), degrees west (negative)
 * time_speed: simulation speed multiplier (0.0 = paused) */
time_hud_t time_hud_compute(double jd, double observer_lon, double time_speed);

/* Convert degrees (0-360) to "HHh MMm SSs" format.
 * Writes to buf, respecting buf_size. */
void degrees_to_hms(double degrees, char *buf, size_t buf_size);

#endif
