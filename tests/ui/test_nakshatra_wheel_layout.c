#include "../unity/unity.h"
#include "../../src/ui/nakshatra_wheel_layout.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* Test values: Moon at ~60 degrees tropical longitude.
 * JD 2460676.5 = 2025-01-01 */
#define TEST_MOON_LON 60.0
#define TEST_JD       2460676.5

/* --- Center and radius --- */

void test_center_x(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_x);
}

void test_center_y(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_y);
}

void test_radius(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.40f, layout.radius);
}

/* --- 27 nakshatras populated --- */

void test_all_27_ids(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.nakshatras[i].id);
    }
}

void test_all_names_not_null(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.nakshatras[i].name);
        TEST_ASSERT_TRUE(strlen(layout.nakshatras[i].name) > 0);
    }
}

void test_all_deities_not_null(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.nakshatras[i].deity);
        TEST_ASSERT_TRUE(strlen(layout.nakshatras[i].deity) > 0);
    }
}

void test_all_qualities_not_null(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.nakshatras[i].quality);
        TEST_ASSERT_TRUE(strlen(layout.nakshatras[i].quality) > 0);
    }
}

void test_all_symbols_not_null(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.nakshatras[i].symbol);
        TEST_ASSERT_TRUE(strlen(layout.nakshatras[i].symbol) > 0);
    }
}

/* --- Angles and arcs --- */

void test_angles_at_correct_intervals(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    float span = 360.0f / 27.0f;
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)i * span,
                                 layout.nakshatras[i].angle_deg);
    }
}

void test_arc_start_matches_angle(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f,
                                 layout.nakshatras[i].angle_deg,
                                 layout.nakshatras[i].arc_start_deg);
    }
}

void test_arc_span_13_333(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        float span = layout.nakshatras[i].arc_end_deg -
                     layout.nakshatras[i].arc_start_deg;
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 360.0f / 27.0f, span);
    }
}

void test_first_arc_starts_at_zero(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
                             layout.nakshatras[0].arc_start_deg);
}

void test_last_arc_ends_at_360(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 360.0f,
                             layout.nakshatras[26].arc_end_deg);
}

/* --- Positions --- */

void test_ashwini_near_top(void)
{
    /* Ashwini (index 0) placed at center of its arc (half span from top) */
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    /* At half-span angle, should be near top */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.50f, layout.nakshatras[0].x);
    TEST_ASSERT_TRUE(layout.nakshatras[0].y < 0.20f);
}

void test_all_positions_normalized(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.nakshatras[i].x >= 0.0f);
        TEST_ASSERT_TRUE(layout.nakshatras[i].x <= 1.0f);
        TEST_ASSERT_TRUE(layout.nakshatras[i].y >= 0.0f);
        TEST_ASSERT_TRUE(layout.nakshatras[i].y <= 1.0f);
    }
}

void test_positions_on_circle(void)
{
    /* All positions should be at distance ~radius from center */
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        float dx = layout.nakshatras[i].x - layout.center_x;
        float dy = layout.nakshatras[i].y - layout.center_y;
        float dist = sqrtf(dx * dx + dy * dy);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, layout.radius, dist);
    }
}

/* --- Highlighting --- */

void test_moon_nakshatra_in_range(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_TRUE(layout.moon_nakshatra >= 0);
    TEST_ASSERT_TRUE(layout.moon_nakshatra < NW_NAKSHATRA_COUNT);
}

void test_exactly_one_highlighted(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    int count = 0;
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        if (layout.nakshatras[i].highlighted) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_highlighted_matches_moon(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    if (layout.moon_nakshatra >= 0) {
        TEST_ASSERT_EQUAL_INT(1,
            layout.nakshatras[layout.moon_nakshatra].highlighted);
    }
}

/* --- No highlighting when jd=0 --- */

void test_no_highlight_when_jd_zero(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, 0.0);
    TEST_ASSERT_EQUAL_INT(-1, layout.moon_nakshatra);
    int count = 0;
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        if (layout.nakshatras[i].highlighted) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_no_highlight_when_negative_lon(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(-1.0, TEST_JD);
    TEST_ASSERT_EQUAL_INT(-1, layout.moon_nakshatra);
}

/* --- Accessor --- */

void test_accessor_valid_index(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    nw_nakshatra_t n = nakshatra_wheel_at(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, n.id);
    TEST_ASSERT_NOT_NULL(n.name);
}

void test_accessor_invalid_index(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    nw_nakshatra_t n = nakshatra_wheel_at(&layout, 27);
    TEST_ASSERT_EQUAL_INT(-1, n.id);
    TEST_ASSERT_EQUAL_STRING("?", n.name);
}

void test_accessor_negative_index(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    nw_nakshatra_t n = nakshatra_wheel_at(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, n.id);
}

void test_accessor_null_layout(void)
{
    nw_nakshatra_t n = nakshatra_wheel_at(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, n.id);
    TEST_ASSERT_EQUAL_STRING("?", n.name);
    TEST_ASSERT_EQUAL_STRING("?", n.deity);
    TEST_ASSERT_EQUAL_STRING("?", n.quality);
    TEST_ASSERT_EQUAL_STRING("?", n.symbol);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    nakshatra_wheel_layout_t a = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    nakshatra_wheel_layout_t b = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.moon_nakshatra, b.moon_nakshatra);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.nakshatras[0].x, b.nakshatras[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.nakshatras[0].y, b.nakshatras[0].y);
}

/* --- First nakshatra name --- */

void test_first_nakshatra_is_ashwini(void)
{
    nakshatra_wheel_layout_t layout = nakshatra_wheel_compute(TEST_MOON_LON, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Ashwini", layout.nakshatras[0].name);
}

int main(void)
{
    UNITY_BEGIN();
    /* Center and radius */
    RUN_TEST(test_center_x);
    RUN_TEST(test_center_y);
    RUN_TEST(test_radius);
    /* Data populated */
    RUN_TEST(test_all_27_ids);
    RUN_TEST(test_all_names_not_null);
    RUN_TEST(test_all_deities_not_null);
    RUN_TEST(test_all_qualities_not_null);
    RUN_TEST(test_all_symbols_not_null);
    /* Angles and arcs */
    RUN_TEST(test_angles_at_correct_intervals);
    RUN_TEST(test_arc_start_matches_angle);
    RUN_TEST(test_arc_span_13_333);
    RUN_TEST(test_first_arc_starts_at_zero);
    RUN_TEST(test_last_arc_ends_at_360);
    /* Positions */
    RUN_TEST(test_ashwini_near_top);
    RUN_TEST(test_all_positions_normalized);
    RUN_TEST(test_positions_on_circle);
    /* Highlighting */
    RUN_TEST(test_moon_nakshatra_in_range);
    RUN_TEST(test_exactly_one_highlighted);
    RUN_TEST(test_highlighted_matches_moon);
    /* No highlighting */
    RUN_TEST(test_no_highlight_when_jd_zero);
    RUN_TEST(test_no_highlight_when_negative_lon);
    /* Accessor */
    RUN_TEST(test_accessor_valid_index);
    RUN_TEST(test_accessor_invalid_index);
    RUN_TEST(test_accessor_negative_index);
    RUN_TEST(test_accessor_null_layout);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    /* Known names */
    RUN_TEST(test_first_nakshatra_is_ashwini);
    return UNITY_END();
}
