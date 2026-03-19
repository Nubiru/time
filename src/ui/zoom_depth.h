#ifndef TIME_ZOOM_DEPTH_H
#define TIME_ZOOM_DEPTH_H

/* Zoom-as-depth navigation — maps camera zoom to depth tier
 * with spring-animated transitions.
 *
 * Bridges camera_scale (zoom levels) with depth_tier (display detail)
 * and depth_ring (timescale rings). As the user zooms in, more detail
 * is revealed; zooming out shows only glyphs.
 *
 * Pure: no GL, no globals, no I/O. Returns by value. */

#include "../systems/unified/depth_tier.h"
#include "../systems/unified/depth_ring.h"
#include "../render/camera_scale.h"
#include "../math/spring.h"

typedef struct {
    depth_tier_t tier;           /* current discrete tier */
    depth_tier_t target_tier;    /* target tier from zoom level */
    spring_t tier_spring;        /* smooth 0.0-4.0 for blending */
    depth_ring_t ring;           /* ring for focused system */
    scale_id_t scale;            /* camera scale level */
    int system_id;               /* focused system (-1 = none) */
} zoom_depth_t;

/* Create at initial zoom level. system_id -1 = no focus. */
zoom_depth_t zoom_depth_create(float log_zoom, int system_id);

/* Update from new camera zoom level. */
zoom_depth_t zoom_depth_update(zoom_depth_t zd, float log_zoom, float dt);

/* Focus on a different system (changes ring assignment). */
zoom_depth_t zoom_depth_focus(zoom_depth_t zd, int system_id);

/* Current discrete tier (rounded from spring position). */
depth_tier_t zoom_depth_tier(zoom_depth_t zd);

/* Smooth blended tier value (e.g. 2.3 = between CARD and PANEL). */
float zoom_depth_blend(zoom_depth_t zd);

/* Ring for focused system. DEPTH_RING_TODAY if no focus. */
depth_ring_t zoom_depth_ring(zoom_depth_t zd);

/* 1 if tier spring is still animating. */
int zoom_depth_transitioning(zoom_depth_t zd);

#endif
