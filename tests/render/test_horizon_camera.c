/*
 * test_horizon_camera.c -- Tests for ground-level horizon camera module.
 *
 * Covers: create defaults, position, up/zenith, look direction,
 * view matrix, projection matrix, rotation wrapping/clamping. 13 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/horizon_camera.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* Helper: vector length from 3-float array */
static float vec_len3(const float v[3])
{
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/* ======================================================================
 * hcam_create
 * ====================================================================== */

/* 1. Create returns sensible defaults */
void test_hcam_create_defaults(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);
    FCLOSE((float)(PI / 2.0), cam.fov);
    FCLOSE(0.0f, cam.look_azimuth);
    FCLOSE(0.0f, cam.look_elevation);
    TEST_ASSERT_TRUE(cam.near_plane > 0.0f);
    TEST_ASSERT_TRUE(cam.far_plane > cam.near_plane);
    FCLOSE(16.0f / 9.0f, cam.aspect);
}

/* ======================================================================
 * hcam_position
 * ====================================================================== */

/* 2. At lat=0, lon=0: position = (globe_radius, 0, 0) */
void test_hcam_position_equator(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 2.0f);
    float xyz[3];
    hcam_position(&cam, xyz);
    FCLOSE(2.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 3. At lat=90, lon=0: position = (0, globe_radius, 0) */
void test_hcam_position_north_pole(void)
{
    horizon_camera_t cam = hcam_create(90.0, 0.0, 3.0f);
    float xyz[3];
    hcam_position(&cam, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(3.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 4. At lat=-90, lon=0: position = (0, -globe_radius, 0) */
void test_hcam_position_south_pole(void)
{
    horizon_camera_t cam = hcam_create(-90.0, 0.0, 1.0f);
    float xyz[3];
    hcam_position(&cam, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(-1.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* ======================================================================
 * hcam_up_direction (zenith)
 * ====================================================================== */

/* 5. At equator: up = (1, 0, 0) normalized */
void test_hcam_up_is_zenith_equator(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);
    float up[3];
    hcam_up_direction(&cam, up);
    FCLOSE(1.0f, up[0]);
    FCLOSE(0.0f, up[1]);
    FCLOSE(0.0f, up[2]);
    FCLOSE(1.0f, vec_len3(up));
}

/* 6. At north pole: up = (0, 1, 0) */
void test_hcam_up_is_zenith_pole(void)
{
    horizon_camera_t cam = hcam_create(90.0, 0.0, 1.0f);
    float up[3];
    hcam_up_direction(&cam, up);
    FCLOSE(0.0f, up[0]);
    FCLOSE(1.0f, up[1]);
    FCLOSE(0.0f, up[2]);
    FCLOSE(1.0f, vec_len3(up));
}

/* ======================================================================
 * hcam_look_direction
 * ====================================================================== */

/* 7. Looking north at equator (az=0, el=0): look dir has positive Y component */
void test_hcam_look_north_at_equator(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);
    cam.look_azimuth = 0.0f;
    cam.look_elevation = 0.0f;
    float dir[3];
    hcam_look_direction(&cam, dir);
    /* At equator (0,0), north is tangent pointing toward pole => positive Y */
    TEST_ASSERT_TRUE(dir[1] > 0.5f);
}

/* 8. Looking east at equator (az=PI/2): look dir tangent along +Z */
void test_hcam_look_east_at_equator(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);
    cam.look_azimuth = (float)(PI / 2.0);
    cam.look_elevation = 0.0f;
    float dir[3];
    hcam_look_direction(&cam, dir);
    /* At equator (0,0), east is along +Z */
    TEST_ASSERT_TRUE(dir[2] > 0.5f);
}

/* 13. Look direction is always unit length */
void test_hcam_look_direction_unit(void)
{
    double lats[] = {0, 45, -45, 90, -90, 33.5};
    double lons[] = {0, 90, -90, 0, 0, 123.4};
    float azs[] = {0.0f, 1.0f, 3.14f, 0.5f, 2.0f, 5.5f};
    float els[] = {0.0f, 0.3f, -0.5f, 1.0f, 0.0f, -0.7f};

    for (int i = 0; i < 6; i++) {
        horizon_camera_t cam = hcam_create(lats[i], lons[i], 1.0f);
        cam.look_azimuth = azs[i];
        cam.look_elevation = els[i];
        float dir[3];
        hcam_look_direction(&cam, dir);
        float len = vec_len3(dir);
        FCLOSE(1.0f, len);
    }
}

/* ======================================================================
 * hcam_view_matrix
 * ====================================================================== */

/* 9. View matrix has orthogonal rotation block (det of upper-left 3x3 ~= 1) */
void test_hcam_view_matrix_valid(void)
{
    horizon_camera_t cam = hcam_create(40.0, -74.0, 1.0f);
    cam.look_azimuth = 0.5f;
    cam.look_elevation = 0.2f;
    mat4_t v = hcam_view_matrix(&cam);

    /* Extract 3x3 rotation block (column-major: m[col*4+row]) */
    /* Columns of the rotation: (m[0..2]), (m[4..6]), (m[8..10]) */
    /* Compute determinant via Sarrus/cofactor */
    float a00 = v.m[0], a10 = v.m[1], a20 = v.m[2];
    float a01 = v.m[4], a11 = v.m[5], a21 = v.m[6];
    float a02 = v.m[8], a12 = v.m[9], a22 = v.m[10];

    float det = a00 * (a11 * a22 - a12 * a21)
              - a01 * (a10 * a22 - a12 * a20)
              + a02 * (a10 * a21 - a11 * a20);

    FCLOSE(1.0f, det);

    /* Bottom row of view matrix should be [0, 0, 0, 1] */
    FCLOSE(0.0f, v.m[3]);
    FCLOSE(0.0f, v.m[7]);
    FCLOSE(0.0f, v.m[11]);
    FCLOSE(1.0f, v.m[15]);
}

/* ======================================================================
 * hcam_projection_matrix
 * ====================================================================== */

/* 10. Projection matches fov/aspect */
void test_hcam_projection_matrix(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);
    cam.fov = (float)(PI / 3.0);  /* 60 degrees */
    cam.aspect = 2.0f;
    mat4_t p = hcam_projection_matrix(&cam);

    float f = 1.0f / tanf(cam.fov / 2.0f);
    FCLOSE(f / cam.aspect, p.m[0]);
    FCLOSE(f, p.m[5]);
    FCLOSE(-1.0f, p.m[11]);
}

/* ======================================================================
 * hcam_rotate
 * ====================================================================== */

/* 11. Azimuth wraps from >2PI to 0-2PI range */
void test_hcam_rotate_azimuth_wraps(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);
    cam.look_azimuth = 6.0f;  /* just under 2PI */
    hcam_rotate(&cam, 1.0f, 0.0f);  /* push over 2PI */
    /* Should wrap to [0, 2PI) */
    TEST_ASSERT_TRUE(cam.look_azimuth >= 0.0f);
    TEST_ASSERT_TRUE(cam.look_azimuth < (float)(2.0 * PI));
}

/* 12. Elevation clamps to [-PI/4, PI/2] */
void test_hcam_rotate_elevation_clamps(void)
{
    horizon_camera_t cam = hcam_create(0.0, 0.0, 1.0f);

    /* Push elevation way up */
    cam.look_elevation = 0.0f;
    hcam_rotate(&cam, 0.0f, 10.0f);
    FCLOSE((float)(PI / 2.0), cam.look_elevation);

    /* Push elevation way down */
    cam.look_elevation = 0.0f;
    hcam_rotate(&cam, 0.0f, -10.0f);
    FCLOSE((float)(-PI / 4.0), cam.look_elevation);
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* hcam_create */
    RUN_TEST(test_hcam_create_defaults);

    /* hcam_position */
    RUN_TEST(test_hcam_position_equator);
    RUN_TEST(test_hcam_position_north_pole);
    RUN_TEST(test_hcam_position_south_pole);

    /* hcam_up_direction */
    RUN_TEST(test_hcam_up_is_zenith_equator);
    RUN_TEST(test_hcam_up_is_zenith_pole);

    /* hcam_look_direction */
    RUN_TEST(test_hcam_look_north_at_equator);
    RUN_TEST(test_hcam_look_east_at_equator);
    RUN_TEST(test_hcam_look_direction_unit);

    /* hcam_view_matrix */
    RUN_TEST(test_hcam_view_matrix_valid);

    /* hcam_projection_matrix */
    RUN_TEST(test_hcam_projection_matrix);

    /* hcam_rotate */
    RUN_TEST(test_hcam_rotate_azimuth_wraps);
    RUN_TEST(test_hcam_rotate_elevation_clamps);

    return UNITY_END();
}
