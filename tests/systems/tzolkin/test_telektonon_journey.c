#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/telektonon_journey.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Yellow Turtle (Pacal Votan) — tube position = day
 * ================================================================ */

static void test_yellow_day1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_yellow_position(1));
}

static void test_yellow_day6(void)
{
    TEST_ASSERT_EQUAL_INT(6, tk_yellow_position(6));
}

static void test_yellow_day7(void)
{
    TEST_ASSERT_EQUAL_INT(7, tk_yellow_position(7));
}

static void test_yellow_day14(void)
{
    TEST_ASSERT_EQUAL_INT(14, tk_yellow_position(14));
}

static void test_yellow_day22(void)
{
    TEST_ASSERT_EQUAL_INT(22, tk_yellow_position(22));
}

static void test_yellow_day23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_yellow_position(23));
}

static void test_yellow_day28(void)
{
    TEST_ASSERT_EQUAL_INT(28, tk_yellow_position(28));
}

/* Sweep: yellow == day for all 28 */
static void test_yellow_sweep_all_28(void)
{
    for (int d = 1; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(d, tk_yellow_position(d));
    }
}

/* Boundaries */
static void test_yellow_day0_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_yellow_position(0));
}

static void test_yellow_day29_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_yellow_position(29));
}

static void test_yellow_negative_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_yellow_position(-1));
}

static void test_yellow_large_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_yellow_position(100));
}

/* ================================================================
 * White Turtle (Bolon Ik) — descends, holds, then walks with Yellow
 * ================================================================ */

/* Earth Walk: white = 29 - day (descending 28->23) */
static void test_white_day1_is_28(void)
{
    TEST_ASSERT_EQUAL_INT(28, tk_white_position(1));
}

static void test_white_day2_is_27(void)
{
    TEST_ASSERT_EQUAL_INT(27, tk_white_position(2));
}

static void test_white_day3_is_26(void)
{
    TEST_ASSERT_EQUAL_INT(26, tk_white_position(3));
}

static void test_white_day4_is_25(void)
{
    TEST_ASSERT_EQUAL_INT(25, tk_white_position(4));
}

static void test_white_day5_is_24(void)
{
    TEST_ASSERT_EQUAL_INT(24, tk_white_position(5));
}

static void test_white_day6_is_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(6));
}

/* Warrior's Cube: white = 23 (fixed) */
static void test_white_day7_is_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(7));
}

static void test_white_day10_is_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(10));
}

static void test_white_day14_is_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(14));
}

static void test_white_day22_is_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(22));
}

/* Sweep: white == 23 for all cube days */
static void test_white_sweep_cube_all_23(void)
{
    for (int d = 7; d <= 22; d++) {
        TEST_ASSERT_EQUAL_INT(23, tk_white_position(d));
    }
}

/* Heaven Walk: white = day (walking with Yellow) */
static void test_white_day23_is_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(23));
}

static void test_white_day24_is_24(void)
{
    TEST_ASSERT_EQUAL_INT(24, tk_white_position(24));
}

static void test_white_day25_is_25(void)
{
    TEST_ASSERT_EQUAL_INT(25, tk_white_position(25));
}

static void test_white_day26_is_26(void)
{
    TEST_ASSERT_EQUAL_INT(26, tk_white_position(26));
}

static void test_white_day27_is_27(void)
{
    TEST_ASSERT_EQUAL_INT(27, tk_white_position(27));
}

static void test_white_day28_is_28(void)
{
    TEST_ASSERT_EQUAL_INT(28, tk_white_position(28));
}

/* Boundaries */
static void test_white_day0_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_white_position(0));
}

static void test_white_day29_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_white_position(29));
}

static void test_white_negative_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_white_position(-1));
}

/* ================================================================
 * Green Turtle (Warrior) — cube positions 1-16 for days 7-22
 * ================================================================ */

static void test_green_day7_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_green_position(7));
}

static void test_green_day8_is_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, tk_green_position(8));
}

static void test_green_day14_is_8(void)
{
    TEST_ASSERT_EQUAL_INT(8, tk_green_position(14));
}

static void test_green_day22_is_16(void)
{
    TEST_ASSERT_EQUAL_INT(16, tk_green_position(22));
}

/* Sweep: green = day-6 for cube days */
static void test_green_sweep_cube_all_16(void)
{
    for (int d = 7; d <= 22; d++) {
        TEST_ASSERT_EQUAL_INT(d - 6, tk_green_position(d));
    }
}

/* Green resting outside cube */
static void test_green_day1_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(1));
}

static void test_green_day6_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(6));
}

static void test_green_day23_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(23));
}

static void test_green_day28_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(28));
}

/* Sweep: green == 0 for non-cube days */
static void test_green_sweep_non_cube_zero(void)
{
    for (int d = 1; d <= 6; d++) {
        TEST_ASSERT_EQUAL_INT(0, tk_green_position(d));
    }
    for (int d = 23; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(0, tk_green_position(d));
    }
}

/* Boundaries */
static void test_green_day0_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(0));
}

static void test_green_day29_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(29));
}

static void test_green_negative_resting(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(-1));
}

/* ================================================================
 * Reunion — day 23 only
 * ================================================================ */

static void test_reunion_day23_yes(void)
{
    tk_journey_t j = tk_journey_for_day(23);
    TEST_ASSERT_EQUAL_INT(1, j.is_reunion);
}

static void test_reunion_day22_no(void)
{
    tk_journey_t j = tk_journey_for_day(22);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
}

static void test_reunion_day24_no(void)
{
    tk_journey_t j = tk_journey_for_day(24);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
}

static void test_reunion_day1_no(void)
{
    tk_journey_t j = tk_journey_for_day(1);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
}

static void test_reunion_day28_no(void)
{
    tk_journey_t j = tk_journey_for_day(28);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
}

/* Sweep: exactly 1 reunion across all 28 days */
static void test_reunion_sweep_count(void)
{
    int count = 0;
    for (int d = 1; d <= 28; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        count += j.is_reunion;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* ================================================================
 * Lovers walking together — days 23-28
 * ================================================================ */

static void test_lovers_day23_yes(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_lovers_together(23));
}

static void test_lovers_day24_yes(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_lovers_together(24));
}

static void test_lovers_day28_yes(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_lovers_together(28));
}

/* Sweep: lovers together for days 23-28 */
static void test_lovers_sweep_heaven_walk(void)
{
    for (int d = 23; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(1, tk_lovers_together(d));
    }
}

static void test_lovers_day1_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(1));
}

static void test_lovers_day6_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(6));
}

static void test_lovers_day7_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(7));
}

static void test_lovers_day22_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(22));
}

/* Sweep: lovers NOT together for days 1-22 */
static void test_lovers_sweep_not_together(void)
{
    for (int d = 1; d <= 22; d++) {
        TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(d));
    }
}

/* Boundaries */
static void test_lovers_day0_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(0));
}

static void test_lovers_day29_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(29));
}

static void test_lovers_negative_no(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_lovers_together(-1));
}

/* ================================================================
 * Phase names
 * ================================================================ */

static void test_phase_name_day1_earth_walk(void)
{
    tk_journey_t j = tk_journey_for_day(1);
    TEST_ASSERT_EQUAL_STRING("Earth Walk", j.phase_name);
}

static void test_phase_name_day6_earth_walk(void)
{
    tk_journey_t j = tk_journey_for_day(6);
    TEST_ASSERT_EQUAL_STRING("Earth Walk", j.phase_name);
}

static void test_phase_name_day7_warriors_cube(void)
{
    tk_journey_t j = tk_journey_for_day(7);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
}

static void test_phase_name_day14_warriors_cube(void)
{
    tk_journey_t j = tk_journey_for_day(14);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
}

static void test_phase_name_day22_warriors_cube(void)
{
    tk_journey_t j = tk_journey_for_day(22);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
}

static void test_phase_name_day23_heaven_walk(void)
{
    tk_journey_t j = tk_journey_for_day(23);
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", j.phase_name);
}

static void test_phase_name_day28_heaven_walk(void)
{
    tk_journey_t j = tk_journey_for_day(28);
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", j.phase_name);
}

/* Sweep: all earth walk days */
static void test_phase_name_sweep_earth_walk(void)
{
    for (int d = 1; d <= 6; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_STRING("Earth Walk", j.phase_name);
    }
}

/* Sweep: all warrior's cube days */
static void test_phase_name_sweep_warriors_cube(void)
{
    for (int d = 7; d <= 22; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
    }
}

/* Sweep: all heaven walk days */
static void test_phase_name_sweep_heaven_walk(void)
{
    for (int d = 23; d <= 28; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_STRING("Heaven Walk", j.phase_name);
    }
}

/* Invalid day: phase_name is NULL */
static void test_phase_name_day0_null(void)
{
    tk_journey_t j = tk_journey_for_day(0);
    TEST_ASSERT_NULL(j.phase_name);
}

/* ================================================================
 * Full journey struct — spot checks at key days
 * ================================================================ */

/* Day 1: Earth Walk start */
static void test_journey_day1_full(void)
{
    tk_journey_t j = tk_journey_for_day(1);
    TEST_ASSERT_EQUAL_INT(1, j.day);
    TEST_ASSERT_EQUAL_INT(1, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(28, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Earth Walk", j.phase_name);
}

/* Day 6: last Earth Walk day */
static void test_journey_day6_full(void)
{
    tk_journey_t j = tk_journey_for_day(6);
    TEST_ASSERT_EQUAL_INT(6, j.day);
    TEST_ASSERT_EQUAL_INT(6, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(23, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Earth Walk", j.phase_name);
}

/* Day 7: Warrior enters cube */
static void test_journey_day7_full(void)
{
    tk_journey_t j = tk_journey_for_day(7);
    TEST_ASSERT_EQUAL_INT(7, j.day);
    TEST_ASSERT_EQUAL_INT(7, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(23, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(1, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
}

/* Day 14: mid-cube */
static void test_journey_day14_full(void)
{
    tk_journey_t j = tk_journey_for_day(14);
    TEST_ASSERT_EQUAL_INT(14, j.day);
    TEST_ASSERT_EQUAL_INT(14, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(23, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(8, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
}

/* Day 22: last cube day */
static void test_journey_day22_full(void)
{
    tk_journey_t j = tk_journey_for_day(22);
    TEST_ASSERT_EQUAL_INT(22, j.day);
    TEST_ASSERT_EQUAL_INT(22, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(23, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(16, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", j.phase_name);
}

/* Day 23: REUNION */
static void test_journey_day23_full(void)
{
    tk_journey_t j = tk_journey_for_day(23);
    TEST_ASSERT_EQUAL_INT(23, j.day);
    TEST_ASSERT_EQUAL_INT(23, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(23, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(1, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(1, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", j.phase_name);
}

/* Day 24: first day walking together after reunion */
static void test_journey_day24_full(void)
{
    tk_journey_t j = tk_journey_for_day(24);
    TEST_ASSERT_EQUAL_INT(24, j.day);
    TEST_ASSERT_EQUAL_INT(24, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(24, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(1, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", j.phase_name);
}

/* Day 28: EXIT */
static void test_journey_day28_full(void)
{
    tk_journey_t j = tk_journey_for_day(28);
    TEST_ASSERT_EQUAL_INT(28, j.day);
    TEST_ASSERT_EQUAL_INT(28, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(28, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(1, j.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", j.phase_name);
}

/* ================================================================
 * Journey struct — boundary / invalid days
 * ================================================================ */

static void test_journey_day0_zeroed(void)
{
    tk_journey_t j = tk_journey_for_day(0);
    TEST_ASSERT_EQUAL_INT(0, j.day);
    TEST_ASSERT_EQUAL_INT(0, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_NULL(j.phase_name);
}

static void test_journey_day29_zeroed(void)
{
    tk_journey_t j = tk_journey_for_day(29);
    TEST_ASSERT_EQUAL_INT(0, j.day);
    TEST_ASSERT_EQUAL_INT(0, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_NULL(j.phase_name);
}

static void test_journey_negative_zeroed(void)
{
    tk_journey_t j = tk_journey_for_day(-1);
    TEST_ASSERT_EQUAL_INT(0, j.day);
    TEST_ASSERT_EQUAL_INT(0, j.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(0, j.is_reunion);
    TEST_ASSERT_EQUAL_INT(0, j.lovers_walking_together);
    TEST_ASSERT_NULL(j.phase_name);
}

/* ================================================================
 * Purity — same input, same output
 * ================================================================ */

static void test_purity_journey_same_result(void)
{
    tk_journey_t a = tk_journey_for_day(15);
    tk_journey_t b = tk_journey_for_day(15);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
    TEST_ASSERT_EQUAL_INT(a.yellow_tube_pos, b.yellow_tube_pos);
    TEST_ASSERT_EQUAL_INT(a.white_tube_pos, b.white_tube_pos);
    TEST_ASSERT_EQUAL_INT(a.green_cube_pos, b.green_cube_pos);
    TEST_ASSERT_EQUAL_INT(a.is_reunion, b.is_reunion);
    TEST_ASSERT_EQUAL_INT(a.lovers_walking_together, b.lovers_walking_together);
    TEST_ASSERT_EQUAL_STRING(a.phase_name, b.phase_name);
}

static void test_purity_yellow_same_result(void)
{
    TEST_ASSERT_EQUAL_INT(tk_yellow_position(10), tk_yellow_position(10));
}

static void test_purity_white_same_result(void)
{
    TEST_ASSERT_EQUAL_INT(tk_white_position(5), tk_white_position(5));
}

static void test_purity_green_same_result(void)
{
    TEST_ASSERT_EQUAL_INT(tk_green_position(12), tk_green_position(12));
}

/* ================================================================
 * Cross-checks — struct fields match standalone functions
 * ================================================================ */

static void test_journey_yellow_matches_standalone(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_INT(tk_yellow_position(d), j.yellow_tube_pos);
    }
}

static void test_journey_white_matches_standalone(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_INT(tk_white_position(d), j.white_tube_pos);
    }
}

static void test_journey_green_matches_standalone(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_INT(tk_green_position(d), j.green_cube_pos);
    }
}

static void test_journey_lovers_matches_standalone(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_journey_t j = tk_journey_for_day(d);
        TEST_ASSERT_EQUAL_INT(tk_lovers_together(d), j.lovers_walking_together);
    }
}

/* ================================================================
 * Narrative: yellow and white converge at day 23
 * ================================================================ */

static void test_yellow_white_converge_day23(void)
{
    TEST_ASSERT_EQUAL_INT(tk_yellow_position(23), tk_white_position(23));
}

/* Before reunion, yellow < white (days 1-6) or yellow != white (days 7-22) */
static void test_yellow_white_separate_before_reunion(void)
{
    for (int d = 1; d <= 22; d++) {
        int y = tk_yellow_position(d);
        int w = tk_white_position(d);
        TEST_ASSERT_TRUE(y != w);
    }
}

/* During heaven walk, yellow == white */
static void test_yellow_white_equal_heaven_walk(void)
{
    for (int d = 23; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(tk_yellow_position(d), tk_white_position(d));
    }
}

/* ================================================================
 * Narrative: white descends exactly to position 23 by day 6
 * ================================================================ */

static void test_white_reaches_23_on_day6(void)
{
    TEST_ASSERT_EQUAL_INT(23, tk_white_position(6));
}

/* White descends monotonically during Earth Walk */
static void test_white_descends_earth_walk(void)
{
    for (int d = 1; d < 6; d++) {
        int this_pos = tk_white_position(d);
        int next_pos = tk_white_position(d + 1);
        TEST_ASSERT_TRUE(this_pos > next_pos);
    }
}

/* ================================================================
 * Narrative: green enters and exits cube cleanly
 * ================================================================ */

/* Green enters cube at position 1 on day 7 */
static void test_green_enters_cube_day7(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(6));
    TEST_ASSERT_EQUAL_INT(1, tk_green_position(7));
}

/* Green exits cube after position 16 on day 22 */
static void test_green_exits_cube_day22(void)
{
    TEST_ASSERT_EQUAL_INT(16, tk_green_position(22));
    TEST_ASSERT_EQUAL_INT(0, tk_green_position(23));
}

/* Green advances monotonically through cube */
static void test_green_advances_monotonically(void)
{
    for (int d = 7; d < 22; d++) {
        int this_pos = tk_green_position(d);
        int next_pos = tk_green_position(d + 1);
        TEST_ASSERT_EQUAL_INT(this_pos + 1, next_pos);
    }
}

/* ================================================================
 * main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Yellow Turtle (Pacal Votan) */
    RUN_TEST(test_yellow_day1);
    RUN_TEST(test_yellow_day6);
    RUN_TEST(test_yellow_day7);
    RUN_TEST(test_yellow_day14);
    RUN_TEST(test_yellow_day22);
    RUN_TEST(test_yellow_day23);
    RUN_TEST(test_yellow_day28);
    RUN_TEST(test_yellow_sweep_all_28);
    RUN_TEST(test_yellow_day0_invalid);
    RUN_TEST(test_yellow_day29_invalid);
    RUN_TEST(test_yellow_negative_invalid);
    RUN_TEST(test_yellow_large_invalid);

    /* White Turtle (Bolon Ik) — Earth Walk descent */
    RUN_TEST(test_white_day1_is_28);
    RUN_TEST(test_white_day2_is_27);
    RUN_TEST(test_white_day3_is_26);
    RUN_TEST(test_white_day4_is_25);
    RUN_TEST(test_white_day5_is_24);
    RUN_TEST(test_white_day6_is_23);

    /* White Turtle — Warrior's Cube hold */
    RUN_TEST(test_white_day7_is_23);
    RUN_TEST(test_white_day10_is_23);
    RUN_TEST(test_white_day14_is_23);
    RUN_TEST(test_white_day22_is_23);
    RUN_TEST(test_white_sweep_cube_all_23);

    /* White Turtle — Heaven Walk */
    RUN_TEST(test_white_day23_is_23);
    RUN_TEST(test_white_day24_is_24);
    RUN_TEST(test_white_day25_is_25);
    RUN_TEST(test_white_day26_is_26);
    RUN_TEST(test_white_day27_is_27);
    RUN_TEST(test_white_day28_is_28);

    /* White Turtle — boundaries */
    RUN_TEST(test_white_day0_invalid);
    RUN_TEST(test_white_day29_invalid);
    RUN_TEST(test_white_negative_invalid);

    /* Green Turtle (Warrior) — cube */
    RUN_TEST(test_green_day7_is_1);
    RUN_TEST(test_green_day8_is_2);
    RUN_TEST(test_green_day14_is_8);
    RUN_TEST(test_green_day22_is_16);
    RUN_TEST(test_green_sweep_cube_all_16);

    /* Green Turtle — resting */
    RUN_TEST(test_green_day1_resting);
    RUN_TEST(test_green_day6_resting);
    RUN_TEST(test_green_day23_resting);
    RUN_TEST(test_green_day28_resting);
    RUN_TEST(test_green_sweep_non_cube_zero);

    /* Green Turtle — boundaries */
    RUN_TEST(test_green_day0_resting);
    RUN_TEST(test_green_day29_resting);
    RUN_TEST(test_green_negative_resting);

    /* Reunion */
    RUN_TEST(test_reunion_day23_yes);
    RUN_TEST(test_reunion_day22_no);
    RUN_TEST(test_reunion_day24_no);
    RUN_TEST(test_reunion_day1_no);
    RUN_TEST(test_reunion_day28_no);
    RUN_TEST(test_reunion_sweep_count);

    /* Lovers walking together */
    RUN_TEST(test_lovers_day23_yes);
    RUN_TEST(test_lovers_day24_yes);
    RUN_TEST(test_lovers_day28_yes);
    RUN_TEST(test_lovers_sweep_heaven_walk);
    RUN_TEST(test_lovers_day1_no);
    RUN_TEST(test_lovers_day6_no);
    RUN_TEST(test_lovers_day7_no);
    RUN_TEST(test_lovers_day22_no);
    RUN_TEST(test_lovers_sweep_not_together);
    RUN_TEST(test_lovers_day0_no);
    RUN_TEST(test_lovers_day29_no);
    RUN_TEST(test_lovers_negative_no);

    /* Phase names */
    RUN_TEST(test_phase_name_day1_earth_walk);
    RUN_TEST(test_phase_name_day6_earth_walk);
    RUN_TEST(test_phase_name_day7_warriors_cube);
    RUN_TEST(test_phase_name_day14_warriors_cube);
    RUN_TEST(test_phase_name_day22_warriors_cube);
    RUN_TEST(test_phase_name_day23_heaven_walk);
    RUN_TEST(test_phase_name_day28_heaven_walk);
    RUN_TEST(test_phase_name_sweep_earth_walk);
    RUN_TEST(test_phase_name_sweep_warriors_cube);
    RUN_TEST(test_phase_name_sweep_heaven_walk);
    RUN_TEST(test_phase_name_day0_null);

    /* Full journey structs — spot checks */
    RUN_TEST(test_journey_day1_full);
    RUN_TEST(test_journey_day6_full);
    RUN_TEST(test_journey_day7_full);
    RUN_TEST(test_journey_day14_full);
    RUN_TEST(test_journey_day22_full);
    RUN_TEST(test_journey_day23_full);
    RUN_TEST(test_journey_day24_full);
    RUN_TEST(test_journey_day28_full);

    /* Boundary structs */
    RUN_TEST(test_journey_day0_zeroed);
    RUN_TEST(test_journey_day29_zeroed);
    RUN_TEST(test_journey_negative_zeroed);

    /* Purity */
    RUN_TEST(test_purity_journey_same_result);
    RUN_TEST(test_purity_yellow_same_result);
    RUN_TEST(test_purity_white_same_result);
    RUN_TEST(test_purity_green_same_result);

    /* Cross-checks: struct fields match standalone functions */
    RUN_TEST(test_journey_yellow_matches_standalone);
    RUN_TEST(test_journey_white_matches_standalone);
    RUN_TEST(test_journey_green_matches_standalone);
    RUN_TEST(test_journey_lovers_matches_standalone);

    /* Narrative: convergence */
    RUN_TEST(test_yellow_white_converge_day23);
    RUN_TEST(test_yellow_white_separate_before_reunion);
    RUN_TEST(test_yellow_white_equal_heaven_walk);

    /* Narrative: white descent */
    RUN_TEST(test_white_reaches_23_on_day6);
    RUN_TEST(test_white_descends_earth_walk);

    /* Narrative: green cube entry/exit */
    RUN_TEST(test_green_enters_cube_day7);
    RUN_TEST(test_green_exits_cube_day22);
    RUN_TEST(test_green_advances_monotonically);

    return UNITY_END();
}
