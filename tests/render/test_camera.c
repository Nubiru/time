#include "../unity/unity.h"
#include "../../src/render/camera.h"
#include <math.h>

#define FLOAT_EPSILON 0.01f
#define PI 3.14159265358979323846f

void setUp(void) { }
void tearDown(void) { }

void test_camera_create_defaults(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.777f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, cam.distance);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, cam.target.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, cam.target.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, cam.target.z);
}

void test_camera_position_default(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = 0.0f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, pos.z);
}

void test_camera_position_azimuth_90(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.azimuth = PI / 2.0f;
    cam.elevation = 0.0f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.z);
}

void test_camera_position_elevation_90(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = PI / 2.0f - 0.02f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_TRUE(pos.y > 4.9f);
}

void test_camera_rotate_clamps_elevation(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = 0.0f;
    camera_rotate(&cam, 0.0f, 10.0f);
    TEST_ASSERT_TRUE(cam.elevation < PI / 2.0f);
    camera_rotate(&cam, 0.0f, -20.0f);
    TEST_ASSERT_TRUE(cam.elevation > -PI / 2.0f);
}

void test_camera_zoom_min_distance(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    camera_zoom(&cam, -100.0f);
    TEST_ASSERT_TRUE(cam.distance > 0.0f);
    TEST_ASSERT_TRUE(cam.distance < 1.0f);
}

void test_camera_zoom_max_distance(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    camera_zoom(&cam, 500.0f);
    TEST_ASSERT_TRUE(cam.distance > 1000.0f);
}

void test_camera_view_matrix_not_identity(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    mat4_t view = camera_view_matrix(&cam);
    mat4_t id = mat4_identity();
    int differs = 0;
    for (int i = 0; i < 16; i++) {
        if (fabsf(view.m[i] - id.m[i]) > 0.001f) differs = 1;
    }
    TEST_ASSERT_TRUE(differs);
}

void test_camera_projection_matrix_valid(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.777f);
    mat4_t proj = camera_projection_matrix(&cam);
    TEST_ASSERT_TRUE(proj.m[0] > 0.0f);
    TEST_ASSERT_TRUE(proj.m[5] > 0.0f);
    TEST_ASSERT_TRUE(proj.m[14] < 0.0f);
}

/* === NEW TESTS === */

void test_camera_create_fov_stored(void) {
    camera_t cam = camera_create(10.0f, 1.2f, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.2f, cam.fov);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.5f, cam.aspect);
}

void test_camera_create_log_zoom(void) {
    camera_t cam = camera_create(10.0f, 0.785f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, logf(10.0f), cam.log_zoom);
}

void test_camera_create_initial_azimuth(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, cam.azimuth);
}

void test_camera_position_azimuth_180(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.azimuth = PI;
    cam.elevation = 0.0f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -5.0f, pos.z);
}

void test_camera_position_azimuth_270(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.azimuth = 3.0f * PI / 2.0f;
    cam.elevation = 0.0f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -5.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, pos.z);
}

void test_camera_position_distance_from_target(void) {
    camera_t cam = camera_create(7.0f, 0.785f, 1.0f);
    cam.azimuth = 1.23f;
    cam.elevation = 0.45f;
    vec3_t pos = camera_position(&cam);
    vec3_t diff = vec3_sub(pos, cam.target);
    float dist = vec3_length(diff);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 7.0f, dist);
}

void test_camera_position_with_offset_target(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.target = vec3_create(10.0f, 20.0f, 30.0f);
    cam.elevation = 0.0f;
    cam.azimuth = 0.0f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 10.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 20.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 35.0f, pos.z);
}

void test_camera_rotate_azimuth_wraps(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.azimuth = 0.0f;
    camera_rotate(&cam, 100.0f, 0.0f);
    TEST_ASSERT_TRUE(cam.azimuth > 10.0f);
}

void test_camera_rotate_small_delta(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    float original_elev = cam.elevation;
    camera_rotate(&cam, 0.0f, 0.01f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, original_elev + 0.01f, cam.elevation);
}

void test_camera_rotate_elevation_upper_clamp(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = PI / 2.0f - 0.02f;
    camera_rotate(&cam, 0.0f, 0.1f);
    TEST_ASSERT_TRUE(cam.elevation < PI / 2.0f);
}

void test_camera_rotate_elevation_lower_clamp(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = -PI / 2.0f + 0.02f;
    camera_rotate(&cam, 0.0f, -0.1f);
    TEST_ASSERT_TRUE(cam.elevation > -PI / 2.0f);
}

void test_camera_zoom_positive_increases_distance(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    float original = cam.distance;
    camera_zoom(&cam, 1.0f);
    TEST_ASSERT_TRUE(cam.distance > original);
}

void test_camera_zoom_negative_decreases_distance(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    float original = cam.distance;
    camera_zoom(&cam, -1.0f);
    TEST_ASSERT_TRUE(cam.distance < original);
}

void test_camera_zoom_distance_always_positive(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    for (int i = 0; i < 20; i++) {
        camera_zoom(&cam, -5.0f);
    }
    TEST_ASSERT_TRUE(cam.distance > 0.0f);
}

void test_camera_view_matrix_changes_with_rotation(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    mat4_t v1 = camera_view_matrix(&cam);
    camera_rotate(&cam, 0.5f, 0.0f);
    mat4_t v2 = camera_view_matrix(&cam);
    int differs = 0;
    for (int i = 0; i < 16; i++) {
        if (fabsf(v1.m[i] - v2.m[i]) > 0.001f) differs = 1;
    }
    TEST_ASSERT_TRUE(differs);
}

void test_camera_projection_fov_effect(void) {
    camera_t cam_narrow = camera_create(5.0f, PI / 6.0f, 1.0f);
    camera_t cam_wide = camera_create(5.0f, PI / 2.0f, 1.0f);
    mat4_t p_narrow = camera_projection_matrix(&cam_narrow);
    mat4_t p_wide = camera_projection_matrix(&cam_wide);
    TEST_ASSERT_TRUE(p_narrow.m[5] > p_wide.m[5]);
}

void test_camera_position_is_pure(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.azimuth = 1.0f;
    cam.elevation = 0.5f;
    vec3_t p1 = camera_position(&cam);
    vec3_t p2 = camera_position(&cam);
    TEST_ASSERT_EQUAL_FLOAT(p1.x, p2.x);
    TEST_ASSERT_EQUAL_FLOAT(p1.y, p2.y);
    TEST_ASSERT_EQUAL_FLOAT(p1.z, p2.z);
}

/* --- Dynamic FOV --- */

void test_dynamic_fov_at_min_zoom(void) {
    float base = (float)(PI / 3.0);
    float fov = camera_dynamic_fov(-4.605f, base);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, base * 0.5f, fov); /* narrow at close zoom */
}

void test_dynamic_fov_at_max_zoom(void) {
    float base = (float)(PI / 3.0);
    float fov = camera_dynamic_fov(10.82f, base);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, base * 1.5f, fov); /* wide at far zoom */
}

void test_dynamic_fov_at_middle(void) {
    float base = (float)(PI / 3.0);
    float mid_zoom = (-4.605f + 10.82f) * 0.5f;
    float fov = camera_dynamic_fov(mid_zoom, base);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, base, fov); /* near base at middle */
}

void test_dynamic_fov_monotonic(void) {
    float base = (float)(PI / 3.0);
    float fov1 = camera_dynamic_fov(-2.0f, base);
    float fov2 = camera_dynamic_fov(2.0f, base);
    float fov3 = camera_dynamic_fov(8.0f, base);
    TEST_ASSERT_TRUE(fov1 < fov2);
    TEST_ASSERT_TRUE(fov2 < fov3);
}

void test_zoom_updates_fov(void) {
    camera_t cam = camera_create(5.0f, (float)(PI / 3.0), 1.0f);
    float fov_before = cam.fov;
    camera_zoom(&cam, 3.0f); /* zoom out */
    TEST_ASSERT_TRUE(cam.fov > fov_before); /* FOV should widen */
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_camera_create_defaults);
    RUN_TEST(test_camera_position_default);
    RUN_TEST(test_camera_position_azimuth_90);
    RUN_TEST(test_camera_position_elevation_90);
    RUN_TEST(test_camera_rotate_clamps_elevation);
    RUN_TEST(test_camera_zoom_min_distance);
    RUN_TEST(test_camera_zoom_max_distance);
    RUN_TEST(test_camera_view_matrix_not_identity);
    RUN_TEST(test_camera_projection_matrix_valid);
    RUN_TEST(test_camera_create_fov_stored);
    RUN_TEST(test_camera_create_log_zoom);
    RUN_TEST(test_camera_create_initial_azimuth);
    RUN_TEST(test_camera_position_azimuth_180);
    RUN_TEST(test_camera_position_azimuth_270);
    RUN_TEST(test_camera_position_distance_from_target);
    RUN_TEST(test_camera_position_with_offset_target);
    RUN_TEST(test_camera_rotate_azimuth_wraps);
    RUN_TEST(test_camera_rotate_small_delta);
    RUN_TEST(test_camera_rotate_elevation_upper_clamp);
    RUN_TEST(test_camera_rotate_elevation_lower_clamp);
    RUN_TEST(test_camera_zoom_positive_increases_distance);
    RUN_TEST(test_camera_zoom_negative_decreases_distance);
    RUN_TEST(test_camera_zoom_distance_always_positive);
    RUN_TEST(test_camera_view_matrix_changes_with_rotation);
    RUN_TEST(test_camera_projection_fov_effect);
    RUN_TEST(test_camera_position_is_pure);
    /* Dynamic FOV */
    RUN_TEST(test_dynamic_fov_at_min_zoom);
    RUN_TEST(test_dynamic_fov_at_max_zoom);
    RUN_TEST(test_dynamic_fov_at_middle);
    RUN_TEST(test_dynamic_fov_monotonic);
    RUN_TEST(test_zoom_updates_fov);
    return UNITY_END();
}
