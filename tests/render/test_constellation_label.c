/*
 * test_constellation_label.c — Tests for constellation name label positions
 */

#include "../unity/unity.h"
#include "../../src/render/constellation_label.h"

#include <math.h>
#include <string.h>

#define SPHERE_R 100.0f

void setUp(void) {}
void tearDown(void) {}

/* 1. cl_compute returns 88 labels */
void test_compute_returns_88(void)
{
    cl_label_t labels[CL_COUNT];
    int count = cl_compute(SPHERE_R, labels);
    TEST_ASSERT_EQUAL_INT(88, count);
}

/* 2. All labels have non-null names */
void test_all_have_names(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    for (int i = 0; i < CL_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(labels[i].name);
        TEST_ASSERT_TRUE(strlen(labels[i].name) > 0);
    }
}

/* 3. All labels have non-null abbreviations */
void test_all_have_abbr(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    for (int i = 0; i < CL_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(labels[i].abbr);
        TEST_ASSERT_EQUAL_INT(3, (int)strlen(labels[i].abbr));
    }
}

/* 4. All positions lie on celestial sphere */
void test_positions_on_sphere(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    for (int i = 0; i < CL_COUNT; i++) {
        float mag = sqrtf(labels[i].x * labels[i].x +
                          labels[i].y * labels[i].y +
                          labels[i].z * labels[i].z);
        TEST_ASSERT_FLOAT_WITHIN(2.0f, SPHERE_R, mag);
    }
}

/* 5. RA is in [0, 24) hours */
void test_ra_range(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    for (int i = 0; i < CL_COUNT; i++) {
        TEST_ASSERT_TRUE(labels[i].ra_hours >= 0.0f);
        TEST_ASSERT_TRUE(labels[i].ra_hours < 24.0f);
    }
}

/* 6. Dec is in [-90, 90] degrees */
void test_dec_range(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    for (int i = 0; i < CL_COUNT; i++) {
        TEST_ASSERT_TRUE(labels[i].dec_deg >= -90.0f);
        TEST_ASSERT_TRUE(labels[i].dec_deg <= 90.0f);
    }
}

/* 7. cl_get returns valid label for index 0 */
void test_get_valid(void)
{
    cl_label_t label = cl_get(0, SPHERE_R);
    TEST_ASSERT_NOT_NULL(label.name);
}

/* 8. cl_get returns empty for invalid index */
void test_get_invalid(void)
{
    cl_label_t label = cl_get(999, SPHERE_R);
    TEST_ASSERT_NULL(label.name);
}

/* 9. cl_compute with NULL returns 0 */
void test_compute_null(void)
{
    int count = cl_compute(SPHERE_R, NULL);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* 10. Some constellations are zodiac */
void test_some_zodiac(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    int zodiac_count = 0;
    for (int i = 0; i < CL_COUNT; i++) {
        if (labels[i].is_zodiac) zodiac_count++;
    }
    /* Should be ~12-13 zodiac constellations */
    TEST_ASSERT_TRUE(zodiac_count >= 12);
    TEST_ASSERT_TRUE(zodiac_count <= 14);
}

/* 11. Different sphere radius scales positions */
void test_radius_scales(void)
{
    cl_label_t l1 = cl_get(0, 50.0f);
    cl_label_t l2 = cl_get(0, 200.0f);
    float m1 = sqrtf(l1.x * l1.x + l1.y * l1.y + l1.z * l1.z);
    float m2 = sqrtf(l2.x * l2.x + l2.y * l2.y + l2.z * l2.z);
    TEST_ASSERT_TRUE(m2 > m1 * 3.0f);
}

/* 12. Orion centroid roughly in expected location */
void test_orion_position(void)
{
    cl_label_t labels[CL_COUNT];
    cl_compute(SPHERE_R, labels);
    /* Find Orion */
    int orion = -1;
    for (int i = 0; i < CL_COUNT; i++) {
        if (strcmp(labels[i].abbr, "Ori") == 0) {
            orion = i;
            break;
        }
    }
    TEST_ASSERT_TRUE(orion >= 0);
    /* Orion centroid: roughly RA 5.5h, Dec 0° */
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 5.5f, labels[orion].ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(15.0f, 0.0f, labels[orion].dec_deg);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_compute_returns_88);
    RUN_TEST(test_all_have_names);
    RUN_TEST(test_all_have_abbr);
    RUN_TEST(test_positions_on_sphere);
    RUN_TEST(test_ra_range);
    RUN_TEST(test_dec_range);
    RUN_TEST(test_get_valid);
    RUN_TEST(test_get_invalid);
    RUN_TEST(test_compute_null);
    RUN_TEST(test_some_zodiac);
    RUN_TEST(test_radius_scales);
    RUN_TEST(test_orion_position);
    return UNITY_END();
}
