/* daylight.h — Daylight state, golden hour, blue hour, and seasonal day length.
 * Pure computation wrapping solar_events.h to provide photographer/observer
 * daylight data at any moment and location. No GL, no malloc, no globals.
 * Algorithm sources: Jean Meeus "Astronomical Algorithms" (hour angle formula),
 * NOAA Solar Calculator (refraction correction at -0.833 degrees). */

#ifndef TIME_DAYLIGHT_H
#define TIME_DAYLIGHT_H

typedef struct {
    int is_day;                /* 1 if sun above horizon (altitude > -0.833 deg) */
    double sun_elevation_deg;  /* current sun altitude in degrees */
    double minutes_to_sunrise; /* minutes until next sunrise. -1.0 if polar night */
    double minutes_to_sunset;  /* minutes until next sunset. -1.0 if midnight sun */
    double day_length_hours;   /* total daylight for this date */
    int polar_day;             /* 1 if midnight sun (no sunset) */
    int polar_night;           /* 1 if polar night (no sunrise) */
} daylight_t;

typedef struct {
    double start_jd;  /* start of window (Julian Day) */
    double end_jd;    /* end of window (Julian Day) */
    int valid;        /* 1 if window exists for this date/location */
} daylight_window_t;

/* Full daylight state at a moment and location.
 * jd: Julian Day (any time of day).
 * lat: latitude in degrees (-90 to 90).
 * lon: longitude in degrees (-180 to 180). */
daylight_t daylight_at(double jd, double lat, double lon);

/* Golden hour: sun altitude roughly 0-6 degrees above horizon.
 * Morning = after sunrise, Evening = before sunset. */
daylight_window_t daylight_golden_hour_morning(double jd, double lat, double lon);
daylight_window_t daylight_golden_hour_evening(double jd, double lat, double lon);

/* Blue hour: sun altitude roughly -4 to -6 degrees below horizon.
 * Morning = before sunrise, Evening = after sunset. */
daylight_window_t daylight_blue_hour_morning(double jd, double lat, double lon);
daylight_window_t daylight_blue_hour_evening(double jd, double lat, double lon);

/* Day length for a date at a location (hours). Wrapper around se_solar_events. */
double daylight_day_length(double jd, double lat, double lon);

/* Longest day of year at latitude (hours). Returns ~24 for high Arctic. */
double daylight_longest_day(double lat);

/* Shortest day of year at latitude (hours). Returns ~0 for high Arctic. */
double daylight_shortest_day(double lat);

#endif /* TIME_DAYLIGHT_H */
