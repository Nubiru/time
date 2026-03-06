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
    /* azimuth=0, elevation=0.3 -> camera at roughly (0, d*sin(0.3), d*cos(0.3)) */
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = 0.0f; /* simplify: look straight from +Z */
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, pos.z);
}

void test_camera_position_azimuth_90(void) {
    /* azimuth=PI/2 -> camera at +X axis */
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.azimuth = PI / 2.0f;
    cam.elevation = 0.0f;
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, pos.z);
}

void test_camera_position_elevation_90(void) {
    /* elevation near PI/2 -> camera above target */
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = PI / 2.0f - 0.02f;
    vec3_t pos = camera_position(&cam);
    /* y should be nearly distance */
    TEST_ASSERT_TRUE(pos.y > 4.9f);
}

void test_camera_rotate_clamps_elevation(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    cam.elevation = 0.0f;
    /* Rotate way up */
    camera_rotate(&cam, 0.0f, 10.0f);
    TEST_ASSERT_TRUE(cam.elevation < PI / 2.0f);
    /* Rotate way down */
    camera_rotate(&cam, 0.0f, -20.0f);
    TEST_ASSERT_TRUE(cam.elevation > -PI / 2.0f);
}

void test_camera_zoom_min_distance(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    camera_zoom(&cam, -100.0f);
    TEST_ASSERT_TRUE(cam.distance > 0.0f);  /* log zoom has a minimum */
    TEST_ASSERT_TRUE(cam.distance < 1.0f);  /* zoomed way in */
}

void test_camera_zoom_max_distance(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    camera_zoom(&cam, 500.0f);
    TEST_ASSERT_TRUE(cam.distance > 1000.0f);  /* zoomed way out */
}

void test_camera_view_matrix_not_identity(void) {
    camera_t cam = camera_create(5.0f, 0.785f, 1.0f);
    mat4_t view = camera_view_matrix(&cam);
    /* View matrix should not be identity (camera is offset) */
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
    /* Perspective matrix: m[0] and m[5] should be positive, m[14] should be negative */
    TEST_ASSERT_TRUE(proj.m[0] > 0.0f);
    TEST_ASSERT_TRUE(proj.m[5] > 0.0f);
    TEST_ASSERT_TRUE(proj.m[14] < 0.0f);
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
    return UNITY_END();
}
