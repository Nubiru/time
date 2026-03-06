/* tectonic.h — Supercontinent cycle and major tectonic events.
 * Static const data covering 4.5 billion years of Earth's tectonic history.
 * All pure functions, no side effects. */

#ifndef TIME_TECTONIC_H
#define TIME_TECTONIC_H

#define SUPERCONTINENT_COUNT 7
#define TECTONIC_EVENT_MAX 32

/* Supercontinent in Earth's history */
typedef struct {
    int id;
    const char *name;           /* e.g., "Pangaea" */
    double start_ma;            /* formation age in Ma (millions of years ago) */
    double end_ma;              /* breakup age in Ma */
    const char *description;    /* brief characterization */
} supercontinent_t;

/* Major tectonic event */
typedef struct {
    int id;
    const char *name;           /* e.g., "Opening of the Atlantic" */
    double start_ma;            /* event start in Ma */
    double end_ma;              /* event end in Ma (same as start for instantaneous) */
    const char *type;           /* "rift", "orogeny", "collision", "subduction", "volcanism" */
    const char *description;    /* brief description */
} tectonic_event_t;

/* Get supercontinent by index (0 to SUPERCONTINENT_COUNT-1).
 * Returns struct with id=-1 and name=NULL for invalid index. */
supercontinent_t tectonic_supercontinent_get(int index);

/* Which supercontinent existed at a given time (Ma).
 * Returns zeroed struct with name=NULL if none (gap between supercontinents). */
supercontinent_t tectonic_supercontinent_at(double ma);

/* Supercontinent count (always 7). */
int tectonic_supercontinent_count(void);

/* Get tectonic event by index.
 * Returns struct with id=-1 and name=NULL for invalid index. */
tectonic_event_t tectonic_event_get(int index);

/* How many tectonic events in the database. */
int tectonic_event_count(void);

/* Find events overlapping a time range [start_ma, end_ma].
 * Both parameters are Ma values. Handles both orderings (swaps if needed).
 * Returns count of matching events written to out_indices (max out_max).
 * Returns 0 if out_indices is NULL. */
int tectonic_events_between(double start_ma, double end_ma,
                            int *out_indices, int out_max);

/* Find events of a given type (e.g., "orogeny").
 * Returns count of matching events written to out_indices.
 * Returns 0 if type or out_indices is NULL. */
int tectonic_events_by_type(const char *type,
                            int *out_indices, int out_max);

/* Name of supercontinent at given Ma, or "None" if gap. */
const char *tectonic_supercontinent_name_at(double ma);

/* Is given Ma within any supercontinent's lifespan? (1=yes, 0=no) */
int tectonic_in_supercontinent(double ma);

/* Time (Ma) of next supercontinent formation from given Ma.
 * "Next" means forward in time (lower Ma value).
 * Returns start_ma of the next supercontinent whose start_ma < ma.
 * Returns 0.0 if no future supercontinent in data. */
double tectonic_next_supercontinent(double ma);

#endif /* TIME_TECTONIC_H */
