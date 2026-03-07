/* tide_predict.c — Tidal prediction engine.
 * Pure functions only. No malloc, no globals, no side effects.
 * Combines tidal harmonic constituents with surf spot preferences
 * to produce tide forecasts and surf quality windows. */

#include "tide_predict.h"
#include "tidal.h"
#include "surf_spots.h"
#include "../astronomy/lunar.h"

#include <math.h>

/* J2000.0 epoch (Julian Date) */
static const double J2000_JD = 2451545.0;

/* Small time delta for numerical differentiation (hours) */
static const double DELTA_H = 0.1;

/* Scan step for finding highs/lows (hours) */
static const double SCAN_STEP_H = 0.2;

/* Scan step for surf quality windows (hours) */
static const double SURF_SCAN_STEP_H = 1.0;

/* Quality threshold for surf windows */
static const double SURF_QUALITY_THRESHOLD = 0.3;

/* Scan step for best-today search (hours) */
static const double BEST_SCAN_STEP_H = 0.5;

/* Mean Moon distance in km (for lunar phase -> approximate distance) */
static const double MEAN_MOON_DIST_KM = 384400.0;


/* Convert JD to hours since J2000.0 */
static double jd_to_hours(double jd)
{
    return (jd - J2000_JD) * 24.0;
}

/* Get approximate Moon-Sun phase angle in degrees from lunar_phase */
static double get_phase_angle_deg(double jd)
{
    lunar_info_t info = lunar_phase(jd);
    return info.phase_fraction * 360.0;
}

/* Normalize tidal height to roughly -1.0..+1.0 range for surf quality.
 * The raw harmonic range is approx -2.66 to +2.66. */
static double normalize_height(double raw_height)
{
    /* Sum of all amplitudes = 2.656 */
    static const double MAX_AMP = TIDAL_M2_AMP + TIDAL_S2_AMP
                                + TIDAL_N2_AMP + TIDAL_K1_AMP
                                + TIDAL_O1_AMP;
    if (MAX_AMP < 0.001) return 0.0;
    double norm = raw_height / MAX_AMP;
    if (norm < -1.0) norm = -1.0;
    if (norm > 1.0) norm = 1.0;
    return norm;
}

double tide_predict_height(double jd)
{
    return tidal_harmonic_height(jd_to_hours(jd));
}

int tide_predict_is_rising(double jd)
{
    double h_now  = tide_predict_height(jd);
    double h_next = tide_predict_height(jd + DELTA_H / 24.0);
    return (h_next > h_now) ? 1 : 0;
}

double tide_predict_hours_to_high(double jd)
{
    /* Scan forward over one+ M2 period looking for a local max.
     * A local max is where height[i] > height[i-1] and height[i] > height[i+1]. */
    double max_scan = TIDAL_M2_PERIOD_H + 1.0;
    int steps = (int)(max_scan / SCAN_STEP_H);

    double prev_h = tide_predict_height(jd);
    for (int i = 1; i <= steps; i++) {
        double t = jd + i * SCAN_STEP_H / 24.0;
        double cur_h = tide_predict_height(t);
        double next_h = tide_predict_height(t + SCAN_STEP_H / 24.0);

        if (cur_h >= prev_h && cur_h >= next_h && i > 0) {
            return i * SCAN_STEP_H;
        }
        prev_h = cur_h;
    }
    /* Fallback: half M2 period */
    return TIDAL_M2_PERIOD_H / 2.0;
}

double tide_predict_hours_to_low(double jd)
{
    /* Scan forward over one+ M2 period looking for a local min. */
    double max_scan = TIDAL_M2_PERIOD_H + 1.0;
    int steps = (int)(max_scan / SCAN_STEP_H);

    double prev_h = tide_predict_height(jd);
    for (int i = 1; i <= steps; i++) {
        double t = jd + i * SCAN_STEP_H / 24.0;
        double cur_h = tide_predict_height(t);
        double next_h = tide_predict_height(t + SCAN_STEP_H / 24.0);

        if (cur_h <= prev_h && cur_h <= next_h && i > 0) {
            return i * SCAN_STEP_H;
        }
        prev_h = cur_h;
    }
    /* Fallback: half M2 period */
    return TIDAL_M2_PERIOD_H / 2.0;
}

tide_state_t tide_predict_state(double jd, double lat, double lon)
{
    (void)lat;
    (void)lon;

    tide_state_t s;

    double phase_deg = get_phase_angle_deg(jd);

    s.height_relative    = tide_predict_height(jd);
    s.rising             = tide_predict_is_rising(jd);
    s.hours_to_next_high = tide_predict_hours_to_high(jd);
    s.hours_to_next_low  = tide_predict_hours_to_low(jd);
    s.is_spring          = tidal_is_spring(phase_deg);
    s.is_neap            = tidal_is_neap(phase_deg);
    s.is_king            = tidal_is_king(MEAN_MOON_DIST_KM, phase_deg);
    s.spring_neap_factor = tidal_spring_neap_factor(phase_deg);

    return s;
}

double tide_predict_surf_quality(double jd, int spot_index)
{
    if (spot_index < 0 || spot_index >= surf_spot_count()) {
        return 0.0;
    }

    double height = tide_predict_height(jd);
    double norm = normalize_height(height);
    return surf_spot_tide_quality(spot_index, norm);
}

double tide_predict_best_today(double jd, int spot_index)
{
    if (spot_index < 0 || spot_index >= surf_spot_count()) {
        return jd;
    }

    double best_jd = jd;
    double best_q  = tide_predict_surf_quality(jd, spot_index);

    int steps = (int)(24.0 / BEST_SCAN_STEP_H);
    for (int i = 1; i <= steps; i++) {
        double t = jd + i * BEST_SCAN_STEP_H / 24.0;
        double q = tide_predict_surf_quality(t, spot_index);
        if (q > best_q) {
            best_q = q;
            best_jd = t;
        }
    }
    return best_jd;
}

surf_forecast_t tide_predict_surf_windows(double jd_start, double jd_end,
                                          int spot_index)
{
    surf_forecast_t fc;
    fc.count = 0;

    if (jd_end <= jd_start) {
        return fc;
    }

    if (spot_index < 0 || spot_index >= surf_spot_count()) {
        return fc;
    }

    double range_hours = (jd_end - jd_start) * 24.0;
    int steps = (int)(range_hours / SURF_SCAN_STEP_H);

    for (int i = 0; i <= steps && fc.count < TIDE_PREDICT_MAX_WINDOWS; i++) {
        double t = jd_start + i * SURF_SCAN_STEP_H / 24.0;
        if (t > jd_end) break;

        double q = tide_predict_surf_quality(t, spot_index);
        if (q >= SURF_QUALITY_THRESHOLD) {
            double phase_deg = get_phase_angle_deg(t);

            fc.windows[fc.count].jd          = t;
            fc.windows[fc.count].quality      = q;
            fc.windows[fc.count].tide_height  = tide_predict_height(t);
            fc.windows[fc.count].is_spring    = tidal_is_spring(phase_deg);
            fc.count++;
        }
    }

    return fc;
}
