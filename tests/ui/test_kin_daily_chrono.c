#include "../unity/unity.h"
#include "../../src/ui/kin_daily_chrono.h"
#include "../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- July 26, 2026: first day of 13-Moon year --- */

static double jd_jul_26_2026(void)
{
    return gregorian_to_jd(2026, 7, 26.5);
}

void test_jul26_2026_kin_positive(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_GREATER_THAN(0, c.kin);
    TEST_ASSERT_TRUE(c.kin >= 1 && c.kin <= 260);
}

void test_jul26_2026_moon_1_day_1(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(1, c.moon);
    TEST_ASSERT_EQUAL_INT(1, c.moon_day);
}

void test_jul26_2026_plasma_valid(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.plasma >= 0 && c.plasma <= 6);
}

/* --- kin_name populated --- */

void test_kin_name_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.kin_name) > 0);
}

/* --- tone_action populated --- */

void test_tone_action_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.tone_action) > 0);
}

/* --- Oracle: all 5 positions valid --- */

void test_oracle_destiny_kin_valid(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.destiny.kin >= 1 && c.destiny.kin <= 260);
}

void test_oracle_guide_kin_valid(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.guide.kin >= 1 && c.guide.kin <= 260);
}

void test_oracle_analog_kin_valid(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.analog.kin >= 1 && c.analog.kin <= 260);
}

void test_oracle_antipode_kin_valid(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.antipode.kin >= 1 && c.antipode.kin <= 260);
}

void test_oracle_occult_kin_valid(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.occult.kin >= 1 && c.occult.kin <= 260);
}

/* --- moon_name contains "Moon" --- */

void test_moon_name_contains_moon(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_NOT_NULL(strstr(c.moon_name, "Moon"));
}

/* --- plasma_name not empty --- */

void test_plasma_name_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.plasma_name) > 0);
}

/* --- mantra not empty --- */

void test_mantra_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.mantra) > 0);
}

/* --- wavespell_info not empty --- */

void test_wavespell_info_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.wavespell_info) > 0);
}

/* --- castle_info not empty --- */

void test_castle_info_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.castle_info) > 0);
}

/* --- wavespell_position 1-13 --- */

void test_wavespell_position_in_range(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(c.wavespell_position >= 1 && c.wavespell_position <= 13);
}

/* --- is_day_out = 0 for regular day --- */

void test_is_day_out_zero_for_regular_day(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(0, c.is_day_out);
}

/* --- July 25, 2027 is Day Out of Time --- */

void test_day_out_of_time_jul25_2027(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.5);
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, c.is_day_out);
}

/* --- color matches seal % 4 --- */

void test_color_matches_seal_mod4(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(c.seal % 4, c.color);
}

/* --- invalid JD returns zeroed --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
    TEST_ASSERT_EQUAL_INT(0, c.seal);
    TEST_ASSERT_EQUAL_INT(0, c.tone);
    TEST_ASSERT_EQUAL_INT(0, c.moon);
    TEST_ASSERT_TRUE(strlen(c.kin_name) == 0);
}

/* --- destiny.kin == chrono.kin --- */

void test_destiny_kin_equals_chrono_kin(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_EQUAL_INT(c.kin, c.destiny.kin);
}

/* --- tone_power populated --- */

void test_tone_power_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.tone_power) > 0);
}

/* --- color_action populated --- */

void test_color_action_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.color_action) > 0);
}

/* --- plasma_chakra populated --- */

void test_plasma_chakra_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.plasma_chakra) > 0);
}

/* --- plasma_action populated --- */

void test_plasma_action_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.plasma_action) > 0);
}

/* --- oracle position names populated --- */

void test_oracle_position_names_not_empty(void)
{
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    TEST_ASSERT_TRUE(strlen(c.destiny.name) > 0);
    TEST_ASSERT_TRUE(strlen(c.guide.name) > 0);
    TEST_ASSERT_TRUE(strlen(c.analog.name) > 0);
    TEST_ASSERT_TRUE(strlen(c.antipode.name) > 0);
    TEST_ASSERT_TRUE(strlen(c.occult.name) > 0);
}

/* --- is_power_day for wavespell position 1 --- */

void test_power_day_at_position_1(void)
{
    /* Find a kin at wavespell position 1 — kin 1 is always position 1 */
    /* July 26, 2013 was Kin 164 in Dreamspell. We test generically. */
    kin_daily_chrono_t c = kin_daily_chrono_compute(jd_jul_26_2026());
    int pos = c.wavespell_position;
    if (pos == 1 || pos == 5 || pos == 9 || pos == 13) {
        TEST_ASSERT_EQUAL_INT(1, c.is_power_day);
    } else {
        TEST_ASSERT_EQUAL_INT(0, c.is_power_day);
    }
}

/* --- different date gives different kin --- */

void test_different_date_different_kin(void)
{
    double jd1 = gregorian_to_jd(2026, 7, 26.5);
    double jd2 = gregorian_to_jd(2026, 7, 27.5);
    kin_daily_chrono_t c1 = kin_daily_chrono_compute(jd1);
    kin_daily_chrono_t c2 = kin_daily_chrono_compute(jd2);
    /* Consecutive days differ by 1 kin (mod 260) */
    TEST_ASSERT_NOT_EQUAL(c1.kin, c2.kin);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_jul26_2026_kin_positive);
    RUN_TEST(test_jul26_2026_moon_1_day_1);
    RUN_TEST(test_jul26_2026_plasma_valid);
    RUN_TEST(test_kin_name_not_empty);
    RUN_TEST(test_tone_action_not_empty);
    RUN_TEST(test_oracle_destiny_kin_valid);
    RUN_TEST(test_oracle_guide_kin_valid);
    RUN_TEST(test_oracle_analog_kin_valid);
    RUN_TEST(test_oracle_antipode_kin_valid);
    RUN_TEST(test_oracle_occult_kin_valid);
    RUN_TEST(test_moon_name_contains_moon);
    RUN_TEST(test_plasma_name_not_empty);
    RUN_TEST(test_mantra_not_empty);
    RUN_TEST(test_wavespell_info_not_empty);
    RUN_TEST(test_castle_info_not_empty);
    RUN_TEST(test_wavespell_position_in_range);
    RUN_TEST(test_is_day_out_zero_for_regular_day);
    RUN_TEST(test_day_out_of_time_jul25_2027);
    RUN_TEST(test_color_matches_seal_mod4);
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_destiny_kin_equals_chrono_kin);
    RUN_TEST(test_tone_power_not_empty);
    RUN_TEST(test_color_action_not_empty);
    RUN_TEST(test_plasma_chakra_not_empty);
    RUN_TEST(test_plasma_action_not_empty);
    RUN_TEST(test_oracle_position_names_not_empty);
    RUN_TEST(test_power_day_at_position_1);
    RUN_TEST(test_different_date_different_kin);
    return UNITY_END();
}
