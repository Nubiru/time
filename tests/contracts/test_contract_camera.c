/* test_contract_camera.c — Exhaustive contract tests for camera.h */

#include "../unity/unity.h"
#include "../../src/render/camera.h"
#include "../../src/math/vec3.h"
#include "../../src/math/mat4.h"
#include <math.h>

#define PI 3.14159265358979323846

void setUp(void) { }
void tearDown(void) { }

static int mat4_all_finite(mat4_t m) {
    for (int i = 0; i < 16; i++) if (!isfinite(m.m[i])) return 0;
    return 1;
}
static int vec3_all_finite(vec3_t v) { return isfinite(v.x) && isfinite(v.y) && isfinite(v.z); }

void test_default_camera_view_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
}

void test_default_camera_projection_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_projection_matrix(&cam)));
}

void test_default_camera_position_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    TEST_ASSERT_TRUE(vec3_all_finite(camera_position(&cam)));
}

void test_rotated_camera_view_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_rotate(&cam, 0.5f, 0.3f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
}

void test_rotated_camera_projection_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_rotate(&cam, 0.5f, 0.3f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_projection_matrix(&cam)));
}

void test_rotated_camera_position_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_rotate(&cam, 0.5f, 0.3f);
    TEST_ASSERT_TRUE(vec3_all_finite(camera_position(&cam)));
}

void test_zoomed_in_camera_view_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_zoom(&cam, -2.0f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
}

void test_zoomed_out_camera_view_finite(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_zoom(&cam, 2.0f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
}

void test_near_less_than_far(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    TEST_ASSERT_TRUE(cam.near_plane < cam.far_plane);
}

void test_position_varies_with_azimuth(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    vec3_t p1 = camera_position(&cam);
    camera_rotate(&cam, (float)(PI / 2.0), 0.0f);
    vec3_t p2 = camera_position(&cam);
    float d = (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) + (p2.z-p1.z)*(p2.z-p1.z);
    TEST_ASSERT_TRUE(d > 0.001f);
}

void test_position_varies_with_elevation(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    vec3_t p1 = camera_position(&cam);
    camera_rotate(&cam, 0.0f, 0.5f);
    vec3_t p2 = camera_position(&cam);
    float d = (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) + (p2.z-p1.z)*(p2.z-p1.z);
    TEST_ASSERT_TRUE(d > 0.001f);
}

void test_extreme_zoom_in(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_zoom(&cam, -20.0f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
    TEST_ASSERT_TRUE(vec3_all_finite(camera_position(&cam)));
}

void test_extreme_zoom_out(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    camera_zoom(&cam, 20.0f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
    TEST_ASSERT_TRUE(vec3_all_finite(camera_position(&cam)));
}

void test_various_aspect_ratios(void) {
    float aspects[] = {0.5f, 1.0f, 1.333f, 1.777f, 2.0f, 3.0f};
    for (int i = 0; i < 6; i++) {
        camera_t cam = camera_create(10.0f, (float)(PI / 4.0), aspects[i]);
        TEST_ASSERT_TRUE_MESSAGE(mat4_all_finite(camera_projection_matrix(&cam)), "proj");
        TEST_ASSERT_TRUE_MESSAGE(mat4_all_finite(camera_view_matrix(&cam)), "view");
    }
}

void test_many_rotations_stable(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    for (int i = 0; i < 100; i++) camera_rotate(&cam, 0.1f, 0.05f);
    TEST_ASSERT_TRUE(mat4_all_finite(camera_view_matrix(&cam)));
    TEST_ASSERT_TRUE(vec3_all_finite(camera_position(&cam)));
}

void test_distance_positive(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    TEST_ASSERT_TRUE(cam.distance > 0.0f);
}

void test_position_distance_matches(void) {
    camera_t cam = camera_create(10.0f, (float)(PI / 4.0), 1.333f);
    vec3_t pos = camera_position(&cam);
    vec3_t diff = vec3_sub(pos, cam.target);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, cam.distance, vec3_length(diff));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_default_camera_view_finite);
    RUN_TEST(test_default_camera_projection_finite);
    RUN_TEST(test_default_camera_position_finite);
    RUN_TEST(test_rotated_camera_view_finite);
    RUN_TEST(test_rotated_camera_projection_finite);
    RUN_TEST(test_rotated_camera_position_finite);
    RUN_TEST(test_zoomed_in_camera_view_finite);
    RUN_TEST(test_zoomed_out_camera_view_finite);
    RUN_TEST(test_near_less_than_far);
    RUN_TEST(test_position_varies_with_azimuth);
    RUN_TEST(test_position_varies_with_elevation);
    RUN_TEST(test_extreme_zoom_in);
    RUN_TEST(test_extreme_zoom_out);
    RUN_TEST(test_various_aspect_ratios);
    RUN_TEST(test_many_rotations_stable);
    RUN_TEST(test_distance_positive);
    RUN_TEST(test_position_distance_matches);
    return UNITY_END();
}
