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
    float  focus_blend;           /* 0.0-1.0 focus transition progress (from view_state) */
    float  card_slide;            /* 0.0=hidden, 1.0=visible (focus card fade) */
    float  card_text_reveal;      /* 0.0-1.0 per-word text appearance progress */
    float  scene_brightness;      /* 0.0-1.0 multiplier for 3D scene (enter_zoom ramp) */
    char   headline[128];         /* daily narrative headline (from brain_narrative) */
    char   wisdom_text[256];      /* daily wisdom quote */
    char   wisdom_author[64];     /* wisdom quote author */
    char   percentile_text[64];   /* "Day #47 of 365 — top 12.9%" */
    char   red_thread[256];       /* convergence narrative (why systems align) */
    char   grand_cycle[128];      /* grand cycle mind-blowing stat */
    char   last_similar[96];      /* last time this convergence happened */
    char   entropy_text[64];     /* time entropy + interpretation */
    char   top_resonance[96];    /* strongest system correlation */
    char   author_line[128];    /* author attribution line */
    br_visual_frame_t brain_visual; /* visual encoding of brain scan (convergence lines, glow) */
    double sun_lon;               /* Sun ecliptic longitude (degrees, 0-360) */
    double planet_lon[8];         /* Mercury..Neptune ecliptic longitudes (degrees) */
    int    viewport_width;        /* screen width in pixels (for 2D passes) */
    int    viewport_height;       /* screen height in pixels */
} render_frame_t;

#endif /* TIME_RENDER_FRAME_H */
