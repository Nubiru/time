/* test_buddhist.c — Buddhist calendar data module tests
 * TDD: tests match the API defined in task.md (Track 16.1).
 * Covers: Buddhist Era, Uposatha detection, month names, Vassa, Vesak,
 *         next_uposatha finder, invalid inputs, struct integrity. */

#include "../../unity/unity.h"
#include "../../../src/systems/buddhist/buddhist.h"
#include "../../../src/systems/astronomy/lunar.h"
#include <math.h>
#include <string.h>

/* Reference new moon: JD 2451550.26 (2000-01-06 18:14 UT)
 * Synodic month: 29.53058868 days
 * Quarter boundaries at phase_fraction: 0.0, 0.25, 0.5, 0.75 */
static const double REF_NEW_MOON = 2451550.26;
static const double SYNODIC = 29.53058868;

void setUp(void) {}
void tearDown(void) {}

/* ===== Buddhist Era conversion (CE -> BE) ===== */

void test_era_2026_ce(void)
{
    TEST_ASSERT_EQUAL_INT(2569, buddhist_era(2026));
}

void test_era_2000_ce(void)
{
    TEST_ASSERT_EQUAL_INT(2543, buddhist_era(2000));
}

void test_era_1_ce(void)
{
    TEST_ASSERT_EQUAL_INT(544, buddhist_era(1));
}

void test_era_0_ce(void)
{
    /* Year 0 CE (= 1 BCE) = 543 BE */
    TEST_ASSERT_EQUAL_INT(543, buddhist_era(0));
}

void test_era_negative_543(void)
{
    /* -543 CE = 0 BE */
    TEST_ASSERT_EQUAL_INT(0, buddhist_era(-543));
}

void test_era_negative_deep(void)
{
    TEST_ASSERT_EQUAL_INT(-457, buddhist_era(-1000));
}

/* ===== Buddhist Era to CE ===== */

void test_to_ce_2569(void)
{
    TEST_ASSERT_EQUAL_INT(2026, buddhist_to_ce(2569));
}

void test_to_ce_543(void)
{
    TEST_ASSERT_EQUAL_INT(0, buddhist_to_ce(543));
}

void test_to_ce_roundtrip(void)
{
    for (int y = -500; y <= 3000; y += 137) {
        TEST_ASSERT_EQUAL_INT(y, buddhist_to_ce(buddhist_era(y)));
    }
}

/* ===== Uposatha detection from lunar phase ===== */

void test_uposatha_new_moon(void)
{
    /* At the reference new moon */
    uposatha_t u = buddhist_uposatha(REF_NEW_MOON);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_NEW_MOON, u.type);
    TEST_ASSERT_NOT_NULL(u.name);
}

void test_uposatha_full_moon(void)
{
    double jd_full = REF_NEW_MOON + SYNODIC * 0.5;
    uposatha_t u = buddhist_uposatha(jd_full);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_FULL_MOON, u.type);
    TEST_ASSERT_TRUE(u.illumination > 0.95);
}

void test_uposatha_first_quarter(void)
{
    double jd_fq = REF_NEW_MOON + SYNODIC * 0.25;
    uposatha_t u = buddhist_uposatha(jd_fq);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_FIRST_QUARTER, u.type);
}

void test_uposatha_last_quarter(void)
{
    double jd_lq = REF_NEW_MOON + SYNODIC * 0.75;
    uposatha_t u = buddhist_uposatha(jd_lq);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_LAST_QUARTER, u.type);
}

void test_uposatha_none_midcycle(void)
{
    /* Between new moon and first quarter: phase_fraction ~0.125 */
    double jd_mid = REF_NEW_MOON + SYNODIC * 0.125;
    uposatha_t u = buddhist_uposatha(jd_mid);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_NONE, u.type);
}

void test_uposatha_illumination_new(void)
{
    uposatha_t u = buddhist_uposatha(REF_NEW_MOON);
    TEST_ASSERT_TRUE(u.illumination < 0.05);
}

void test_uposatha_moon_age_at_full(void)
{
    double jd_full = REF_NEW_MOON + SYNODIC * 0.5;
    uposatha_t u = buddhist_uposatha(jd_full);
    TEST_ASSERT_FLOAT_WITHIN(1.0, SYNODIC * 0.5, u.moon_age_days);
}

void test_uposatha_description_not_null(void)
{
    uposatha_t u = buddhist_uposatha(REF_NEW_MOON + SYNODIC * 0.5);
    TEST_ASSERT_NOT_NULL(u.description);
}

/* ===== Uposatha name lookup ===== */

void test_uposatha_name_purnima(void)
{
    TEST_ASSERT_EQUAL_STRING("Purnima", buddhist_uposatha_name(UPOSATHA_FULL_MOON));
}

void test_uposatha_name_amavasya(void)
{
    TEST_ASSERT_EQUAL_STRING("Amavasya", buddhist_uposatha_name(UPOSATHA_NEW_MOON));
}

void test_uposatha_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("None", buddhist_uposatha_name(UPOSATHA_NONE));
}

void test_uposatha_name_first_quarter(void)
{
    TEST_ASSERT_EQUAL_STRING("First Quarter", buddhist_uposatha_name(UPOSATHA_FIRST_QUARTER));
}

void test_uposatha_name_last_quarter(void)
{
    TEST_ASSERT_EQUAL_STRING("Last Quarter", buddhist_uposatha_name(UPOSATHA_LAST_QUARTER));
}

/* ===== Next Uposatha finder ===== */

void test_next_uposatha_advances(void)
{
    double jd = REF_NEW_MOON + 3.0;
    double next = buddhist_next_uposatha(jd);
    TEST_ASSERT_TRUE(next > jd);
}

void test_next_uposatha_within_quarter(void)
{
    /* Next uposatha should be at most ~7.4 days away */
    double jd = REF_NEW_MOON + 3.0;
    double next = buddhist_next_uposatha(jd);
    TEST_ASSERT_TRUE((next - jd) <= 8.0);
}

void test_next_uposatha_is_valid(void)
{
    /* The returned JD should itself be an uposatha day */
    double jd = REF_NEW_MOON + 2.0;
    double next = buddhist_next_uposatha(jd);
    uposatha_t u = buddhist_uposatha(next);
    TEST_ASSERT_TRUE(u.type != UPOSATHA_NONE);
}

void test_next_uposatha_from_exact_new(void)
{
    /* Starting at exact new moon + tiny offset, next should be first quarter */
    double next = buddhist_next_uposatha(REF_NEW_MOON + 0.5);
    double expected_fq = REF_NEW_MOON + SYNODIC * 0.25;
    TEST_ASSERT_FLOAT_WITHIN(1.5, expected_fq, next);
}

/* ===== Buddhist month names ===== */

void test_month_count(void)
{
    TEST_ASSERT_EQUAL_INT(12, buddhist_month_count());
}

void test_month_name_citta(void)
{
    TEST_ASSERT_EQUAL_STRING("Citta", buddhist_month_name(1));
}

void test_month_name_vesakha(void)
{
    TEST_ASSERT_EQUAL_STRING("Vesakha", buddhist_month_name(2));
}

void test_month_name_asalha(void)
{
    TEST_ASSERT_EQUAL_STRING("Asalha", buddhist_month_name(4));
}

void test_month_name_phalguna(void)
{
    TEST_ASSERT_EQUAL_STRING("Phalguna", buddhist_month_name(12));
}

void test_month_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", buddhist_month_name(0));
}

void test_month_name_invalid_13(void)
{
    TEST_ASSERT_EQUAL_STRING("?", buddhist_month_name(13));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", buddhist_month_name(-1));
}

/* ===== Buddhist month struct ===== */

void test_month_returns_valid_range(void)
{
    /* JD 2460310.5 ~ 2024-01-01 */
    buddhist_month_t m = buddhist_month(2460310.5);
    TEST_ASSERT_TRUE(m.month >= 1 && m.month <= 12);
    TEST_ASSERT_NOT_NULL(m.name);
}

void test_month_has_valid_name(void)
{
    buddhist_month_t m = buddhist_month(2460310.5);
    TEST_ASSERT_TRUE(strcmp(m.name, "?") != 0);
}

/* ===== Vassa (Rains Retreat) ===== */

void test_vassa_not_during_january(void)
{
    /* JD 2460324.5 ~ 2024-01-15. Definitely outside Vassa. */
    TEST_ASSERT_FALSE(buddhist_is_vassa(2460324.5));
}

void test_vassa_not_during_december(void)
{
    /* JD 2460659.5 ~ 2024-12-15 */
    TEST_ASSERT_FALSE(buddhist_is_vassa(2460659.5));
}

void test_vassa_not_during_march(void)
{
    /* JD 2460384.5 ~ 2024-03-15 */
    TEST_ASSERT_FALSE(buddhist_is_vassa(2460384.5));
}

/* ===== Vesak ===== */

void test_vesak_2024_in_may(void)
{
    /* Vesak 2024: full moon in May. Should fall between May 1 and June 1.
     * May 1 2024 ~ JD 2460432.5, June 1 ~ JD 2460463.5 */
    double vesak = buddhist_vesak_jd(2024);
    TEST_ASSERT_TRUE(vesak >= 2460432.5 && vesak <= 2460463.5);
}

void test_vesak_2025_in_may(void)
{
    /* May 1 2025 ~ JD 2460797.5, June 1 ~ JD 2460828.5 */
    double vesak = buddhist_vesak_jd(2025);
    TEST_ASSERT_TRUE(vesak >= 2460797.5 && vesak <= 2460828.5);
}

void test_vesak_is_full_moon(void)
{
    double vesak = buddhist_vesak_jd(2024);
    lunar_info_t info = lunar_phase(vesak);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.5, info.phase_fraction);
}

void test_vesak_different_years(void)
{
    double v2024 = buddhist_vesak_jd(2024);
    double v2025 = buddhist_vesak_jd(2025);
    TEST_ASSERT_TRUE(fabs(v2025 - v2024) > 300.0);
    TEST_ASSERT_TRUE(fabs(v2025 - v2024) < 400.0);
}

/* ===== Struct integrity / purity ===== */

void test_uposatha_struct_all_fields(void)
{
    double jd_full = REF_NEW_MOON + SYNODIC * 0.5;
    uposatha_t u = buddhist_uposatha(jd_full);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_FULL_MOON, u.type);
    TEST_ASSERT_NOT_NULL(u.name);
    TEST_ASSERT_NOT_NULL(u.description);
    TEST_ASSERT_TRUE(u.moon_age_days >= 0.0);
    TEST_ASSERT_TRUE(u.illumination >= 0.0 && u.illumination <= 1.0);
}

void test_month_struct_all_fields(void)
{
    buddhist_month_t m = buddhist_month(2460310.5);
    TEST_ASSERT_TRUE(m.month >= 1 && m.month <= 12);
    TEST_ASSERT_NOT_NULL(m.name);
    /* is_vassa is bool, no null check needed */
}

/* ===== Main runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Buddhist Era (6 tests) */
    RUN_TEST(test_era_2026_ce);
    RUN_TEST(test_era_2000_ce);
    RUN_TEST(test_era_1_ce);
    RUN_TEST(test_era_0_ce);
    RUN_TEST(test_era_negative_543);
    RUN_TEST(test_era_negative_deep);

    /* BE to CE (3 tests) */
    RUN_TEST(test_to_ce_2569);
    RUN_TEST(test_to_ce_543);
    RUN_TEST(test_to_ce_roundtrip);

    /* Uposatha detection (8 tests) */
    RUN_TEST(test_uposatha_new_moon);
    RUN_TEST(test_uposatha_full_moon);
    RUN_TEST(test_uposatha_first_quarter);
    RUN_TEST(test_uposatha_last_quarter);
    RUN_TEST(test_uposatha_none_midcycle);
    RUN_TEST(test_uposatha_illumination_new);
    RUN_TEST(test_uposatha_moon_age_at_full);
    RUN_TEST(test_uposatha_description_not_null);

    /* Uposatha names (5 tests) */
    RUN_TEST(test_uposatha_name_purnima);
    RUN_TEST(test_uposatha_name_amavasya);
    RUN_TEST(test_uposatha_name_none);
    RUN_TEST(test_uposatha_name_first_quarter);
    RUN_TEST(test_uposatha_name_last_quarter);

    /* Next Uposatha (4 tests) */
    RUN_TEST(test_next_uposatha_advances);
    RUN_TEST(test_next_uposatha_within_quarter);
    RUN_TEST(test_next_uposatha_is_valid);
    RUN_TEST(test_next_uposatha_from_exact_new);

    /* Month names (8 tests) */
    RUN_TEST(test_month_count);
    RUN_TEST(test_month_name_citta);
    RUN_TEST(test_month_name_vesakha);
    RUN_TEST(test_month_name_asalha);
    RUN_TEST(test_month_name_phalguna);
    RUN_TEST(test_month_name_invalid_zero);
    RUN_TEST(test_month_name_invalid_13);
    RUN_TEST(test_month_name_invalid_negative);

    /* Month struct (2 tests) */
    RUN_TEST(test_month_returns_valid_range);
    RUN_TEST(test_month_has_valid_name);

    /* Vassa (3 tests) */
    RUN_TEST(test_vassa_not_during_january);
    RUN_TEST(test_vassa_not_during_december);
    RUN_TEST(test_vassa_not_during_march);

    /* Vesak (4 tests) */
    RUN_TEST(test_vesak_2024_in_may);
    RUN_TEST(test_vesak_2025_in_may);
    RUN_TEST(test_vesak_is_full_moon);
    RUN_TEST(test_vesak_different_years);

    /* Struct integrity (2 tests) */
    RUN_TEST(test_uposatha_struct_all_fields);
    RUN_TEST(test_month_struct_all_fields);

    return UNITY_END();
}
