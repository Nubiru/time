#ifndef TIME_SOLAR_EVENTS_H
#define TIME_SOLAR_EVENTS_H

typedef struct {
    double sunrise_jd;    /* JD of sunrise (NAN if polar — no sunrise) */
    double sunset_jd;     /* JD of sunset (NAN if polar — no sunset) */
    double solar_noon_jd; /* JD of solar transit (highest point) */
    double day_length;    /* hours of daylight (0.0 polar night, 24.0 midnight sun) */
    int always_day;       /* 1 if midnight sun (no sunset) */
    int always_night;     /* 1 if polar night (no sunrise) */
} solar_day_t;

typedef struct {
    double civil_dawn_jd;       /* Sun center at -6 degrees */
    double civil_dusk_jd;
    double nautical_dawn_jd;    /* Sun center at -12 degrees */
    double nautical_dusk_jd;
    double astro_dawn_jd;       /* Sun center at -18 degrees */
    double astro_dusk_jd;
} twilight_t;

/* Solar declination from Julian Day (approximate). Returns degrees. */
double se_solar_declination(double jd);

/* Equation of time in minutes. */
double se_equation_of_time(double jd);

/* Hour angle at sunrise/sunset in degrees for given latitude and declination.
 * Returns NAN for polar cases (sun never rises or never sets). */
double se_sunrise_hour_angle(double latitude_deg, double declination_deg);

/* Full sunrise/sunset for a date (JD at noon UTC) and location. */
solar_day_t se_solar_events(double jd, double latitude_deg, double longitude_deg);

/* Twilight times for a date and location. */
twilight_t se_twilight(double jd, double latitude_deg, double longitude_deg);

/* Solar altitude in degrees above horizon at a given JD and location. */
double se_solar_altitude(double jd, double latitude_deg, double longitude_deg);

#endif
