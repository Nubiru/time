/* brain_visual_bridge.h — Bridge: brain intelligence → visual encoding
 *
 * Converts brain correlation scores and insight data into visual parameters:
 * connection line opacity/thickness, ring glow, pulse intensity, scene warmth.
 *
 * These parameters drive the render pipeline's convergence visualizations.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_VISUAL_BRIDGE_H
#define TIME_BRAIN_VISUAL_BRIDGE_H

#include "brain_types.h"

#define BR_VISUAL_MAX_LINKS    32
#define BR_VISUAL_MAX_INSIGHTS 16

/* Visual encoding of a correlation between two systems */
typedef struct {
    int    system_a;
    int    system_b;
    float  line_opacity;     /* 0.0-1.0: connection line visibility */
    float  line_thickness;   /* 1.0-4.0: line width scale */
    float  glow_strength;    /* 0.0-1.0: bloom around connection */
    float  color_warmth;     /* 0.0=cool (blue), 1.0=warm (gold) */
} br_visual_link_t;

/* Visual encoding of an insight highlight */
typedef struct {
    int    highlight_system;  /* which system to highlight (-1 for global) */
    float  pulse_intensity;   /* 0.0-1.0: ring pulse strength */
    float  ring_glow;         /* 0.0-1.0: additional ring glow */
    float  background_shift;  /* 0.0=no change, 1.0=full atmosphere shift */
} br_visual_insight_t;

/* Complete visual encoding for one brain scan frame */
typedef struct {
    br_visual_link_t     links[BR_VISUAL_MAX_LINKS];
    int                  link_count;
    br_visual_insight_t  insights[BR_VISUAL_MAX_INSIGHTS];
    int                  insight_count;
    float                overall_intensity;  /* 0.0-1.0: scene convergence glow */
    float                atmosphere_warmth;  /* 0.0-1.0: background color temp */
} br_visual_frame_t;

/* Convert brain result into visual encoding.
 * frame is zeroed internally. If result is NULL, frame is zeroed. */
void br_visual_from_result(const br_result_t *result,
                           br_visual_frame_t *frame);

/* Map a single correlation to visual link parameters. */
br_visual_link_t br_visual_link_from_correlation(const br_correlation_t *c);

/* Map a single insight to visual highlight parameters. */
br_visual_insight_t br_visual_insight_from_brain(const br_insight_t *insight);

/* Map convergence strength to overall scene intensity.
 * Returns 0.0-1.0. */
float br_visual_scene_intensity(double convergence_strength);

#endif /* TIME_BRAIN_VISUAL_BRIDGE_H */
