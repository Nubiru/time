/* scroll_layers.c -- Scroll-driven layer visibility and parallax system
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "scroll_layers.h"

#include <math.h>

/* phi constants */
static const float PHI      = 1.6180339887498948482f;
static const float PHI_INV  = 0.6180339887498948482f;
static const float PHI_INV2 = 0.3819660112501051518f;

/* Layer visibility rules table */
static const scroll_layer_rule_t LAYER_RULES[SCROLL_LAYER_COUNT] = {
    /* STARS:   0.0 - 4.0, BACKGROUND */
    { SCROLL_LAYER_STARS,   0.0f, 4.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_BACKGROUND, 0.3819660112501051518f },

    /* ORBITS:  0.5 - 3.5, MID */
    { SCROLL_LAYER_ORBITS,  0.5f, 3.5f, 0.5f, 0.5f,
      SCROLL_DEPTH_MID, 0.6180339887498948482f },

    /* PLANETS: 1.0 - 3.0, FOREGROUND */
    { SCROLL_LAYER_PLANETS, 1.0f, 3.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_FOREGROUND, 1.0f },

    /* ZODIAC:  1.5 - 3.0, MID */
    { SCROLL_LAYER_ZODIAC,  1.5f, 3.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_MID, 0.6180339887498948482f },

    /* LABELS:  1.0 - 3.5, FOREGROUND */
    { SCROLL_LAYER_LABELS,  1.0f, 3.5f, 0.5f, 0.5f,
      SCROLL_DEPTH_FOREGROUND, 1.0f },

    /* ASPECTS: 1.5 - 3.0, MID */
    { SCROLL_LAYER_ASPECTS, 1.5f, 3.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_MID, 0.6180339887498948482f },

    /* HOUSES:  1.5 - 3.0, MID */
    { SCROLL_LAYER_HOUSES,  1.5f, 3.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_MID, 0.6180339887498948482f },

    /* CARDS:   1.0 - 4.0, OVERLAY */
    { SCROLL_LAYER_CARDS,   1.0f, 4.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_OVERLAY, 0.0f },

    /* HUD:     0.0 - 6.0, OVERLAY */
    { SCROLL_LAYER_HUD,     0.0f, 6.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_OVERLAY, 0.0f },

    /* GALAXY:  4.0 - 6.0, BACKGROUND */
    { SCROLL_LAYER_GALAXY,  4.0f, 6.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_BACKGROUND, 0.3819660112501051518f },

    /* EARTH:   0.0 - 2.0, FOREGROUND */
    { SCROLL_LAYER_EARTH,   0.0f, 2.0f, 0.5f, 0.5f,
      SCROLL_DEPTH_FOREGROUND, 1.0f }
};

/* Layer name strings */
static const char *LAYER_NAMES[SCROLL_LAYER_COUNT] = {
    "STARS", "ORBITS", "PLANETS", "ZODIAC", "LABELS",
    "ASPECTS", "HOUSES", "CARDS", "HUD", "GALAXY", "EARTH"
};

/* Depth name strings */
static const char *DEPTH_NAMES[SCROLL_DEPTH_COUNT] = {
    "BACKGROUND", "MID", "FOREGROUND", "OVERLAY"
};

/* Camera perspective keyframes: {zoom, pitch, distance, fov} */
static const float KEYFRAME_ZOOM[3]     = { 0.0f, 3.0f, 6.0f };
static const float KEYFRAME_PITCH[3]    = { 0.3f, 0.6f, 1.0f };
static const float KEYFRAME_DISTANCE[3] = { 1.0f, 10.0f, 100.0f };

/* --- Implementation --- */

const char *scroll_layer_name(scroll_layer_t layer) {
    if (layer < 0 || layer >= SCROLL_LAYER_COUNT) {
        return "UNKNOWN";
    }
    return LAYER_NAMES[layer];
}

const char *scroll_depth_name(scroll_depth_t depth) {
    if (depth < 0 || depth >= SCROLL_DEPTH_COUNT) {
        return "UNKNOWN";
    }
    return DEPTH_NAMES[depth];
}

scroll_layer_rule_t scroll_layer_rule(scroll_layer_t layer) {
    if (layer < 0 || layer >= SCROLL_LAYER_COUNT) {
        scroll_layer_rule_t empty = {
            SCROLL_LAYER_STARS, 0.0f, 0.0f, 0.0f, 0.0f,
            SCROLL_DEPTH_BACKGROUND, 0.0f
        };
        return empty;
    }
    return LAYER_RULES[layer];
}

float scroll_layer_opacity(scroll_layer_t layer, float zoom_level) {
    if (layer < 0 || layer >= SCROLL_LAYER_COUNT) {
        return 0.0f;
    }

    const scroll_layer_rule_t *r = &LAYER_RULES[layer];

    /* Below zoom_min: invisible */
    if (zoom_level <= r->zoom_min) {
        return 0.0f;
    }

    /* Above zoom_max: invisible */
    if (zoom_level >= r->zoom_max) {
        return 0.0f;
    }

    float fade_in_end  = r->zoom_min + r->fade_in_width;
    float fade_out_start = r->zoom_max - r->fade_out_width;

    /* Fade-in region */
    if (zoom_level < fade_in_end) {
        float t = (zoom_level - r->zoom_min) / r->fade_in_width;
        return t;
    }

    /* Fade-out region */
    if (zoom_level > fade_out_start) {
        float t = (r->zoom_max - zoom_level) / r->fade_out_width;
        return t;
    }

    /* Full opacity region */
    return 1.0f;
}

float scroll_depth_parallax(scroll_depth_t depth) {
    switch (depth) {
    case SCROLL_DEPTH_BACKGROUND: return PHI_INV2;
    case SCROLL_DEPTH_MID:        return PHI_INV;
    case SCROLL_DEPTH_FOREGROUND: return 1.0f;
    case SCROLL_DEPTH_OVERLAY:    return 0.0f;
    default:                      return 0.0f;
    }
}

float scroll_parallax_offset(scroll_layer_t layer, float scroll_delta) {
    if (layer < 0 || layer >= SCROLL_LAYER_COUNT) {
        return 0.0f;
    }
    return scroll_delta * LAYER_RULES[layer].parallax_factor;
}

float scroll_to_zoom(float scroll_position) {
    return powf(PHI, scroll_position);
}

float zoom_to_scroll(float zoom_level) {
    if (zoom_level <= 0.0f) {
        return -100.0f;
    }
    return logf(zoom_level) / logf(PHI);
}

scroll_perspective_t scroll_perspective(float zoom_level) {
    /* FOV keyframes computed from PI */
    float keyframe_fov[3];
    keyframe_fov[0] = (float)PI / 3.0f;
    keyframe_fov[1] = (float)PI / 4.0f;
    keyframe_fov[2] = (float)PI / 6.0f;

    scroll_perspective_t result;

    /* Clamp below first keyframe */
    if (zoom_level <= KEYFRAME_ZOOM[0]) {
        result.pitch    = KEYFRAME_PITCH[0];
        result.distance = KEYFRAME_DISTANCE[0];
        result.fov      = keyframe_fov[0];
        return result;
    }

    /* Clamp above last keyframe */
    if (zoom_level >= KEYFRAME_ZOOM[2]) {
        result.pitch    = KEYFRAME_PITCH[2];
        result.distance = KEYFRAME_DISTANCE[2];
        result.fov      = keyframe_fov[2];
        return result;
    }

    /* Find segment and interpolate */
    int seg = 0;
    if (zoom_level >= KEYFRAME_ZOOM[1]) {
        seg = 1;
    }

    float z0 = KEYFRAME_ZOOM[seg];
    float z1 = KEYFRAME_ZOOM[seg + 1];
    float t = (zoom_level - z0) / (z1 - z0);

    result.pitch    = KEYFRAME_PITCH[seg]    + (KEYFRAME_PITCH[seg + 1]    - KEYFRAME_PITCH[seg])    * t;
    result.distance = KEYFRAME_DISTANCE[seg] + (KEYFRAME_DISTANCE[seg + 1] - KEYFRAME_DISTANCE[seg]) * t;
    result.fov      = keyframe_fov[seg]      + (keyframe_fov[seg + 1]      - keyframe_fov[seg])      * t;

    return result;
}

int scroll_is_visible(scroll_layer_t layer, float zoom_level) {
    return scroll_layer_opacity(layer, zoom_level) > 0.0f ? 1 : 0;
}

int scroll_visible_count(float zoom_level) {
    int count = 0;
    for (int i = 0; i < SCROLL_LAYER_COUNT; i++) {
        if (scroll_layer_opacity((scroll_layer_t)i, zoom_level) > 0.0f) {
            count++;
        }
    }
    return count;
}
