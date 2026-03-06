/* climate_history.h — Major climate events in Earth's 4.6-billion-year history.
 * Static const data for geological timeline display. Each event includes
 * time range, temperature anomaly, CO2 levels, type, and severity.
 * All pure functions, no side effects. */

#ifndef TIME_CLIMATE_HISTORY_H
#define TIME_CLIMATE_HISTORY_H

#include <stdbool.h>

/* Climate event severity */
typedef enum {
    CLIMATE_SEVERITY_MINOR = 0,    /* Regional or short-lived */
    CLIMATE_SEVERITY_MODERATE,      /* Continental-scale or multi-century */
    CLIMATE_SEVERITY_MAJOR,         /* Global, multi-million year */
    CLIMATE_SEVERITY_CATASTROPHIC   /* Mass extinction or planetary transformation */
} climate_severity_t;

/* Climate event type */
typedef enum {
    CLIMATE_TYPE_GLACIATION = 0,   /* Ice ages, snowball events */
    CLIMATE_TYPE_WARMING,           /* Thermal maxima, greenhouse events */
    CLIMATE_TYPE_ATMOSPHERIC,       /* Composition changes (O2, CO2) */
    CLIMATE_TYPE_OSCILLATION,       /* Periodic cycles (Milankovitch, etc.) */
    CLIMATE_TYPE_IMPACT             /* Extraterrestrial cause */
} climate_type_t;

typedef struct {
    int id;                     /* Unique index (-1 for invalid) */
    const char *name;           /* Event name */
    double start_ma;            /* Start (millions of years ago) */
    double end_ma;              /* End (millions of years ago, 0 = present) */
    double temp_anomaly_c;      /* Temperature anomaly in C (+ = warming, - = cooling) */
    double co2_ppm;             /* Approximate CO2 level (ppm), 0 if unknown */
    climate_type_t type;        /* Event category */
    climate_severity_t severity; /* How impactful */
    const char *description;    /* Brief explanation */
} climate_event_t;

/* Get total number of climate events. */
int climate_event_count(void);

/* Get climate event by index. Returns zeroed struct with id=-1 for invalid. */
climate_event_t climate_event_get(int index);

/* Find the climate event active at a given time (Ma).
 * Returns the most severe event overlapping that time, or zeroed struct
 * with id=-1 if none. start_ma >= ma >= end_ma for match. */
climate_event_t climate_event_at(double ma);

/* Get event name by index. Returns "?" for invalid. */
const char *climate_event_name(int index);

/* Get severity name string. */
const char *climate_severity_name(climate_severity_t severity);

/* Get type name string. */
const char *climate_type_name(climate_type_t type);

/* Count events of a given type. */
int climate_events_of_type(climate_type_t type);

/* Count events of a given severity. */
int climate_events_of_severity(climate_severity_t severity);

/* Find events within a time range (Ma).
 * Writes matching indices to out_indices (up to max_results).
 * An event matches if it overlaps with [start_ma, end_ma] at all.
 * Returns number of matches found. */
int climate_events_in_range(double start_ma, double end_ma,
                            int *out_indices, int max_results);

/* Is there a glaciation event at this time? */
bool climate_is_glaciated(double ma);

#endif /* TIME_CLIMATE_HISTORY_H */
