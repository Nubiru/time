/* solar_radiation.c — Solar radiation model implementation.
 * Pure functions: no globals, no malloc, no side effects.
 * Formulas: Meinel atmospheric model (1976), standard lapse rate. */

#include "solar_radiation.h"

#include <math.h>

/* Degrees to radians */
static double deg2rad(double deg)
{
    return deg * PI / 180.0;
}

/* Radians to degrees */
static double rad2deg(double rad)
{
    return rad * 180.0 / PI;
}

double solar_declination(int day_of_year)
{
    /* delta = 23.44 * sin(2*PI*(284 + doy)/365) */
    return 23.44 * sin(2.0 * PI * (284.0 + day_of_year) / 365.0);
}

solar_irradiance_t solar_irradiance(double lat_deg, double decl_deg,
                                    double hour_angle_deg)
{
    solar_irradiance_t result = { 0.0, 0.0, 0.0, 0.0 };

    double lat_r = deg2rad(lat_deg);
    double decl_r = deg2rad(decl_deg);
    double ha_r = deg2rad(hour_angle_deg);

    /* sin(elevation) = sin(lat)*sin(decl) + cos(lat)*cos(decl)*cos(ha) */
    double sin_elev = sin(lat_r) * sin(decl_r) +
                      cos(lat_r) * cos(decl_r) * cos(ha_r);

    /* Clamp to [-1, 1] for safety */
    if (sin_elev > 1.0) sin_elev = 1.0;
    if (sin_elev < -1.0) sin_elev = -1.0;

    double elev_rad = asin(sin_elev);
    double elev_deg = rad2deg(elev_rad);

    result.solar_elevation = elev_deg;

    /* Sun below horizon */
    if (sin_elev <= 0.0) {
        result.irradiance_toa = 0.0;
        result.irradiance_surface = 0.0;
        result.air_mass = 100.0; /* large value for below horizon */
        return result;
    }

    /* TOA irradiance = SOLAR_CONSTANT * sin(elevation) */
    result.irradiance_toa = SOLAR_CONSTANT * sin_elev;

    /* Air mass = 1/sin(elevation), clamp minimum elevation to ~1 degree */
    double min_sin_elev = sin(deg2rad(1.0));
    double am_sin = sin_elev > min_sin_elev ? sin_elev : min_sin_elev;
    result.air_mass = 1.0 / am_sin;

    /* Surface irradiance: Meinel model */
    /* I_surface = I_toa * 0.7^(AM^0.678) */
    result.irradiance_surface = result.irradiance_toa *
                                pow(0.7, pow(result.air_mass, 0.678));

    return result;
}

double solar_daily_mean_irradiance(double lat_deg, int day_of_year)
{
    double decl = solar_declination(day_of_year);
    day_length_t dl = solar_day_length(lat_deg, decl);

    /* Polar night: no irradiance */
    if (dl.is_polar_night || dl.hours < 0.01) {
        return 0.0;
    }

    /* Integrate over daylight hours in 1-hour steps */
    double total = 0.0;
    int steps = 0;

    /* Step through the day from sunrise to sunset */
    double start = dl.sunrise_hour;
    double end = dl.sunset_hour;

    for (double hour = start + 0.5; hour < end; hour += 1.0) {
        /* Hour angle: 0 at noon, +/- 180 at midnight */
        double ha_deg = (hour - 12.0) * 15.0;
        solar_irradiance_t ir = solar_irradiance(lat_deg, decl, ha_deg);
        total += ir.irradiance_toa;
        steps++;
    }

    if (steps == 0) {
        return 0.0;
    }

    /* Average over 24 hours (not just daylight hours) */
    return total / 24.0;
}

day_length_t solar_day_length(double lat_deg, double decl_deg)
{
    day_length_t result = { 0.0, 6.0, 18.0, 0, 0 };

    double lat_r = deg2rad(lat_deg);
    double decl_r = deg2rad(decl_deg);

    /* cos(ha_sunset) = -tan(lat)*tan(decl) */
    double cos_ha = -tan(lat_r) * tan(decl_r);

    if (cos_ha < -1.0) {
        /* Polar day: sun never sets */
        result.hours = 24.0;
        result.is_polar_day = 1;
        result.is_polar_night = 0;
        result.sunrise_hour = 0.0;
        result.sunset_hour = 24.0;
        return result;
    }

    if (cos_ha > 1.0) {
        /* Polar night: sun never rises */
        result.hours = 0.0;
        result.is_polar_day = 0;
        result.is_polar_night = 1;
        result.sunrise_hour = 12.0;
        result.sunset_hour = 12.0;
        return result;
    }

    /* Normal day */
    double ha_sunset = acos(cos_ha);
    result.hours = 2.0 * ha_sunset * 12.0 / PI;
    result.sunrise_hour = 12.0 - result.hours / 2.0;
    result.sunset_hour = 12.0 + result.hours / 2.0;
    result.is_polar_day = 0;
    result.is_polar_night = 0;

    return result;
}

day_length_t solar_day_length_doy(double lat_deg, int day_of_year)
{
    double decl = solar_declination(day_of_year);
    return solar_day_length(lat_deg, decl);
}

seasonal_temp_t solar_seasonal_temp(double lat_deg, int day_of_year,
                                    double elevation_m)
{
    seasonal_temp_t result;

    double abs_lat = fabs(lat_deg);

    /* Seasonal amplitude: stronger at higher latitudes */
    double seasonal_amplitude = 0.3 * abs_lat;

    /* Base temperature at sea level:
     * T_base = 27 - 0.7*|lat| + amplitude * sin(2*PI*(doy-80)/365) */
    double seasonal_phase = sin(2.0 * PI * (day_of_year - 80.0) / 365.0);

    /* In southern hemisphere, seasons are reversed */
    if (lat_deg < 0.0) {
        seasonal_phase = -seasonal_phase;
    }

    double t_base = 27.0 - 0.7 * abs_lat +
                    seasonal_amplitude * seasonal_phase;

    /* Elevation: lapse rate -6.5 C per 1000m */
    result.temp_c = t_base - 6.5 * elevation_m / 1000.0;
    result.temp_k = result.temp_c + 273.15;

    /* Daily range: ~10 C at equator, increases with latitude */
    result.daily_range_c = 10.0 + 0.1 * abs_lat;

    return result;
}

double solar_freezing_elevation(double lat_deg, int day_of_year)
{
    /* Get sea-level temperature */
    seasonal_temp_t t = solar_seasonal_temp(lat_deg, day_of_year, 0.0);

    if (t.temp_c <= 0.0) {
        return 0.0;
    }

    /* Solve: T_base - 6.5 * h / 1000 = 0
     * h = T_base * 1000 / 6.5 */
    return t.temp_c * 1000.0 / 6.5;
}

double solar_noon_elevation(double lat_deg, double decl_deg)
{
    /* elev = 90 - |lat - decl| */
    return 90.0 - fabs(lat_deg - decl_deg);
}
