/* snow_season.c — Snow season modeling implementation.
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Temperature model: latitude gradient + lapse rate + seasonal cosine.
 * Snow probability: linear interpolation from temperature.
 * All inputs validated and clamped to physical ranges. */

#include "snow_season.h"

#include <math.h>

#define PI 3.14159265358979323846

/* Lapse rate: degrees C per 1000m elevation */
static const double LAPSE_RATE = 6.5;

/* Clamp value to [lo, hi] */
static double clamp(double val, double lo, double hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

double snow_temperature(double lat, double elevation_m, int month) {
    double abs_lat = fabs(lat);

    /* Base temperature at sea level from latitude */
    double t_base = 30.0 - 0.7 * abs_lat;

    /* Lapse rate: -6.5 C per 1000m */
    double lapse = LAPSE_RATE * elevation_m / 1000.0;

    /* Seasonal amplitude: larger at higher latitudes */
    double amplitude = 15.0 + 0.3 * abs_lat;

    /* Warmest month: July (7) for northern, January (1) for southern */
    double warmest_month = (lat >= 0.0) ? 7.0 : 1.0;

    /* Seasonal variation using cosine: peaks at warmest month */
    double seasonal = amplitude * cos(2.0 * PI * ((double)month - warmest_month) / 12.0);

    return t_base - lapse + seasonal;
}

double snow_probability(double lat, double elevation_m, int month) {
    double t = snow_temperature(lat, elevation_m, month);

    if (t < -10.0) return 1.0;
    if (t > 5.0) return 0.0;

    /* Linear interpolation between -10 and 5 */
    double p = (5.0 - t) / 15.0;
    return clamp(p, 0.0, 1.0);
}

season_window_t snow_ski_season(double lat, double elevation_m) {
    season_window_t result = { .start_month = 0, .end_month = 0, .duration = 0 };

    /* Check each month for snow probability > 0.5 */
    int snow_months[12];
    int count = 0;

    for (int m = 1; m <= 12; m++) {
        double p = snow_probability(lat, elevation_m, m);
        snow_months[m - 1] = (p > 0.5) ? 1 : 0;
        if (p > 0.5) count++;
    }

    if (count == 0) return result;

    /* If all 12 months have snow */
    if (count == 12) {
        result.start_month = 1;
        result.end_month = 12;
        result.duration = 12;
        return result;
    }

    /* Find the start of the snow season.
     * Look for a transition from no-snow to snow, handling wrap-around.
     * Strategy: find the first snow month after a non-snow month. */
    int start = -1;
    for (int i = 0; i < 12; i++) {
        int prev = (i + 11) % 12;
        if (snow_months[i] && !snow_months[prev]) {
            start = i;
            break;
        }
    }

    /* If no transition found but we have snow months, all snow months
     * are contiguous starting from month 0 (January) */
    if (start < 0) {
        start = 0;
    }

    /* Walk forward from start to find end */
    int end = start;
    int dur = 0;
    for (int i = 0; i < 12; i++) {
        int idx = (start + i) % 12;
        if (snow_months[idx]) {
            end = idx;
            dur++;
        } else {
            break;
        }
    }

    result.start_month = start + 1;  /* 1-indexed */
    result.end_month = end + 1;      /* 1-indexed */
    result.duration = dur;

    return result;
}

double snow_powder_likelihood(double lat, double elevation_m, int month) {
    double t = snow_temperature(lat, elevation_m, month);

    /* Powder peaks when T ~ -10 C, drops off linearly within +/-10 range */
    double p_powder = 1.0 - fabs(t - (-10.0)) / 10.0;
    if (p_powder < 0.0) p_powder = 0.0;

    /* Precipitation proxy: higher latitude = more frontal systems */
    double precip_factor = 0.5 + 0.5 * fmin(1.0, fabs(lat) / 60.0);

    double result = p_powder * precip_factor;
    return clamp(result, 0.0, 1.0);
}

avalanche_risk_t snow_avalanche_risk(double lat, double elevation_m, int month) {
    avalanche_risk_t risk;

    double sp = snow_probability(lat, elevation_m, month);

    /* Temperature risk: highest in transitional conditions (spring thaw) */
    /* Peak when snow_probability is between 0.3 and 0.7 */
    double temp_risk;
    if (sp >= 0.3 && sp <= 0.7) {
        /* Transitional: high risk */
        temp_risk = 0.8;
    } else if (sp > 0.7) {
        /* Deep winter: moderate risk from cold temperature gradients */
        temp_risk = 0.4;
    } else {
        /* No significant snow: low risk */
        temp_risk = sp * 0.5;
    }

    /* Snowpack risk: new snow loading. Highest during peak snow months */
    double snowpack_risk = sp * 0.8;

    /* Wind risk: higher at higher elevations */
    double wind_risk = fmin(1.0, elevation_m / 4000.0) * 0.7;

    risk.temperature_risk = clamp(temp_risk, 0.0, 1.0);
    risk.snowpack_risk = clamp(snowpack_risk, 0.0, 1.0);
    risk.wind_risk = clamp(wind_risk, 0.0, 1.0);
    risk.overall_risk = (risk.temperature_risk + risk.snowpack_risk + risk.wind_risk) / 3.0;

    return risk;
}

double snow_line_elevation(double lat, int month) {
    /* Temperature at sea level for this lat/month */
    double t_sea = snow_temperature(lat, 0.0, month);

    /* If sea level is already freezing, snow extends to sea level */
    if (t_sea <= 0.0) return 0.0;

    /* Solve: t_sea - lapse_rate * h / 1000 = 0
     * h = t_sea * 1000 / lapse_rate */
    double h = t_sea * 1000.0 / LAPSE_RATE;

    /* Clamp to reasonable range */
    if (h < 0.0) h = 0.0;

    return h;
}

bool snow_is_winter(double lat, int month) {
    double abs_lat = fabs(lat);

    /* Near equator (within ~10 degrees): never truly winter */
    if (abs_lat < 10.0) return false;

    if (lat > 0.0) {
        /* Northern hemisphere: Nov(11), Dec(12), Jan(1), Feb(2), Mar(3) */
        return (month >= 11 || month <= 3);
    } else {
        /* Southern hemisphere: May(5), Jun(6), Jul(7), Aug(8), Sep(9) */
        return (month >= 5 && month <= 9);
    }
}
