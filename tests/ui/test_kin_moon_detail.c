#include "../unity/unity.h"
#include "../../src/ui/kin_moon_detail.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Basic page computation --- */

void test_july_26_2026_is_moon1_day1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, detail.moon_number);
    TEST_ASSERT_EQUAL_INT(1, detail.today_day);
    TEST_ASSERT_EQUAL_INT(1, detail.days[0].highlighted);
}

void test_moon_name_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    TEST_ASSERT_NOT_NULL(detail.moon_name);
    TEST_ASSERT_EQUAL_STRING("Magnetic", detail.moon_name);
}

void test_28_days_total(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    /* Last day should be day 28 */
    TEST_ASSERT_EQUAL_INT(28, detail.days[KIN_MOON_DETAIL_DAYS - 1].day);
}

/* --- All 28 days have valid kin, seal, tone, color --- */

void test_all_days_valid_kin(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].kin >= 1 && detail.days[i].kin <= 260);
    }
}

void test_all_days_valid_seal(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].seal >= 0 && detail.days[i].seal <= 19);
    }
}

void test_all_days_valid_tone(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].tone >= 1 && detail.days[i].tone <= 13);
    }
}

void test_all_days_valid_color(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].color >= 0 && detail.days[i].color <= 3);
    }
}

/* --- Plasma valid and named --- */

void test_all_days_valid_plasma(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].plasma >= 0 && detail.days[i].plasma <= 6);
        TEST_ASSERT_NOT_NULL(detail.days[i].plasma_name);
    }
}

/* --- Portal count >= 0 --- */

void test_portal_count_non_negative(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    TEST_ASSERT_TRUE(detail.portal_count >= 0);
    TEST_ASSERT_TRUE(detail.portal_count <= KIN_MOON_DETAIL_DAYS);
}

/* --- Wavespell position and number valid --- */

void test_all_days_valid_wavespell_pos(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].wavespell_pos >= 1
                      && detail.days[i].wavespell_pos <= 13);
    }
}

void test_all_days_valid_wavespell_number(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].wavespell_number >= 1
                      && detail.days[i].wavespell_number <= 20);
    }
}

/* --- Gregorian month and day populated --- */

void test_all_days_gregorian_populated(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_TRUE(detail.days[i].greg_month >= 1
                      && detail.days[i].greg_month <= 12);
        TEST_ASSERT_TRUE(detail.days[i].greg_day >= 1
                      && detail.days[i].greg_day <= 31);
    }
}

/* --- Grid positions --- */

void test_day1_grid_position(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, detail.days[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, detail.days[0].y);
}

void test_day7_grid_position(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    /* Day 7: col=6, row=0 -> x = 6/7 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 6.0f / 7.0f, detail.days[6].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, detail.days[6].y);
}

void test_day8_grid_position(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    /* Day 8: col=0, row=1 -> x=0, y=0.25 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, detail.days[7].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, detail.days[7].y);
}

/* --- Only 1 day highlighted --- */

void test_only_one_day_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 8, 10.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    int count = 0;
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        if (detail.days[i].highlighted) count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Consecutive day numbers 1-28 --- */

void test_day_numbers_consecutive(void)
{
    double jd = gregorian_to_jd(2026, 9, 15.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, detail.days[i].day);
    }
}

/* --- today_day in range 1-28 --- */

void test_today_day_range(void)
{
    double jd = gregorian_to_jd(2026, 12, 1.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    TEST_ASSERT_TRUE(detail.today_day >= 1 && detail.today_day <= 28);
}

/* --- Invalid JD returns zeroed --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_moon_detail_t detail = kin_moon_detail_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, detail.moon_number);
    TEST_ASSERT_NULL(detail.moon_name);
    TEST_ASSERT_EQUAL_INT(0, detail.today_day);
}

/* --- Day Out of Time returns zeroed --- */

void test_day_out_of_time_returns_zeroed(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.0);
    kin_moon_detail_t detail = kin_moon_detail_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, detail.moon_number);
}

/* --- Different months have different Gregorian dates --- */

void test_different_months_different_greg_dates(void)
{
    double jd1 = gregorian_to_jd(2026, 7, 26.0);   /* Moon 1 */
    double jd2 = gregorian_to_jd(2026, 8, 23.0);   /* Moon 2 */
    kin_moon_detail_t d1 = kin_moon_detail_compute(jd1);
    kin_moon_detail_t d2 = kin_moon_detail_compute(jd2);
    /* Day 1 of different moons should have different Gregorian dates */
    TEST_ASSERT_TRUE(d1.days[0].greg_month != d2.days[0].greg_month
                  || d1.days[0].greg_day != d2.days[0].greg_day);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    double jd = gregorian_to_jd(2026, 11, 11.0);
    kin_moon_detail_t a = kin_moon_detail_compute(jd);
    kin_moon_detail_t b = kin_moon_detail_compute(jd);
    TEST_ASSERT_EQUAL_INT(a.moon_number, b.moon_number);
    TEST_ASSERT_EQUAL_INT(a.today_day, b.today_day);
    TEST_ASSERT_EQUAL_INT(a.portal_count, b.portal_count);
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        TEST_ASSERT_EQUAL_INT(a.days[i].kin, b.days[i].kin);
        TEST_ASSERT_EQUAL_INT(a.days[i].wavespell_pos, b.days[i].wavespell_pos);
        TEST_ASSERT_EQUAL_INT(a.days[i].greg_day, b.days[i].greg_day);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* basic */
    RUN_TEST(test_july_26_2026_is_moon1_day1);
    RUN_TEST(test_moon_name_not_null);
    RUN_TEST(test_28_days_total);
    /* valid kin/seal/tone/color */
    RUN_TEST(test_all_days_valid_kin);
    RUN_TEST(test_all_days_valid_seal);
    RUN_TEST(test_all_days_valid_tone);
    RUN_TEST(test_all_days_valid_color);
    /* plasma */
    RUN_TEST(test_all_days_valid_plasma);
    /* portal count */
    RUN_TEST(test_portal_count_non_negative);
    /* wavespell */
    RUN_TEST(test_all_days_valid_wavespell_pos);
    RUN_TEST(test_all_days_valid_wavespell_number);
    /* gregorian */
    RUN_TEST(test_all_days_gregorian_populated);
    /* grid positions */
    RUN_TEST(test_day1_grid_position);
    RUN_TEST(test_day7_grid_position);
    RUN_TEST(test_day8_grid_position);
    /* highlight */
    RUN_TEST(test_only_one_day_highlighted);
    /* day numbers */
    RUN_TEST(test_day_numbers_consecutive);
    /* today_day range */
    RUN_TEST(test_today_day_range);
    /* invalid */
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_day_out_of_time_returns_zeroed);
    /* different months */
    RUN_TEST(test_different_months_different_greg_dates);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
