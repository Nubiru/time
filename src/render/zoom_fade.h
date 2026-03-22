/*
 * zoom_fade.h — Zoom-level opacity for render passes
 *
 * Maps camera log_zoom to per-pass opacity (0.0-1.0) using smoothstep.
 * Each pass category defines a show range: fully visible within the range,
 * fading in/out at the edges. Passes outside their range draw nothing.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Header-only — all functions are static inline.
 *
 * Design principle: at EVERY zoom level, the visual should be appropriate,
 * beautiful, and uncluttered. No pass should draw when its content is
 * meaningless at the current scale.
 */

#ifndef TIME_ZOOM_FADE_H
#define TIME_ZOOM_FADE_H

/* Pass categories — each has a distinct zoom visibility range */
typedef enum {
    ZF_MILKYWAY,       /* Galaxy band: visible at galaxy/universe scale */
    ZF_STARS,          /* Star field: visible from local to galaxy */
    ZF_DEEP_SKY,       /* Nebulae, deep sky objects: local to galaxy */
    ZF_CONSTELLATION,  /* Constellation lines/labels: local to solar */
    ZF_ORBIT_TRAIL,    /* Planet orbit lines: inner to solar */
    ZF_PLANET,         /* Planet billboards: earth to solar */
    ZF_RING,           /* Concentric depth rings: inner to solar */
    ZF_ZODIAC,         /* Zodiac wheel/arcs: inner to solar */
    ZF_SUN,            /* Sun shader: earth to solar */
    ZF_CARD,           /* Knowledge cards: personal to inner */
    ZF_TEXT_LABEL,     /* Planet/star labels: earth to inner */
    ZF_MOON,           /* Moon: earth scale */
    ZF_EARTH_GLOBE,    /* Earth globe: earth scale */
    ZF_OVERLAY,        /* Always visible: wordmark, headline, wisdom */
    ZF_CATEGORY_COUNT
} zf_category_t;

/* Fade range: pass is visible between show_min and show_max (log_zoom).
 * Fades in over [show_min - FADE_WIDTH, show_min].
 * Fades out over [show_max, show_max + FADE_WIDTH]. */
typedef struct {
    float show_min;    /* log_zoom where pass becomes visible */
    float show_max;    /* log_zoom where pass starts fading out */
} zf_range_t;

/* Smooth transition width (log_zoom units) */
#define ZF_FADE_WIDTH 1.0f

/* Smoothstep helper */
static inline float zf_smoothstep(float edge0, float edge1, float x)
{
    float t = (x - edge0) / (edge1 - edge0);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return t * t * (3.0f - 2.0f * t);
}

/* Get the visibility range for a pass category.
 *
 * Log-zoom reference (from camera_scale.c):
 *   PERSONAL:  -4.6 to -0.7  (center -2.3)
 *   EARTH:     -0.7 to  0.7  (center  0.0)
 *   INNER:      0.7 to  2.1  (center  1.4)
 *   SOLAR:      2.1 to  4.4  (center  3.4)
 *   LOCAL:      4.4 to  6.2  (center  5.3)
 *   GALAXY:     6.2 to  8.5  (center  7.6)
 *   UNIVERSE:   8.5 to 10.8  (center  9.9)
 */
static inline zf_range_t zf_range(zf_category_t cat)
{
    zf_range_t r;
    switch (cat) {
    case ZF_MILKYWAY:      r.show_min = 5.0f;  r.show_max = 11.0f; break;
    case ZF_STARS:         r.show_min = 3.0f;  r.show_max =  9.0f; break;
    case ZF_DEEP_SKY:      r.show_min = 4.0f;  r.show_max =  9.0f; break;
    case ZF_CONSTELLATION: r.show_min = 3.0f;  r.show_max =  6.5f; break;
    case ZF_ORBIT_TRAIL:   r.show_min = 1.0f;  r.show_max =  5.0f; break;
    case ZF_PLANET:        r.show_min = 0.0f;  r.show_max =  5.0f; break;
    case ZF_RING:          r.show_min = 0.5f;  r.show_max =  5.0f; break;
    case ZF_ZODIAC:        r.show_min = 0.5f;  r.show_max =  4.0f; break;
    case ZF_SUN:           r.show_min = -1.0f; r.show_max =  5.0f; break;
    case ZF_CARD:          r.show_min = -5.0f; r.show_max =  2.5f; break;
    case ZF_TEXT_LABEL:    r.show_min = -1.0f; r.show_max =  3.5f; break;
    case ZF_MOON:          r.show_min = -1.0f; r.show_max =  1.5f; break;
    case ZF_EARTH_GLOBE:   r.show_min = -1.0f; r.show_max =  2.0f; break;
    case ZF_OVERLAY:       r.show_min = -99.0f; r.show_max = 99.0f; break;
    default:               r.show_min = -99.0f; r.show_max = 99.0f; break;
    }
    return r;
}

/* Compute opacity (0.0-1.0) for a pass at the given log_zoom.
 * Returns 0.0 if completely invisible, 1.0 if fully visible,
 * and intermediate values during fade transitions. */
static inline float zf_opacity(zf_category_t cat, float log_zoom)
{
    zf_range_t r = zf_range(cat);

    /* Fade in: smoothstep from (show_min - FADE_WIDTH) to show_min */
    float fade_in = zf_smoothstep(r.show_min - ZF_FADE_WIDTH, r.show_min, log_zoom);

    /* Fade out: smoothstep from show_max to (show_max + FADE_WIDTH) */
    float fade_out = 1.0f - zf_smoothstep(r.show_max, r.show_max + ZF_FADE_WIDTH, log_zoom);

    /* Combined: both must be satisfied */
    float alpha = fade_in * fade_out;
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;
    return alpha;
}

#endif /* TIME_ZOOM_FADE_H */
