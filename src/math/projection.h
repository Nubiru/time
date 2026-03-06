#ifndef TIME_PROJECTION_H
#define TIME_PROJECTION_H

#include "mat4.h"
#include "vec3.h"

typedef struct {
    float x, y;      /* screen position (pixels) */
    float depth;      /* normalized depth (0=near, 1=far) */
    int visible;      /* 1 if in front of camera, 0 if behind */
} screen_point_t;

/* Project a 3D world position to 2D screen coordinates.
 * view: camera view matrix, proj: projection matrix.
 * screen_w, screen_h: viewport size in pixels.
 * Returns screen position with (0,0) at top-left. */
screen_point_t project_to_screen(vec3_t world_pos,
                                  const mat4_t *view,
                                  const mat4_t *proj,
                                  float screen_w, float screen_h);

/* Unproject a 2D screen position to a 3D ray direction.
 * screen_x, screen_y: pixel coordinates.
 * Returns normalized ray direction in world space. */
vec3_t unproject_ray(float screen_x, float screen_y,
                     const mat4_t *view, const mat4_t *proj,
                     float screen_w, float screen_h);

/* Check if a world point is within a screen-space radius of a screen position.
 * Useful for click/hover detection on planets. */
int point_near_screen(vec3_t world_pos,
                      float screen_x, float screen_y, float radius_px,
                      const mat4_t *view, const mat4_t *proj,
                      float screen_w, float screen_h);

/* Compute screen-space distance between two projected world points.
 * Returns -1.0 if either point is behind the camera. */
float screen_distance(vec3_t a, vec3_t b,
                      const mat4_t *view, const mat4_t *proj,
                      float screen_w, float screen_h);

#endif
