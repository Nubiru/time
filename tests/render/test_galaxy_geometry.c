#include "../unity/unity.h"
#include "../../src/render/galaxy_geometry.h"

#include <math.h>

#define PI 3.14159265358979323846
#define TOL 0.1f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Four arms generated */
void test_four_arms(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    for (int i = 0; i < GALAXY_ARM_COUNT; i++)
        TEST_ASSERT_EQUAL_INT(32, m.arms[i].count);
}

/* 2. Points per arm clamped to max */
void test_points_clamped(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 999);
    TEST_ASSERT_EQUAL_INT(GALAXY_MAX_ARM_POINTS, m.arms[0].count);
}

/* 3. Points per arm minimum 1 */
void test_points_minimum(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 0);
    TEST_ASSERT_EQUAL_INT(1, m.arms[0].count);
}

/* 4. All y coordinates zero */
void test_y_zero(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    for (int arm = 0; arm < GALAXY_ARM_COUNT; arm++)
        for (int i = 0; i < m.arms[arm].count; i++)
            TEST_ASSERT_TRUE(m.arms[arm].positions[i * 3 + 1] == 0.0f);
}

/* 5. Points increase in radius along each arm */
void test_radius_increases(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    for (int arm = 0; arm < GALAXY_ARM_COUNT; arm++) {
        float prev_r = 0.0f;
        for (int i = 0; i < m.arms[arm].count; i++) {
            float x = m.arms[arm].positions[i * 3 + 0];
            float z = m.arms[arm].positions[i * 3 + 2];
            float r = sqrtf(x * x + z * z);
            TEST_ASSERT_TRUE(r >= prev_r - 0.01f);
            prev_r = r;
        }
    }
}

/* 6. First point near center (10% of radius) */
void test_start_near_center(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    float x = m.arms[0].positions[0];
    float z = m.arms[0].positions[2];
    float r = sqrtf(x * x + z * z);
    TEST_ASSERT_TRUE(r < 10.0f); /* 10% of 50 = 5, with some tolerance */
}

/* 7. Last point near galaxy radius */
void test_end_near_radius(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 64);
    int last = m.arms[0].count - 1;
    float x = m.arms[0].positions[last * 3 + 0];
    float z = m.arms[0].positions[last * 3 + 2];
    float r = sqrtf(x * x + z * z);
    TEST_ASSERT_TRUE(r > 40.0f); /* Should approach 50 */
}

/* 8. Sun at correct distance */
void test_sun_distance(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    float r = sqrtf(m.sun_x * m.sun_x + m.sun_z * m.sun_z);
    TEST_ASSERT_TRUE(NEAR(27.0f, r));
}

/* 9. Sun y = 0 */
void test_sun_y_zero(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    TEST_ASSERT_TRUE(m.sun_y == 0.0f);
}

/* 10. Sun position changes with rotation */
void test_sun_rotates(void)
{
    galaxy_mesh_t a = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    galaxy_mesh_t b = galaxy_generate(50.0f, 27.0f, 90.0f, 32);
    /* Sun position should differ after 90° rotation */
    TEST_ASSERT_TRUE(fabsf(a.sun_x - b.sun_x) > 1.0f ||
                     fabsf(a.sun_z - b.sun_z) > 1.0f);
}

/* 11. Arms offset by 90 degrees */
void test_arms_offset(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 32);
    /* Check first points of arms 0 and 1 are at different angles */
    float a0 = atan2f(m.arms[0].positions[2], m.arms[0].positions[0]);
    float a1 = atan2f(m.arms[1].positions[2], m.arms[1].positions[0]);
    float diff = fabsf(a1 - a0);
    /* Should be ~PI/2 (90 degrees) */
    TEST_ASSERT_TRUE(diff > 1.2f && diff < 1.9f);
}

/* 12. Pitch angle ~12 degrees */
void test_pitch_angle(void)
{
    float p = galaxy_pitch_angle();
    TEST_ASSERT_TRUE(NEAR(12.0f, p));
}

/* 13. Galactic year ~225 million */
void test_galactic_year(void)
{
    double y = galaxy_year();
    TEST_ASSERT_TRUE(y > 224000000.0 && y < 226000000.0);
}

/* 14. Sun angle at rotation 0 */
void test_sun_angle_base(void)
{
    float a = galaxy_sun_angle(0.0f);
    TEST_ASSERT_TRUE(a >= 0.0f && a < 360.0f);
}

/* 15. Sun angle advances with rotation */
void test_sun_angle_rotates(void)
{
    float a0 = galaxy_sun_angle(0.0f);
    float a90 = galaxy_sun_angle(90.0f);
    float diff = a90 - a0;
    if (diff < 0.0f) diff += 360.0f;
    TEST_ASSERT_TRUE(NEAR(90.0f, diff));
}

/* 16. Rotation 360 equals rotation 0 */
void test_rotation_wraps(void)
{
    galaxy_mesh_t a = galaxy_generate(50.0f, 27.0f, 0.0f, 16);
    galaxy_mesh_t b = galaxy_generate(50.0f, 27.0f, 360.0f, 16);
    for (int i = 0; i < a.arms[0].count; i++) {
        TEST_ASSERT_TRUE(NEAR(a.arms[0].positions[i * 3 + 0],
                              b.arms[0].positions[i * 3 + 0]));
        TEST_ASSERT_TRUE(NEAR(a.arms[0].positions[i * 3 + 2],
                              b.arms[0].positions[i * 3 + 2]));
    }
}

/* 17. Brightness values in [0, 1] */
void test_brightness_range(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 64);
    for (int arm = 0; arm < GALAXY_ARM_COUNT; arm++)
        for (int i = 0; i < m.arms[arm].count; i++) {
            float br = m.arms[arm].brightnesses[i];
            TEST_ASSERT_TRUE(br >= 0.0f && br <= 1.0f);
        }
}

/* 18. Brightness fades at edges, peaks mid-arm */
void test_brightness_peaks_mid(void)
{
    galaxy_mesh_t m = galaxy_generate(50.0f, 27.0f, 0.0f, 64);
    int mid = m.arms[0].count / 2;
    float br_start = m.arms[0].brightnesses[0];
    float br_mid = m.arms[0].brightnesses[mid];
    float br_end = m.arms[0].brightnesses[m.arms[0].count - 1];
    TEST_ASSERT_TRUE(br_mid > br_start);
    TEST_ASSERT_TRUE(br_mid > br_end);
}

/* 19. Different radii produce different geometries */
void test_different_radii(void)
{
    galaxy_mesh_t a = galaxy_generate(50.0f, 27.0f, 0.0f, 16);
    galaxy_mesh_t b = galaxy_generate(100.0f, 27.0f, 0.0f, 16);
    int last = a.arms[0].count - 1;
    float ra = sqrtf(a.arms[0].positions[last * 3] * a.arms[0].positions[last * 3] +
                     a.arms[0].positions[last * 3 + 2] * a.arms[0].positions[last * 3 + 2]);
    float rb = sqrtf(b.arms[0].positions[last * 3] * b.arms[0].positions[last * 3] +
                     b.arms[0].positions[last * 3 + 2] * b.arms[0].positions[last * 3 + 2]);
    TEST_ASSERT_TRUE(rb > ra);
}

/* 20. Purity */
void test_purity(void)
{
    galaxy_mesh_t a = galaxy_generate(50.0f, 27.0f, 45.0f, 32);
    galaxy_mesh_t b = galaxy_generate(50.0f, 27.0f, 45.0f, 32);
    for (int arm = 0; arm < GALAXY_ARM_COUNT; arm++) {
        TEST_ASSERT_EQUAL_INT(a.arms[arm].count, b.arms[arm].count);
        for (int i = 0; i < a.arms[arm].count; i++) {
            TEST_ASSERT_TRUE(a.arms[arm].positions[i * 3] == b.arms[arm].positions[i * 3]);
            TEST_ASSERT_TRUE(a.arms[arm].positions[i * 3 + 2] == b.arms[arm].positions[i * 3 + 2]);
        }
    }
    TEST_ASSERT_TRUE(a.sun_x == b.sun_x);
    TEST_ASSERT_TRUE(a.sun_z == b.sun_z);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_four_arms);
    RUN_TEST(test_points_clamped);
    RUN_TEST(test_points_minimum);
    RUN_TEST(test_y_zero);
    RUN_TEST(test_radius_increases);
    RUN_TEST(test_start_near_center);
    RUN_TEST(test_end_near_radius);
    RUN_TEST(test_sun_distance);
    RUN_TEST(test_sun_y_zero);
    RUN_TEST(test_sun_rotates);
    RUN_TEST(test_arms_offset);
    RUN_TEST(test_pitch_angle);
    RUN_TEST(test_galactic_year);
    RUN_TEST(test_sun_angle_base);
    RUN_TEST(test_sun_angle_rotates);
    RUN_TEST(test_rotation_wraps);
    RUN_TEST(test_brightness_range);
    RUN_TEST(test_brightness_peaks_mid);
    RUN_TEST(test_different_radii);
    RUN_TEST(test_purity);
    return UNITY_END();
}
