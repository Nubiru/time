/* solar_radiation.h — Solar radiation model.
 * Computes solar declination, irradiance (TOA and surface), day length,
 * noon elevation, seasonal temperature estimates, and freezing elevation.
 * Formulas: Meinel atmospheric model, standard lapse rate.
 * Data source: TSI from SORCE/TIM (Kopp & Lean 2011). */

#ifndef TIME_SOLAR_RADIATION_H
#define TIME_SOLAR_RADIATION_H

#define PI 3.14159265358979323846
#define SOLAR_CONSTANT 1361.0  /* W/m^2 (TSI at 1 AU) */

typedef struct {
    double irradiance_toa;    /* top-of-atmosphere irradiance (W/m^2) */
    double irradiance_surface;/* approximate surface irradiance (W/m^2) */
    double air_mass;          /* atmospheric path length (1.0 at zenith) */
    double solar_elevation;   /* sun elevation angle (degrees) */
} solar_irradiance_t;

typedef struct {
    double hours;
    double sunrise_hour;
    double sunset_hour;
    int is_polar_day;
    int is_polar_night;
} day_length_t;

typedef struct {
    double temp_c;
    double temp_k;
    double daily_range_c;
} seasonal_temp_t;

/* Solar declination angle for a given day of year (1-365).
 * Returns degrees in [-23.44, +23.44]. */
double solar_declination(int day_of_year);

/* Instantaneous solar irradiance at given latitude, declination, and hour angle.
 * All inputs in degrees. Returns irradiance struct. */
solar_irradiance_t solar_irradiance(double lat_deg, double decl_deg,
                                    double hour_angle_deg);

/* Daily mean irradiance at given latitude and day of year.
 * Integrates over daylight hours in 1-hour steps. Returns W/m^2. */
double solar_daily_mean_irradiance(double lat_deg, int day_of_year);

/* Day length from latitude and declination (both in degrees).
 * Handles polar day/night edge cases. */
day_length_t solar_day_length(double lat_deg, double decl_deg);

/* Day length from latitude and day of year.
 * Convenience wrapper that computes declination internally. */
day_length_t solar_day_length_doy(double lat_deg, int day_of_year);

/* Seasonal temperature estimate from latitude, day of year, and elevation.
 * Uses latitude gradient, seasonal amplitude, and lapse rate. */
seasonal_temp_t solar_seasonal_temp(double lat_deg, int day_of_year,
                                    double elevation_m);

/* Elevation (meters) at which temperature reaches 0 C.
 * Returns 0.0 if base temperature is already <= 0. */
double solar_freezing_elevation(double lat_deg, int day_of_year);

/* Solar noon elevation angle in degrees.
 * lat_deg and decl_deg both in degrees. */
double solar_noon_elevation(double lat_deg, double decl_deg);

#endif /* TIME_SOLAR_RADIATION_H */
