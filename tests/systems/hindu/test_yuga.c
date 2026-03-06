/* test_yuga.c -- Yuga cycle data module tests
 * TDD: tests match the API defined in task.md (Track 17.3).
 * Covers: all 4 Yugas data, position calculations, cycling through
 *         Maha-Yugas, names, meanings, dharma fractions, larger cycles,
 *         epoch constants, invalid inputs, purity. */

#include "../../unity/unity.h"
#include "../../../src/systems/hindu/yuga.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Yuga data: yuga_get for all 4 Yugas ===== */

void test_satya_yuga_data(void)
{
    yuga_t y = yuga_get(YUGA_SATYA);
    TEST_ASSERT_EQUAL_INT(0, y.id);
    TEST_ASSERT_EQUAL_STRING("Satya", y.name);
    TEST_ASSERT_EQUAL_INT64(SATYA_YUGA_YEARS, y.years);
    TEST_ASSERT_EQUAL_INT(4, y.ratio);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, y.dharma_fraction);
}

void test_treta_yuga_data(void)
{
    yuga_t y = yuga_get(YUGA_TRETA);
    TEST_ASSERT_EQUAL_INT(1, y.id);
    TEST_ASSERT_EQUAL_STRING("Treta", y.name);
    TEST_ASSERT_EQUAL_INT64(TRETA_YUGA_YEARS, y.years);
    TEST_ASSERT_EQUAL_INT(3, y.ratio);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.75, y.dharma_fraction);
}

void test_dvapara_yuga_data(void)
{
    yuga_t y = yuga_get(YUGA_DVAPARA);
    TEST_ASSERT_EQUAL_INT(2, y.id);
    TEST_ASSERT_EQUAL_STRING("Dvapara", y.name);
    TEST_ASSERT_EQUAL_INT64(DVAPARA_YUGA_YEARS, y.years);
    TEST_ASSERT_EQUAL_INT(2, y.ratio);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, y.dharma_fraction);
}

void test_kali_yuga_data(void)
{
    yuga_t y = yuga_get(YUGA_KALI);
    TEST_ASSERT_EQUAL_INT(3, y.id);
    TEST_ASSERT_EQUAL_STRING("Kali", y.name);
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS, y.years);
    TEST_ASSERT_EQUAL_INT(1, y.ratio);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, y.dharma_fraction);
}

/* ===== Invalid index ===== */

void test_get_invalid_negative(void)
{
    yuga_t y = yuga_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, y.id);
    TEST_ASSERT_EQUAL_INT64(0, y.years);
}

void test_get_invalid_high(void)
{
    yuga_t y = yuga_get(4);
    TEST_ASSERT_EQUAL_INT(-1, y.id);
    TEST_ASSERT_EQUAL_INT64(0, y.years);
}

/* ===== Names and meanings ===== */

void test_yuga_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Satya", yuga_name(YUGA_SATYA));
    TEST_ASSERT_EQUAL_STRING("Treta", yuga_name(YUGA_TRETA));
    TEST_ASSERT_EQUAL_STRING("Dvapara", yuga_name(YUGA_DVAPARA));
    TEST_ASSERT_EQUAL_STRING("Kali", yuga_name(YUGA_KALI));
}

void test_yuga_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", yuga_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", yuga_name(99));
}

void test_yuga_meanings(void)
{
    TEST_ASSERT_EQUAL_STRING("Truth/Golden Age", yuga_meaning(YUGA_SATYA));
    TEST_ASSERT_EQUAL_STRING("Third/Silver Age", yuga_meaning(YUGA_TRETA));
    TEST_ASSERT_EQUAL_STRING("Second/Bronze Age", yuga_meaning(YUGA_DVAPARA));
    TEST_ASSERT_EQUAL_STRING("Strife/Iron Age", yuga_meaning(YUGA_KALI));
}

void test_yuga_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", yuga_meaning(-1));
}

/* ===== Dharma fractions ===== */

void test_dharma_satya(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, yuga_dharma(YUGA_SATYA));
}

void test_dharma_kali(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, yuga_dharma(YUGA_KALI));
}

void test_dharma_invalid(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, yuga_dharma(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, yuga_dharma(4));
}

/* ===== Larger cycle calculations ===== */

void test_maha_yuga_years(void)
{
    TEST_ASSERT_EQUAL_INT64(4320000LL, yuga_maha_yuga_years());
}

void test_kalpa_years(void)
{
    TEST_ASSERT_EQUAL_INT64(4320000000LL, yuga_kalpa_years());
}

void test_manvantara_years(void)
{
    TEST_ASSERT_EQUAL_INT64(306720000LL, yuga_manvantara_years());
}

void test_yuga_count(void)
{
    TEST_ASSERT_EQUAL_INT(4, yuga_count());
}

/* ===== Yuga sum equals Maha-Yuga ===== */

void test_yuga_sum_equals_maha_yuga(void)
{
    long long sum = SATYA_YUGA_YEARS + TRETA_YUGA_YEARS
                  + DVAPARA_YUGA_YEARS + KALI_YUGA_YEARS;
    TEST_ASSERT_EQUAL_INT64(MAHA_YUGA_YEARS, sum);
}

/* ===== Ratio structure (4:3:2:1) ===== */

void test_ratio_structure(void)
{
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS * 4, SATYA_YUGA_YEARS);
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS * 3, TRETA_YUGA_YEARS);
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS * 2, DVAPARA_YUGA_YEARS);
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS * 1, KALI_YUGA_YEARS);
}

/* ===== Position: modern date (2026 CE) ===== */

void test_position_2026_ce(void)
{
    yuga_position_t p = yuga_for_year(2026);
    TEST_ASSERT_EQUAL_INT(YUGA_KALI, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(5127, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS - 5127, p.years_remaining);
    TEST_ASSERT_EQUAL_INT(1, p.maha_yuga_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 5127.0 / 432000.0, p.fraction);
}

/* ===== Position: epoch start (year 0 of Kali Yuga) ===== */

void test_position_epoch_start(void)
{
    yuga_position_t p = yuga_position(0);
    TEST_ASSERT_EQUAL_INT(YUGA_KALI, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(0, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT64(KALI_YUGA_YEARS, p.years_remaining);
    TEST_ASSERT_EQUAL_INT(1, p.maha_yuga_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.fraction);
}

/* ===== Position: end of Kali Yuga / start of next Maha-Yuga ===== */

void test_position_kali_end_boundary(void)
{
    /* At exactly 432,000 years: Kali Yuga ends, next Maha-Yuga Satya starts */
    yuga_position_t p = yuga_position(KALI_YUGA_YEARS);
    TEST_ASSERT_EQUAL_INT(YUGA_SATYA, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(0, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT(2, p.maha_yuga_number);
}

/* ===== Position: cycling through multiple Maha-Yugas ===== */

void test_position_second_maha_yuga_satya(void)
{
    /* 432,001 years -> 1 year into Satya of Maha-Yuga 2 */
    yuga_position_t p = yuga_position(KALI_YUGA_YEARS + 1);
    TEST_ASSERT_EQUAL_INT(YUGA_SATYA, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(1, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT(2, p.maha_yuga_number);
}

void test_position_second_maha_yuga_treta(void)
{
    /* Past Kali (432k) + past Satya (1,728k) + 1 year into Treta */
    long long years = KALI_YUGA_YEARS + SATYA_YUGA_YEARS + 1;
    yuga_position_t p = yuga_position(years);
    TEST_ASSERT_EQUAL_INT(YUGA_TRETA, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(1, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT(2, p.maha_yuga_number);
}

void test_position_third_maha_yuga(void)
{
    /* Past initial Kali + 1 full Maha-Yuga = 432,000 + 4,320,000 = 4,752,000
     * -> start of Maha-Yuga 3 */
    long long years = KALI_YUGA_YEARS + MAHA_YUGA_YEARS;
    yuga_position_t p = yuga_position(years);
    TEST_ASSERT_EQUAL_INT(YUGA_SATYA, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(0, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT(3, p.maha_yuga_number);
}

/* ===== Position: within Dvapara of Maha-Yuga 2 ===== */

void test_position_second_maha_yuga_dvapara(void)
{
    /* Past initial Kali + Satya + Treta + halfway into Dvapara */
    long long years = KALI_YUGA_YEARS + SATYA_YUGA_YEARS + TRETA_YUGA_YEARS
                    + DVAPARA_YUGA_YEARS / 2;
    yuga_position_t p = yuga_position(years);
    TEST_ASSERT_EQUAL_INT(YUGA_DVAPARA, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(DVAPARA_YUGA_YEARS / 2, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT(2, p.maha_yuga_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, p.fraction);
}

/* ===== Position from Julian Day ===== */

void test_position_from_jd_epoch(void)
{
    yuga_position_t p = yuga_position_from_jd(KALI_YUGA_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(YUGA_KALI, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(0, p.years_elapsed);
    TEST_ASSERT_EQUAL_INT(1, p.maha_yuga_number);
}

void test_position_from_jd_modern(void)
{
    /* J2000.0 = JD 2451545.0 = 2000-01-01 12:00 TT */
    double jd_2000 = 2451545.0;
    yuga_position_t p = yuga_position_from_jd(jd_2000);
    TEST_ASSERT_EQUAL_INT(YUGA_KALI, p.yuga_index);
    /* JD-based: (2451545.0 - 588465.5) / 365.25 = ~5100.17 -> floor = 5100
     * Slight difference from CE-based (5101) due to 365.25 approximation */
    TEST_ASSERT_EQUAL_INT64(5100, p.years_elapsed);
}

/* ===== Position for specific CE years ===== */

void test_for_year_epoch_year(void)
{
    /* -3101 CE (3102 BCE): year 0 of Kali Yuga */
    yuga_position_t p = yuga_for_year(KALI_YUGA_EPOCH_CE);
    TEST_ASSERT_EQUAL_INT(YUGA_KALI, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(0, p.years_elapsed);
}

void test_for_year_1_ce(void)
{
    /* 1 CE -> 1 + 3101 = 3102 years */
    yuga_position_t p = yuga_for_year(1);
    TEST_ASSERT_EQUAL_INT(YUGA_KALI, p.yuga_index);
    TEST_ASSERT_EQUAL_INT64(3102, p.years_elapsed);
}

/* ===== Constant consistency ===== */

void test_kalpa_equals_1000_maha_yugas(void)
{
    TEST_ASSERT_EQUAL_INT64(KALPA_YEARS, (long long)MAHA_YUGAS_PER_KALPA * MAHA_YUGA_YEARS);
}

void test_manvantara_equals_71_maha_yugas(void)
{
    TEST_ASSERT_EQUAL_INT64(yuga_manvantara_years(),
                            (long long)MANVANTARA_YUGAS * MAHA_YUGA_YEARS);
}

void test_divine_year(void)
{
    TEST_ASSERT_EQUAL_INT(360, DIVINE_YEAR);
}

/* ===== Description field populated ===== */

void test_descriptions_not_null(void)
{
    for (int i = 0; i < YUGA_COUNT; i++) {
        yuga_t y = yuga_get(i);
        TEST_ASSERT_NOT_NULL(y.description);
        TEST_ASSERT_TRUE(strlen(y.description) > 0);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Yuga data (4 yugas) */
    RUN_TEST(test_satya_yuga_data);
    RUN_TEST(test_treta_yuga_data);
    RUN_TEST(test_dvapara_yuga_data);
    RUN_TEST(test_kali_yuga_data);

    /* Invalid index */
    RUN_TEST(test_get_invalid_negative);
    RUN_TEST(test_get_invalid_high);

    /* Names and meanings */
    RUN_TEST(test_yuga_names);
    RUN_TEST(test_yuga_name_invalid);
    RUN_TEST(test_yuga_meanings);
    RUN_TEST(test_yuga_meaning_invalid);

    /* Dharma */
    RUN_TEST(test_dharma_satya);
    RUN_TEST(test_dharma_kali);
    RUN_TEST(test_dharma_invalid);

    /* Larger cycles */
    RUN_TEST(test_maha_yuga_years);
    RUN_TEST(test_kalpa_years);
    RUN_TEST(test_manvantara_years);
    RUN_TEST(test_yuga_count);

    /* Data consistency */
    RUN_TEST(test_yuga_sum_equals_maha_yuga);
    RUN_TEST(test_ratio_structure);

    /* Position calculations */
    RUN_TEST(test_position_2026_ce);
    RUN_TEST(test_position_epoch_start);
    RUN_TEST(test_position_kali_end_boundary);
    RUN_TEST(test_position_second_maha_yuga_satya);
    RUN_TEST(test_position_second_maha_yuga_treta);
    RUN_TEST(test_position_third_maha_yuga);
    RUN_TEST(test_position_second_maha_yuga_dvapara);

    /* JD position */
    RUN_TEST(test_position_from_jd_epoch);
    RUN_TEST(test_position_from_jd_modern);

    /* CE year position */
    RUN_TEST(test_for_year_epoch_year);
    RUN_TEST(test_for_year_1_ce);

    /* Constants */
    RUN_TEST(test_kalpa_equals_1000_maha_yugas);
    RUN_TEST(test_manvantara_equals_71_maha_yugas);
    RUN_TEST(test_divine_year);

    /* Descriptions */
    RUN_TEST(test_descriptions_not_null);

    return UNITY_END();
}
