/* scroll_layers.h -- Scroll-driven layer visibility and parallax system
 *
 * Maps continuous scroll input to a logarithmic zoom level (phi-based),
 * determines which information layers are visible at each zoom level,
 * applies fade-in/fade-out transitions at layer boundaries, and provides
 * parallax depth factors for layered motion.
 *
 * Camera perspective (pitch, distance, FOV) interpolates between
 * keyframes as the user scrolls through scale levels.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_SCROLL_LAYERS_H
#define TIME_SCROLL_LAYERS_H

#define PI 3.14159265358979323846

/* Information layers that can be shown/hidden during scroll */
typedef enum {
    SCROLL_LAYER_STARS = 0,       /* background star field */
    SCROLL_LAYER_ORBITS,          /* planetary orbit lines */
    SCROLL_LAYER_PLANETS,         /* planet markers */
    SCROLL_LAYER_ZODIAC,          /* zodiac ring */
    SCROLL_LAYER_LABELS,          /* text labels */
    SCROLL_LAYER_ASPECTS,         /* aspect lines */
    SCROLL_LAYER_HOUSES,          /* house divisions */
    SCROLL_LAYER_CARDS,           /* info cards overlay */
    SCROLL_LAYER_HUD,             /* heads-up display data */
    SCROLL_LAYER_GALAXY,          /* galaxy scale visualization */
    SCROLL_LAYER_EARTH,           /* earth surface view */
    SCROLL_LAYER_COUNT
} scroll_layer_t;

/* Parallax depth tier */
typedef enum {
    SCROLL_DEPTH_BACKGROUND = 0,  /* moves slowest */
    SCROLL_DEPTH_MID,             /* middle parallax */
    SCROLL_DEPTH_FOREGROUND,      /* moves with scroll */
    SCROLL_DEPTH_OVERLAY,         /* fixed to screen */
    SCROLL_DEPTH_COUNT
} scroll_depth_t;

/* Layer visibility rule: defines when a layer appears */
typedef struct {
    scroll_layer_t layer;
    float zoom_min;          /* minimum zoom level for visibility (0.0+) */
    float zoom_max;          /* maximum zoom level for visibility */
    float fade_in_width;     /* zoom range for fade-in transition */
    float fade_out_width;    /* zoom range for fade-out transition */
    scroll_depth_t depth;    /* parallax depth tier */
    float parallax_factor;   /* 0.0 = fixed, 1.0 = full scroll, >1.0 = foreground */
} scroll_layer_rule_t;

/* Camera perspective at a scroll position */
typedef struct {
    float pitch;       /* camera pitch angle in radians */
    float distance;    /* camera distance from origin */
    float fov;         /* field of view in radians */
} scroll_perspective_t;

/* Get layer opacity for a given zoom level (0.0-1.0).
 * Applies fade-in/fade-out at boundaries. */
float scroll_layer_opacity(scroll_layer_t layer, float zoom_level);

/* Get the visibility rule for a layer. */
scroll_layer_rule_t scroll_layer_rule(scroll_layer_t layer);

/* Get the parallax offset factor for a layer at given scroll delta.
 * Returns scaled scroll delta (slower for background, faster for foreground). */
float scroll_parallax_offset(scroll_layer_t layer, float scroll_delta);

/* Convert continuous scroll position to logarithmic zoom level.
 * scroll: raw scroll wheel accumulator (can be negative).
 * Returns zoom = phi^scroll (always positive). */
float scroll_to_zoom(float scroll_position);

/* Inverse: zoom level to scroll position.
 * Returns log(zoom) / log(phi). */
float zoom_to_scroll(float zoom_level);

/* Get camera perspective for a zoom level.
 * Linearly interpolates between keyframes at zoom 0, 3, and 6. */
scroll_perspective_t scroll_perspective(float zoom_level);

/* Get layer name string. */
const char *scroll_layer_name(scroll_layer_t layer);

/* Get depth tier name string. */
const char *scroll_depth_name(scroll_depth_t depth);

/* Get number of visible layers at a given zoom level (opacity > 0). */
int scroll_visible_count(float zoom_level);

/* Is a layer visible at the given zoom level? (opacity > 0) */
int scroll_is_visible(scroll_layer_t layer, float zoom_level);

/* Get the parallax factor for a depth tier.
 * Background=phi^-2, Mid=phi^-1, Foreground=1.0, Overlay=0.0 */
float scroll_depth_parallax(scroll_depth_t depth);

#endif /* TIME_SCROLL_LAYERS_H */
