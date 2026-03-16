/* local_events.c — Local event data for the Earth Extrovert Layer.
 * Pure data module: no malloc, no globals, no side effects.
 * Haversine formula: standard geodesy.
 * Astronomical events: solar declination from Jean Meeus (via solar_events.h). */

#include "local_events.h"
#include "../astronomy/solar_events.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define EARTH_RADIUS_KM 6371.0

/* Interest ID for stargazing (from interest_profile.h definitions) */
#define IP_STARGAZING_ID 1

/* --- String tables --- */

static const char *TYPE_NAMES[LE_TYPE_COUNT] = {
    "Nature",
    "Culture",
    "Social",
    "Alert",
    "Astronomical"
};

static const char *SOURCE_NAMES[LE_SRC_COUNT] = {
    "Auto",
    "User",
    "External"
};

/* --- Event creation --- */

le_event_t le_create(le_type_t type, const char *title, const char *desc,
                     double lat, double lon, double radius_km,
                     double start_jd, double end_jd)
{
    le_event_t e;
    memset(&e, 0, sizeof(e));
    e.type = type;
    e.source = LE_SRC_USER;
    e.lat = lat;
    e.lon = lon;
    e.radius_km = radius_km;
    e.start_jd = start_jd;
    e.end_jd = end_jd;
    e.tag_count = 0;

    if (title != NULL) {
        snprintf(e.title, LE_TITLE_MAX, "%s", title);
    } else {
        e.title[0] = '\0';
    }

    if (desc != NULL) {
        snprintf(e.description, LE_DESC_MAX, "%s", desc);
    } else {
        e.description[0] = '\0';
    }

    return e;
}

le_event_t le_add_tag(le_event_t event, int interest_id)
{
    if (event.tag_count < LE_MAX_TAGS) {
        event.tags[event.tag_count] = interest_id;
        event.tag_count++;
    }
    return event;
}

/* --- Filters --- */

le_filter_t le_filter_default(void)
{
    le_filter_t f;
    memset(&f, 0, sizeof(f));
    return f;
}

le_filter_t le_filter_nearby(double lat, double lon, double radius_km)
{
    le_filter_t f = le_filter_default();
    f.center_lat = lat;
    f.center_lon = lon;
    f.max_distance_km = radius_km;
    return f;
}

/* --- Haversine distance --- */

double le_distance_km(double lat1, double lon1, double lat2, double lon2)
{
    double dlat = (lat2 - lat1) * DEG2RAD;
    double dlon = (lon2 - lon1) * DEG2RAD;
    double la1 = lat1 * DEG2RAD;
    double la2 = lat2 * DEG2RAD;

    double a = sin(dlat * 0.5) * sin(dlat * 0.5) +
               cos(la1) * cos(la2) *
               sin(dlon * 0.5) * sin(dlon * 0.5);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return EARTH_RADIUS_KM * c;
}

/* --- Scoring --- */

static double compute_distance_decay(double distance_km, double max_distance_km)
{
    if (max_distance_km <= 0.0) {
        return 1.0;
    }
    if (distance_km >= max_distance_km) {
        return 0.0;
    }
    return 1.0 - (distance_km / max_distance_km);
}

static double compute_interest_match(const le_event_t *event,
                                     const ip_profile_t *profile)
{
    if (profile == NULL || event->tag_count == 0) {
        return 1.0;
    }

    double total_weight = 0.0;
    double max_possible = 0.0;
    for (int i = 0; i < event->tag_count; i++) {
        int tid = event->tags[i];
        if (tid >= 0 && tid < IP_MAX_INTERESTS) {
            total_weight += profile->weights[tid];
            max_possible += 1.0;
        }
    }

    if (max_possible <= 0.0) {
        return 0.0;
    }

    return total_weight / max_possible;
}

static double compute_recency(double event_jd, double filter_jd)
{
    if (filter_jd <= 0.0) {
        return 1.0;
    }

    double diff = fabs(event_jd - filter_jd);

    if (diff >= 30.0) {
        return 0.0;
    }

    return 1.0 - (diff / 30.0);
}

double le_score(const le_event_t *event, const le_filter_t *filter)
{
    if (event == NULL || filter == NULL) {
        return 0.0;
    }

    double dist = le_distance_km(filter->center_lat, filter->center_lon,
                                 event->lat, event->lon);
    double distance_decay = compute_distance_decay(dist, filter->max_distance_km);

    if (distance_decay <= 0.0) {
        return 0.0;
    }

    double interest = compute_interest_match(event, filter->profile);
    double recency = compute_recency(event->start_jd, filter->start_jd);

    double score = distance_decay * interest * recency;

    if (score < 0.0) score = 0.0;
    if (score > 1.0) score = 1.0;

    return score;
}

/* --- Filtering --- */

static void insertion_sort_scored(le_scored_t *items, int count)
{
    for (int i = 1; i < count; i++) {
        le_scored_t key = items[i];
        int j = i - 1;
        while (j >= 0 && items[j].relevance < key.relevance) {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = key;
    }
}

le_result_t le_filter_events(const le_event_t *events, int count,
                             const le_filter_t *filter)
{
    le_result_t result;
    result.count = 0;

    if (events == NULL || count <= 0 || filter == NULL) {
        return result;
    }

    for (int i = 0; i < count && result.count < LE_MAX_EVENTS; i++) {
        const le_event_t *e = &events[i];

        if (filter->type_mask != 0) {
            if (!(filter->type_mask & (1 << e->type))) {
                continue;
            }
        }

        double score = le_score(e, filter);

        if (score < filter->min_relevance) {
            continue;
        }

        le_scored_t scored;
        scored.event = *e;
        scored.relevance = score;
        scored.distance_km = le_distance_km(filter->center_lat, filter->center_lon,
                                            e->lat, e->lon);
        scored.interest_match = compute_interest_match(e, filter->profile);
        scored.recency = compute_recency(e->start_jd, filter->start_jd);

        result.items[result.count] = scored;
        result.count++;
    }

    insertion_sort_scored(result.items, result.count);

    return result;
}

/* --- Astronomical event generation --- */

static double find_declination_zero(double jd_a, double jd_b)
{
    double dec_a = se_solar_declination(jd_a);
    for (int iter = 0; iter < 50; iter++) {
        double mid = (jd_a + jd_b) * 0.5;
        double dec_mid = se_solar_declination(mid);
        if ((dec_a > 0.0 && dec_mid > 0.0) || (dec_a < 0.0 && dec_mid < 0.0)) {
            jd_a = mid;
            dec_a = dec_mid;
        } else {
            jd_b = mid;
        }
    }
    return (jd_a + jd_b) * 0.5;
}

static double find_declination_extremum(double jd_a, double jd_b)
{
    static const double PHI_INV = 0.6180339887498949;
    double c = jd_b - PHI_INV * (jd_b - jd_a);
    double d = jd_a + PHI_INV * (jd_b - jd_a);

    for (int iter = 0; iter < 50; iter++) {
        double fc = fabs(se_solar_declination(c));
        double fd = fabs(se_solar_declination(d));
        if (fc > fd) {
            jd_b = d;
            d = c;
            c = jd_b - PHI_INV * (jd_b - jd_a);
        } else {
            jd_a = c;
            c = d;
            d = jd_a + PHI_INV * (jd_b - jd_a);
        }
    }
    return (jd_a + jd_b) * 0.5;
}

static int add_astro_event(le_event_t *out, int idx, int max,
                           const char *title, const char *desc,
                           double jd, double lat, double lon)
{
    if (idx >= max) return idx;
    out[idx] = le_create(LE_TYPE_ASTRONOMICAL, title, desc,
                         lat, lon, 0.0, jd, 0.0);
    out[idx].source = LE_SRC_AUTO;
    out[idx] = le_add_tag(out[idx], IP_STARGAZING_ID);
    return idx + 1;
}

int le_generate_astronomical(double jd_start, double jd_end,
                             double lat, double lon,
                             le_event_t *out, int max)
{
    if (jd_start >= jd_end || out == NULL || max <= 0) {
        return 0;
    }

    int count = 0;
    static const double STEP = 10.0;
    double prev_dec = se_solar_declination(jd_start);
    double prev_jd = jd_start;

    double last_solstice_jd = 0.0;

    for (double jd = jd_start + STEP; jd <= jd_end && count < max; jd += STEP) {
        double dec = se_solar_declination(jd);

        /* Equinox: declination crosses zero */
        if ((prev_dec > 0.0 && dec <= 0.0) || (prev_dec < 0.0 && dec >= 0.0)) {
            double eq_jd = find_declination_zero(prev_jd, jd);
            if (eq_jd >= jd_start && eq_jd <= jd_end) {
                if (prev_dec > 0.0) {
                    count = add_astro_event(out, count, max,
                        "Autumnal Equinox",
                        "Sun crosses celestial equator southward",
                        eq_jd, lat, lon);
                } else {
                    count = add_astro_event(out, count, max,
                        "Vernal Equinox",
                        "Sun crosses celestial equator northward",
                        eq_jd, lat, lon);
                }
            }
        }

        /* Summer solstice: declination was above 23 deg and starts decreasing */
        if (prev_dec > 23.0 && dec < prev_dec &&
            fabs(jd - last_solstice_jd) > 30.0) {
            double sol_jd = find_declination_extremum(prev_jd - STEP, jd);
            if (sol_jd >= jd_start && sol_jd <= jd_end) {
                count = add_astro_event(out, count, max,
                    "Summer Solstice",
                    "Longest day, sun reaches maximum declination",
                    sol_jd, lat, lon);
                last_solstice_jd = sol_jd;
            }
        }

        /* Winter solstice: declination was below -23 deg and starts increasing */
        if (prev_dec < -23.0 && dec > prev_dec &&
            fabs(jd - last_solstice_jd) > 30.0) {
            double sol_jd = find_declination_extremum(prev_jd - STEP, jd);
            if (sol_jd >= jd_start && sol_jd <= jd_end) {
                count = add_astro_event(out, count, max,
                    "Winter Solstice",
                    "Shortest day, sun reaches minimum declination",
                    sol_jd, lat, lon);
                last_solstice_jd = sol_jd;
            }
        }

        prev_dec = dec;
        prev_jd = jd;
    }

    return count;
}

/* --- Name lookups --- */

const char *le_type_name(le_type_t type)
{
    if (type < 0 || type >= LE_TYPE_COUNT) {
        return "Unknown";
    }
    return TYPE_NAMES[type];
}

const char *le_source_name(le_source_t source)
{
    if (source < 0 || source >= LE_SRC_COUNT) {
        return "Unknown";
    }
    return SOURCE_NAMES[source];
}

/* --- Count --- */

int le_count_by_type(const le_result_t *results, le_type_t type)
{
    if (results == NULL) return 0;
    int n = 0;
    for (int i = 0; i < results->count; i++) {
        if (results->items[i].event.type == type) {
            n++;
        }
    }
    return n;
}
