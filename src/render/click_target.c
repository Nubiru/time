/*
 * click_target.c — Hit-test geometry for interactive celestial objects
 *
 * Projects world-space objects to screen-space NDC, then performs
 * circular distance checks to determine click hits. Depth resolves
 * overlapping targets (closer wins).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "click_target.h"

#include <math.h>
#include <stddef.h>

/* Minimum screen radius so distant objects remain clickable */
static const float CT_MIN_RADIUS = 0.01f;

/* ──────────────────────────────────────────────────────────────────────────
 * ct_no_hit — Empty hit result
 * ────────────────────────────────────────────────────────────────────── */

ct_hit_t ct_no_hit(void)
{
    ct_hit_t result;
    result.hit = 0;
    result.type = CT_PLANET;
    result.object_id = -1;
    result.distance = 0.0f;
    result.depth = 0.0f;
    return result;
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_project — Project a single target from world space to screen space
 * ────────────────────────────────────────────────────────────────────── */

void ct_project(ct_target_t *target, const float view_proj[16])
{
    if (target == NULL || view_proj == NULL) {
        return;
    }

    /* Multiply: clip = view_proj * vec4(world_pos, 1.0) */
    float clip[4];
    for (int i = 0; i < 4; i++) {
        clip[i] = view_proj[i]      * target->world_pos[0]
                + view_proj[4 + i]  * target->world_pos[1]
                + view_proj[8 + i]  * target->world_pos[2]
                + view_proj[12 + i];
    }

    /* Behind camera check */
    if (clip[3] <= 0.0f) {
        target->visible = 0;
        return;
    }

    /* Perspective divide */
    target->screen_pos[0] = clip[0] / clip[3];
    target->screen_pos[1] = clip[1] / clip[3];
    target->depth = clip[3];

    /* Off-screen check with margin for large targets */
    float margin = target->screen_radius + 0.1f;
    if (fabsf(target->screen_pos[0]) > 1.0f + margin ||
        fabsf(target->screen_pos[1]) > 1.0f + margin) {
        target->visible = 0;
        return;
    }

    target->visible = 1;

    /* Scale screen_radius by inverse depth (closer = larger target) */
    float base = target->screen_radius;
    target->screen_radius = fmaxf(base / target->depth, CT_MIN_RADIUS);
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_project_all — Project an array of targets
 * ────────────────────────────────────────────────────────────────────── */

void ct_project_all(ct_target_t *targets, int count, const float view_proj[16])
{
    if (targets == NULL || view_proj == NULL || count <= 0) {
        return;
    }

    for (int i = 0; i < count; i++) {
        ct_project(&targets[i], view_proj);
    }
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_test_one — Test a screen-space click against a single target
 * ────────────────────────────────────────────────────────────────────── */

ct_hit_t ct_test_one(const ct_target_t *target, const float click_ndc[2])
{
    if (target == NULL || click_ndc == NULL) {
        return ct_no_hit();
    }

    if (!target->visible) {
        return ct_no_hit();
    }

    float dx = click_ndc[0] - target->screen_pos[0];
    float dy = click_ndc[1] - target->screen_pos[1];
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > target->screen_radius) {
        return ct_no_hit();
    }

    ct_hit_t hit;
    hit.hit = 1;
    hit.type = target->type;
    hit.object_id = target->object_id;
    hit.distance = dist;
    hit.depth = target->depth;
    return hit;
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_test_all — Test click against all targets, return closest hit
 * ────────────────────────────────────────────────────────────────────── */

ct_hit_t ct_test_all(const ct_target_t *targets, int count,
                     const float click_ndc[2])
{
    if (targets == NULL || click_ndc == NULL || count <= 0) {
        return ct_no_hit();
    }

    ct_hit_t best = ct_no_hit();

    for (int i = 0; i < count; i++) {
        ct_hit_t result = ct_test_one(&targets[i], click_ndc);
        if (result.hit) {
            if (!best.hit || result.depth < best.depth) {
                best = result;
            }
        }
    }

    return best;
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_planet_target — Create a planet target with size-class radius
 * ────────────────────────────────────────────────────────────────────── */

ct_target_t ct_planet_target(int planet_id, const float world_pos[3])
{
    ct_target_t t;
    t.type = CT_PLANET;
    t.object_id = planet_id;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.visible = 0;
    t.depth = 0.0f;

    if (world_pos != NULL) {
        t.world_pos[0] = world_pos[0];
        t.world_pos[1] = world_pos[1];
        t.world_pos[2] = world_pos[2];
    } else {
        t.world_pos[0] = 0.0f;
        t.world_pos[1] = 0.0f;
        t.world_pos[2] = 0.0f;
    }

    /* Base radius by planet size class (scaled by depth in ct_project)
     * Sun=0, Mercury=1, Venus=2, Earth=3, Mars=4,
     * Jupiter=5, Saturn=6, Uranus=7, Neptune=8 */
    static const float radii[] = {
        0.15f, 0.03f, 0.04f, 0.04f, 0.035f,
        0.08f, 0.07f, 0.05f, 0.05f
    };
    static const int radii_count = (int)(sizeof(radii) / sizeof(radii[0]));

    int idx = (planet_id >= 0 && planet_id < radii_count) ? planet_id : 3;
    t.screen_radius = radii[idx];

    return t;
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_star_target — Create a star target with magnitude-based radius
 * ────────────────────────────────────────────────────────────────────── */

ct_target_t ct_star_target(int star_id, const float world_pos[3],
                           float magnitude)
{
    ct_target_t t;
    t.type = CT_STAR;
    t.object_id = star_id;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.visible = 0;
    t.depth = 0.0f;

    if (world_pos != NULL) {
        t.world_pos[0] = world_pos[0];
        t.world_pos[1] = world_pos[1];
        t.world_pos[2] = world_pos[2];
    } else {
        t.world_pos[0] = 0.0f;
        t.world_pos[1] = 0.0f;
        t.world_pos[2] = 0.0f;
    }

    /* Brighter = lower magnitude = larger target
     * Magnitude -1 (Sirius) -> 0.05, magnitude 4 -> 0.015 */
    float base = 0.05f - (magnitude + 1.0f) * 0.007f;
    t.screen_radius = fmaxf(base, CT_MIN_RADIUS);

    return t;
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_dso_target — Create a DSO target with angular-size-based radius
 * ────────────────────────────────────────────────────────────────────── */

ct_target_t ct_dso_target(int dso_id, const float world_pos[3],
                          float angular_size)
{
    ct_target_t t;
    t.type = CT_DSO;
    t.object_id = dso_id;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.visible = 0;
    t.depth = 0.0f;

    if (world_pos != NULL) {
        t.world_pos[0] = world_pos[0];
        t.world_pos[1] = world_pos[1];
        t.world_pos[2] = world_pos[2];
    } else {
        t.world_pos[0] = 0.0f;
        t.world_pos[1] = 0.0f;
        t.world_pos[2] = 0.0f;
    }

    /* Angular size in degrees mapped to screen radius.
     * Moon (~0.5 deg) -> 0.03, large nebula (~2 deg) -> 0.06 */
    float base = fmaxf(angular_size * 0.03f, CT_MIN_RADIUS);
    t.screen_radius = base;

    return t;
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_sort_by_depth — Sort targets front to back (insertion sort)
 * ────────────────────────────────────────────────────────────────────── */

void ct_sort_by_depth(ct_target_t *targets, int count)
{
    if (targets == NULL || count <= 1) {
        return;
    }

    for (int i = 1; i < count; i++) {
        ct_target_t key = targets[i];
        int j = i - 1;
        while (j >= 0 && targets[j].depth > key.depth) {
            targets[j + 1] = targets[j];
            j--;
        }
        targets[j + 1] = key;
    }
}

/* ──────────────────────────────────────────────────────────────────────────
 * ct_type_name — Get name string for a target type
 * ────────────────────────────────────────────────────────────────────── */

const char *ct_type_name(ct_type_t type)
{
    static const char *names[] = {
        "Planet", "Star", "DSO", "Moon",
        "Zodiac Sign", "Card", "UI Element"
    };

    if (type < 0 || type >= CT_TYPE_COUNT) {
        return "Unknown";
    }

    return names[type];
}
