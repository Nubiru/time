/*
 * click_target.h — Hit-test geometry for interactive celestial objects
 *
 * Provides click-to-focus: project world-space objects to screen space,
 * then test screen-space click coordinates against bounding circles
 * to identify what the user clicked.
 *
 * Targets are projected from 3D world space to 2D NDC (-1 to +1) using
 * a combined view*projection matrix. Hit testing is a simple distance
 * check in screen space. Depth resolves overlapping targets (closer wins).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#ifndef TIME_CLICK_TARGET_H
#define TIME_CLICK_TARGET_H

#define CT_MAX_TARGETS 128

/* Object categories for click targets */
typedef enum {
    CT_PLANET = 0,
    CT_STAR,
    CT_DSO,
    CT_MOON,
    CT_ZODIAC_SIGN,
    CT_CARD,
    CT_UI_ELEMENT,
    CT_TYPE_COUNT
} ct_type_t;

/* Single clickable target */
typedef struct {
    ct_type_t type;
    int object_id;        /* Index within its category */
    float world_pos[3];   /* 3D world position */
    float screen_pos[2];  /* Computed NDC screen position (-1 to +1) */
    float screen_radius;  /* Hit-test radius in NDC units */
    int visible;          /* 1 if on-screen and in front of camera */
    float depth;          /* Distance from camera (for sorting) */
} ct_target_t;

/* Hit test result */
typedef struct {
    int hit;              /* 1 if something was hit, 0 if not */
    ct_type_t type;       /* Type of hit object */
    int object_id;        /* Index of hit object */
    float distance;       /* Screen-space distance from click to center (NDC) */
    float depth;          /* World-space depth of hit object */
} ct_hit_t;

/* Empty hit result (no hit). */
ct_hit_t ct_no_hit(void);

/* Project a single target from world space to screen space.
 * view_proj: combined 4x4 matrix (16 floats, column-major).
 * Updates target->screen_pos, target->visible, target->depth.
 * screen_radius is scaled inversely by depth (closer = larger target). */
void ct_project(ct_target_t *target, const float view_proj[16]);

/* Project an array of targets. */
void ct_project_all(ct_target_t *targets, int count, const float view_proj[16]);

/* Test a screen-space click against a single target.
 * click_ndc: click position in NDC (-1 to +1).
 * Returns hit result (hit=1 if within screen_radius). */
ct_hit_t ct_test_one(const ct_target_t *target, const float click_ndc[2]);

/* Test a click against all targets, return the closest hit.
 * Closer depth wins when multiple targets overlap.
 * Returns ct_no_hit() if nothing was hit. */
ct_hit_t ct_test_all(const ct_target_t *targets, int count, const float click_ndc[2]);

/* Create a planet target with appropriate screen radius based on planet index.
 * Planet radii: Sun largest, Jupiter next, etc. Base radius scaled by planet size class. */
ct_target_t ct_planet_target(int planet_id, const float world_pos[3]);

/* Create a star target with magnitude-based screen radius.
 * Brighter stars (lower magnitude) get larger click targets. */
ct_target_t ct_star_target(int star_id, const float world_pos[3], float magnitude);

/* Create a DSO target with angular-size-based screen radius. */
ct_target_t ct_dso_target(int dso_id, const float world_pos[3], float angular_size);

/* Sort targets by depth (front to back) for priority resolution.
 * Uses insertion sort (in-place, no malloc). */
void ct_sort_by_depth(ct_target_t *targets, int count);

/* Get the name string for a target type. */
const char *ct_type_name(ct_type_t type);

#endif /* TIME_CLICK_TARGET_H */
