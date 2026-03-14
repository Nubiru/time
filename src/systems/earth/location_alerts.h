/* location_alerts.h -- Location-aware alerts for surf, tide, snow,
 * golden hour, prayer, storm, convergence, solstice, avalanche.
 * Pure computation: no GL, no malloc, no globals, no side effects.
 * Dependencies: tide_predict.h, daylight.h, snow_season.h, surf_spots.h,
 *   ski_resorts.h, prayer_times.h, solar_events.h, storm_data.h,
 *   convergence_detect.h, solar_radiation.h */

#ifndef TIME_LOCATION_ALERTS_H
#define TIME_LOCATION_ALERTS_H

#define LA_MAX_ALERTS 12

typedef enum {
    LA_SURF = 0,       /* Surf conditions alert */
    LA_TIDE,           /* Tidal event (spring/king tide) */
    LA_SNOW,           /* Snow conditions / powder alert */
    LA_GOLDEN_HOUR,    /* Photography golden hour */
    LA_PRAYER,         /* Next prayer time */
    LA_STORM,          /* Storm season / cyclone risk */
    LA_CONVERGENCE,    /* Multi-system calendar convergence */
    LA_SOLSTICE,       /* Upcoming solstice/equinox */
    LA_AVALANCHE,      /* Avalanche risk factors */
    LA_TYPE_COUNT
} la_type_t;

typedef enum {
    LA_INFO = 0,       /* Informational */
    LA_SUGGESTION,     /* Actionable suggestion */
    LA_ALERT           /* Time-sensitive alert */
} la_urgency_t;

typedef struct {
    la_type_t type;
    la_urgency_t urgency;
    const char *message;       /* Static description string */
    double time_until;         /* Hours until event (0 = now, <0 = ongoing) */
    double quality;            /* 0.0-1.0 quality score where applicable */
} la_entry_t;

typedef struct {
    la_entry_t alerts[LA_MAX_ALERTS];
    int count;
} la_result_t;

/* Evaluate all location alerts for a given position and time.
 * jd: Julian Day
 * lat, lon: observer coordinates (degrees)
 * Returns alerts sorted by urgency (ALERT first, then SUGGESTION,
 * then INFO). */
la_result_t la_evaluate(double jd, double lat, double lon);

/* Check if any alerts are active at this location/time. */
int la_any_active(double jd, double lat, double lon);

/* Get alerts of a specific type only. */
int la_filter_type(la_result_t result, la_type_t type,
                   la_entry_t *out, int max);

/* Type name string. */
const char *la_type_name(la_type_t type);

/* Urgency name string. */
const char *la_urgency_name(la_urgency_t urgency);

/* Count of alert types. */
int la_type_count(void);

#endif /* TIME_LOCATION_ALERTS_H */
