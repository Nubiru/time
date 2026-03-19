/*
 * test_earth_view_frame.c -- Tests for Earth View per-frame data aggregator.
 *
 * Covers: frame computation, camera vectors, sun position, daytime check,
 * sky config consistency, cardinal points, simplified API, matrix validity,
 * sun altitude range. 14 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/earth_view_frame.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define FTOL 0.01f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

/* Astronomical tolerance: 1 degree */
#define ASTRO_TOL 1.0f

void setUp(void) {}
void tearDown(void) {}

/* Helper: vector length from 3-float array */
static float vec_len3(const float v[3])
{
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/* JD for 2024-06-21 noon UTC (summer solstice, solar noon at lon=0).
 * se_solar_altitude uses jd_noon = floor(jd-0.5)+0.5, so JD .5 = UT noon. */
#define JD_SUMMER_NOON 2460482.5

/* JD for 2024-06-21 midnight UTC */
#define JD_SUMMER_MIDNIGHT 2460483.0

/* ======================================================================
 * ev_compute_frame — basic validity
 * ====================================================================== */

/* 1. Frame returns valid data with non-zero matrices */
void test_ev_compute_frame_valid(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     0.0, 0.0,        /* equator, prime meridian */
                                     0.0f, 0.0f,      /* looking north, horizon */
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    /* View matrix should not be all zeros */
    int has_nonzero = 0;
    for (int i = 0; i < 16; i++) {
        if (f.view_matrix.m[i] != 0.0f) has_nonzero = 1;
    }
    TEST_ASSERT_TRUE(has_nonzero);

    /* Proj matrix should not be all zeros */
    has_nonzero = 0;
    for (int i = 0; i < 16; i++) {
        if (f.proj_matrix.m[i] != 0.0f) has_nonzero = 1;
    }
    TEST_ASSERT_TRUE(has_nonzero);
}

/* ======================================================================
 * Camera vectors
 * ====================================================================== */

/* 2. Camera position length ~= globe_radius */
void test_ev_camera_pos_on_globe(void)
{
    float globe_r = 2.5f;
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     45.0, 10.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, globe_r);

    float len = vec_len3(f.camera_pos);
    FCLOSE(globe_r, len);
}

/* 3. Camera up vector is unit length */
void test_ev_camera_up_unit(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     30.0, -74.0,
                                     1.0f, 0.2f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    float len = vec_len3(f.camera_up);
    FCLOSE(1.0f, len);
}

/* 4. Camera look vector is unit length */
void test_ev_camera_look_unit(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     -33.9, 151.2,    /* Sydney */
                                     (float)PI, 0.1f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    float len = vec_len3(f.camera_look);
    FCLOSE(1.0f, len);
}

/* ======================================================================
 * Sun position
 * ====================================================================== */

/* 5. Summer solstice noon at equator (lon=0): sun altitude > 60 degrees */
void test_ev_sun_position_summer_noon(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     0.0, 0.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    TEST_ASSERT_TRUE(f.sun_altitude_deg > 60.0f);
}

/* 6. Midnight at equator (lon=0): sun altitude < 0 */
void test_ev_sun_position_night(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_MIDNIGHT,
                                     0.0, 0.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    TEST_ASSERT_TRUE(f.sun_altitude_deg < 0.0f);
}

/* ======================================================================
 * ev_is_daytime
 * ====================================================================== */

/* 7. Daytime when sun > 0 */
void test_ev_is_daytime_day(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     0.0, 0.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    TEST_ASSERT_EQUAL_INT(1, ev_is_daytime(&f));
}

/* 8. Not daytime when sun < 0 */
void test_ev_is_daytime_night(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_MIDNIGHT,
                                     0.0, 0.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    TEST_ASSERT_EQUAL_INT(0, ev_is_daytime(&f));
}

/* ======================================================================
 * Sky config consistency
 * ====================================================================== */

/* 9. sky_config sun altitude/azimuth matches frame sun values */
void test_ev_sky_config_matches_sun(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     40.0, -74.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    TEST_ASSERT_FLOAT_WITHIN(ASTRO_TOL,
        (float)f.sky_config.sun_altitude_deg, f.sun_altitude_deg);
    TEST_ASSERT_FLOAT_WITHIN(ASTRO_TOL,
        (float)f.sky_config.sun_azimuth_deg, f.sun_azimuth_deg);
}

/* ======================================================================
 * Cardinal points
 * ====================================================================== */

/* 10. Always 8 cardinal points */
void test_ev_cardinals_eight(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     51.5, -0.1,      /* London */
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    TEST_ASSERT_EQUAL_INT(8, f.cardinal_count);
}

/* ======================================================================
 * Simplified API
 * ====================================================================== */

/* 11. ev_compute_simple produces valid frame with reasonable defaults */
void test_ev_compute_simple_defaults(void)
{
    ev_frame_t f = ev_compute_simple(JD_SUMMER_NOON, 0.0, 0.0, 0.0f, 0.0f);

    /* Should have valid camera position */
    float len = vec_len3(f.camera_pos);
    TEST_ASSERT_TRUE(len > 0.0f);

    /* Should have 8 cardinals */
    TEST_ASSERT_EQUAL_INT(8, f.cardinal_count);

    /* Should have at least 1 object (Sun) */
    TEST_ASSERT_TRUE(f.object_count >= 1);
}

/* ======================================================================
 * Matrix validity
 * ====================================================================== */

/* 12. View matrix rotation block has det ~= 1 */
void test_ev_view_matrix_valid(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     40.0, -74.0,
                                     0.5f, 0.2f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    /* Extract 3x3 rotation block (column-major) */
    float a00 = f.view_matrix.m[0],  a10 = f.view_matrix.m[1],  a20 = f.view_matrix.m[2];
    float a01 = f.view_matrix.m[4],  a11 = f.view_matrix.m[5],  a21 = f.view_matrix.m[6];
    float a02 = f.view_matrix.m[8],  a12 = f.view_matrix.m[9],  a22 = f.view_matrix.m[10];

    float det = a00 * (a11 * a22 - a12 * a21)
              - a01 * (a10 * a22 - a12 * a20)
              + a02 * (a10 * a21 - a11 * a20);

    FCLOSE(1.0f, det);
}

/* 13. Projection matrix has expected structure (perspective) */
void test_ev_proj_matrix_valid(void)
{
    float fov = (float)(PI / 3.0); /* 60 degrees */
    float aspect = 2.0f;

    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     0.0, 0.0,
                                     0.0f, 0.0f,
                                     fov, aspect, 1.0f);

    /* m[0] = f_val / aspect, m[5] = f_val, m[11] = -1 */
    float f_val = 1.0f / tanf(fov / 2.0f);
    FCLOSE(f_val / aspect, f.proj_matrix.m[0]);
    FCLOSE(f_val, f.proj_matrix.m[5]);
    FCLOSE(-1.0f, f.proj_matrix.m[11]);
}

/* ======================================================================
 * Sun altitude range
 * ====================================================================== */

/* 14. Sun altitude always in [-90, 90] */
void test_ev_sun_altitude_range(void)
{
    /* Test at various locations and times */
    double jds[]  = {JD_SUMMER_NOON, JD_SUMMER_MIDNIGHT, 2460300.0, 2460400.0};
    double lats[] = {0.0, 45.0, -45.0, 80.0};
    double lons[] = {0.0, 90.0, -90.0, -120.0};

    for (int i = 0; i < 4; i++) {
        ev_frame_t f = ev_compute_frame(jds[i], lats[i], lons[i],
                                         0.0f, 0.0f,
                                         (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

        TEST_ASSERT_TRUE(f.sun_altitude_deg >= -90.0f);
        TEST_ASSERT_TRUE(f.sun_altitude_deg <= 90.0f);
    }
}

/* ======================================================================
 * ev_sun_position accessor
 * ====================================================================== */

/* 15. ev_sun_position returns same values as frame fields */
void test_ev_sun_position_accessor(void)
{
    ev_frame_t f = ev_compute_frame(JD_SUMMER_NOON,
                                     30.0, 20.0,
                                     0.0f, 0.0f,
                                     (float)(PI / 2.0), 16.0f / 9.0f, 1.0f);

    float alt, az;
    ev_sun_position(&f, &alt, &az);

    FCLOSE(f.sun_altitude_deg, alt);
    FCLOSE(f.sun_azimuth_deg, az);
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Frame validity */
    RUN_TEST(test_ev_compute_frame_valid);

    /* Camera vectors */
    RUN_TEST(test_ev_camera_pos_on_globe);
    RUN_TEST(test_ev_camera_up_unit);
    RUN_TEST(test_ev_camera_look_unit);

    /* Sun position */
    RUN_TEST(test_ev_sun_position_summer_noon);
    RUN_TEST(test_ev_sun_position_night);

    /* Daytime */
    RUN_TEST(test_ev_is_daytime_day);
    RUN_TEST(test_ev_is_daytime_night);

    /* Sky config */
    RUN_TEST(test_ev_sky_config_matches_sun);

    /* Cardinals */
    RUN_TEST(test_ev_cardinals_eight);

    /* Simplified API */
    RUN_TEST(test_ev_compute_simple_defaults);

    /* Matrix validity */
    RUN_TEST(test_ev_view_matrix_valid);
    RUN_TEST(test_ev_proj_matrix_valid);

    /* Sun altitude range */
    RUN_TEST(test_ev_sun_altitude_range);

    /* Accessor */
    RUN_TEST(test_ev_sun_position_accessor);

    return UNITY_END();
}
