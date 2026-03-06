#include "../unity/unity.h"
#include "../../src/render/cusp_lines.h"

#include <math.h>

#define PI 3.14159265358979323846
#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* Whole-sign cusps starting at 0 degrees (Aries ASC) */
static void make_cusps(double *cusps)
{
    for (int i = 0; i < 12; i++)
        cusps[i] = (double)(i * 30);
}

/* 1. Generate 12 lines */
void test_count(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    TEST_ASSERT_EQUAL_INT(12, set.count);
}

/* 2. Inner point at correct radius */
void test_inner_radius(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    for (int i = 0; i < 12; i++) {
        float r = sqrtf(set.lines[i].x1 * set.lines[i].x1 +
                        set.lines[i].z1 * set.lines[i].z1);
        TEST_ASSERT_TRUE(NEAR(5.0f, r));
    }
}

/* 3. Outer point at correct radius */
void test_outer_radius(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    for (int i = 0; i < 12; i++) {
        float r = sqrtf(set.lines[i].x2 * set.lines[i].x2 +
                        set.lines[i].z2 * set.lines[i].z2);
        TEST_ASSERT_TRUE(NEAR(8.0f, r));
    }
}

/* 4. All y coordinates zero (ecliptic plane) */
void test_y_zero(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(set.lines[i].y1 == 0.0f);
        TEST_ASSERT_TRUE(set.lines[i].y2 == 0.0f);
    }
}

/* 5. First cusp (0 degrees) at +X axis */
void test_first_cusp_position(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    TEST_ASSERT_TRUE(NEAR(5.0f, set.lines[0].x1));
    TEST_ASSERT_TRUE(NEAR(0.0f, set.lines[0].z1));
    TEST_ASSERT_TRUE(NEAR(8.0f, set.lines[0].x2));
    TEST_ASSERT_TRUE(NEAR(0.0f, set.lines[0].z2));
}

/* 6. Cusp at 90 degrees → +Z axis */
void test_cusp_90_degrees(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    /* House 3 = 90 degrees */
    TEST_ASSERT_TRUE(NEAR(0.0f, set.lines[3].x1));
    TEST_ASSERT_TRUE(NEAR(5.0f, set.lines[3].z1));
}

/* 7. Cusp at 180 degrees → -X axis */
void test_cusp_180_degrees(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    /* House 6 = 180 degrees */
    TEST_ASSERT_TRUE(NEAR(-5.0f, set.lines[6].x1));
    TEST_ASSERT_TRUE(NEAR(0.0f, set.lines[6].z1));
}

/* 8. House indices correct */
void test_house_indices(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    for (int i = 0; i < 12; i++)
        TEST_ASSERT_EQUAL_INT(i, set.lines[i].house_index);
}

/* 9. ASC (house 0) is angular */
void test_angular_asc(void)
{
    TEST_ASSERT_EQUAL_INT(1, cusp_is_angular(0));
}

/* 10. IC (house 3) is angular */
void test_angular_ic(void)
{
    TEST_ASSERT_EQUAL_INT(1, cusp_is_angular(3));
}

/* 11. DSC (house 6) is angular */
void test_angular_dsc(void)
{
    TEST_ASSERT_EQUAL_INT(1, cusp_is_angular(6));
}

/* 12. MC (house 9) is angular */
void test_angular_mc(void)
{
    TEST_ASSERT_EQUAL_INT(1, cusp_is_angular(9));
}

/* 13. Non-angular houses */
void test_not_angular(void)
{
    TEST_ASSERT_EQUAL_INT(0, cusp_is_angular(1));
    TEST_ASSERT_EQUAL_INT(0, cusp_is_angular(2));
    TEST_ASSERT_EQUAL_INT(0, cusp_is_angular(5));
    TEST_ASSERT_EQUAL_INT(0, cusp_is_angular(11));
}

/* 14. Angular cusps have thickness 2.0 */
void test_angular_thickness(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    TEST_ASSERT_TRUE(NEAR(2.0f, set.lines[0].thickness));
    TEST_ASSERT_TRUE(NEAR(2.0f, set.lines[3].thickness));
    TEST_ASSERT_TRUE(NEAR(2.0f, set.lines[6].thickness));
    TEST_ASSERT_TRUE(NEAR(2.0f, set.lines[9].thickness));
}

/* 15. Non-angular cusps have thickness 1.0 */
void test_normal_thickness(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    TEST_ASSERT_TRUE(NEAR(1.0f, set.lines[1].thickness));
    TEST_ASSERT_TRUE(NEAR(1.0f, set.lines[4].thickness));
    TEST_ASSERT_TRUE(NEAR(1.0f, set.lines[7].thickness));
}

/* 16. cusp_axis_name for angular cusps */
void test_axis_names(void)
{
    TEST_ASSERT_NOT_NULL(cusp_axis_name(0));
    TEST_ASSERT_NOT_NULL(cusp_axis_name(3));
    TEST_ASSERT_NOT_NULL(cusp_axis_name(6));
    TEST_ASSERT_NOT_NULL(cusp_axis_name(9));
}

/* 17. cusp_axis_name returns NULL for non-angular */
void test_axis_name_null(void)
{
    TEST_ASSERT_NULL(cusp_axis_name(1));
    TEST_ASSERT_NULL(cusp_axis_name(5));
    TEST_ASSERT_NULL(cusp_axis_name(11));
}

/* 18. Axis name values correct */
void test_axis_name_values(void)
{
    TEST_ASSERT_EQUAL_STRING("ASC", cusp_axis_name(0));
    TEST_ASSERT_EQUAL_STRING("IC", cusp_axis_name(3));
    TEST_ASSERT_EQUAL_STRING("DSC", cusp_axis_name(6));
    TEST_ASSERT_EQUAL_STRING("MC", cusp_axis_name(9));
}

/* 19. Non-standard cusps (not evenly spaced) */
void test_custom_cusps(void)
{
    double cusps[12] = {15.0, 45.0, 75.0, 105.0, 135.0, 165.0,
                        195.0, 225.0, 255.0, 285.0, 315.0, 345.0};
    cusp_line_set_t set = cusp_lines_generate(cusps, 3.0f, 6.0f);
    TEST_ASSERT_EQUAL_INT(12, set.count);
    /* First cusp at 15 degrees: inner x = 3*cos(15°) ≈ 2.898 */
    float expected_x = 3.0f * cosf((float)(15.0 * PI / 180.0));
    TEST_ASSERT_TRUE(NEAR(expected_x, set.lines[0].x1));
}

/* 20. is_angular flag set correctly in generated lines */
void test_generated_angular_flag(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t set = cusp_lines_generate(cusps, 5.0f, 8.0f);
    TEST_ASSERT_EQUAL_INT(1, set.lines[0].is_angular);
    TEST_ASSERT_EQUAL_INT(0, set.lines[1].is_angular);
    TEST_ASSERT_EQUAL_INT(1, set.lines[3].is_angular);
    TEST_ASSERT_EQUAL_INT(0, set.lines[4].is_angular);
}

/* 21. Purity */
void test_purity(void)
{
    double cusps[12];
    make_cusps(cusps);
    cusp_line_set_t a = cusp_lines_generate(cusps, 5.0f, 8.0f);
    cusp_line_set_t b = cusp_lines_generate(cusps, 5.0f, 8.0f);
    TEST_ASSERT_EQUAL_INT(a.count, b.count);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_TRUE(a.lines[i].x1 == b.lines[i].x1);
        TEST_ASSERT_TRUE(a.lines[i].z1 == b.lines[i].z1);
        TEST_ASSERT_TRUE(a.lines[i].x2 == b.lines[i].x2);
        TEST_ASSERT_TRUE(a.lines[i].z2 == b.lines[i].z2);
    }
}

/* 22. Null cusps returns empty set */
void test_null_cusps(void)
{
    cusp_line_set_t set = cusp_lines_generate(NULL, 5.0f, 8.0f);
    TEST_ASSERT_EQUAL_INT(0, set.count);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_count);
    RUN_TEST(test_inner_radius);
    RUN_TEST(test_outer_radius);
    RUN_TEST(test_y_zero);
    RUN_TEST(test_first_cusp_position);
    RUN_TEST(test_cusp_90_degrees);
    RUN_TEST(test_cusp_180_degrees);
    RUN_TEST(test_house_indices);
    RUN_TEST(test_angular_asc);
    RUN_TEST(test_angular_ic);
    RUN_TEST(test_angular_dsc);
    RUN_TEST(test_angular_mc);
    RUN_TEST(test_not_angular);
    RUN_TEST(test_angular_thickness);
    RUN_TEST(test_normal_thickness);
    RUN_TEST(test_axis_names);
    RUN_TEST(test_axis_name_null);
    RUN_TEST(test_axis_name_values);
    RUN_TEST(test_custom_cusps);
    RUN_TEST(test_generated_angular_flag);
    RUN_TEST(test_purity);
    RUN_TEST(test_null_cusps);
    return UNITY_END();
}
