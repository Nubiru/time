#include "../unity/unity.h"
#include "../../src/ui/kin_moon_schematic.h"
#include "../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Helper: July 26, 2026 = first day of 13-Moon year --- */

static double jd_jul_26_2026(void)
{
    return gregorian_to_jd(2026, 7, 26.5);
}

/* --- July 26 is Moon 1, Day 1 --- */

void test_jul26_2026_current_moon_1(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(1, s.current_moon);
}

void test_jul26_2026_current_day_1(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(1, s.current_day);
}

/* --- All 13 moons have valid names --- */

void test_all_moons_have_names(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    for (int i = 0; i < KIN_SCHEMATIC_MOONS; i++) {
        TEST_ASSERT_NOT_NULL(s.moons[i].name);
        TEST_ASSERT_TRUE(strlen(s.moons[i].name) > 0);
    }
}

/* --- Moon numbers 1-13 sequential --- */

void test_moon_numbers_sequential(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    for (int i = 0; i < KIN_SCHEMATIC_MOONS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, s.moons[i].moon_number);
    }
}

/* --- Moon 13 at bottom center (0.5, 0.85) --- */

void test_moon_13_at_bottom_center(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, s.moons[12].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, s.moons[12].y);
}

/* --- Only 1 moon highlighted (the current one) --- */

void test_only_current_moon_highlighted(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    int count = 0;
    for (int i = 0; i < KIN_SCHEMATIC_MOONS; i++) {
        if (s.moons[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
    /* The highlighted one should be the current moon */
    TEST_ASSERT_EQUAL_INT(1, s.moons[s.current_moon - 1].highlighted);
}

/* --- start_day_of_year: moon 1=0, moon 2=28, moon 13=336 --- */

void test_start_day_of_year_moon_1(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(0, s.moons[0].start_day_of_year);
}

void test_start_day_of_year_moon_2(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(28, s.moons[1].start_day_of_year);
}

void test_start_day_of_year_moon_13(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(336, s.moons[12].start_day_of_year);
}

/* --- Moons 1-4 on top row (y ~ 0.1) --- */

void test_moons_1_to_4_top_row(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, s.moons[i].y);
    }
}

/* --- Moons 5-8 on second row (y ~ 0.35) --- */

void test_moons_5_to_8_second_row(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    for (int i = 4; i < 8; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.35f, s.moons[i].y);
    }
}

/* --- Moons 9-12 on third row (y ~ 0.6) --- */

void test_moons_9_to_12_third_row(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    for (int i = 8; i < 12; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.6f, s.moons[i].y);
    }
}

/* --- Day Out of Time position at (0.9, 0.85) --- */

void test_day_out_of_time_position(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.9f, s.day_out_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, s.day_out_y);
}

/* --- Different dates give different current_moon --- */

void test_different_date_different_moon(void)
{
    /* Moon 1 starts Jul 26; Moon 2 starts Aug 23 */
    double jd1 = gregorian_to_jd(2026, 7, 26.5);
    double jd2 = gregorian_to_jd(2026, 8, 23.5);
    kin_moon_schematic_t s1 = kin_moon_schematic_compute(jd1);
    kin_moon_schematic_t s2 = kin_moon_schematic_compute(jd2);
    TEST_ASSERT_EQUAL_INT(1, s1.current_moon);
    TEST_ASSERT_EQUAL_INT(2, s2.current_moon);
}

/* --- Year is populated --- */

void test_year_populated(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(s.year > 0);
}

/* --- Invalid JD returns zeroed --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, s.current_moon);
    TEST_ASSERT_EQUAL_INT(0, s.current_day);
    TEST_ASSERT_EQUAL_INT(0, s.year);
    for (int i = 0; i < KIN_SCHEMATIC_MOONS; i++) {
        TEST_ASSERT_EQUAL_INT(0, s.moons[i].moon_number);
    }
}

/* --- current_day in range 1-28 for regular dates --- */

void test_current_day_in_range(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(s.current_day >= 1 && s.current_day <= 28);
}

/* --- Day Out of Time: current_moon = 0 --- */

void test_day_out_of_time_current_moon_zero(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.5);
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, s.current_moon);
}

/* --- Moon 1 name is "Magnetic" --- */

void test_moon_1_name_magnetic(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_STRING("Magnetic", s.moons[0].name);
}

/* --- X positions increase across columns --- */

void test_x_positions_increase_across_columns(void)
{
    kin_moon_schematic_t s = kin_moon_schematic_compute(jd_jul_26_2026());
    /* First row: moons 1-4 should have increasing x */
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_TRUE(s.moons[i + 1].x > s.moons[i].x);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_jul26_2026_current_moon_1);
    RUN_TEST(test_jul26_2026_current_day_1);
    RUN_TEST(test_all_moons_have_names);
    RUN_TEST(test_moon_numbers_sequential);
    RUN_TEST(test_moon_13_at_bottom_center);
    RUN_TEST(test_only_current_moon_highlighted);
    RUN_TEST(test_start_day_of_year_moon_1);
    RUN_TEST(test_start_day_of_year_moon_2);
    RUN_TEST(test_start_day_of_year_moon_13);
    RUN_TEST(test_moons_1_to_4_top_row);
    RUN_TEST(test_moons_5_to_8_second_row);
    RUN_TEST(test_moons_9_to_12_third_row);
    RUN_TEST(test_day_out_of_time_position);
    RUN_TEST(test_different_date_different_moon);
    RUN_TEST(test_year_populated);
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_current_day_in_range);
    RUN_TEST(test_day_out_of_time_current_moon_zero);
    RUN_TEST(test_moon_1_name_magnetic);
    RUN_TEST(test_x_positions_increase_across_columns);
    return UNITY_END();
}
