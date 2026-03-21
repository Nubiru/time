#include "../unity/unity.h"
#include "../../src/ui/kin_cube_layout.h"
#include "../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Helpers ---
 * July 26, 2026 = Moon 1 Day 1 (13-Moon calendar start).
 * Day 7  = August 1, 2026  (first cube day, position 1)
 * Day 22 = August 16, 2026 (last cube day, position 16)
 * Day 1  = July 26, 2026   (Earth Walk, not in cube)
 * Day 25 = August 19, 2026 (Heaven Walk, not in cube)
 */

static double jd_day7(void)
{
    return gregorian_to_jd(2026, 8, 1.5);  /* Moon 1 Day 7 */
}

static double jd_day22(void)
{
    return gregorian_to_jd(2026, 8, 16.5); /* Moon 1 Day 22 */
}

static double jd_day1(void)
{
    return gregorian_to_jd(2026, 7, 26.5); /* Moon 1 Day 1 */
}

static double jd_day25(void)
{
    return gregorian_to_jd(2026, 8, 19.5); /* Moon 1 Day 25 */
}

/* --- 16 positions --- */

void test_16_positions_total(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    int count = 0;
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        if (c.cells[i].position >= 1 && c.cells[i].position <= 16)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(16, count);
}

/* --- Position 1 --- */

void test_position_1_moon_day_7(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_EQUAL_INT(1, c.cells[0].position);
    TEST_ASSERT_EQUAL_INT(7, c.cells[0].moon_day);
}

void test_position_1_at_origin(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.cells[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.cells[0].y);
}

/* --- Position 4 --- */

void test_position_4_moon_day_10(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_EQUAL_INT(4, c.cells[3].position);
    TEST_ASSERT_EQUAL_INT(10, c.cells[3].moon_day);
}

void test_position_4_at_x_075_y_0(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, c.cells[3].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.cells[3].y);
}

/* --- Position 16 --- */

void test_position_16_moon_day_22(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_EQUAL_INT(16, c.cells[15].position);
    TEST_ASSERT_EQUAL_INT(22, c.cells[15].moon_day);
}

void test_position_16_at_075_075(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, c.cells[15].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, c.cells[15].y);
}

/* --- All powers not NULL --- */

void test_all_powers_not_null(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        TEST_ASSERT_NOT_NULL(c.cells[i].power);
    }
}

/* --- All codon numbers 1-16 --- */

void test_codon_numbers_1_to_16(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, c.cells[i].codon_number);
    }
}

/* --- Day 7: first cube day, active --- */

void test_day7_is_active(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_EQUAL_INT(1, c.is_active);
    TEST_ASSERT_EQUAL_INT(0, c.today_position);
}

/* --- Day 22: last cube day, active --- */

void test_day22_is_active_position_15(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day22());
    TEST_ASSERT_EQUAL_INT(1, c.is_active);
    TEST_ASSERT_EQUAL_INT(15, c.today_position);
}

/* --- Day 1: Earth Walk, not active --- */

void test_day1_not_active(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day1());
    TEST_ASSERT_EQUAL_INT(0, c.is_active);
    TEST_ASSERT_EQUAL_INT(-1, c.today_position);
}

/* --- Day 25: Heaven Walk, not active --- */

void test_day25_not_active(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day25());
    TEST_ASSERT_EQUAL_INT(0, c.is_active);
    TEST_ASSERT_EQUAL_INT(-1, c.today_position);
}

/* --- Only 1 highlighted when active --- */

void test_only_one_highlighted_when_active(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    int count = 0;
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        if (c.cells[i].highlighted) count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- No highlighted when not active --- */

void test_no_highlighted_when_not_active(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day1());
    int count = 0;
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        if (c.cells[i].highlighted) count++;
    }
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* --- Power names --- */

void test_position_1_power_memory(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_EQUAL_STRING("Memory", c.cells[0].power);
}

void test_position_16_power_intelligence(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    TEST_ASSERT_EQUAL_STRING("Intelligence", c.cells[15].power);
}

/* --- Invalid JD --- */

void test_invalid_jd_not_active(void)
{
    kin_cube_layout_t c = kin_cube_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, c.is_active);
    TEST_ASSERT_EQUAL_INT(-1, c.today_position);
}

/* --- Moon days sequential 7-22 --- */

void test_moon_days_sequential(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day7());
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 7, c.cells[i].moon_day);
    }
}

/* --- Day 22 highlights position 16 --- */

void test_day22_highlights_position_16(void)
{
    kin_cube_layout_t c = kin_cube_compute(jd_day22());
    TEST_ASSERT_EQUAL_INT(1, c.cells[15].highlighted);
    /* And position 1 is not highlighted */
    TEST_ASSERT_EQUAL_INT(0, c.cells[0].highlighted);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_16_positions_total);
    RUN_TEST(test_position_1_moon_day_7);
    RUN_TEST(test_position_1_at_origin);
    RUN_TEST(test_position_4_moon_day_10);
    RUN_TEST(test_position_4_at_x_075_y_0);
    RUN_TEST(test_position_16_moon_day_22);
    RUN_TEST(test_position_16_at_075_075);
    RUN_TEST(test_all_powers_not_null);
    RUN_TEST(test_codon_numbers_1_to_16);
    RUN_TEST(test_day7_is_active);
    RUN_TEST(test_day22_is_active_position_15);
    RUN_TEST(test_day1_not_active);
    RUN_TEST(test_day25_not_active);
    RUN_TEST(test_only_one_highlighted_when_active);
    RUN_TEST(test_no_highlighted_when_not_active);
    RUN_TEST(test_position_1_power_memory);
    RUN_TEST(test_position_16_power_intelligence);
    RUN_TEST(test_invalid_jd_not_active);
    RUN_TEST(test_moon_days_sequential);
    RUN_TEST(test_day22_highlights_position_16);
    return UNITY_END();
}
