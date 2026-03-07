/* tide_predict.h — Tidal prediction engine.
 * Predicts tidal state for any location and time from lunar/solar
 * positions. Combines tidal harmonic constituents with surf spot
 * preferences to produce tide forecasts and surf quality windows.
 * Dependencies: tidal.h, surf_spots.h, lunar.h */

#ifndef TIME_TIDE_PREDICT_H
#define TIME_TIDE_PREDICT_H

#define TIDE_PREDICT_MAX_WINDOWS 32

/* Tidal state at a specific time and location */
typedef struct {
    double height_relative;      /* harmonic height (approx -2.66 to +2.66) */
    int rising;                  /* 1 if tide is rising, 0 if falling */
    double hours_to_next_high;   /* hours until next local high */
    double hours_to_next_low;    /* hours until next local low */
    int is_spring;               /* 1 if spring tide (new/full moon) */
    int is_neap;                 /* 1 if neap tide (quarter moon) */
    int is_king;                 /* 1 if king tide (spring + perigee) */
    double spring_neap_factor;   /* 0.0 (neap) to 1.0 (spring) */
} tide_state_t;

/* A single surf quality window */
typedef struct {
    double jd;                   /* Julian date of the window */
    double quality;              /* surf quality 0.0 to 1.0 */
    double tide_height;          /* tidal height at this moment */
    int is_spring;               /* 1 if spring tide */
} surf_window_t;

/* Collection of surf windows for a forecast period */
typedef struct {
    surf_window_t windows[TIDE_PREDICT_MAX_WINDOWS];
    int count;                   /* number of valid windows */
} surf_forecast_t;

/* Predict full tidal state at a given time and location.
 * jd: Julian date. lat/lon: geographic coordinates (degrees). */
tide_state_t tide_predict_state(double jd, double lat, double lon);

/* Find surf quality windows for a spot over a time range.
 * jd_start/jd_end: Julian date range. spot_index: surf_spot index (0..49).
 * Returns empty forecast for invalid spot or reversed range. */
surf_forecast_t tide_predict_surf_windows(double jd_start, double jd_end,
                                          int spot_index);

/* Surf quality at a specific time for a given spot.
 * Returns 0.0 for invalid spot_index, otherwise 0.0..1.0. */
double tide_predict_surf_quality(double jd, int spot_index);

/* Hours until next local tidal high from the given JD.
 * Scans forward in small steps over one M2 period. */
double tide_predict_hours_to_high(double jd);

/* Hours until next local tidal low from the given JD.
 * Scans forward in small steps over one M2 period. */
double tide_predict_hours_to_low(double jd);

/* Tidal harmonic height at a given JD.
 * Wrapper around tidal_harmonic_height using hours since J2000. */
double tide_predict_height(double jd);

/* Is the tide rising at the given JD?
 * Compares height at jd vs jd + small delta. */
int tide_predict_is_rising(double jd);

/* Find the JD of best surf quality within 24 hours of jd.
 * Returns a JD in range [jd, jd+1.0].
 * For invalid spot_index, returns jd. */
double tide_predict_best_today(double jd, int spot_index);

#endif /* TIME_TIDE_PREDICT_H */
