#include "../unity/unity.h"
#include "../../src/ui/kin_moon_page.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Basic page computation --- */

void test_july_26_2026_is_moon1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, page.moon_number);
    TEST_ASSERT_EQUAL_INT(1, page.today_day);
    TEST_ASSERT_EQUAL_INT(2026, page.year);
}

void test_moon_name_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    TEST_ASSERT_NOT_NULL(page.moon_name);
    TEST_ASSERT_EQUAL_STRING("Magnetic", page.moon_name);
}

void test_day1_highlighted(void)
{
    /* July 26 = day 1 of Moon 1 -> day 1 highlighted */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, page.days[0].highlighted);
}

void test_only_one_day_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 8, 10.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    int count = 0;
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        if (page.days[i].highlighted) count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- All 28 days valid --- */

void test_all_28_days_valid_kin(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_TRUE(page.days[i].kin >= 1 && page.days[i].kin <= 260);
    }
}

void test_all_28_days_valid_seal(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_TRUE(page.days[i].seal >= 0 && page.days[i].seal <= 19);
    }
}

void test_all_28_days_valid_tone(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_TRUE(page.days[i].tone >= 1 && page.days[i].tone <= 13);
    }
}

void test_all_28_days_valid_color(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_TRUE(page.days[i].color >= 0 && page.days[i].color <= 3);
    }
}

void test_all_28_days_valid_plasma(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_TRUE(page.days[i].plasma >= 0 && page.days[i].plasma <= 6);
    }
}

/* --- Grid positions --- */

void test_day1_position_origin(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, page.days[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, page.days[0].y);
}

void test_day7_position_last_col(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    /* Day 7: col=6, row=0 -> x = 6/7 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 6.0f / 7.0f, page.days[6].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, page.days[6].y);
}

void test_day8_position_second_row(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    /* Day 8: col=0, row=1 -> x=0, y=1/4 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, page.days[7].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, page.days[7].y);
}

/* --- Day numbers consecutive --- */

void test_day_numbers_consecutive(void)
{
    double jd = gregorian_to_jd(2026, 9, 15.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, page.days[i].day);
    }
}

/* --- Kin values increase (Tzolkin is sequential) --- */

void test_kin_values_sequential(void)
{
    double jd = gregorian_to_jd(2026, 10, 1.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    for (int i = 1; i < KIN_MOON_TOTAL_DAYS; i++) {
        /* Kin wraps at 260, so next = prev + 1 OR prev = 260 and next = 1 */
        int prev = page.days[i - 1].kin;
        int curr = page.days[i].kin;
        int expected = (prev % 260) + 1;
        TEST_ASSERT_EQUAL_INT(expected, curr);
    }
}

/* --- Different months have different kins --- */

void test_different_months_different_kins(void)
{
    double jd1 = gregorian_to_jd(2026, 7, 26.0);  /* Moon 1 */
    double jd2 = gregorian_to_jd(2026, 8, 23.0);  /* Moon 2 */
    kin_moon_page_t page1 = kin_moon_page_compute(jd1);
    kin_moon_page_t page2 = kin_moon_page_compute(jd2);
    /* First days should have different kin numbers */
    TEST_ASSERT_TRUE(page1.days[0].kin != page2.days[0].kin);
}

/* --- Moon number range --- */

void test_moon_number_range(void)
{
    /* Check several dates throughout the year */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int m = 0; m < 13; m++) {
        double day_jd = jd + m * 28;
        kin_moon_page_t page = kin_moon_page_compute(day_jd);
        TEST_ASSERT_TRUE(page.moon_number >= 1 && page.moon_number <= 13);
    }
}

/* --- Invalid JD returns zeroed --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_moon_page_t page = kin_moon_page_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, page.moon_number);
    TEST_ASSERT_NULL(page.moon_name);
    TEST_ASSERT_EQUAL_INT(0, page.today_day);
}

/* --- Day Out of Time returns zeroed --- */

void test_day_out_of_time_returns_zeroed(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, page.moon_number);
}

/* --- today_day is in range 1-28 --- */

void test_today_day_range(void)
{
    double jd = gregorian_to_jd(2026, 12, 1.0);
    kin_moon_page_t page = kin_moon_page_compute(jd);
    TEST_ASSERT_TRUE(page.today_day >= 1 && page.today_day <= 28);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    double jd = gregorian_to_jd(2026, 11, 11.0);
    kin_moon_page_t a = kin_moon_page_compute(jd);
    kin_moon_page_t b = kin_moon_page_compute(jd);
    TEST_ASSERT_EQUAL_INT(a.moon_number, b.moon_number);
    TEST_ASSERT_EQUAL_INT(a.today_day, b.today_day);
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(a.days[i].kin, b.days[i].kin);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* basic */
    RUN_TEST(test_july_26_2026_is_moon1);
    RUN_TEST(test_moon_name_not_null);
    RUN_TEST(test_day1_highlighted);
    RUN_TEST(test_only_one_day_highlighted);
    /* 28 days valid */
    RUN_TEST(test_all_28_days_valid_kin);
    RUN_TEST(test_all_28_days_valid_seal);
    RUN_TEST(test_all_28_days_valid_tone);
    RUN_TEST(test_all_28_days_valid_color);
    RUN_TEST(test_all_28_days_valid_plasma);
    /* grid positions */
    RUN_TEST(test_day1_position_origin);
    RUN_TEST(test_day7_position_last_col);
    RUN_TEST(test_day8_position_second_row);
    /* day numbers */
    RUN_TEST(test_day_numbers_consecutive);
    /* kin values */
    RUN_TEST(test_kin_values_sequential);
    RUN_TEST(test_different_months_different_kins);
    /* moon number range */
    RUN_TEST(test_moon_number_range);
    /* invalid */
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_day_out_of_time_returns_zeroed);
    /* today_day */
    RUN_TEST(test_today_day_range);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
