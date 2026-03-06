#ifndef TIME_CAMERA_SCALE_H
#define TIME_CAMERA_SCALE_H

/* Multi-scale camera system.
 * Logarithmic zoom across 7 scale levels, from Personal Chart to Observable Universe.
 * All functions are pure — no GL, no globals. */

#define SCALE_COUNT 7

typedef enum {
    SCALE_PERSONAL  = 0,  /* Personal chart / natal view */
    SCALE_EARTH     = 1,  /* Earth-Moon system, observer horizon */
    SCALE_INNER     = 2,  /* Inner solar system, zodiac ring visible */
    SCALE_SOLAR     = 3,  /* Full solar system (current default) */
    SCALE_LOCAL     = 4,  /* Solar neighborhood, ~100 light-years */
    SCALE_GALAXY    = 5,  /* Milky Way */
    SCALE_UNIVERSE  = 6   /* Observable universe */
} scale_id_t;

typedef struct {
    scale_id_t id;
    const char *name;       /* "Solar System", etc. */
    float log_zoom_min;     /* log(distance) lower bound */
    float log_zoom_max;     /* log(distance) upper bound */
    float log_zoom_center;  /* ideal camera distance (log) */
} scale_level_t;

/* Get the scale level definition by ID. */
scale_level_t scale_get(scale_id_t id);

/* Convert log_zoom value to camera distance. */
float scale_log_to_distance(float log_zoom);

/* Convert camera distance to log_zoom value. */
float scale_distance_to_log(float distance);

/* Determine which scale level a log_zoom value falls in. */
scale_id_t scale_from_log_zoom(float log_zoom);

/* Get the name of a scale level. */
const char *scale_name(scale_id_t id);

/* Get the center log_zoom for a scale level (for jump-to). */
float scale_center_log_zoom(scale_id_t id);

/* Minimum and maximum log_zoom across all scales. */
float scale_log_zoom_min(void);
float scale_log_zoom_max(void);

/* --- Scale transitions (smooth camera zoom between levels) --- */

typedef struct {
    float from_log_zoom;
    float to_log_zoom;
    float duration;       /* seconds */
    float elapsed;        /* seconds elapsed */
    int active;           /* 1 = transitioning, 0 = idle */
} scale_transition_t;

/* Create a transition to jump to a target scale level. */
scale_transition_t scale_transition_create(float current_log_zoom,
                                           scale_id_t target,
                                           float duration);

/* Advance the transition by dt seconds. Returns updated transition. */
scale_transition_t scale_transition_tick(scale_transition_t t, float dt);

/* Get the current log_zoom during a transition (ease-in-out interpolation). */
float scale_transition_value(scale_transition_t t);

/* Check if the transition is complete. */
int scale_transition_done(scale_transition_t t);

#endif
