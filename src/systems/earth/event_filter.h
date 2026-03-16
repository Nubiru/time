/* event_filter.h — Event Filter Engine for the Earth Extrovert Layer.
 * Multi-criteria filtering, smart presets, and anti-spam rate limiting.
 * Extends local_events.h filtering with blacklists, tag requirements,
 * and composable filter construction.
 * Pure module: no malloc, no globals, no side effects. */

#ifndef TIME_EVENT_FILTER_H
#define TIME_EVENT_FILTER_H

#include "local_events.h"
#include "interest_profile.h"

#define EF_MAX_TYPE_BLACKLIST 8
#define EF_MAX_TAG_REQUIRE 8

/* Filter preset identifiers */
typedef enum {
    EF_PRESET_ALL = 0,           /* No filtering */
    EF_PRESET_NATURE_LOVER,      /* Nature + stargazing + astronomical */
    EF_PRESET_NIGHT_OWL,         /* Astronomical + culture (evening) */
    EF_PRESET_CULTURAL,          /* Culture + knowledge + social */
    EF_PRESET_SOCIAL_BUTTERFLY,  /* Social + culture + nature (group) */
    EF_PRESET_COUNT
} ef_preset_t;

/* Time window presets */
typedef enum {
    EF_WINDOW_NEXT_HOUR = 0,
    EF_WINDOW_TODAY,
    EF_WINDOW_THIS_WEEK,
    EF_WINDOW_THIS_MONTH,
    EF_WINDOW_COUNT
} ef_window_t;

/* Advanced filter (extends le_filter_t) */
typedef struct {
    le_filter_t base;                    /* base location + time + type filter */
    int type_blacklist[EF_MAX_TYPE_BLACKLIST]; /* blocked types (-1 terminated) */
    int type_blacklist_count;
    int required_tags[EF_MAX_TAG_REQUIRE];    /* must-have interest tags (-1 terminated) */
    int required_tag_count;
    int require_all_tags;                /* 1=AND (all tags), 0=OR (any tag) */
    double max_events_per_hour;          /* anti-spam: 0 = no limit */
    int astronomical_only;               /* 1 = only show LE_TYPE_ASTRONOMICAL */
} ef_filter_t;

/* Rate limiter state (passed by value, functional style) */
typedef struct {
    double last_surface_jd;     /* JD when last event was surfaced */
    int events_this_hour;       /* counter */
    double hour_start_jd;       /* start of current hour window */
} ef_rate_t;

/* Create a default advanced filter (no restrictions). */
ef_filter_t ef_default(void);

/* Create a filter from a preset. */
ef_filter_t ef_from_preset(ef_preset_t preset, double lat, double lon, double radius_km);

/* Apply a time window to a filter. */
ef_filter_t ef_set_window(ef_filter_t filter, ef_window_t window, double now_jd);

/* Add a type to the blacklist. */
ef_filter_t ef_blacklist_type(ef_filter_t filter, le_type_t type);

/* Require a specific interest tag. */
ef_filter_t ef_require_tag(ef_filter_t filter, int interest_id);

/* Set AND/OR mode for required tags. */
ef_filter_t ef_set_tag_mode(ef_filter_t filter, int require_all);

/* Set anti-spam rate limit. */
ef_filter_t ef_set_rate_limit(ef_filter_t filter, double max_per_hour);

/* Apply advanced filter to events. Returns filtered + scored results. */
le_result_t ef_apply(const le_event_t *events, int count, const ef_filter_t *filter);

/* Check if an event passes the advanced filter (without scoring). */
int ef_passes(const le_event_t *event, const ef_filter_t *filter);

/* Rate limiter: initialize. */
ef_rate_t ef_rate_init(void);

/* Rate limiter: check if we can surface another event. */
int ef_rate_allow(ef_rate_t state, double now_jd, double max_per_hour);

/* Rate limiter: record that an event was surfaced. */
ef_rate_t ef_rate_record(ef_rate_t state, double now_jd, double max_per_hour);

/* Preset name string. */
const char *ef_preset_name(ef_preset_t preset);

/* Window name string. */
const char *ef_window_name(ef_window_t window);

/* Window duration in days. */
double ef_window_days(ef_window_t window);

#endif /* TIME_EVENT_FILTER_H */
