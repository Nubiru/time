#include "solar_events.h"

#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

/* Sun center correction for atmospheric refraction at horizon */
#define SUN_HORIZON_DEG (-0.833)

static double normalize_deg(double d)
{
    d = fmod(d, 360.0);
    if (d < 0.0) d += 360.0;
    return d;
}

/* Solar position helpers — simplified Meeus / NOAA */

static double days_from_j2000(double jd)
{
    return jd - 2451545.0;
}

static double mean_solar_longitude(double n)
{
    return normalize_deg(280.460 + 0.9856474 * n);
}

static double mean_anomaly(double n)
{
    return normalize_deg(357.528 + 0.9856003 * n);
}

static double ecliptic_longitude(double L, double g)
{
    double g_rad = g * DEG2RAD;
    return normalize_deg(L + 1.915 * sin(g_rad) + 0.020 * sin(2.0 * g_rad));
}

double se_solar_declination(double jd)
{
    double n = days_from_j2000(jd);
    double L = mean_solar_longitude(n);
    double g = mean_anomaly(n);
    double lambda = ecliptic_longitude(L, g);
    double obliquity = 23.439 - 0.0000004 * n;

    double sin_dec = sin(obliquity * DEG2RAD) * sin(lambda * DEG2RAD);
    return asin(sin_dec) * RAD2DEG;
}

double se_equation_of_time(double jd)
{
    double n = days_from_j2000(jd);
    double B = 2.0 * PI * (n - 81.0) / 365.0;
    return 9.87 * sin(2.0 * B) - 7.53 * cos(B) - 1.5 * sin(B);
}

static double hour_angle_for_altitude(double lat_deg, double dec_deg, double alt_deg)
{
    double lat_r = lat_deg * DEG2RAD;
    double dec_r = dec_deg * DEG2RAD;
    double alt_r = alt_deg * DEG2RAD;

    double cos_omega = (sin(alt_r) - sin(lat_r) * sin(dec_r))
                     / (cos(lat_r) * cos(dec_r));

    if (cos_omega > 1.0) return NAN;  /* sun never reaches this altitude (polar night) */
    if (cos_omega < -1.0) return NAN; /* sun never goes below this altitude (midnight sun) */

    return acos(cos_omega) * RAD2DEG;
}

double se_sunrise_hour_angle(double latitude_deg, double declination_deg)
{
    return hour_angle_for_altitude(latitude_deg, declination_deg, SUN_HORIZON_DEG);
}

/* Compute JD of noon (UT) for a given JD */
static double jd_noon(double jd)
{
    return floor(jd - 0.5) + 0.5;  /* round to nearest noon UT */
}

solar_day_t se_solar_events(double jd, double latitude_deg, double longitude_deg)
{
    solar_day_t result;
    double noon = jd_noon(jd);
    double dec = se_solar_declination(noon);
    double eot = se_equation_of_time(noon);

    /* Solar noon JD: adjust for longitude and equation of time */
    result.solar_noon_jd = noon - longitude_deg / 360.0 - eot / (60.0 * 24.0);

    double omega = hour_angle_for_altitude(latitude_deg, dec, SUN_HORIZON_DEG);

    if (isnan(omega)) {
        /* Polar case — check if always day or always night */
        double lat_r = latitude_deg * DEG2RAD;
        double dec_r = dec * DEG2RAD;
        double cos_check = (sin(SUN_HORIZON_DEG * DEG2RAD) - sin(lat_r) * sin(dec_r))
                         / (cos(lat_r) * cos(dec_r));

        if (cos_check < -1.0) {
            /* Sun never sets — midnight sun */
            result.always_day = 1;
            result.always_night = 0;
            result.day_length = 24.0;
            result.sunrise_jd = NAN;
            result.sunset_jd = NAN;
        } else {
            /* Sun never rises — polar night */
            result.always_day = 0;
            result.always_night = 1;
            result.day_length = 0.0;
            result.sunrise_jd = NAN;
            result.sunset_jd = NAN;
        }
    } else {
        result.always_day = 0;
        result.always_night = 0;
        result.sunrise_jd = result.solar_noon_jd - omega / 360.0;
        result.sunset_jd = result.solar_noon_jd + omega / 360.0;
        result.day_length = (result.sunset_jd - result.sunrise_jd) * 24.0;
    }

    return result;
}

twilight_t se_twilight(double jd, double latitude_deg, double longitude_deg)
{
    twilight_t tw;
    double noon = jd_noon(jd);
    double dec = se_solar_declination(noon);
    double eot = se_equation_of_time(noon);
    double solar_noon = noon - longitude_deg / 360.0 - eot / (60.0 * 24.0);

    double civil_ha = hour_angle_for_altitude(latitude_deg, dec, -6.0);
    double naut_ha = hour_angle_for_altitude(latitude_deg, dec, -12.0);
    double astro_ha = hour_angle_for_altitude(latitude_deg, dec, -18.0);

    tw.civil_dawn_jd = isnan(civil_ha) ? NAN : solar_noon - civil_ha / 360.0;
    tw.civil_dusk_jd = isnan(civil_ha) ? NAN : solar_noon + civil_ha / 360.0;

    tw.nautical_dawn_jd = isnan(naut_ha) ? NAN : solar_noon - naut_ha / 360.0;
    tw.nautical_dusk_jd = isnan(naut_ha) ? NAN : solar_noon + naut_ha / 360.0;

    tw.astro_dawn_jd = isnan(astro_ha) ? NAN : solar_noon - astro_ha / 360.0;
    tw.astro_dusk_jd = isnan(astro_ha) ? NAN : solar_noon + astro_ha / 360.0;

    return tw;
}

double se_solar_altitude(double jd, double latitude_deg, double longitude_deg)
{
    double dec = se_solar_declination(jd);
    double eot = se_equation_of_time(jd);

    /* Hour angle from solar noon */
    double noon = jd_noon(jd);
    double solar_noon = noon - longitude_deg / 360.0 - eot / (60.0 * 24.0);
    double hour_angle = (jd - solar_noon) * 360.0; /* degrees */

    double lat_r = latitude_deg * DEG2RAD;
    double dec_r = dec * DEG2RAD;
    double ha_r = hour_angle * DEG2RAD;

    double sin_alt = sin(lat_r) * sin(dec_r)
                   + cos(lat_r) * cos(dec_r) * cos(ha_r);

    return asin(sin_alt) * RAD2DEG;
}
