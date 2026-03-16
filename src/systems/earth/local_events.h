/* local_events.h — Local event data for the Earth Extrovert Layer.
 * Event structures, filtering by distance/interest/time, relevance scoring,
 * and auto-generation of astronomical events from solar declination.
 * Source: Haversine formula (standard geodesy), Jean Meeus (solar events). */

#ifndef TIME_LOCAL_EVENTS_H
#define TIME_LOCAL_EVENTS_H

#include "interest_profile.h"

#define LE_MAX_EVENTS 64
#define LE_MAX_TAGS 8
#define LE_TITLE_MAX 96
#define LE_DESC_MAX 256

/* Event types */
typedef enum {
    LE_TYPE_NATURE = 0,       /* hike, surf, stargaze */
    LE_TYPE_CULTURE,          /* show, exhibit, food */
    LE_TYPE_SOCIAL,           /* meetup, game, sport */
    LE_TYPE_ALERT,            /* weather, closure */
    LE_TYPE_ASTRONOMICAL,     /* eclipse, conjunction, solstice, meteor shower */
    LE_TYPE_COUNT
} le_type_t;

/* Event source */
typedef enum {
    LE_SRC_AUTO = 0,          /* auto-generated from astronomical data */
    LE_SRC_USER,              /* user-created */
    LE_SRC_EXTERNAL,          /* future: external API */
    LE_SRC_COUNT
} le_source_t;

/* A single local event */
typedef struct {
    le_type_t type;
    le_source_t source;
    char title[LE_TITLE_MAX];
    char description[LE_DESC_MAX];
    double lat;                /* event latitude */
    double lon;                /* event longitude */
    double radius_km;          /* relevant radius */
    double start_jd;           /* event start (Julian Day) */
    double end_jd;             /* event end (0 = instantaneous) */
    int tags[LE_MAX_TAGS];     /* interest IDs from interest_profile (-1 terminated) */
    int tag_count;
} le_event_t;

/* Filter criteria */
typedef struct {
    double center_lat;         /* observer lat */
    double center_lon;         /* observer lon */
    double max_distance_km;    /* 0 = no distance filter */
    double start_jd;           /* time window start (0 = now) */
    double end_jd;             /* time window end (0 = no limit) */
    int type_mask;             /* bitmask of le_type_t (0 = all) */
    const ip_profile_t *profile; /* NULL = no interest filtering */
    double min_relevance;      /* 0.0-1.0 threshold */
} le_filter_t;

/* Scored event result */
typedef struct {
    le_event_t event;
    double relevance;          /* 0.0-1.0 composite score */
    double distance_km;        /* from observer */
    double interest_match;     /* 0.0-1.0 tag match */
    double recency;            /* 0.0-1.0 time proximity */
} le_scored_t;

/* Result set */
typedef struct {
    le_scored_t items[LE_MAX_EVENTS];
    int count;
} le_result_t;

/* Create an event. Source defaults to LE_SRC_USER. */
le_event_t le_create(le_type_t type, const char *title, const char *desc,
                     double lat, double lon, double radius_km,
                     double start_jd, double end_jd);

/* Add a tag to an event. Returns modified event. Ignores if full. */
le_event_t le_add_tag(le_event_t event, int interest_id);

/* Create a default filter (no restrictions). */
le_filter_t le_filter_default(void);

/* Create a filter centered on a location with radius. */
le_filter_t le_filter_nearby(double lat, double lon, double radius_km);

/* Haversine distance between two lat/lon points in km. */
double le_distance_km(double lat1, double lon1, double lat2, double lon2);

/* Score an event against a filter. Returns 0.0-1.0 relevance. */
double le_score(const le_event_t *event, const le_filter_t *filter);

/* Filter and score a batch of events. Results sorted by relevance descending. */
le_result_t le_filter_events(const le_event_t *events, int count,
                             const le_filter_t *filter);

/* Generate astronomical events for a JD range at a location.
 * Uses solar declination to find equinoxes and solstices.
 * Source: Jean Meeus, Astronomical Algorithms. */
int le_generate_astronomical(double jd_start, double jd_end,
                             double lat, double lon,
                             le_event_t *out, int max);

/* Type name string. */
const char *le_type_name(le_type_t type);

/* Source name string. */
const char *le_source_name(le_source_t source);

/* Count events matching a type in a result set. */
int le_count_by_type(const le_result_t *results, le_type_t type);

#endif /* TIME_LOCAL_EVENTS_H */
