/* location_alerts.h — Location-based alert evaluation.
 * Pure computation: evaluates surf, snow, golden hour, prayer, tide,
 * and daylight conditions to produce human-readable notifications.
 * No GL, no malloc, no globals, no side effects.
 * Dependencies: tide_predict.h, daylight.h, snow_season.h, surf_spots.h,
 *               ski_resorts.h, prayer_times.h, solar_events.h */

#ifndef TIME_LOCATION_ALERTS_H
#define TIME_LOCATION_ALERTS_H

typedef enum {
    ALERT_SURF = 0,
    ALERT_SNOW,
    ALERT_GOLDEN_HOUR,
    ALERT_BLUE_HOUR,
    ALERT_PRAYER,
    ALERT_DAYLIGHT,
    ALERT_TIDE,
    ALERT_TYPE_COUNT
} alert_type_t;

typedef enum {
    ALERT_INFO = 0,
    ALERT_SUGGESTION,
    ALERT_NOTABLE
} alert_urgency_t;

typedef struct {
    alert_type_t type;
    alert_urgency_t urgency;
    char message[128];
    double hours_until;
    double quality;
} alert_t;

#define ALERT_MAX 16

typedef struct {
    alert_t alerts[ALERT_MAX];
    int count;
} alert_result_t;

/* Evaluate all location-based alerts for a given time and place.
 * jd: Julian Day. lat/lon: geographic coordinates (degrees).
 * Returns sorted alerts (NOTABLE first, then soonest). */
alert_result_t location_alerts_evaluate(double jd, double lat, double lon);

/* Individual alert checkers. Each writes up to max alerts into out[].
 * Returns the count of alerts written. */
int alert_check_surf(double jd, double lat, double lon,
                     alert_t *out, int max);
int alert_check_snow(double jd, double lat, double lon,
                     alert_t *out, int max);
int alert_check_golden_hour(double jd, double lat, double lon,
                            alert_t *out, int max);
int alert_check_prayer(double jd, double lat, double lon,
                       alert_t *out, int max);
int alert_check_tide(double jd, double lat, double lon,
                     alert_t *out, int max);
int alert_check_daylight(double jd, double lat, double lon,
                         alert_t *out, int max);

/* Sort alerts: urgency DESC (NOTABLE first), then hours_until ASC. */
void alert_sort(alert_t *alerts, int count);

/* Human-readable name for an alert type. */
const char *alert_type_name(alert_type_t type);

/* Human-readable name for an urgency level. */
const char *alert_urgency_name(alert_urgency_t urgency);

#endif /* TIME_LOCATION_ALERTS_H */
