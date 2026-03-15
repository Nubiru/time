/*
 * test_click_target.c — Tests for click target hit-test geometry
 *
 * Tests: no_hit, projection, hit testing, planet/star/DSO target creation,
 *        depth sorting, type names, null safety, edge cases.
 */

#include "../unity/unity.h"
#include "../../src/render/click_target.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helper: identity matrix (column-major 4x4)
 * =================================================================== */

static void identity_matrix(float m[16])
{
    memset(m, 0, 16 * sizeof(float));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
}

/* Helper: perspective-like matrix that maps z=-d to w=d (positive)
 * This simulates a camera looking down -Z. Objects at z=-d get w=d. */
static void simple_proj_matrix(float m[16])
{
    memset(m, 0, 16 * sizeof(float));
    m[0] = 1.0f;    /* scale x */
    m[5] = 1.0f;    /* scale y */
    m[10] = -1.0f;  /* map z */
    m[11] = -1.0f;  /* perspective divide: w = -z */
    m[14] = -1.0f;  /* z offset */
}

/* Helper: make a target at a given position with given radius */
static ct_target_t make_target(ct_type_t type, int id,
                               float x, float y, float z,
                               float radius)
{
    ct_target_t t;
    t.type = type;
    t.object_id = id;
    t.world_pos[0] = x;
    t.world_pos[1] = y;
    t.world_pos[2] = z;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.screen_radius = radius;
    t.visible = 0;
    t.depth = 0.0f;
    return t;
}

/* ===================================================================
 * 1. ct_no_hit — Empty hit result
 * =================================================================== */

void test_no_hit_returns_zero(void)
{
    ct_hit_t h = ct_no_hit();
    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_no_hit_object_id_negative(void)
{
    ct_hit_t h = ct_no_hit();
    TEST_ASSERT_EQUAL_INT(-1, h.object_id);
}

/* ===================================================================
 * 2. ct_project — World to screen projection
 * =================================================================== */

void test_project_origin_identity_visible(void)
{
    /* With identity matrix, origin maps to NDC (0,0) with w=1 */
    float m[16];
    identity_matrix(m);
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);

    ct_project(&t, m);

    TEST_ASSERT_EQUAL_INT(1, t.visible);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.screen_pos[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.screen_pos[1]);
}

void test_project_behind_camera_invisible(void)
{
    /* With proj matrix, z=+1 is behind camera (w = -(-1)*1 - 1 ... negative) */
    float m[16];
    simple_proj_matrix(m);
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 1.0f, 0.05f);

    ct_project(&t, m);

    TEST_ASSERT_EQUAL_INT(0, t.visible);
}

void test_project_off_screen_invisible(void)
{
    /* With identity, x=5 maps to NDC x=5 which is way off screen */
    float m[16];
    identity_matrix(m);
    ct_target_t t = make_target(CT_PLANET, 0, 5.0f, 0.0f, 0.0f, 0.05f);

    ct_project(&t, m);

    TEST_ASSERT_EQUAL_INT(0, t.visible);
}

void test_project_null_target_no_crash(void)
{
    float m[16];
    identity_matrix(m);
    ct_project(NULL, m);
    /* No crash = pass */
}

void test_project_null_view_proj_no_crash(void)
{
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    ct_project(&t, NULL);
    /* No crash = pass */
}

void test_project_depth_set_correctly(void)
{
    /* With proj matrix, object at z=-2: w = -(-1)*(-2) + (-1) ...
     * clip[3] = m[11]*z + m[15] = (-1)*(-2) + 0 = 2 (wait, let me trace)
     * m[11]=-1, m[15]=0 for our proj. m[14]=-1, m[10]=-1.
     * clip[3] = m[3]*x + m[7]*y + m[11]*z + m[15] = 0 + 0 + (-1)*(-2) + 0 = 2
     * So depth = clip[3] = 2 */
    float m[16];
    simple_proj_matrix(m);
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, -2.0f, 0.05f);

    ct_project(&t, m);

    TEST_ASSERT_EQUAL_INT(1, t.visible);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f, t.depth);
}

void test_project_screen_radius_scaled_by_depth(void)
{
    float m[16];
    simple_proj_matrix(m);

    /* Object at z=-2: depth=2, base_radius=0.10 -> scaled = 0.10/2 = 0.05 */
    ct_target_t t1 = make_target(CT_PLANET, 0, 0.0f, 0.0f, -2.0f, 0.10f);
    ct_project(&t1, m);

    /* Object at z=-4: depth=4, base_radius=0.10 -> scaled = 0.10/4 = 0.025 */
    ct_target_t t2 = make_target(CT_PLANET, 1, 0.0f, 0.0f, -4.0f, 0.10f);
    ct_project(&t2, m);

    /* Closer object should have larger screen radius */
    TEST_ASSERT_TRUE(t1.screen_radius > t2.screen_radius);
}

/* ===================================================================
 * 3. ct_project_all — Batch projection
 * =================================================================== */

void test_project_all_projects_array(void)
{
    float m[16];
    identity_matrix(m);
    ct_target_t targets[3];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[1] = make_target(CT_STAR, 1, 0.5f, 0.5f, 0.0f, 0.03f);
    targets[2] = make_target(CT_DSO, 2, -0.3f, 0.2f, 0.0f, 0.04f);

    ct_project_all(targets, 3, m);

    TEST_ASSERT_EQUAL_INT(1, targets[0].visible);
    TEST_ASSERT_EQUAL_INT(1, targets[1].visible);
    TEST_ASSERT_EQUAL_INT(1, targets[2].visible);
}

void test_project_all_null_inputs_no_crash(void)
{
    float m[16];
    identity_matrix(m);
    ct_project_all(NULL, 3, m);

    ct_target_t targets[2];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    ct_project_all(targets, 2, NULL);
    /* No crash = pass */
}

void test_project_all_count_zero_no_crash(void)
{
    float m[16];
    identity_matrix(m);
    ct_target_t targets[1];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);

    ct_project_all(targets, 0, m);
    /* No crash = pass */
}

/* ===================================================================
 * 4. ct_test_one — Single target hit test
 * =================================================================== */

void test_test_one_click_on_target_hit(void)
{
    ct_target_t t = make_target(CT_STAR, 42, 0.0f, 0.0f, 0.0f, 0.05f);
    t.visible = 1;
    t.screen_pos[0] = 0.3f;
    t.screen_pos[1] = 0.4f;
    t.depth = 1.0f;

    float click[2] = {0.3f, 0.4f};
    ct_hit_t h = ct_test_one(&t, click);

    TEST_ASSERT_EQUAL_INT(1, h.hit);
    TEST_ASSERT_EQUAL_INT(CT_STAR, h.type);
    TEST_ASSERT_EQUAL_INT(42, h.object_id);
}

void test_test_one_click_outside_radius_miss(void)
{
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    t.visible = 1;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.screen_radius = 0.05f;
    t.depth = 1.0f;

    float click[2] = {0.5f, 0.5f};
    ct_hit_t h = ct_test_one(&t, click);

    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_one_invisible_target_miss(void)
{
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 1.0f);
    t.visible = 0;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.depth = 1.0f;

    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_one(&t, click);

    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_one_null_target_no_crash(void)
{
    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_one(NULL, click);
    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_one_null_click_no_crash(void)
{
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    t.visible = 1;
    ct_hit_t h = ct_test_one(&t, NULL);
    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_one_distance_correct(void)
{
    ct_target_t t = make_target(CT_STAR, 0, 0.0f, 0.0f, 0.0f, 0.5f);
    t.visible = 1;
    t.screen_pos[0] = 0.0f;
    t.screen_pos[1] = 0.0f;
    t.depth = 1.0f;

    float click[2] = {0.3f, 0.4f};  /* distance = sqrt(0.09+0.16) = 0.5 */
    ct_hit_t h = ct_test_one(&t, click);

    TEST_ASSERT_EQUAL_INT(1, h.hit);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, h.distance);
}

/* ===================================================================
 * 5. ct_test_all — Multi-target hit test
 * =================================================================== */

void test_test_all_closest_wins(void)
{
    ct_target_t targets[2];
    /* Target 0: far (depth=10), big radius so click hits */
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.5f);
    targets[0].visible = 1;
    targets[0].screen_pos[0] = 0.0f;
    targets[0].screen_pos[1] = 0.0f;
    targets[0].depth = 10.0f;

    /* Target 1: close (depth=2), same position */
    targets[1] = make_target(CT_STAR, 1, 0.0f, 0.0f, 0.0f, 0.5f);
    targets[1].visible = 1;
    targets[1].screen_pos[0] = 0.0f;
    targets[1].screen_pos[1] = 0.0f;
    targets[1].depth = 2.0f;

    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_all(targets, 2, click);

    TEST_ASSERT_EQUAL_INT(1, h.hit);
    TEST_ASSERT_EQUAL_INT(1, h.object_id); /* Closer target wins */
    TEST_ASSERT_EQUAL_INT(CT_STAR, h.type);
}

void test_test_all_invisible_skipped(void)
{
    ct_target_t targets[2];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.5f);
    targets[0].visible = 0; /* invisible */
    targets[0].screen_pos[0] = 0.0f;
    targets[0].screen_pos[1] = 0.0f;
    targets[0].depth = 1.0f;

    targets[1] = make_target(CT_STAR, 1, 0.0f, 0.0f, 0.0f, 0.5f);
    targets[1].visible = 1;
    targets[1].screen_pos[0] = 0.0f;
    targets[1].screen_pos[1] = 0.0f;
    targets[1].depth = 5.0f;

    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_all(targets, 2, click);

    TEST_ASSERT_EQUAL_INT(1, h.hit);
    TEST_ASSERT_EQUAL_INT(1, h.object_id); /* Only visible target */
}

void test_test_all_no_hit(void)
{
    ct_target_t targets[2];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.01f);
    targets[0].visible = 1;
    targets[0].screen_pos[0] = 0.0f;
    targets[0].screen_pos[1] = 0.0f;
    targets[0].depth = 1.0f;

    targets[1] = make_target(CT_STAR, 1, 0.0f, 0.0f, 0.0f, 0.01f);
    targets[1].visible = 1;
    targets[1].screen_pos[0] = 0.5f;
    targets[1].screen_pos[1] = 0.5f;
    targets[1].depth = 1.0f;

    float click[2] = {-0.9f, -0.9f}; /* Far from both */
    ct_hit_t h = ct_test_all(targets, 2, click);

    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_all_null_targets_no_crash(void)
{
    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_all(NULL, 5, click);
    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_all_null_click_no_crash(void)
{
    ct_target_t targets[1];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.5f);
    targets[0].visible = 1;

    ct_hit_t h = ct_test_all(targets, 1, NULL);
    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

void test_test_all_count_zero_no_crash(void)
{
    ct_target_t targets[1];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.5f);
    float click[2] = {0.0f, 0.0f};

    ct_hit_t h = ct_test_all(targets, 0, click);
    TEST_ASSERT_EQUAL_INT(0, h.hit);
}

/* ===================================================================
 * 6. ct_planet_target — Planet target creation
 * =================================================================== */

void test_planet_target_creates_planet_type(void)
{
    float pos[3] = {1.0f, 2.0f, 3.0f};
    ct_target_t t = ct_planet_target(3, pos);

    TEST_ASSERT_EQUAL_INT(CT_PLANET, t.type);
    TEST_ASSERT_EQUAL_INT(3, t.object_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, t.world_pos[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, t.world_pos[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, t.world_pos[2]);
}

void test_planet_target_sun_larger_than_earth(void)
{
    float pos[3] = {0.0f, 0.0f, 0.0f};
    ct_target_t sun = ct_planet_target(0, pos);    /* Sun */
    ct_target_t earth = ct_planet_target(3, pos);  /* Earth */

    TEST_ASSERT_TRUE(sun.screen_radius > earth.screen_radius);
}

void test_planet_target_jupiter_larger_than_mars(void)
{
    float pos[3] = {0.0f, 0.0f, 0.0f};
    ct_target_t jupiter = ct_planet_target(5, pos);
    ct_target_t mars = ct_planet_target(4, pos);

    TEST_ASSERT_TRUE(jupiter.screen_radius > mars.screen_radius);
}

void test_planet_target_out_of_range_clamped(void)
{
    float pos[3] = {0.0f, 0.0f, 0.0f};
    ct_target_t t_neg = ct_planet_target(-1, pos);
    ct_target_t t_big = ct_planet_target(99, pos);
    ct_target_t t_earth = ct_planet_target(3, pos);

    /* Out-of-range IDs should get Earth's radius (index 3) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, t_earth.screen_radius, t_neg.screen_radius);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, t_earth.screen_radius, t_big.screen_radius);
}

void test_planet_target_null_world_pos_safe(void)
{
    ct_target_t t = ct_planet_target(0, NULL);
    TEST_ASSERT_EQUAL_INT(CT_PLANET, t.type);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.world_pos[0]);
}

/* ===================================================================
 * 7. ct_star_target — Star target creation
 * =================================================================== */

void test_star_target_creates_star_type(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    ct_target_t t = ct_star_target(7, pos, 2.0f);

    TEST_ASSERT_EQUAL_INT(CT_STAR, t.type);
    TEST_ASSERT_EQUAL_INT(7, t.object_id);
}

void test_star_target_brighter_larger(void)
{
    float pos[3] = {0.0f, 0.0f, 0.0f};
    ct_target_t bright = ct_star_target(0, pos, -1.0f);  /* Sirius-class */
    ct_target_t dim = ct_star_target(1, pos, 4.0f);      /* Dim star */

    TEST_ASSERT_TRUE(bright.screen_radius > dim.screen_radius);
}

void test_star_target_very_dim_minimum_radius(void)
{
    float pos[3] = {0.0f, 0.0f, 0.0f};
    ct_target_t t = ct_star_target(0, pos, 20.0f); /* Very dim */

    /* Should be at minimum radius (0.01) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.01f, t.screen_radius);
}

void test_star_target_null_world_pos_safe(void)
{
    ct_target_t t = ct_star_target(0, NULL, 1.0f);
    TEST_ASSERT_EQUAL_INT(CT_STAR, t.type);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.world_pos[0]);
}

/* ===================================================================
 * 8. ct_dso_target — DSO target creation
 * =================================================================== */

void test_dso_target_creates_dso_type(void)
{
    float pos[3] = {0.0f, 0.0f, -5.0f};
    ct_target_t t = ct_dso_target(13, pos, 1.0f);

    TEST_ASSERT_EQUAL_INT(CT_DSO, t.type);
    TEST_ASSERT_EQUAL_INT(13, t.object_id);
}

void test_dso_target_larger_angular_size_larger_radius(void)
{
    float pos[3] = {0.0f, 0.0f, 0.0f};
    ct_target_t big = ct_dso_target(0, pos, 2.0f);
    ct_target_t small = ct_dso_target(1, pos, 0.5f);

    TEST_ASSERT_TRUE(big.screen_radius > small.screen_radius);
}

void test_dso_target_null_world_pos_safe(void)
{
    ct_target_t t = ct_dso_target(0, NULL, 1.0f);
    TEST_ASSERT_EQUAL_INT(CT_DSO, t.type);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.world_pos[0]);
}

/* ===================================================================
 * 9. ct_sort_by_depth — Depth sorting
 * =================================================================== */

void test_sort_by_depth_sorts_front_to_back(void)
{
    ct_target_t targets[4];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[0].depth = 10.0f;
    targets[1] = make_target(CT_STAR, 1, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[1].depth = 2.0f;
    targets[2] = make_target(CT_DSO, 2, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[2].depth = 5.0f;
    targets[3] = make_target(CT_MOON, 3, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[3].depth = 1.0f;

    ct_sort_by_depth(targets, 4);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, targets[0].depth);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f, targets[1].depth);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.0f, targets[2].depth);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, targets[3].depth);
}

void test_sort_by_depth_already_sorted(void)
{
    ct_target_t targets[3];
    targets[0] = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[0].depth = 1.0f;
    targets[1] = make_target(CT_STAR, 1, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[1].depth = 2.0f;
    targets[2] = make_target(CT_DSO, 2, 0.0f, 0.0f, 0.0f, 0.05f);
    targets[2].depth = 3.0f;

    ct_sort_by_depth(targets, 3);

    TEST_ASSERT_EQUAL_INT(0, targets[0].object_id);
    TEST_ASSERT_EQUAL_INT(1, targets[1].object_id);
    TEST_ASSERT_EQUAL_INT(2, targets[2].object_id);
}

void test_sort_by_depth_single_element(void)
{
    ct_target_t t = make_target(CT_PLANET, 0, 0.0f, 0.0f, 0.0f, 0.05f);
    t.depth = 5.0f;

    ct_sort_by_depth(&t, 1);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.0f, t.depth);
}

void test_sort_by_depth_null_no_crash(void)
{
    ct_sort_by_depth(NULL, 5);
    /* No crash = pass */
}

/* ===================================================================
 * 10. ct_type_name — Type name strings
 * =================================================================== */

void test_type_name_all_types_non_null(void)
{
    for (int i = 0; i < CT_TYPE_COUNT; i++) {
        const char *name = ct_type_name((ct_type_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

void test_type_name_planet(void)
{
    const char *name = ct_type_name(CT_PLANET);
    TEST_ASSERT_EQUAL_STRING("Planet", name);
}

void test_type_name_star(void)
{
    const char *name = ct_type_name(CT_STAR);
    TEST_ASSERT_EQUAL_STRING("Star", name);
}

void test_type_name_invalid_returns_unknown(void)
{
    const char *name = ct_type_name((ct_type_t)99);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

void test_type_name_negative_returns_unknown(void)
{
    const char *name = ct_type_name((ct_type_t)-1);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

/* ===================================================================
 * 11. Integration — full project + hit pipeline
 * =================================================================== */

void test_full_pipeline_project_then_hit(void)
{
    float m[16];
    simple_proj_matrix(m);

    /* Object at (0, 0, -3): should project to center of screen */
    ct_target_t t = make_target(CT_MOON, 0, 0.0f, 0.0f, -3.0f, 0.15f);

    ct_project(&t, m);

    TEST_ASSERT_EQUAL_INT(1, t.visible);

    /* Click at center should hit */
    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_one(&t, click);
    TEST_ASSERT_EQUAL_INT(1, h.hit);
    TEST_ASSERT_EQUAL_INT(CT_MOON, h.type);
}

void test_full_pipeline_multiple_targets(void)
{
    float m[16];
    simple_proj_matrix(m);

    ct_target_t targets[3];
    targets[0] = ct_planet_target(0, (float[]){0.0f, 0.0f, -1.0f}); /* Sun close */
    targets[1] = ct_planet_target(5, (float[]){0.0f, 0.0f, -5.0f}); /* Jupiter far */
    targets[2] = ct_star_target(0, (float[]){0.5f, 0.0f, -2.0f}, 1.0f);

    ct_project_all(targets, 3, m);

    /* Click at center should hit Sun (closest) */
    float click[2] = {0.0f, 0.0f};
    ct_hit_t h = ct_test_all(targets, 3, click);

    TEST_ASSERT_EQUAL_INT(1, h.hit);
    /* Sun is at depth 1, should win over Jupiter at depth 5 */
    TEST_ASSERT_EQUAL_INT(CT_PLANET, h.type);
    TEST_ASSERT_EQUAL_INT(0, h.object_id);
}

/* ===================================================================
 * MAIN
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* ct_no_hit */
    RUN_TEST(test_no_hit_returns_zero);
    RUN_TEST(test_no_hit_object_id_negative);

    /* ct_project */
    RUN_TEST(test_project_origin_identity_visible);
    RUN_TEST(test_project_behind_camera_invisible);
    RUN_TEST(test_project_off_screen_invisible);
    RUN_TEST(test_project_null_target_no_crash);
    RUN_TEST(test_project_null_view_proj_no_crash);
    RUN_TEST(test_project_depth_set_correctly);
    RUN_TEST(test_project_screen_radius_scaled_by_depth);

    /* ct_project_all */
    RUN_TEST(test_project_all_projects_array);
    RUN_TEST(test_project_all_null_inputs_no_crash);
    RUN_TEST(test_project_all_count_zero_no_crash);

    /* ct_test_one */
    RUN_TEST(test_test_one_click_on_target_hit);
    RUN_TEST(test_test_one_click_outside_radius_miss);
    RUN_TEST(test_test_one_invisible_target_miss);
    RUN_TEST(test_test_one_null_target_no_crash);
    RUN_TEST(test_test_one_null_click_no_crash);
    RUN_TEST(test_test_one_distance_correct);

    /* ct_test_all */
    RUN_TEST(test_test_all_closest_wins);
    RUN_TEST(test_test_all_invisible_skipped);
    RUN_TEST(test_test_all_no_hit);
    RUN_TEST(test_test_all_null_targets_no_crash);
    RUN_TEST(test_test_all_null_click_no_crash);
    RUN_TEST(test_test_all_count_zero_no_crash);

    /* ct_planet_target */
    RUN_TEST(test_planet_target_creates_planet_type);
    RUN_TEST(test_planet_target_sun_larger_than_earth);
    RUN_TEST(test_planet_target_jupiter_larger_than_mars);
    RUN_TEST(test_planet_target_out_of_range_clamped);
    RUN_TEST(test_planet_target_null_world_pos_safe);

    /* ct_star_target */
    RUN_TEST(test_star_target_creates_star_type);
    RUN_TEST(test_star_target_brighter_larger);
    RUN_TEST(test_star_target_very_dim_minimum_radius);
    RUN_TEST(test_star_target_null_world_pos_safe);

    /* ct_dso_target */
    RUN_TEST(test_dso_target_creates_dso_type);
    RUN_TEST(test_dso_target_larger_angular_size_larger_radius);
    RUN_TEST(test_dso_target_null_world_pos_safe);

    /* ct_sort_by_depth */
    RUN_TEST(test_sort_by_depth_sorts_front_to_back);
    RUN_TEST(test_sort_by_depth_already_sorted);
    RUN_TEST(test_sort_by_depth_single_element);
    RUN_TEST(test_sort_by_depth_null_no_crash);

    /* ct_type_name */
    RUN_TEST(test_type_name_all_types_non_null);
    RUN_TEST(test_type_name_planet);
    RUN_TEST(test_type_name_star);
    RUN_TEST(test_type_name_invalid_returns_unknown);
    RUN_TEST(test_type_name_negative_returns_unknown);

    /* Integration */
    RUN_TEST(test_full_pipeline_project_then_hit);
    RUN_TEST(test_full_pipeline_multiple_targets);

    return UNITY_END();
}
