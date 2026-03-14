/* location_alerts.c -- Location-aware alert evaluation.
 * Evaluates surf, tide, snow, golden hour, prayer, storm, convergence,
 * solstice, and avalanche conditions using existing Earth/astronomy
 * modules. Pure functions: no GL, no malloc, no globals. */

#include "location_alerts.h"
#include "tide_predict.h"
#include "daylight.h"
#include "snow_season.h"
#include "surf_spots.h"
#include "ski_resorts.h"
#include "storm_data.h"
#include "solar_radiation.h"
#include "../islamic/prayer_times.h"
#include "../astronomy/solar_events.h"
#include "../unified/convergence_detect.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

/* Proximity thresholds (km) */
#define LA_SURF_RANGE_KM     200.0
#define LA_SKI_RANGE_KM      200.0

/* Timing thresholds */
#define LA_GOLDEN_HOUR_WINDOW_H   2.0
#define LA_PRAYER_URGENT_H        0.5
#define LA_TIDE_KING_WINDOW_H    24.0
#define LA_TIDE_SPRING_WINDOW_H  72.0
#define LA_SOLSTICE_WINDOW_DAYS   7.0
#define LA_CONVERGENCE_THRESH     0.5
#define LA_CONVERGENCE_HIGH       0.8
#define LA_AVALANCHE_RISK_THRESH  0.6
#define LA_SURF_QUALITY_THRESH    0.7
#define LA_SNOW_POWDER_THRESH     0.6

/* Earth radius for Haversine (km) */
#define LA_EARTH_R_KM 6371.0

/* ===== Helpers ===== */

static double deg_to_rad(double d) { return d * PI / 180.0; }

/* Haversine great-circle distance in km */
static double haversine_km(double lat1, double lon1,
                           double lat2, double lon2)
{
    double rlat1 = deg_to_rad(lat1);
    double rlat2 = deg_to_rad(lat2);
    double dlat = deg_to_rad(lat2 - lat1);
    double dlon = deg_to_rad(lon2 - lon1);

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(rlat1) * cos(rlat2) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return LA_EARTH_R_KM * c;
}

/* Extract month (1-12) from Julian Day */
static int month_from_jd(double jd) {
    int a = (int)(jd + 0.5) + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - (146097 * b / 4);
    int d = (4 * c + 3) / 1461;
    int e = c - (1461 * d / 4);
    int m = (5 * e + 2) / 153;
    return m + 3 - 12 * (m / 10);
}

/* Day of year from Julian Day (1-366) */
static int day_of_year_from_jd(double jd) {
    int a = (int)(jd + 0.5) + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - (146097 * b / 4);
    int d = (4 * c + 3) / 1461;
    int e = c - (1461 * d / 4);
    int m = (5 * e + 2) / 153;
    int day = e - (153 * m + 2) / 5 + 1;
    int month = m + 3 - 12 * (m / 10);
    int year = 100 * b + d - 4800 + (m / 10);

    static const int days_before[] = {
        0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };
    if (month < 1) month = 1;
    if (month > 12) month = 12;
    int doy = days_before[month] + day;
    /* Leap year check */
    if (month > 2 &&
        ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        doy += 1;
    return doy;
}

/* Approximate days to nearest solstice or equinox.
 * Seasonal markers at approximate day-of-year:
 *   Vernal equinox ~80, Summer solstice ~172,
 *   Autumnal equinox ~266, Winter solstice ~355.
 * Returns days to nearest marker (always >= 0). */
static double days_to_nearest_solar_event(int doy) {
    static const int markers[] = { 80, 172, 266, 355 };
    double best = 366.0;
    for (int i = 0; i < 4; i++) {
        double diff = (double)(doy - markers[i]);
        if (diff < 0.0) diff = -diff;
        /* Also check wrap-around */
        double wrap = 365.0 - diff;
        if (wrap < diff) diff = wrap;
        if (diff < best) best = diff;
    }
    return best;
}

/* Name of nearest solar event */
static const char *nearest_solar_event_name(int doy) {
    static const int markers[] = { 80, 172, 266, 355 };
    static const char *names[] = {
        "Vernal equinox", "Summer solstice",
        "Autumnal equinox", "Winter solstice"
    };
    double best = 366.0;
    int best_idx = 0;
    for (int i = 0; i < 4; i++) {
        double diff = (double)(doy - markers[i]);
        if (diff < 0.0) diff = -diff;
        double wrap = 365.0 - diff;
        if (wrap < diff) diff = wrap;
        if (diff < best) {
            best = diff;
            best_idx = i;
        }
    }
    return names[best_idx];
}

/* Sort alerts: urgency DESC (ALERT first), then time_until ASC.
 * Insertion sort -- max LA_MAX_ALERTS items. */
static void la_sort(la_entry_t *alerts, int count) {
    if (!alerts || count <= 1) return;
    for (int i = 1; i < count; i++) {
        la_entry_t key = alerts[i];
        int j = i - 1;
        while (j >= 0 &&
               (alerts[j].urgency < key.urgency ||
                (alerts[j].urgency == key.urgency &&
                 alerts[j].time_until > key.time_until))) {
            alerts[j + 1] = alerts[j];
            j--;
        }
        alerts[j + 1] = key;
    }
}

/* Add an alert entry if count < max. Returns new count. */
static int la_add(la_entry_t *alerts, int count, int max,
                  la_type_t type, la_urgency_t urgency,
                  const char *message, double time_until, double quality)
{
    if (count >= max) return count;
    alerts[count].type = type;
    alerts[count].urgency = urgency;
    alerts[count].message = message;
    alerts[count].time_until = time_until;
    alerts[count].quality = quality;
    return count + 1;
}

/* ===== Individual checkers ===== */

/* Check surf conditions (up to 2 alerts) */
static int la_check_surf(double jd, double lat, double lon,
                         la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    int count = 0;

    int spot_idx = surf_spot_nearest(lat, lon);
    surf_spot_t spot = surf_spot_get(spot_idx);
    if (spot.id < 0) return 0;

    /* Check proximity */
    double dist = haversine_km(lat, lon, spot.lat, spot.lon);
    if (dist > LA_SURF_RANGE_KM) return 0;

    /* King tide at surf spot = notable */
    tide_state_t tide = tide_predict_state(jd, spot.lat, spot.lon);
    if (tide.is_king) {
        count = la_add(out, count, max, LA_SURF, LA_ALERT,
                       "King tide at nearby surf spot", 0.0, 0.9);
    }

    /* Surf quality */
    double quality = tide_predict_surf_quality(jd, spot_idx);
    if (quality > LA_SURF_QUALITY_THRESH) {
        count = la_add(out, count, max, LA_SURF, LA_SUGGESTION,
                       "Good surf conditions nearby", 0.0, quality);
    } else {
        count = la_add(out, count, max, LA_SURF, LA_INFO,
                       "Surf conditions at nearby break", 0.0, quality);
    }

    return count;
}

/* Check tidal events (up to 2 alerts) */
static int la_check_tide(double jd, double lat, double lon,
                         la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    int count = 0;

    tide_state_t tide = tide_predict_state(jd, lat, lon);

    /* King tide -- ALERT */
    if (tide.is_king) {
        count = la_add(out, count, max, LA_TIDE, LA_ALERT,
                       "King tide active", 0.0, 1.0);
    }
    /* Spring tide -- SUGGESTION */
    else if (tide.is_spring) {
        count = la_add(out, count, max, LA_TIDE, LA_SUGGESTION,
                       "Spring tide -- higher highs, lower lows",
                       0.0, tide.spring_neap_factor);
    }

    /* General tide info */
    if (tide.rising) {
        count = la_add(out, count, max, LA_TIDE, LA_INFO,
                       "Tide rising", tide.hours_to_next_high,
                       tide.spring_neap_factor);
    } else {
        count = la_add(out, count, max, LA_TIDE, LA_INFO,
                       "Tide falling", tide.hours_to_next_low,
                       tide.spring_neap_factor);
    }

    return count;
}

/* Check snow/ski conditions (up to 2 alerts) */
static int la_check_snow(double jd, double lat, double lon,
                         la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    int count = 0;

    int resort_idx = ski_resort_nearest(lat, lon);
    ski_resort_t resort = ski_resort_get(resort_idx);
    if (resort.id < 0) return 0;

    /* Check proximity */
    double dist = haversine_km(lat, lon, resort.lat, resort.lon);
    if (dist > LA_SKI_RANGE_KM) return 0;

    int month = month_from_jd(jd);
    int in_season = ski_resort_in_season(resort_idx, month);

    if (in_season) {
        double powder = snow_powder_likelihood(resort.lat,
                                               resort.summit_elev_m,
                                               month);
        if (powder > LA_SNOW_POWDER_THRESH) {
            count = la_add(out, count, max, LA_SNOW, LA_SUGGESTION,
                           "Peak powder probability nearby",
                           0.0, powder);
        } else {
            double snow_q = ski_resort_snow_quality(resort_idx, month);
            count = la_add(out, count, max, LA_SNOW, LA_INFO,
                           "Ski resort in season nearby", 0.0, snow_q);
        }
    } else {
        /* Check if season approaching */
        int season_start = resort.season_start;
        int months_until = season_start - month;
        if (months_until < 0) months_until += 12;
        if (months_until == 1) {
            count = la_add(out, count, max, LA_SNOW, LA_INFO,
                           "Ski season opens soon nearby",
                           (double)months_until * 30.0 * 24.0, 0.0);
        }
    }

    return count;
}

/* Check golden hour (up to 2 alerts) */
static int la_check_golden_hour(double jd, double lat, double lon,
                                la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    int count = 0;

    /* Morning golden hour */
    daylight_window_t gm = daylight_golden_hour_morning(jd, lat, lon);
    if (gm.valid) {
        double h_start = (gm.start_jd - jd) * 24.0;
        double h_end = (gm.end_jd - jd) * 24.0;

        if (h_start <= 0.0 && h_end > 0.0) {
            /* Currently in golden hour */
            count = la_add(out, count, max, LA_GOLDEN_HOUR, LA_SUGGESTION,
                           "Morning golden hour active now",
                           -1.0, 1.0);
        } else if (h_start > 0.0 && h_start <= LA_GOLDEN_HOUR_WINDOW_H) {
            count = la_add(out, count, max, LA_GOLDEN_HOUR, LA_SUGGESTION,
                           "Morning golden hour approaching",
                           h_start, 0.8);
        }
    }

    /* Evening golden hour */
    daylight_window_t ge = daylight_golden_hour_evening(jd, lat, lon);
    if (ge.valid) {
        double h_start = (ge.start_jd - jd) * 24.0;
        double h_end = (ge.end_jd - jd) * 24.0;

        if (h_start <= 0.0 && h_end > 0.0) {
            count = la_add(out, count, max, LA_GOLDEN_HOUR, LA_SUGGESTION,
                           "Evening golden hour active now",
                           -1.0, 1.0);
        } else if (h_start > 0.0 && h_start <= LA_GOLDEN_HOUR_WINDOW_H) {
            count = la_add(out, count, max, LA_GOLDEN_HOUR, LA_SUGGESTION,
                           "Evening golden hour approaching",
                           h_start, 0.8);
        }
    }

    return count;
}

/* Check prayer times (1 alert) */
static int la_check_prayer(double jd, double lat, double lon,
                           la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    int count = 0;

    double jd_noon = floor(jd) + 0.5;
    if (jd_noon > jd + 0.5) jd_noon -= 1.0;

    prayer_times_t times = prayer_times_compute(jd_noon, lat, lon,
                                                 PRAYER_METHOD_MWL,
                                                 ASR_SHAFII);
    next_prayer_t next = prayer_next(times, jd);

    if (next.name) {
        double hours = (next.jd - jd) * 24.0;
        if (hours < 0.0) hours = 0.0;

        if (hours <= LA_PRAYER_URGENT_H) {
            count = la_add(out, count, max, LA_PRAYER, LA_SUGGESTION,
                           next.name, hours, 1.0);
        } else {
            count = la_add(out, count, max, LA_PRAYER, LA_INFO,
                           next.name, hours, 0.5);
        }
    }

    return count;
}

/* Check storm season (up to 1 alert) */
static int la_check_storm(double jd, double lat, double lon,
                          la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    (void)lon; /* storm_risk_at uses lat/lon/month */

    int month = month_from_jd(jd);
    storm_risk_t risk = storm_risk_at(lat, lon, month);

    if (risk.cyclone_risk > 0.3) {
        la_urgency_t urg = (risk.cyclone_risk > 0.7) ? LA_ALERT
                         : (risk.cyclone_risk > 0.5) ? LA_SUGGESTION
                         : LA_INFO;
        return la_add(out, 0, max, LA_STORM, urg,
                      "Cyclone season active in this region",
                      0.0, risk.cyclone_risk);
    }

    return 0;
}

/* Check convergence (up to 1 alert) */
static int la_check_convergence(double jd, la_entry_t *out, int max) {
    if (max <= 0) return 0;

    double sig = cd_significance(jd);

    if (sig > LA_CONVERGENCE_HIGH) {
        return la_add(out, 0, max, LA_CONVERGENCE, LA_ALERT,
                      "Major calendar convergence today",
                      0.0, sig);
    } else if (sig > LA_CONVERGENCE_THRESH) {
        return la_add(out, 0, max, LA_CONVERGENCE, LA_INFO,
                      "Calendar convergence today",
                      0.0, sig);
    }

    return 0;
}

/* Check solstice/equinox proximity (up to 1 alert) */
static int la_check_solstice(double jd, la_entry_t *out, int max) {
    if (max <= 0) return 0;

    int doy = day_of_year_from_jd(jd);
    double days = days_to_nearest_solar_event(doy);

    if (days <= LA_SOLSTICE_WINDOW_DAYS) {
        const char *event_name = nearest_solar_event_name(doy);
        la_urgency_t urg = (days <= 1.0) ? LA_SUGGESTION : LA_INFO;
        return la_add(out, 0, max, LA_SOLSTICE, urg,
                      event_name, days * 24.0, 1.0 - days / 7.0);
    }

    return 0;
}

/* Check avalanche risk (up to 1 alert) */
static int la_check_avalanche(double jd, double lat, double lon,
                              la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    (void)lon;

    /* Use nearest ski resort for elevation reference */
    int resort_idx = ski_resort_nearest(lat, lon);
    ski_resort_t resort = ski_resort_get(resort_idx);
    if (resort.id < 0) return 0;

    /* Only check if within range */
    double dist = haversine_km(lat, lon, resort.lat, resort.lon);
    if (dist > LA_SKI_RANGE_KM) return 0;

    int month = month_from_jd(jd);
    avalanche_risk_t risk = snow_avalanche_risk(resort.lat,
                                                 resort.summit_elev_m,
                                                 month);

    if (risk.overall_risk > LA_AVALANCHE_RISK_THRESH) {
        la_urgency_t urg = (risk.overall_risk > 0.8) ? LA_ALERT
                                                      : LA_SUGGESTION;
        return la_add(out, 0, max, LA_AVALANCHE, urg,
                      "Elevated avalanche risk in area",
                      0.0, risk.overall_risk);
    }

    return 0;
}

/* ===== Public API ===== */

const char *la_type_name(la_type_t type) {
    switch (type) {
        case LA_SURF:        return "Surf";
        case LA_TIDE:        return "Tide";
        case LA_SNOW:        return "Snow";
        case LA_GOLDEN_HOUR: return "Golden Hour";
        case LA_PRAYER:      return "Prayer";
        case LA_STORM:       return "Storm";
        case LA_CONVERGENCE: return "Convergence";
        case LA_SOLSTICE:    return "Solstice/Equinox";
        case LA_AVALANCHE:   return "Avalanche";
        default:             return "Unknown";
    }
}

const char *la_urgency_name(la_urgency_t urgency) {
    switch (urgency) {
        case LA_INFO:       return "Info";
        case LA_SUGGESTION: return "Suggestion";
        case LA_ALERT:      return "Alert";
        default:            return "Unknown";
    }
}

int la_type_count(void) {
    return LA_TYPE_COUNT;
}

la_result_t la_evaluate(double jd, double lat, double lon) {
    la_result_t result;
    memset(&result, 0, sizeof(result));

    int total = 0;
    int remaining = LA_MAX_ALERTS;
    int n;

    /* Surf */
    n = la_check_surf(jd, lat, lon, &result.alerts[total], remaining);
    total += n;
    remaining -= n;

    /* Tide */
    if (remaining > 0) {
        n = la_check_tide(jd, lat, lon,
                          &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Snow */
    if (remaining > 0) {
        n = la_check_snow(jd, lat, lon,
                          &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Golden hour */
    if (remaining > 0) {
        n = la_check_golden_hour(jd, lat, lon,
                                  &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Prayer */
    if (remaining > 0) {
        n = la_check_prayer(jd, lat, lon,
                            &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Storm */
    if (remaining > 0) {
        n = la_check_storm(jd, lat, lon,
                           &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Convergence (location-independent) */
    if (remaining > 0) {
        n = la_check_convergence(jd,
                                  &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Solstice/equinox (location-independent) */
    if (remaining > 0) {
        n = la_check_solstice(jd,
                               &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    /* Avalanche */
    if (remaining > 0) {
        n = la_check_avalanche(jd, lat, lon,
                                &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    result.count = total;
    la_sort(result.alerts, result.count);

    return result;
}

int la_any_active(double jd, double lat, double lon) {
    la_result_t r = la_evaluate(jd, lat, lon);
    return (r.count > 0) ? 1 : 0;
}

int la_filter_type(la_result_t result, la_type_t type,
                   la_entry_t *out, int max)
{
    if (max <= 0) return 0;
    int count = 0;
    for (int i = 0; i < result.count && count < max; i++) {
        if (result.alerts[i].type == type) {
            out[count] = result.alerts[i];
            count++;
        }
    }
    return count;
}
