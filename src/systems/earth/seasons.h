/* seasons.h — Seasonal cycle visualizer data module.
 * Pure computation: season identification, day length, growing season,
 * cardinal dates (solstices/equinoxes), temperature deviation, solar
 * declination from latitude and day-of-year.
 * Models: simplified sinusoidal declination, hour angle day length,
 * latitude-based growing season estimation. No external dependencies
 * beyond math.h. */

#ifndef TIME_SEASONS_H
#define TIME_SEASONS_H

#define PI 3.14159265358979323846

/* Season identifiers */
typedef enum {
    SEASON_SPRING = 0,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER,
    SEASON_COUNT
} season_id_t;

/* Season info at a specific latitude and day */
typedef struct {
    season_id_t season;          /* Current season */
    const char *name;            /* "Spring", "Summer", etc. */
    double progress;             /* 0.0 (start) to 1.0 (end) within season */
    int day_in_season;           /* Day number within this season */
    int season_length;           /* Total days in this season */
    int is_southern;             /* 1 if southern hemisphere (seasons flipped) */
} season_info_t;

/* Growing season (frost-free period) */
typedef struct {
    int start_doy;               /* First frost-free day of year */
    int end_doy;                 /* Last frost-free day of year */
    int length_days;             /* Growing season length */
    const char *classification;  /* "Tropical", "Long", "Moderate", "Short", "None" */
} growing_season_t;

/* Solstice/equinox dates (approximate day of year) */
typedef struct {
    int vernal_equinox;          /* ~Mar 20 (day 80) */
    int summer_solstice;         /* ~Jun 21 (day 172) */
    int autumnal_equinox;        /* ~Sep 22 (day 265) */
    int winter_solstice;         /* ~Dec 21 (day 355) */
} cardinal_dates_t;

/* Get season info at latitude and day of year.
 * lat_deg: -90 to 90 (negative = southern hemisphere).
 * day_of_year: 1-365.
 * Southern hemisphere seasons are flipped. */
season_info_t season_at(double lat_deg, int day_of_year);

/* Get season name string. */
const char *season_name(season_id_t season);

/* Growing season estimate at latitude.
 * Based on last/first frost approximation from latitude. */
growing_season_t growing_season(double lat_deg);

/* Get cardinal dates (solstices and equinoxes) for northern hemisphere.
 * These are approximate day-of-year values. */
cardinal_dates_t season_cardinal_dates(void);

/* Solar declination at day of year (degrees).
 * Simplified formula: dec = 23.44 * sin(2*PI*(doy-81)/365) */
double season_solar_declination(int day_of_year);

/* Day length in hours at latitude and day of year.
 * Uses hour angle formula. Returns 0-24 (clamped for polar). */
double season_day_length(double lat_deg, int day_of_year);

/* Is it the opposite season in the other hemisphere?
 * Returns the season that the opposite hemisphere is experiencing. */
season_id_t season_opposite(season_id_t season);

/* Which hemisphere is experiencing summer at this day of year?
 * Returns 1 for northern, -1 for southern. */
int season_summer_hemisphere(int day_of_year);

/* Days until next solstice or equinox from given day of year.
 * Returns count and which event. */
int season_days_to_next_cardinal(int day_of_year, const char **event_name);

/* Average temperature deviation from annual mean (simplified model).
 * Returns degrees C offset: positive in summer, negative in winter.
 * Amplitude varies with latitude. */
double season_temp_deviation(double lat_deg, int day_of_year);

#endif /* TIME_SEASONS_H */
