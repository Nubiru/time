#include "../unity/unity.h"
#include "../../src/ui/zodiac_wheel_layout.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* --- Center and radius --- */

void test_center_x(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_x);
}

void test_center_y(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_y);
}

void test_radius(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.38f, layout.radius);
}

/* --- Sign data --- */

void test_aries_is_sign_0(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_EQUAL_INT(0, layout.signs[0].sign);
    TEST_ASSERT_EQUAL_STRING("Aries", layout.signs[0].name);
}

void test_pisces_is_sign_11(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_EQUAL_INT(11, layout.signs[11].sign);
    TEST_ASSERT_EQUAL_STRING("Pisces", layout.signs[11].name);
}

void test_all_12_signs_populated(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.signs[i].sign);
        TEST_ASSERT_NOT_NULL(layout.signs[i].name);
        TEST_ASSERT_NOT_NULL(layout.signs[i].symbol);
        TEST_ASSERT_TRUE(strlen(layout.signs[i].name) > 0);
    }
}

void test_sign_symbols_not_empty(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(strlen(layout.signs[i].symbol) > 0);
    }
}

/* --- Arc angles --- */

void test_aries_arc(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, layout.signs[0].arc_start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, layout.signs[0].arc_end_deg);
}

void test_taurus_arc(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, layout.signs[1].arc_start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, layout.signs[1].arc_end_deg);
}

void test_pisces_arc(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 330.0f, layout.signs[11].arc_start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 360.0f, layout.signs[11].arc_end_deg);
}

void test_arcs_cover_360(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, layout.signs[0].arc_start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 360.0f, layout.signs[11].arc_end_deg);
}

/* --- Label positions --- */

void test_label_within_bounds(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(layout.signs[i].label_x >= 0.0f);
        TEST_ASSERT_TRUE(layout.signs[i].label_x <= 1.0f);
        TEST_ASSERT_TRUE(layout.signs[i].label_y >= 0.0f);
        TEST_ASSERT_TRUE(layout.signs[i].label_y <= 1.0f);
    }
}

void test_aries_label_at_midpoint_15_degrees(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    /* 15 degrees: x = 0.50 + 0.38*0.75*cos(15deg), y = 0.50 - 0.38*0.75*sin(15deg) */
    float r = 0.38f * 0.75f;
    float expect_x = 0.50f + r * cosf(15.0f * (float)(M_PI / 180.0));
    float expect_y = 0.50f - r * sinf(15.0f * (float)(M_PI / 180.0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expect_x, layout.signs[0].label_x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expect_y, layout.signs[0].label_y);
}

/* --- Sun position: Aries 0 degrees --- */

void test_sun_sign_at_0(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_EQUAL_INT(0, layout.sun_sign);  /* Aries */
}

void test_sun_degree_at_0(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, layout.sun_degree);
}

void test_sun_angle_at_0(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, layout.sun_angle_deg);
}

void test_sun_xy_at_0(void)
{
    /* 0 degrees = rightmost point on wheel */
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f + 0.38f, layout.sun_x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.sun_y);
}

/* --- Sun position: Leo 15 degrees (135 total) --- */

void test_sun_sign_at_135(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(135.0);
    TEST_ASSERT_EQUAL_INT(4, layout.sun_sign);  /* Leo */
}

void test_sun_degree_at_135(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(135.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 15.0f, layout.sun_degree);
}

/* --- Sun position: 90 degrees (top of wheel) --- */

void test_sun_xy_at_90(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(90.0);
    /* 90 degrees = top of wheel (y decreases) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.sun_x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f - 0.38f, layout.sun_y);
}

/* --- Negative longitude normalization --- */

void test_negative_longitude(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(-30.0);
    /* -30 normalizes to 330 = Pisces (sign 11) */
    TEST_ASSERT_EQUAL_INT(11, layout.sun_sign);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 330.0f, layout.sun_angle_deg);
}

/* --- Longitude > 360 --- */

void test_longitude_over_360(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(390.0);
    /* 390 normalizes to 30 = Taurus (sign 1) */
    TEST_ASSERT_EQUAL_INT(1, layout.sun_sign);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 30.0f, layout.sun_angle_deg);
}

/* --- zodiac_wheel_sign_at accessor --- */

void test_sign_at_valid(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    zw_sign_t s = zodiac_wheel_sign_at(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, s.sign);
    TEST_ASSERT_EQUAL_STRING("Aries", s.name);
}

void test_sign_at_last(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    zw_sign_t s = zodiac_wheel_sign_at(&layout, 11);
    TEST_ASSERT_EQUAL_INT(11, s.sign);
    TEST_ASSERT_EQUAL_STRING("Pisces", s.name);
}

void test_sign_at_invalid_negative(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    zw_sign_t s = zodiac_wheel_sign_at(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

void test_sign_at_invalid_12(void)
{
    zodiac_wheel_layout_t layout = zodiac_wheel_compute(0.0);
    zw_sign_t s = zodiac_wheel_sign_at(&layout, 12);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
}

void test_sign_at_null_layout(void)
{
    zw_sign_t s = zodiac_wheel_sign_at(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    zodiac_wheel_layout_t a = zodiac_wheel_compute(42.5);
    zodiac_wheel_layout_t b = zodiac_wheel_compute(42.5);
    TEST_ASSERT_EQUAL_INT(a.sun_sign, b.sun_sign);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.sun_degree, b.sun_degree);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.sun_x, b.sun_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.sun_y, b.sun_y);
    TEST_ASSERT_EQUAL_STRING(a.signs[0].name, b.signs[0].name);
}

void test_purity_different_input_different_sun(void)
{
    zodiac_wheel_layout_t a = zodiac_wheel_compute(0.0);
    zodiac_wheel_layout_t b = zodiac_wheel_compute(180.0);
    TEST_ASSERT_TRUE(a.sun_sign != b.sun_sign);
    TEST_ASSERT_TRUE(fabsf(a.sun_x - b.sun_x) > 0.01f);
}

int main(void)
{
    UNITY_BEGIN();
    /* Center and radius */
    RUN_TEST(test_center_x);
    RUN_TEST(test_center_y);
    RUN_TEST(test_radius);
    /* Sign data */
    RUN_TEST(test_aries_is_sign_0);
    RUN_TEST(test_pisces_is_sign_11);
    RUN_TEST(test_all_12_signs_populated);
    RUN_TEST(test_sign_symbols_not_empty);
    /* Arc angles */
    RUN_TEST(test_aries_arc);
    RUN_TEST(test_taurus_arc);
    RUN_TEST(test_pisces_arc);
    RUN_TEST(test_arcs_cover_360);
    /* Label positions */
    RUN_TEST(test_label_within_bounds);
    RUN_TEST(test_aries_label_at_midpoint_15_degrees);
    /* Sun position: 0 degrees */
    RUN_TEST(test_sun_sign_at_0);
    RUN_TEST(test_sun_degree_at_0);
    RUN_TEST(test_sun_angle_at_0);
    RUN_TEST(test_sun_xy_at_0);
    /* Sun position: 135 degrees */
    RUN_TEST(test_sun_sign_at_135);
    RUN_TEST(test_sun_degree_at_135);
    /* Sun position: 90 degrees */
    RUN_TEST(test_sun_xy_at_90);
    /* Normalization */
    RUN_TEST(test_negative_longitude);
    RUN_TEST(test_longitude_over_360);
    /* Accessor */
    RUN_TEST(test_sign_at_valid);
    RUN_TEST(test_sign_at_last);
    RUN_TEST(test_sign_at_invalid_negative);
    RUN_TEST(test_sign_at_invalid_12);
    RUN_TEST(test_sign_at_null_layout);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_input_different_sun);
    return UNITY_END();
}
