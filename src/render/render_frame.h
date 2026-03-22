/*
 * render_frame.h — Per-frame render data
 *
 * Built once per frame in main_loop(), passed to all render passes
 * by const pointer. No GL types, no pointers to mutable state.
 */

#ifndef TIME_RENDER_FRAME_H
#define TIME_RENDER_FRAME_H

#include "../math/mat4.h"
#include "render_layers.h"
#include "../systems/unified/brain_visual_bridge.h"

typedef struct {
    double simulation_jd;
    float  time_sec;          /* wall-clock seconds since start (for shader animation) */
    mat4_t view;
    mat4_t proj;
    mat4_t view_proj;
    layer_state_t layers;
    int show_trails;
    float  log_zoom;              /* camera log-zoom level (for depth-aware rendering) */
    double observer_lat;
    double observer_lon;
    int    theme_id;              /* active theme (0=COSMOS, 1=DAWN) for themed rendering */
    int    focus_mode;            /* focus_mode_e: 0=overview, 1-5=system focus */
    char   headline[128];         /* daily narrative headline (from brain_narrative) */
    char   wisdom_text[256];      /* daily wisdom quote */
    char   wisdom_author[64];     /* wisdom quote author */
    br_visual_frame_t brain_visual; /* visual encoding of brain scan (convergence lines, glow) */
    double sun_lon;               /* Sun ecliptic longitude (degrees, 0-360) */
    double planet_lon[8];         /* Mercury..Neptune ecliptic longitudes (degrees) */
    int    viewport_width;        /* screen width in pixels (for 2D passes) */
    int    viewport_height;       /* screen height in pixels */
} render_frame_t;

#endif /* TIME_RENDER_FRAME_H */
