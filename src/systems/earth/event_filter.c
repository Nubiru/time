/* event_filter.c — Event Filter Engine for the Earth Extrovert Layer.
 * Pure data module: no malloc, no globals, no side effects.
 * Extends local_events filtering with blacklists, tag requirements,
 * presets, and functional rate limiting. */

#include "event_filter.h"

#include <string.h>

/* --- Time window durations in days --- */

static const double WINDOW_DAYS[EF_WINDOW_COUNT] = {
    1.0 / 24.0,   /* NEXT_HOUR */
    1.0,           /* TODAY */
    7.0,           /* THIS_WEEK */
    30.0           /* THIS_MONTH */
};

/* --- String tables --- */

static const char *PRESET_NAMES[EF_PRESET_COUNT] = {
    "All",
    "Nature Lover",
    "Night Owl",
    "Cultural",
    "Social Butterfly"
};

static const char *WINDOW_NAMES[EF_WINDOW_COUNT] = {
    "Next Hour",
    "Today",
    "This Week",
    "This Month"
};

/* --- Default filter --- */

ef_filter_t ef_default(void)
{
    ef_filter_t f;
    memset(&f, 0, sizeof(f));
    return f;
}

/* --- Presets --- */

ef_filter_t ef_from_preset(ef_preset_t preset, double lat, double lon, double radius_km)
{
    ef_filter_t f = ef_default();
    f.base.center_lat = lat;
    f.base.center_lon = lon;

    switch (preset) {
    case EF_PRESET_NATURE_LOVER:
        f.base.type_mask = (1 << LE_TYPE_NATURE) | (1 << LE_TYPE_ASTRONOMICAL);
        f.base.max_distance_km = (radius_km > 0.0) ? radius_km : 100.0;
        break;

    case EF_PRESET_NIGHT_OWL:
        f.base.type_mask = (1 << LE_TYPE_ASTRONOMICAL) | (1 << LE_TYPE_CULTURE);
        f.base.max_distance_km = (radius_km > 0.0) ? radius_km : 50.0;
        break;

    case EF_PRESET_CULTURAL:
        f.base.type_mask = (1 << LE_TYPE_CULTURE) | (1 << LE_TYPE_SOCIAL);
        f.base.max_distance_km = (radius_km > 0.0) ? radius_km : 30.0;
        break;

    case EF_PRESET_SOCIAL_BUTTERFLY:
        f.base.type_mask = (1 << LE_TYPE_SOCIAL) | (1 << LE_TYPE_CULTURE) |
                           (1 << LE_TYPE_NATURE);
        f.base.max_distance_km = (radius_km > 0.0) ? radius_km : 50.0;
        break;

    case EF_PRESET_ALL:
    default:
        f.base.max_distance_km = (radius_km > 0.0) ? radius_km : 0.0;
        break;
    }

    return f;
}

/* --- Window --- */

ef_filter_t ef_set_window(ef_filter_t filter, ef_window_t window, double now_jd)
{
    filter.base.start_jd = now_jd;
    if (window >= 0 && window < EF_WINDOW_COUNT) {
        filter.base.end_jd = now_jd + WINDOW_DAYS[window];
    }
    return filter;
}

/* --- Filter builders --- */

ef_filter_t ef_blacklist_type(ef_filter_t filter, le_type_t type)
{
    if (filter.type_blacklist_count < EF_MAX_TYPE_BLACKLIST) {
        filter.type_blacklist[filter.type_blacklist_count] = (int)type;
        filter.type_blacklist_count++;
    }
    return filter;
}

ef_filter_t ef_require_tag(ef_filter_t filter, int interest_id)
{
    if (filter.required_tag_count < EF_MAX_TAG_REQUIRE) {
        filter.required_tags[filter.required_tag_count] = interest_id;
        filter.required_tag_count++;
    }
    return filter;
}

ef_filter_t ef_set_tag_mode(ef_filter_t filter, int require_all)
{
    filter.require_all_tags = require_all;
    return filter;
}

ef_filter_t ef_set_rate_limit(ef_filter_t filter, double max_per_hour)
{
    filter.max_events_per_hour = max_per_hour;
    return filter;
}

/* --- ef_passes --- */

int ef_passes(const le_event_t *event, const ef_filter_t *filter)
{
    if (event == NULL || filter == NULL) {
        return 0;
    }

    /* Check astronomical_only */
    if (filter->astronomical_only && event->type != LE_TYPE_ASTRONOMICAL) {
        return 0;
    }

    /* Check type blacklist */
    for (int i = 0; i < filter->type_blacklist_count; i++) {
        if (filter->type_blacklist[i] == (int)event->type) {
            return 0;
        }
    }

    /* Check required tags */
    if (filter->required_tag_count > 0) {
        if (filter->require_all_tags) {
            /* AND mode: every required tag must be present in event */
            for (int r = 0; r < filter->required_tag_count; r++) {
                int found = 0;
                for (int t = 0; t < event->tag_count; t++) {
                    if (event->tags[t] == filter->required_tags[r]) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    return 0;
                }
            }
        } else {
            /* OR mode: at least one required tag must be present */
            int any_found = 0;
            for (int r = 0; r < filter->required_tag_count; r++) {
                for (int t = 0; t < event->tag_count; t++) {
                    if (event->tags[t] == filter->required_tags[r]) {
                        any_found = 1;
                        break;
                    }
                }
                if (any_found) break;
            }
            if (!any_found) {
                return 0;
            }
        }
    }

    return 1;
}

/* --- ef_apply --- */

le_result_t ef_apply(const le_event_t *events, int count, const ef_filter_t *filter)
{
    le_result_t result;
    result.count = 0;

    if (events == NULL || count <= 0 || filter == NULL) {
        return result;
    }

    /* First pass: use base filter via le_filter_events */
    le_result_t base_result = le_filter_events(events, count, &filter->base);

    /* Second pass: apply advanced criteria via ef_passes */
    for (int i = 0; i < base_result.count && result.count < LE_MAX_EVENTS; i++) {
        if (ef_passes(&base_result.items[i].event, filter)) {
            result.items[result.count] = base_result.items[i];
            result.count++;
        }
    }

    return result;
}

/* --- Rate limiter --- */

ef_rate_t ef_rate_init(void)
{
    ef_rate_t rate;
    memset(&rate, 0, sizeof(rate));
    return rate;
}

static int is_new_hour(double hour_start_jd, double now_jd)
{
    double hour_in_days = 1.0 / 24.0;
    return (now_jd - hour_start_jd) >= hour_in_days;
}

int ef_rate_allow(ef_rate_t state, double now_jd, double max_per_hour)
{
    /* Zero limit means no limit */
    if (max_per_hour <= 0.0) {
        return 1;
    }

    /* If we're in a new hour window, counter would reset — always allow */
    if (is_new_hour(state.hour_start_jd, now_jd)) {
        return 1;
    }

    /* Under the limit? */
    return (state.events_this_hour < (int)max_per_hour) ? 1 : 0;
}

ef_rate_t ef_rate_record(ef_rate_t state, double now_jd, double max_per_hour)
{
    (void)max_per_hour;

    /* If new hour window, reset counter */
    if (is_new_hour(state.hour_start_jd, now_jd)) {
        state.hour_start_jd = now_jd;
        state.events_this_hour = 0;
    }

    state.events_this_hour++;
    state.last_surface_jd = now_jd;

    return state;
}

/* --- Name lookups --- */

const char *ef_preset_name(ef_preset_t preset)
{
    if (preset < 0 || preset >= EF_PRESET_COUNT) {
        return "Unknown";
    }
    return PRESET_NAMES[preset];
}

const char *ef_window_name(ef_window_t window)
{
    if (window < 0 || window >= EF_WINDOW_COUNT) {
        return "Unknown";
    }
    return WINDOW_NAMES[window];
}

double ef_window_days(ef_window_t window)
{
    if (window < 0 || window >= EF_WINDOW_COUNT) {
        return 0.0;
    }
    return WINDOW_DAYS[window];
}
