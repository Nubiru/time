#include "../unity/unity.h"
#include "../../src/ui/kin_telektonon_layout.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- July 26, 2026: Moon 1 Day 1 (first day of moon) --- */

void test_jul26_today_day_1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.today_day);
}

void test_jul26_phase_earth_walk(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.today_phase); /* TK_PHASE_EARTH_WALK */
}

/* --- 28 days with valid day numbers --- */

void test_28_days_valid_numbers(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 0; i < KIN_TK_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, layout.days[i].day);
    }
}

/* --- Phase assignments --- */

void test_days_1_6_earth_walk(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(1, layout.days[i].phase); /* EARTH_WALK */
    }
}

void test_days_7_22_warriors_cube(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 6; i < 22; i++) {
        TEST_ASSERT_EQUAL_INT(2, layout.days[i].phase); /* WARRIORS_CUBE */
    }
}

void test_days_23_28_heaven_walk(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 22; i < 28; i++) {
        TEST_ASSERT_EQUAL_INT(3, layout.days[i].phase); /* HEAVEN_WALK */
    }
}

/* --- Tower days --- */

void test_tower_days(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.days[0].is_tower);   /* day 1 */
    TEST_ASSERT_EQUAL_INT(1, layout.days[5].is_tower);   /* day 6 */
    TEST_ASSERT_EQUAL_INT(1, layout.days[22].is_tower);  /* day 23 */
    TEST_ASSERT_EQUAL_INT(1, layout.days[27].is_tower);  /* day 28 */
    /* non-tower day */
    TEST_ASSERT_EQUAL_INT(0, layout.days[1].is_tower);   /* day 2 */
}

/* --- Reunion day --- */

void test_reunion_day_23(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.days[22].is_reunion); /* day 23 */
    TEST_ASSERT_EQUAL_INT(0, layout.days[21].is_reunion); /* day 22 */
}

/* --- Cube positions --- */

void test_cube_positions_7_22(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 6; i < 22; i++) {
        TEST_ASSERT_EQUAL_INT(i - 5, layout.days[i].cube_position);
    }
}

/* --- Only 1 day highlighted --- */

void test_only_one_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    int count = 0;
    for (int i = 0; i < KIN_TK_DAYS; i++) {
        if (layout.days[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Earth Walk y position --- */

void test_earth_walk_y_bottom(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.85f, layout.days[i].y);
    }
}

/* --- Heaven Walk y position --- */

void test_heaven_walk_y_top(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 22; i < 28; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, layout.days[i].y);
    }
}

/* --- Cube grid in center area --- */

void test_cube_grid_center_area(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    for (int i = 6; i < 22; i++) {
        TEST_ASSERT_TRUE(layout.days[i].x >= 0.15f && layout.days[i].x <= 0.85f);
        TEST_ASSERT_TRUE(layout.days[i].y >= 0.25f && layout.days[i].y <= 0.80f);
    }
}

/* --- today_phase matches today_day --- */

void test_today_phase_matches_day(void)
{
    /* Day 1 -> Earth Walk (phase 1) */
    double jd1 = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t l1 = kin_telektonon_compute(jd1);
    TEST_ASSERT_EQUAL_INT(1, l1.today_phase);

    /* Day 10 -> Warrior's Cube (phase 2) */
    double jd2 = gregorian_to_jd(2026, 8, 4.0); /* July 26 + 9 = Aug 4 */
    kin_telektonon_layout_t l2 = kin_telektonon_compute(jd2);
    TEST_ASSERT_EQUAL_INT(2, l2.today_phase);

    /* Day 25 -> Heaven Walk (phase 3) */
    double jd3 = gregorian_to_jd(2026, 8, 19.0); /* July 26 + 24 = Aug 19 */
    kin_telektonon_layout_t l3 = kin_telektonon_compute(jd3);
    TEST_ASSERT_EQUAL_INT(3, l3.today_phase);
}

/* --- phase_name not NULL --- */

void test_phase_name_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_NOT_NULL(layout.phase_name);
}

void test_phase_name_earth_walk(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_STRING("Earth Walk", layout.phase_name);
}

/* --- Invalid JD returns zeroed --- */

void test_invalid_jd_negative(void)
{
    kin_telektonon_layout_t layout = kin_telektonon_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, layout.today_day);
    TEST_ASSERT_NULL(layout.phase_name);
}

/* --- Day Out of Time returns zeroed --- */

void test_day_out_of_time_zeroed(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, layout.today_day);
    TEST_ASSERT_NULL(layout.phase_name);
}

/* --- Cube day field --- */

void test_cube_day_in_cube_phase(void)
{
    /* Day 10 -> cube_position 4 */
    double jd = gregorian_to_jd(2026, 8, 4.0); /* day 10 of Moon 1 */
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(4, layout.cube_day);
}

void test_cube_day_outside_cube_phase(void)
{
    /* Day 1 -> cube_day = 0 */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_telektonon_layout_t layout = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, layout.cube_day);
}

/* --- Purity: same input same output --- */

void test_purity_same_input_same_output(void)
{
    double jd = gregorian_to_jd(2026, 8, 10.0);
    kin_telektonon_layout_t l1 = kin_telektonon_compute(jd);
    kin_telektonon_layout_t l2 = kin_telektonon_compute(jd);
    TEST_ASSERT_EQUAL_INT(l1.today_day, l2.today_day);
    TEST_ASSERT_EQUAL_INT(l1.today_phase, l2.today_phase);
    for (int i = 0; i < KIN_TK_DAYS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, l1.days[i].x, l2.days[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, l1.days[i].y, l2.days[i].y);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_jul26_today_day_1);
    RUN_TEST(test_jul26_phase_earth_walk);
    RUN_TEST(test_28_days_valid_numbers);
    RUN_TEST(test_days_1_6_earth_walk);
    RUN_TEST(test_days_7_22_warriors_cube);
    RUN_TEST(test_days_23_28_heaven_walk);
    RUN_TEST(test_tower_days);
    RUN_TEST(test_reunion_day_23);
    RUN_TEST(test_cube_positions_7_22);
    RUN_TEST(test_only_one_highlighted);
    RUN_TEST(test_earth_walk_y_bottom);
    RUN_TEST(test_heaven_walk_y_top);
    RUN_TEST(test_cube_grid_center_area);
    RUN_TEST(test_today_phase_matches_day);
    RUN_TEST(test_phase_name_not_null);
    RUN_TEST(test_phase_name_earth_walk);
    RUN_TEST(test_invalid_jd_negative);
    RUN_TEST(test_day_out_of_time_zeroed);
    RUN_TEST(test_cube_day_in_cube_phase);
    RUN_TEST(test_cube_day_outside_cube_phase);
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
