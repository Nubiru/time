/* convergence_visual.h — Convergence visualization geometry.
 *
 * Converts brain correlation/insight data into renderable elements:
 *   - Connection lines between system rings (with position, color, opacity)
 *   - Ring glow overlays (per-ring pulse/highlight)
 *   - Scene atmosphere parameters (warmth, overall glow)
 *
 * Decoupled from brain_types.h: the render pass iterates br_visual_frame_t
 * links and calls these functions individually.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CONVERGENCE_VISUAL_H
#define TIME_CONVERGENCE_VISUAL_H

#include "concentric_ring.h"

#define CV_MAX_LINES  32
#define CV_MAX_GLOWS  8

/* Connection line between two system rings in the XZ plane */
typedef struct {
    float x1, z1;           /* start point (on system A's ring midpoint) */
    float x2, z2;           /* end point (on system B's ring midpoint) */
    float opacity;          /* 0-1 */
    float thickness;        /* 1-4 pixel scale */
    float color[4];         /* RGBA */
} cv_line_t;

/* Ring glow overlay */
typedef struct {
    int ring_index;         /* which ring in the layout (-1 = all) */
    float intensity;        /* glow brightness 0-1 */
    float color[3];         /* glow color RGB */
} cv_glow_t;

/* Complete convergence visualization for one frame */
typedef struct {
    cv_line_t lines[CV_MAX_LINES];
    int line_count;
    cv_glow_t glows[CV_MAX_GLOWS];
    int glow_count;
    float atmosphere_warmth; /* 0=cool, 1=warm */
    float scene_intensity;   /* 0=quiet, 1=fully converging */
} cv_frame_t;

/* Initialize an empty cv_frame_t (all zeroed). */
cv_frame_t cv_frame_init(void);

/* Add a line to the frame. Returns 1 on success, 0 if full. */
int cv_frame_add_line(cv_frame_t *frame, const cv_line_t *line);

/* Add a glow to the frame. Returns 1 on success, 0 if full. */
int cv_frame_add_glow(cv_frame_t *frame, const cv_glow_t *glow);

/* Compute ring midpoint radius (average of inner and outer).
 * Returns 0 if layout is NULL. */
float cv_ring_midpoint(const cr_layout_t *layout, int ring_index);

/* Compute a single connection line between two rings.
 * ring_a_idx, ring_b_idx: indices in layout.
 * angle_a_deg, angle_b_deg: angular positions on each ring (degrees).
 *   0 degrees = +X axis, 90 degrees = +Z axis, CCW.
 * opacity, thickness: visual parameters.
 * warmth: 0=cool celestial teal, 1=warm solar gold.
 * Returns cv_line_t with computed XZ positions and color. */
cv_line_t cv_line_between_rings(const cr_layout_t *layout,
                                int ring_a_idx, int ring_b_idx,
                                float angle_a_deg, float angle_b_deg,
                                float opacity, float thickness, float warmth);

/* Map warmth (0-1) to RGBA color. Clamped to [0, 1].
 * 0.0 = cool celestial teal {0.2, 0.75, 0.8, 1.0}
 * 1.0 = warm solar gold {1.0, 0.85, 0.55, 1.0} */
void cv_warmth_to_color(float warmth, float rgba[4]);

#endif /* TIME_CONVERGENCE_VISUAL_H */
