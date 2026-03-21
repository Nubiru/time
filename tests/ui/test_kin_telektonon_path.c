#include "../unity/unity.h"
#include "../../src/ui/kin_telektonon_path.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- July 26, 2026: Moon 1 Day 1 --- */

void test_jul26_today_day_1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.today_day);
}

void test_jul26_phase_earth_walk(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.today_phase); /* TK_PHASE_EARTH_WALK */
}

/* --- 28 days total --- */

void test_28_days_total(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 0; i < KIN_TK_PATH_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, layout.days[i].day);
    }
}

/* --- Day 1: yellow_pos=1, phase_name not NULL --- */

void test_day1_yellow_pos(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.days[0].yellow_pos);
}

void test_day1_phase_name_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_NOT_NULL(layout.days[0].phase_name);
}

/* --- Day 23: is_reunion=1, lovers_together=1 --- */

void test_day23_reunion(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.days[22].is_reunion);
    TEST_ASSERT_EQUAL_INT(1, layout.days[22].lovers_together);
}

/* --- Days 23-28: lovers_together=1 --- */

void test_days_23_28_lovers_together(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 22; i < 28; i++) {
        TEST_ASSERT_EQUAL_INT(1, layout.days[i].lovers_together);
    }
}

/* --- Days 7-22: green_pos 1-16 (in cube) --- */

void test_days_7_22_green_pos_in_cube(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 6; i < 22; i++) {
        TEST_ASSERT_EQUAL_INT(i - 5, layout.days[i].green_pos);
    }
}

/* --- Days 1-6: green_pos=0 (resting) --- */

void test_days_1_6_green_pos_resting(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.days[i].green_pos);
    }
}

/* --- Tower days (1, 6, 23, 28) --- */

void test_tower_days_marked(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, layout.days[0].is_tower);   /* day 1 */
    TEST_ASSERT_EQUAL_INT(1, layout.days[5].is_tower);   /* day 6 */
    TEST_ASSERT_EQUAL_INT(1, layout.days[22].is_tower);  /* day 23 */
    TEST_ASSERT_EQUAL_INT(1, layout.days[27].is_tower);  /* day 28 */
    /* non-tower day */
    TEST_ASSERT_EQUAL_INT(0, layout.days[1].is_tower);   /* day 2 */
    TEST_ASSERT_EQUAL_INT(0, layout.days[10].is_tower);  /* day 11 */
}

/* --- Only 1 day highlighted --- */

void test_only_one_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    int count = 0;
    for (int i = 0; i < KIN_TK_PATH_DAYS; i++) {
        if (layout.days[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Earth Walk y ~ 0.90 --- */

void test_earth_walk_y(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.90f, layout.days[i].y);
    }
}

/* --- Heaven Walk y ~ 0.10 --- */

void test_heaven_walk_y(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 22; i < 28; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.10f, layout.days[i].y);
    }
}

/* --- Cube rows: day 7-10 y ~ 0.70, day 19-22 y ~ 0.25 --- */

void test_cube_row_0_y(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 6; i < 10; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.70f, layout.days[i].y);
    }
}

void test_cube_row_3_y(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 18; i < 22; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.25f, layout.days[i].y);
    }
}

/* --- Boustrophedon: odd rows reverse x --- */

void test_cube_boustrophedon_row1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    /* Row 1 (days 11-14) should go right to left */
    /* Day 11 (idx 10): col=(11-7)%4=0 -> reversed to col=3 -> x = 0.10+3*0.27 = 0.91 */
    /* Day 14 (idx 13): col=(14-7)%4=3 -> reversed to col=0 -> x = 0.10 */
    TEST_ASSERT_TRUE(layout.days[10].x > layout.days[13].x);
}

/* --- Invalid JD returns zeroed --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_tk_path_layout_t layout = kin_tk_path_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, layout.today_day);
    TEST_ASSERT_EQUAL_INT(0, layout.today_phase);
    for (int i = 0; i < KIN_TK_PATH_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.days[i].day);
    }
}

/* --- Day Out of Time returns zeroed --- */

void test_day_out_of_time_returns_zeroed(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, layout.today_day);
    TEST_ASSERT_EQUAL_INT(0, layout.today_phase);
}

/* --- Cube positions 1-16 for days 7-22 --- */

void test_cube_positions_7_22(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 6; i < 22; i++) {
        TEST_ASSERT_EQUAL_INT(i - 5, layout.days[i].cube_position);
    }
}

/* --- Cube positions 0 for non-cube days --- */

void test_cube_positions_0_outside_cube(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.days[i].cube_position);
    }
    for (int i = 22; i < 28; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.days[i].cube_position);
    }
}

/* --- Purity: same input same output --- */

void test_purity(void)
{
    double jd = gregorian_to_jd(2026, 8, 10.0);
    kin_tk_path_layout_t l1 = kin_tk_path_compute(jd);
    kin_tk_path_layout_t l2 = kin_tk_path_compute(jd);
    TEST_ASSERT_EQUAL_INT(l1.today_day, l2.today_day);
    TEST_ASSERT_EQUAL_INT(l1.today_phase, l2.today_phase);
    for (int i = 0; i < KIN_TK_PATH_DAYS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, l1.days[i].x, l2.days[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, l1.days[i].y, l2.days[i].y);
        TEST_ASSERT_EQUAL_INT(l1.days[i].yellow_pos, l2.days[i].yellow_pos);
        TEST_ASSERT_EQUAL_INT(l1.days[i].white_pos, l2.days[i].white_pos);
        TEST_ASSERT_EQUAL_INT(l1.days[i].green_pos, l2.days[i].green_pos);
    }
}

/* --- White turtle positions correct --- */

void test_white_turtle_positions(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tk_path_layout_t layout = kin_tk_path_compute(jd);
    /* Days 1-6: descending 28->23 */
    TEST_ASSERT_EQUAL_INT(28, layout.days[0].white_pos);  /* day 1: 29-1=28 */
    TEST_ASSERT_EQUAL_INT(23, layout.days[5].white_pos);  /* day 6: 29-6=23 */
    /* Days 7-22: fixed at 23 */
    TEST_ASSERT_EQUAL_INT(23, layout.days[6].white_pos);
    TEST_ASSERT_EQUAL_INT(23, layout.days[21].white_pos);
    /* Days 23-28: walking with Yellow */
    TEST_ASSERT_EQUAL_INT(23, layout.days[22].white_pos);
    TEST_ASSERT_EQUAL_INT(28, layout.days[27].white_pos);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_jul26_today_day_1);
    RUN_TEST(test_jul26_phase_earth_walk);
    RUN_TEST(test_28_days_total);
    RUN_TEST(test_day1_yellow_pos);
    RUN_TEST(test_day1_phase_name_not_null);
    RUN_TEST(test_day23_reunion);
    RUN_TEST(test_days_23_28_lovers_together);
    RUN_TEST(test_days_7_22_green_pos_in_cube);
    RUN_TEST(test_days_1_6_green_pos_resting);
    RUN_TEST(test_tower_days_marked);
    RUN_TEST(test_only_one_highlighted);
    RUN_TEST(test_earth_walk_y);
    RUN_TEST(test_heaven_walk_y);
    RUN_TEST(test_cube_row_0_y);
    RUN_TEST(test_cube_row_3_y);
    RUN_TEST(test_cube_boustrophedon_row1);
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_day_out_of_time_returns_zeroed);
    RUN_TEST(test_cube_positions_7_22);
    RUN_TEST(test_cube_positions_0_outside_cube);
    RUN_TEST(test_purity);
    RUN_TEST(test_white_turtle_positions);
    return UNITY_END();
}
