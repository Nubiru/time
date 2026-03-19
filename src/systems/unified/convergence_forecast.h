/* convergence_forecast.h — Predicts upcoming convergence events
 *
 * Given a start JD, scans forward and identifies dates where multiple
 * knowledge systems have significant moments. Uses known cycle lengths
 * and significance patterns for approximate but useful forecasts.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Cycle periods derived from each calendar system's known
 * significant-moment intervals. */

#ifndef TIME_CONVERGENCE_FORECAST_H
#define TIME_CONVERGENCE_FORECAST_H

#include "today_summary.h"  /* ts_system_t, TS_SYS_COUNT */

#define CF_MAX_FORECASTS    16
#define CF_MAX_SYSTEMS_PER  8

/* A single forecasted convergence */
typedef struct {
    double jd;                          /* Julian Day of the event */
    int days_from_now;                  /* Days from start_jd */
    int system_count;                   /* How many systems converge */
    int systems[CF_MAX_SYSTEMS_PER];    /* ts_system_t values involved */
    int strength;                       /* 1=minor(2), 2=notable(3), 3=major(4), 4=rare(5+) */
    char headline[128];                 /* "Lunar Triple -- Hebrew, Islamic, Buddhist" */
    char detail[256];                   /* Longer description */
} cf_event_t;

/* Forecast result */
typedef struct {
    cf_event_t events[CF_MAX_FORECASTS];
    int count;
    int strongest;                      /* Max strength found (0 if none) */
} cf_forecast_t;

/* Known significant-moment schedule for a system */
typedef struct {
    int system_id;
    double period_days;                 /* How often significant moment occurs */
    const char *event_name;             /* "New Month", "Sabbat", "Uposatha", etc. */
} cf_schedule_t;

/* Scan max_days forward from start_jd for convergence events.
 * max_days clamped to [1, 365]. Invalid input -> empty result. */
cf_forecast_t cf_forecast(double start_jd, int max_days);

/* Returns the significance schedule for a system (0-19).
 * Invalid system -> system_id=-1, period=0. */
cf_schedule_t cf_schedule(int system_id);

/* Returns 20 (TS_SYS_COUNT). */
int cf_schedule_count(void);

/* Returns "Minor", "Notable", "Major", "Rare", or "?" for invalid. */
const char *cf_strength_name(int strength);

#endif /* TIME_CONVERGENCE_FORECAST_H */
