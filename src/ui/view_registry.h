#ifndef TIME_VIEW_REGISTRY_H
#define TIME_VIEW_REGISTRY_H

/* Canonical view definitions and configuration for the Time project.
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Self-contained: does not depend on camera_scale.h or render_layers.h.
 * Uses int values for scale levels and layer bitmasks. */

#define VIEW_COUNT 7

typedef enum {
    VIEW_SPACE     = 0,  /* Solar system from outside (existing default) */
    VIEW_EARTH     = 1,  /* Globe surface (existing, E key) */
    VIEW_GALAXY    = 2,  /* Milky Way scale (existing, Shift+5-6) */
    VIEW_PERSONAL  = 3,  /* Birth chart focus (planned) */
    VIEW_DEEP_TIME = 4,  /* Geological timeline (planned) */
    VIEW_CITY      = 5,  /* Local horizon + sky (planned) */
    VIEW_ROOM      = 6   /* Personal 3D space (far future) */
} view_id_t;

typedef struct {
    view_id_t   id;
    const char *name;           /* "Space View", "Earth View", etc. */
    const char *short_name;     /* "Space", "Earth", etc. */
    const char *description;    /* one-line description */
    const char *shortcut;       /* keyboard shortcut string */
    int         scale_min;      /* camera scale range lower bound */
    int         scale_max;      /* camera scale range upper bound */
    int         default_scale;  /* starting scale level */
    int         layers_enabled; /* bitmask of active layers */
    int         implemented;    /* 1 = exists in code, 0 = planned */
} view_config_t;

/* Get full configuration for a view by ID.
 * Returns a sentinel config with id=-1 and name="Unknown" for invalid IDs. */
view_config_t view_get(view_id_t id);

/* Get the display name for a view. Returns "Unknown" for invalid IDs. */
const char *view_name(view_id_t id);

/* Total number of defined views (equals VIEW_COUNT). */
int view_count(void);

/* Number of views currently implemented in code. */
int view_implemented_count(void);

/* Find which view a camera scale level belongs to.
 * Searches implemented views first. Returns VIEW_SPACE as fallback. */
view_id_t view_for_scale(int scale_level);

/* Find a view by name (case-insensitive).
 * Matches against both full name ("Space View") and short name ("Space").
 * Returns -1 (cast to view_id_t) if NULL, empty, or no match. */
view_id_t view_from_name(const char *name);

#endif /* TIME_VIEW_REGISTRY_H */
