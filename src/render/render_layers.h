#ifndef TIME_RENDER_LAYERS_H
#define TIME_RENDER_LAYERS_H

#include "camera_scale.h"

/* Visual layers that can be toggled and faded by scale.
 * Each layer has a scale range where it is visible,
 * with opacity ramping at the edges. */

#define LAYER_COUNT 8

typedef enum {
    LAYER_STARS       = 0,  /* Background star field */
    LAYER_GALAXY      = 1,  /* Milky Way spiral */
    LAYER_ORBITS      = 2,  /* Orbital paths */
    LAYER_PLANETS     = 3,  /* Planet spheres */
    LAYER_ZODIAC_RING = 4,  /* Ecliptic zodiac wheel */
    LAYER_LABELS      = 5,  /* Text labels on planets/signs */
    LAYER_CARDS       = 6,  /* Knowledge system info cards */
    LAYER_HUD         = 7   /* Time HUD overlay */
} layer_id_t;

typedef struct {
    layer_id_t id;
    const char *name;
    int scale_min;       /* first scale where layer appears (inclusive) */
    int scale_max;       /* last scale where layer appears (inclusive) */
    float base_opacity;  /* maximum opacity when fully visible (0.0-1.0) */
    int user_visible;    /* 1 = user toggled on, 0 = user toggled off */
} layer_config_t;

typedef struct {
    float opacity[LAYER_COUNT]; /* computed opacity for each layer (0.0-1.0) */
} layer_state_t;

/* Get default config for a layer. */
layer_config_t layer_default_config(layer_id_t id);

/* Compute opacity for a single layer given current log_zoom.
 * Ramps from 0 to base_opacity over a fade zone at scale boundaries.
 * Returns 0.0 if user_visible is 0. */
float layer_opacity(layer_config_t config, float log_zoom);

/* Compute opacity for all layers at once. */
layer_state_t layers_compute(const layer_config_t configs[LAYER_COUNT],
                             float log_zoom);

/* Get the name of a layer. */
const char *layer_name(layer_id_t id);

/* Check if a layer is effectively visible (opacity > 0.01). */
int layer_is_visible(layer_state_t state, layer_id_t id);

#endif
