#include "../unity/unity.h"
#include "../../src/ui/kin_heptad_row.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- July 26, 2026: first day of Moon 1, Heptad 1 --- */

void test_july_26_2026_heptad1_moon1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, row.heptad_number);
    TEST_ASSERT_EQUAL_INT(1, row.moon_number);
}

void test_july_26_2026_seven_days_valid(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_TRUE(row.days[i].kin >= 1 && row.days[i].kin <= 260);
    }
}

/* --- today_index range --- */

void test_today_index_range(void)
{
    /* July 26 = day 1 of heptad -> day_of_week=1 -> today_index=0 */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_TRUE(row.today_index >= 0 && row.today_index <= 6);
}

void test_today_index_day1_is_0(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, row.today_index);
}

void test_today_index_day4_is_3(void)
{
    /* July 29 = day 4 of Moon 1 -> day_of_week=4 -> today_index=3 */
    double jd = gregorian_to_jd(2026, 7, 29.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_EQUAL_INT(3, row.today_index);
}

/* --- All 7 days have valid seal/tone/color --- */

void test_all_days_valid_seal(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_TRUE(row.days[i].seal >= 0 && row.days[i].seal <= 19);
    }
}

void test_all_days_valid_tone(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_TRUE(row.days[i].tone >= 1 && row.days[i].tone <= 13);
    }
}

void test_all_days_valid_color(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_TRUE(row.days[i].color >= 0 && row.days[i].color <= 3);
    }
}

/* --- Plasma validity --- */

void test_all_days_valid_plasma(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_TRUE(row.days[i].plasma >= 0 && row.days[i].plasma <= 6);
    }
}

void test_all_days_plasma_name_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_NOT_NULL(row.days[i].plasma_name);
    }
}

void test_first_day_plasma_is_dali(void)
{
    /* Day 1 of any heptad has plasma=0 = "Dali" */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_EQUAL_STRING("Dali", row.days[0].plasma_name);
}

void test_all_days_plasma_chakra_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_NOT_NULL(row.days[i].plasma_chakra);
    }
}

/* --- Day positions --- */

void test_day0_x_is_zero(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, row.days[0].x);
}

void test_day6_x_is_six_sevenths(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 6.0f / 7.0f, row.days[6].x);
}

void test_all_days_width_one_seventh(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f / 7.0f, row.days[i].w);
    }
}

void test_all_days_y_zero_h_one(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, row.days[i].y);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, row.days[i].h);
    }
}

/* --- Consecutive days --- */

void test_days_in_moon_consecutive(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 1; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(row.days[i - 1].day_in_moon + 1,
                              row.days[i].day_in_moon);
    }
}

/* --- Only one day highlighted --- */

void test_only_one_day_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 8, 5.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    int count = 0;
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        if (row.days[i].highlighted) count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_highlighted_matches_today_index(void)
{
    double jd = gregorian_to_jd(2026, 8, 5.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_TRUE(row.today_index >= 0 && row.today_index <= 6);
    TEST_ASSERT_EQUAL_INT(1, row.days[row.today_index].highlighted);
}

/* --- Heptad/moon number ranges --- */

void test_heptad_number_in_range(void)
{
    /* Test several dates across a moon */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int h = 0; h < 4; h++) {
        double day_jd = jd + h * 7;
        kin_heptad_row_t row = kin_heptad_compute(day_jd);
        TEST_ASSERT_TRUE(row.heptad_number >= 1 && row.heptad_number <= 4);
    }
}

void test_moon_number_in_range(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int m = 0; m < 13; m++) {
        double day_jd = jd + m * 28;
        kin_heptad_row_t row = kin_heptad_compute(day_jd);
        TEST_ASSERT_TRUE(row.moon_number >= 1 && row.moon_number <= 13);
    }
}

/* --- Invalid JD --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_heptad_row_t row = kin_heptad_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, row.heptad_number);
    TEST_ASSERT_EQUAL_INT(0, row.moon_number);
    TEST_ASSERT_EQUAL_INT(0, row.today_index);
}

/* --- Day Out of Time --- */

void test_day_out_of_time_returns_zeroed(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, row.heptad_number);
    TEST_ASSERT_EQUAL_INT(0, row.moon_number);
}

/* --- Kin values sequential --- */

void test_kin_values_sequential(void)
{
    double jd = gregorian_to_jd(2026, 10, 1.0);
    kin_heptad_row_t row = kin_heptad_compute(jd);
    for (int i = 1; i < KIN_HEPTAD_DAYS; i++) {
        int prev = row.days[i - 1].kin;
        int curr = row.days[i].kin;
        int expected = (prev % 260) + 1;
        TEST_ASSERT_EQUAL_INT(expected, curr);
    }
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    double jd = gregorian_to_jd(2026, 11, 11.0);
    kin_heptad_row_t a = kin_heptad_compute(jd);
    kin_heptad_row_t b = kin_heptad_compute(jd);
    TEST_ASSERT_EQUAL_INT(a.heptad_number, b.heptad_number);
    TEST_ASSERT_EQUAL_INT(a.moon_number, b.moon_number);
    TEST_ASSERT_EQUAL_INT(a.today_index, b.today_index);
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(a.days[i].kin, b.days[i].kin);
        TEST_ASSERT_EQUAL_INT(a.days[i].day_in_moon, b.days[i].day_in_moon);
    }
}

/* --- Different heptads have different starting kin --- */

void test_different_heptads_different_kins(void)
{
    double jd1 = gregorian_to_jd(2026, 7, 26.0);  /* Heptad 1 */
    double jd2 = gregorian_to_jd(2026, 8, 2.0);   /* Heptad 2 */
    kin_heptad_row_t row1 = kin_heptad_compute(jd1);
    kin_heptad_row_t row2 = kin_heptad_compute(jd2);
    TEST_ASSERT_TRUE(row1.days[0].kin != row2.days[0].kin);
}

int main(void)
{
    UNITY_BEGIN();
    /* basic computation */
    RUN_TEST(test_july_26_2026_heptad1_moon1);
    RUN_TEST(test_july_26_2026_seven_days_valid);
    /* today_index */
    RUN_TEST(test_today_index_range);
    RUN_TEST(test_today_index_day1_is_0);
    RUN_TEST(test_today_index_day4_is_3);
    /* seal/tone/color */
    RUN_TEST(test_all_days_valid_seal);
    RUN_TEST(test_all_days_valid_tone);
    RUN_TEST(test_all_days_valid_color);
    /* plasma */
    RUN_TEST(test_all_days_valid_plasma);
    RUN_TEST(test_all_days_plasma_name_not_null);
    RUN_TEST(test_first_day_plasma_is_dali);
    RUN_TEST(test_all_days_plasma_chakra_not_null);
    /* positions */
    RUN_TEST(test_day0_x_is_zero);
    RUN_TEST(test_day6_x_is_six_sevenths);
    RUN_TEST(test_all_days_width_one_seventh);
    RUN_TEST(test_all_days_y_zero_h_one);
    /* consecutive days */
    RUN_TEST(test_days_in_moon_consecutive);
    /* highlighting */
    RUN_TEST(test_only_one_day_highlighted);
    RUN_TEST(test_highlighted_matches_today_index);
    /* heptad/moon range */
    RUN_TEST(test_heptad_number_in_range);
    RUN_TEST(test_moon_number_in_range);
    /* invalid */
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_day_out_of_time_returns_zeroed);
    /* sequential kin */
    RUN_TEST(test_kin_values_sequential);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    /* different heptads */
    RUN_TEST(test_different_heptads_different_kins);
    return UNITY_END();
}
