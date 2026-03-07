/* snow_season.h — Snow season modeling.
 * Pure computation: snow probability, temperature estimates, ski season
 * windows, powder likelihood, avalanche risk factors, and snow line
 * elevation from latitude, elevation, and month.
 * Models: latitude-based temperature gradient, lapse rate, seasonal
 * cosine variation. No external dependencies beyond math.h. */

#ifndef TIME_SNOW_SEASON_H
#define TIME_SNOW_SEASON_H

#include <stdbool.h>

/* Season window (start/end months) */
typedef struct {
    int start_month;  /* 1-12, 0 if no season */
    int end_month;    /* 1-12, 0 if no season */
    int duration;     /* months */
} season_window_t;

/* Avalanche risk factors */
typedef struct {
    double temperature_risk;   /* 0.0-1.0: rapid warming increases risk */
    double snowpack_risk;      /* 0.0-1.0: new snow loading risk */
    double wind_risk;          /* 0.0-1.0: wind slab formation risk */
    double overall_risk;       /* 0.0-1.0: combined risk */
} avalanche_risk_t;

/* Snow probability from latitude, elevation, and month.
 * lat: degrees (-90 to 90)
 * elevation_m: meters above sea level
 * month: 1-12
 * Returns 0.0 (no snow) to 1.0 (certain snow). */
double snow_probability(double lat, double elevation_m, int month);

/* Estimate average temperature at lat/elevation/month in Celsius.
 * Simple model: base temp from latitude + lapse rate + seasonal variation. */
double snow_temperature(double lat, double elevation_m, int month);

/* Ski season window for a given latitude and elevation.
 * Returns months where snow_probability > 0.5. */
season_window_t snow_ski_season(double lat, double elevation_m);

/* Powder likelihood: probability of fresh, dry snow (0.0-1.0).
 * Best when temperature is well below freezing and precipitation is likely. */
double snow_powder_likelihood(double lat, double elevation_m, int month);

/* Avalanche risk factors for a given location and month. */
avalanche_risk_t snow_avalanche_risk(double lat, double elevation_m, int month);

/* Snow line elevation: approximate elevation (m) where snow starts
 * for a given latitude and month. Returns 0 if snow to sea level. */
double snow_line_elevation(double lat, int month);

/* Is it winter at this latitude and month?
 * Northern hemisphere: Nov-Mar. Southern: May-Sep. */
bool snow_is_winter(double lat, int month);

#endif /* TIME_SNOW_SEASON_H */
