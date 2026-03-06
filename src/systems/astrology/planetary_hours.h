#ifndef TIME_PLANETARY_HOURS_H
#define TIME_PLANETARY_HOURS_H

typedef struct {
    int planet;           /* 0-6 Chaldean index (Saturn=0...Moon=6) */
    int hour_index;       /* 0-23 (which planetary hour of the day) */
    int day_ruler;        /* planet that rules this day */
    int day_of_week;      /* 0=Sunday...6=Saturday */
} planetary_hour_t;

/* Compute planetary hour from Julian Day */
planetary_hour_t planetary_hour(double jd);

/* Day ruler for a given day of week (0=Sunday..6=Saturday) */
int planetary_day_ruler(int day_of_week);

/* Planet name in Chaldean order (0-6). Returns "?" for invalid. */
const char *planet_chaldean_name(int planet);

/* Planet Unicode symbol (0-6). Returns "?" for invalid. */
const char *planet_chaldean_symbol(int planet);

/* Day name from day of week (0=Sunday..6=Saturday). Returns "?" for invalid. */
const char *planetary_day_name(int day_of_week);

#endif
