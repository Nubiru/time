/* test_birth_profile.c -- Birth Profile Computer tests
 * TDD RED phase: tests written before implementation.
 * Tests multi-system birth profile computation for given dates. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/birth_profile.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== bp_compute basic fields ===== */

void test_compute_sets_date_fields(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_EQUAL_INT(2024, p.year);
    TEST_ASSERT_EQUAL_INT(1, p.month);
    TEST_ASSERT_EQUAL_INT(1, p.day);
}

void test_compute_sets_jd(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    /* JD for 2024-01-01 noon ~ 2460310.5 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, (float)2460310.5, (float)p.jd);
}

void test_compute_no_location(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_FALSE(p.has_location);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.latitude);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.longitude);
}

void test_compute_day_of_year_jan1(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_EQUAL_INT(1, p.day_of_year);
}

void test_compute_day_of_year_dec31(void)
{
    birth_profile_t p = bp_compute(2024, 12, 31);
    /* 2024 is leap year: 366 */
    TEST_ASSERT_EQUAL_INT(366, p.day_of_year);
}

void test_compute_day_of_year_feb29(void)
{
    birth_profile_t p = bp_compute(2024, 2, 29);
    TEST_ASSERT_EQUAL_INT(60, p.day_of_year);
}

void test_compute_day_of_year_mar1_non_leap(void)
{
    birth_profile_t p = bp_compute(2023, 3, 1);
    TEST_ASSERT_EQUAL_INT(60, p.day_of_year);
}

/* ===== earth_age_ratio ===== */

void test_earth_age_ratio_2024(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    float expected = (float)(2024.0 / 4540000000.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-12f, expected, (float)p.earth_age_ratio);
}

void test_earth_age_ratio_year_1(void)
{
    birth_profile_t p = bp_compute(1, 1, 1);
    float expected = (float)(1.0 / 4540000000.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-12f, expected, (float)p.earth_age_ratio);
}

/* ===== Tzolkin sub-system ===== */

void test_tzolkin_kin_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.tzolkin.kin >= 1 && p.tzolkin.kin <= 260);
}

void test_tzolkin_tone_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.tzolkin.tone >= 1 && p.tzolkin.tone <= 13);
}

void test_tzolkin_seal_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.tzolkin.seal >= 0 && p.tzolkin.seal <= 19);
}

void test_tzolkin_seal_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_NOT_NULL(p.tzolkin.seal_name);
    TEST_ASSERT_TRUE(strlen(p.tzolkin.seal_name) > 0);
}

void test_tzolkin_tone_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_NOT_NULL(p.tzolkin.tone_name);
    TEST_ASSERT_TRUE(strlen(p.tzolkin.tone_name) > 0);
}

/* 2012-12-21 = Kin 207 Blue Crystal Hand (anchor date) */
void test_tzolkin_known_date_2012_12_21(void)
{
    birth_profile_t p = bp_compute(2012, 12, 21);
    TEST_ASSERT_EQUAL_INT(207, p.tzolkin.kin);
    TEST_ASSERT_EQUAL_INT(12, p.tzolkin.tone); /* Crystal = 12 */
    TEST_ASSERT_EQUAL_INT(6, p.tzolkin.seal);  /* Hand = 6 */
    TEST_ASSERT_EQUAL_STRING("Blue Hand", p.tzolkin.seal_name);
    TEST_ASSERT_EQUAL_STRING("Crystal", p.tzolkin.tone_name);
}

void test_tzolkin_oracle_fields(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.tzolkin.guide_kin >= 1 && p.tzolkin.guide_kin <= 260);
    TEST_ASSERT_TRUE(p.tzolkin.analog_kin >= 1 && p.tzolkin.analog_kin <= 260);
    TEST_ASSERT_TRUE(p.tzolkin.antipode_kin >= 1 && p.tzolkin.antipode_kin <= 260);
    TEST_ASSERT_TRUE(p.tzolkin.occult_kin >= 1 && p.tzolkin.occult_kin <= 260);
}

void test_tzolkin_wavespell_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.tzolkin.wavespell >= 1 && p.tzolkin.wavespell <= 20);
}

void test_tzolkin_castle_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.tzolkin.castle >= 1 && p.tzolkin.castle <= 5);
}

/* ===== Chinese sub-system ===== */

void test_chinese_animal_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.chinese.animal >= 0 && p.chinese.animal <= 11);
}

void test_chinese_element_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.chinese.element >= 0 && p.chinese.element <= 4);
}

void test_chinese_stem_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.chinese.stem >= 0 && p.chinese.stem <= 9);
}

void test_chinese_branch_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.chinese.branch >= 0 && p.chinese.branch <= 11);
}

void test_chinese_polarity_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.chinese.polarity == 0 || p.chinese.polarity == 1);
}

/* 2024 = Year of the Wood Dragon (Yang) */
void test_chinese_2024_dragon(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_EQUAL_STRING("Dragon", p.chinese.animal_name);
    TEST_ASSERT_EQUAL_STRING("Wood", p.chinese.element_name);
    TEST_ASSERT_EQUAL_INT(0, p.chinese.polarity); /* Yang */
}

/* ===== Hebrew sub-system ===== */

void test_hebrew_year_positive(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.hebrew.year > 5700);
}

void test_hebrew_month_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.hebrew.month >= 1 && p.hebrew.month <= 13);
}

void test_hebrew_day_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.hebrew.day >= 1 && p.hebrew.day <= 30);
}

void test_hebrew_month_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_NOT_NULL(p.hebrew.month_name);
    TEST_ASSERT_TRUE(strlen(p.hebrew.month_name) > 0);
}

/* ===== Islamic sub-system ===== */

void test_islamic_year_positive(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.islamic.year > 1400);
}

void test_islamic_month_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.islamic.month >= 1 && p.islamic.month <= 12);
}

void test_islamic_day_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.islamic.day >= 1 && p.islamic.day <= 30);
}

void test_islamic_month_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_NOT_NULL(p.islamic.month_name);
    TEST_ASSERT_TRUE(strlen(p.islamic.month_name) > 0);
}

/* ===== Buddhist sub-system ===== */

void test_buddhist_era_2024(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    /* BE = CE + 543 */
    TEST_ASSERT_EQUAL_INT(2567, p.buddhist.era_year);
}

void test_buddhist_era_year1(void)
{
    birth_profile_t p = bp_compute(1, 1, 1);
    TEST_ASSERT_EQUAL_INT(544, p.buddhist.era_year);
}

/* ===== I Ching sub-system ===== */

void test_iching_king_wen_range(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_TRUE(p.iching.king_wen >= 1 && p.iching.king_wen <= 64);
}

void test_iching_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_NOT_NULL(p.iching.name);
    TEST_ASSERT_TRUE(strlen(p.iching.name) > 0);
}

/* ===== Astrology sub-system ===== */

void test_astrology_sun_sign_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.astrology.sun_sign >= 0 && p.astrology.sun_sign <= 11);
}

void test_astrology_sun_sign_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_NOT_NULL(p.astrology.sun_sign_name);
    TEST_ASSERT_TRUE(strlen(p.astrology.sun_sign_name) > 0);
}

void test_astrology_sun_longitude_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.astrology.sun_longitude >= 0.0);
    TEST_ASSERT_TRUE(p.astrology.sun_longitude < 360.0);
}

/* June 15 sun should be around Gemini (sign 2) or near boundary */
void test_astrology_june_gemini(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    /* Sun around 84 degrees = Gemini (sign 2) */
    TEST_ASSERT_EQUAL_INT(2, p.astrology.sun_sign);
    TEST_ASSERT_EQUAL_STRING("Gemini", p.astrology.sun_sign_name);
}

/* January 1 sun should be Capricorn (sign 9) or Sagittarius (sign 8) */
void test_astrology_january_capricorn(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    /* Sun around 280 degrees = Capricorn */
    TEST_ASSERT_EQUAL_INT(9, p.astrology.sun_sign);
}

void test_astrology_hd_sun_gate_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.astrology.hd_sun_gate >= 1 && p.astrology.hd_sun_gate <= 64);
}

void test_astrology_hd_earth_gate_range(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_TRUE(p.astrology.hd_earth_gate >= 1 && p.astrology.hd_earth_gate <= 64);
}

void test_astrology_hd_sun_gate_name_not_null(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_NOT_NULL(p.astrology.hd_sun_gate_name);
    TEST_ASSERT_TRUE(strlen(p.astrology.hd_sun_gate_name) > 0);
}

/* ===== western_zodiac matches sun_sign_name ===== */

void test_western_zodiac_matches_sun_sign(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    TEST_ASSERT_EQUAL_STRING(p.astrology.sun_sign_name, p.western_zodiac);
}

void test_western_zodiac_january(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    TEST_ASSERT_EQUAL_STRING(p.astrology.sun_sign_name, p.western_zodiac);
}

/* ===== bp_compute_with_location ===== */

void test_with_location_sets_flag(void)
{
    birth_profile_t p = bp_compute_with_location(2024, 6, 15, 40.7128, -74.0060);
    TEST_ASSERT_TRUE(p.has_location);
}

void test_with_location_stores_coords(void)
{
    birth_profile_t p = bp_compute_with_location(2024, 6, 15, 40.7128, -74.0060);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 40.7128f, (float)p.latitude);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -74.0060f, (float)p.longitude);
}

void test_with_location_same_systems(void)
{
    birth_profile_t p1 = bp_compute(2024, 6, 15);
    birth_profile_t p2 = bp_compute_with_location(2024, 6, 15, 40.7128, -74.0060);
    /* All calendar systems should match regardless of location */
    TEST_ASSERT_EQUAL_INT(p1.tzolkin.kin, p2.tzolkin.kin);
    TEST_ASSERT_EQUAL_INT(p1.chinese.animal, p2.chinese.animal);
    TEST_ASSERT_EQUAL_INT(p1.hebrew.year, p2.hebrew.year);
    TEST_ASSERT_EQUAL_INT(p1.islamic.year, p2.islamic.year);
    TEST_ASSERT_EQUAL_INT(p1.buddhist.era_year, p2.buddhist.era_year);
    TEST_ASSERT_EQUAL_INT(p1.iching.king_wen, p2.iching.king_wen);
}

/* ===== bp_system_count and bp_system_name ===== */

void test_system_count_positive(void)
{
    int count = bp_system_count();
    TEST_ASSERT_TRUE(count >= 7);
}

void test_system_name_valid_index(void)
{
    int count = bp_system_count();
    for (int i = 0; i < count; i++) {
        const char *name = bp_system_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_system_name_invalid_index(void)
{
    TEST_ASSERT_NULL(bp_system_name(-1));
    TEST_ASSERT_NULL(bp_system_name(100));
}

/* ===== bp_summary ===== */

void test_summary_tzolkin(void)
{
    birth_profile_t p = bp_compute(2012, 12, 21);
    const char *s = bp_summary(&p, "tzolkin");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
    /* Should contain kin number */
    TEST_ASSERT_NOT_NULL(strstr(s, "207"));
}

void test_summary_chinese(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    const char *s = bp_summary(&p, "chinese");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(strstr(s, "Dragon"));
}

void test_summary_hebrew(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    const char *s = bp_summary(&p, "hebrew");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_summary_islamic(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    const char *s = bp_summary(&p, "islamic");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_summary_buddhist(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    const char *s = bp_summary(&p, "buddhist");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(strstr(s, "2567"));
}

void test_summary_iching(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    const char *s = bp_summary(&p, "iching");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_summary_astrology(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    const char *s = bp_summary(&p, "astrology");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(strstr(s, "Gemini"));
}

void test_summary_case_insensitive(void)
{
    birth_profile_t p = bp_compute(2024, 6, 15);
    const char *s1 = bp_summary(&p, "Tzolkin");
    const char *s2 = bp_summary(&p, "TZOLKIN");
    const char *s3 = bp_summary(&p, "tzolkin");
    TEST_ASSERT_NOT_NULL(s1);
    TEST_ASSERT_NOT_NULL(s2);
    TEST_ASSERT_NOT_NULL(s3);
}

void test_summary_unknown_system(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    const char *s = bp_summary(&p, "nonexistent");
    TEST_ASSERT_NULL(s);
}

void test_summary_null_profile(void)
{
    const char *s = bp_summary(NULL, "tzolkin");
    TEST_ASSERT_NULL(s);
}

void test_summary_null_system(void)
{
    birth_profile_t p = bp_compute(2024, 1, 1);
    const char *s = bp_summary(&p, NULL);
    TEST_ASSERT_NULL(s);
}

/* ===== Edge cases ===== */

void test_year_2000(void)
{
    birth_profile_t p = bp_compute(2000, 1, 1);
    TEST_ASSERT_EQUAL_INT(2000, p.year);
    TEST_ASSERT_TRUE(p.tzolkin.kin >= 1 && p.tzolkin.kin <= 260);
    TEST_ASSERT_TRUE(p.chinese.animal >= 0 && p.chinese.animal <= 11);
    TEST_ASSERT_TRUE(p.hebrew.year > 5700);
    TEST_ASSERT_TRUE(p.islamic.year > 1400);
    TEST_ASSERT_EQUAL_INT(2543, p.buddhist.era_year);
}

void test_year_1900(void)
{
    birth_profile_t p = bp_compute(1900, 7, 4);
    TEST_ASSERT_EQUAL_INT(1900, p.year);
    TEST_ASSERT_TRUE(p.tzolkin.kin >= 1 && p.tzolkin.kin <= 260);
    /* 1900 not leap year: day_of_year for July 4 = 31+28+31+30+31+30+4=185 */
    TEST_ASSERT_EQUAL_INT(185, p.day_of_year);
}

void test_cross_system_consistency(void)
{
    /* Same date should always produce same results */
    birth_profile_t p1 = bp_compute(2024, 3, 20);
    birth_profile_t p2 = bp_compute(2024, 3, 20);
    TEST_ASSERT_EQUAL_INT(p1.tzolkin.kin, p2.tzolkin.kin);
    TEST_ASSERT_EQUAL_INT(p1.chinese.animal, p2.chinese.animal);
    TEST_ASSERT_EQUAL_INT(p1.hebrew.year, p2.hebrew.year);
    TEST_ASSERT_EQUAL_INT(p1.hebrew.month, p2.hebrew.month);
    TEST_ASSERT_EQUAL_INT(p1.hebrew.day, p2.hebrew.day);
    TEST_ASSERT_EQUAL_INT(p1.islamic.year, p2.islamic.year);
    TEST_ASSERT_EQUAL_INT(p1.iching.king_wen, p2.iching.king_wen);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)p1.astrology.sun_longitude,
                             (float)p2.astrology.sun_longitude);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* basic fields */
    RUN_TEST(test_compute_sets_date_fields);
    RUN_TEST(test_compute_sets_jd);
    RUN_TEST(test_compute_no_location);
    RUN_TEST(test_compute_day_of_year_jan1);
    RUN_TEST(test_compute_day_of_year_dec31);
    RUN_TEST(test_compute_day_of_year_feb29);
    RUN_TEST(test_compute_day_of_year_mar1_non_leap);

    /* earth age */
    RUN_TEST(test_earth_age_ratio_2024);
    RUN_TEST(test_earth_age_ratio_year_1);

    /* tzolkin */
    RUN_TEST(test_tzolkin_kin_range);
    RUN_TEST(test_tzolkin_tone_range);
    RUN_TEST(test_tzolkin_seal_range);
    RUN_TEST(test_tzolkin_seal_name_not_null);
    RUN_TEST(test_tzolkin_tone_name_not_null);
    RUN_TEST(test_tzolkin_known_date_2012_12_21);
    RUN_TEST(test_tzolkin_oracle_fields);
    RUN_TEST(test_tzolkin_wavespell_range);
    RUN_TEST(test_tzolkin_castle_range);

    /* chinese */
    RUN_TEST(test_chinese_animal_range);
    RUN_TEST(test_chinese_element_range);
    RUN_TEST(test_chinese_stem_range);
    RUN_TEST(test_chinese_branch_range);
    RUN_TEST(test_chinese_polarity_range);
    RUN_TEST(test_chinese_2024_dragon);

    /* hebrew */
    RUN_TEST(test_hebrew_year_positive);
    RUN_TEST(test_hebrew_month_range);
    RUN_TEST(test_hebrew_day_range);
    RUN_TEST(test_hebrew_month_name_not_null);

    /* islamic */
    RUN_TEST(test_islamic_year_positive);
    RUN_TEST(test_islamic_month_range);
    RUN_TEST(test_islamic_day_range);
    RUN_TEST(test_islamic_month_name_not_null);

    /* buddhist */
    RUN_TEST(test_buddhist_era_2024);
    RUN_TEST(test_buddhist_era_year1);

    /* iching */
    RUN_TEST(test_iching_king_wen_range);
    RUN_TEST(test_iching_name_not_null);

    /* astrology */
    RUN_TEST(test_astrology_sun_sign_range);
    RUN_TEST(test_astrology_sun_sign_name_not_null);
    RUN_TEST(test_astrology_sun_longitude_range);
    RUN_TEST(test_astrology_june_gemini);
    RUN_TEST(test_astrology_january_capricorn);
    RUN_TEST(test_astrology_hd_sun_gate_range);
    RUN_TEST(test_astrology_hd_earth_gate_range);
    RUN_TEST(test_astrology_hd_sun_gate_name_not_null);

    /* western_zodiac consistency */
    RUN_TEST(test_western_zodiac_matches_sun_sign);
    RUN_TEST(test_western_zodiac_january);

    /* bp_compute_with_location */
    RUN_TEST(test_with_location_sets_flag);
    RUN_TEST(test_with_location_stores_coords);
    RUN_TEST(test_with_location_same_systems);

    /* bp_system_count / bp_system_name */
    RUN_TEST(test_system_count_positive);
    RUN_TEST(test_system_name_valid_index);
    RUN_TEST(test_system_name_invalid_index);

    /* bp_summary */
    RUN_TEST(test_summary_tzolkin);
    RUN_TEST(test_summary_chinese);
    RUN_TEST(test_summary_hebrew);
    RUN_TEST(test_summary_islamic);
    RUN_TEST(test_summary_buddhist);
    RUN_TEST(test_summary_iching);
    RUN_TEST(test_summary_astrology);
    RUN_TEST(test_summary_case_insensitive);
    RUN_TEST(test_summary_unknown_system);
    RUN_TEST(test_summary_null_profile);
    RUN_TEST(test_summary_null_system);

    /* edge cases */
    RUN_TEST(test_year_2000);
    RUN_TEST(test_year_1900);
    RUN_TEST(test_cross_system_consistency);

    return UNITY_END();
}
