/* test_lunar_harmonics.c -- Lunar Phase Harmonics tests (Track 54.2)
 *
 * TDD: tests written before implementation.
 * Tests Chinese Xiu (28 lodges), Arabic Manzil (28 mansions),
 * Brown Lunation Number, next/prev phase, cross-calendar month status,
 * and alignment detection. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/lunar_harmonics.h"
#include "../../../src/math/julian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helper: Julian Day for a known Gregorian date
 * =================================================================== */

static double jd_from(int y, int m, int d)
{
    return gregorian_to_jd(y, m, (double)d);
}

/* ===================================================================
 * Chinese Xiu (28 Lodges)
 * =================================================================== */

void test_xiu_count(void)
{
    TEST_ASSERT_EQUAL_INT(28, lunar_xiu_count());
}

void test_xiu_get_first(void)
{
    lunar_mansion_t x = lunar_xiu_get(0);
    TEST_ASSERT_EQUAL_INT(0, x.id);
    TEST_ASSERT_NOT_NULL(x.name);
    TEST_ASSERT_NOT_NULL(x.meaning);
    TEST_ASSERT_TRUE(x.span_deg > 0.0);
}

void test_xiu_get_last(void)
{
    lunar_mansion_t x = lunar_xiu_get(27);
    TEST_ASSERT_EQUAL_INT(27, x.id);
    TEST_ASSERT_NOT_NULL(x.name);
    TEST_ASSERT_NOT_NULL(x.meaning);
}

void test_xiu_get_invalid_returns_minus1(void)
{
    lunar_mansion_t x = lunar_xiu_get(28);
    TEST_ASSERT_EQUAL_INT(-1, x.id);
    x = lunar_xiu_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, x.id);
}

void test_xiu_name_valid(void)
{
    const char *name = lunar_xiu_name(0);
    TEST_ASSERT_NOT_NULL(name);
    /* First Xiu: Jiao (Horn) */
    TEST_ASSERT_EQUAL_STRING("Jiao", name);
}

void test_xiu_name_invalid(void)
{
    const char *name = lunar_xiu_name(28);
    TEST_ASSERT_EQUAL_STRING("?", name);
}

void test_xiu_from_longitude_horn(void)
{
    /* Jiao (Horn) starts near 202 deg ecliptic (Spica region) */
    lunar_mansion_t x = lunar_xiu_from_longitude(203.0);
    TEST_ASSERT_EQUAL_INT(0, x.id);
    TEST_ASSERT_EQUAL_STRING("Jiao", x.name);
}

void test_xiu_from_longitude_wraps(void)
{
    /* Test wrapping: longitude near 360 -> 0 */
    lunar_mansion_t x = lunar_xiu_from_longitude(361.0);
    TEST_ASSERT_TRUE(x.id >= 0 && x.id < 28);
}

void test_xiu_spans_cover_360(void)
{
    /* All 28 Xiu spans should sum to 360 degrees */
    double total = 0.0;
    for (int i = 0; i < 28; i++) {
        lunar_mansion_t x = lunar_xiu_get(i);
        total += x.span_deg;
    }
    TEST_ASSERT_FLOAT_WITHIN(0.5, 360.0, total);
}

void test_xiu_from_longitude_negative(void)
{
    /* Negative longitude should be normalized */
    lunar_mansion_t x = lunar_xiu_from_longitude(-10.0);
    TEST_ASSERT_TRUE(x.id >= 0 && x.id < 28);
}

/* ===================================================================
 * Arabic Manzil (28 Mansions)
 * =================================================================== */

void test_manzil_count(void)
{
    TEST_ASSERT_EQUAL_INT(28, lunar_manzil_count());
}

void test_manzil_get_first(void)
{
    lunar_mansion_t m = lunar_manzil_get(0);
    TEST_ASSERT_EQUAL_INT(0, m.id);
    TEST_ASSERT_NOT_NULL(m.name);
    TEST_ASSERT_NOT_NULL(m.meaning);
    TEST_ASSERT_TRUE(m.span_deg > 0.0);
}

void test_manzil_get_last(void)
{
    lunar_mansion_t m = lunar_manzil_get(27);
    TEST_ASSERT_EQUAL_INT(27, m.id);
    TEST_ASSERT_NOT_NULL(m.name);
}

void test_manzil_get_invalid(void)
{
    lunar_mansion_t m = lunar_manzil_get(28);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

void test_manzil_name_first(void)
{
    /* Al-Sharatain (The Two Signs) starts at Aries ~0 deg */
    const char *name = lunar_manzil_name(0);
    TEST_ASSERT_EQUAL_STRING("Al-Sharatain", name);
}

void test_manzil_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lunar_manzil_name(-1));
}

void test_manzil_from_longitude_aries(void)
{
    /* ~5 degrees should be in Al-Sharatain (starts at 0) */
    lunar_mansion_t m = lunar_manzil_from_longitude(5.0);
    TEST_ASSERT_EQUAL_INT(0, m.id);
}

void test_manzil_spans_cover_360(void)
{
    double total = 0.0;
    for (int i = 0; i < 28; i++) {
        lunar_mansion_t m = lunar_manzil_get(i);
        total += m.span_deg;
    }
    TEST_ASSERT_FLOAT_WITHIN(0.5, 360.0, total);
}

void test_manzil_from_longitude_wraps(void)
{
    lunar_mansion_t m = lunar_manzil_from_longitude(720.0);
    TEST_ASSERT_TRUE(m.id >= 0 && m.id < 28);
}

/* ===================================================================
 * Brown Lunation Number
 * =================================================================== */

void test_lunation_number_reference(void)
{
    /* Lunation 1: new moon of Jan 17, 1923 (~JD 2423436.6) */
    /* A date shortly after should be lunation 1 */
    int n = lunar_lunation_number(2423440.0);
    TEST_ASSERT_EQUAL_INT(1, n);
}

void test_lunation_number_modern(void)
{
    /* 2024-01-11 new moon is approximately lunation 1249 */
    double jd = jd_from(2024, 1, 12);
    int n = lunar_lunation_number(jd);
    /* Should be in the right ballpark */
    TEST_ASSERT_TRUE(n >= 1248 && n <= 1250);
}

void test_lunation_number_before_epoch(void)
{
    /* Before 1923 should give lunation <= 0 */
    double jd = jd_from(1922, 6, 15);
    int n = lunar_lunation_number(jd);
    TEST_ASSERT_TRUE(n <= 0);
}

void test_lunation_number_increases(void)
{
    /* 30 days apart should differ by 1 */
    double jd1 = jd_from(2000, 1, 10);
    double jd2 = jd1 + 35.0;
    int n1 = lunar_lunation_number(jd1);
    int n2 = lunar_lunation_number(jd2);
    TEST_ASSERT_TRUE(n2 > n1);
    TEST_ASSERT_TRUE(n2 - n1 <= 2);
}

/* ===================================================================
 * Lunation context
 * =================================================================== */

void test_lunation_context(void)
{
    double jd = jd_from(2024, 3, 15);
    lunation_t lun = lunar_lunation(jd);
    /* new_moon_jd should be before jd */
    TEST_ASSERT_TRUE(lun.new_moon_jd <= jd);
    /* next_new_moon_jd should be after jd */
    TEST_ASSERT_TRUE(lun.next_new_moon_jd > jd);
    /* full_moon should be between new and next_new */
    TEST_ASSERT_TRUE(lun.full_moon_jd >= lun.new_moon_jd);
    TEST_ASSERT_TRUE(lun.full_moon_jd <= lun.next_new_moon_jd);
    /* progress between 0 and 1 */
    TEST_ASSERT_TRUE(lun.progress >= 0.0 && lun.progress <= 1.0);
}

void test_lunation_number_matches(void)
{
    double jd = jd_from(2020, 7, 20);
    lunation_t lun = lunar_lunation(jd);
    int n = lunar_lunation_number(jd);
    TEST_ASSERT_EQUAL_INT(n, lun.lunation_number);
}

/* ===================================================================
 * Next/prev phase prediction
 * =================================================================== */

void test_next_phase_new_moon(void)
{
    double jd = jd_from(2024, 3, 1);
    double next = lunar_next_phase_of(jd, MOON_NEW);
    TEST_ASSERT_TRUE(next > jd);
    /* Should be within one synodic month */
    TEST_ASSERT_TRUE(next - jd < 30.0);
    /* Phase at that JD should be new moon */
    lunar_info_t info = lunar_phase(next);
    TEST_ASSERT_TRUE(info.phase_fraction < 0.05 || info.phase_fraction > 0.95);
}

void test_next_phase_full_moon(void)
{
    double jd = jd_from(2024, 6, 10);
    double next = lunar_next_phase_of(jd, MOON_FULL);
    TEST_ASSERT_TRUE(next > jd);
    TEST_ASSERT_TRUE(next - jd < 30.0);
    lunar_info_t info = lunar_phase(next);
    TEST_ASSERT_FLOAT_WITHIN(0.07, 0.5, info.phase_fraction);
}

void test_next_phase_first_quarter(void)
{
    double jd = jd_from(2024, 1, 1);
    double next = lunar_next_phase_of(jd, MOON_FIRST_QUARTER);
    TEST_ASSERT_TRUE(next > jd);
    TEST_ASSERT_TRUE(next - jd < 30.0);
    lunar_info_t info = lunar_phase(next);
    TEST_ASSERT_FLOAT_WITHIN(0.07, 0.25, info.phase_fraction);
}

void test_prev_phase_new_moon(void)
{
    double jd = jd_from(2024, 3, 15);
    double prev = lunar_prev_phase_of(jd, MOON_NEW);
    TEST_ASSERT_TRUE(prev < jd);
    TEST_ASSERT_TRUE(jd - prev < 30.0);
}

void test_prev_phase_full_moon(void)
{
    double jd = jd_from(2024, 6, 25);
    double prev = lunar_prev_phase_of(jd, MOON_FULL);
    TEST_ASSERT_TRUE(prev < jd);
    TEST_ASSERT_TRUE(jd - prev < 30.0);
    lunar_info_t info = lunar_phase(prev);
    TEST_ASSERT_FLOAT_WITHIN(0.07, 0.5, info.phase_fraction);
}

/* ===================================================================
 * Cross-calendar lunar month status
 * =================================================================== */

void test_system_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", lunar_system_name(LUNAR_SYS_HEBREW));
}

void test_system_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", lunar_system_name(LUNAR_SYS_ISLAMIC));
}

void test_system_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", lunar_system_name(LUNAR_SYS_CHINESE));
}

void test_system_name_buddhist(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddhist", lunar_system_name(LUNAR_SYS_BUDDHIST));
}

void test_system_name_hindu(void)
{
    TEST_ASSERT_EQUAL_STRING("Hindu", lunar_system_name(LUNAR_SYS_HINDU));
}

void test_month_status_hebrew(void)
{
    double jd = jd_from(2024, 3, 15);
    lunar_month_status_t s = lunar_month_status(jd, LUNAR_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(LUNAR_SYS_HEBREW, s.system);
    TEST_ASSERT_EQUAL_STRING("Hebrew", s.system_name);
    TEST_ASSERT_TRUE(s.approx_day_in_month >= 1 && s.approx_day_in_month <= 30);
    TEST_ASSERT_TRUE(s.month_length == 29 || s.month_length == 30);
}

void test_month_status_islamic(void)
{
    double jd = jd_from(2024, 3, 15);
    lunar_month_status_t s = lunar_month_status(jd, LUNAR_SYS_ISLAMIC);
    TEST_ASSERT_EQUAL_INT(LUNAR_SYS_ISLAMIC, s.system);
    /* Islamic months start after first visibility (~phase 0.03-0.05) */
    TEST_ASSERT_FLOAT_WITHIN(0.06, 0.04, s.phase_at_month_start);
}

void test_month_status_all(void)
{
    double jd = jd_from(2024, 6, 20);
    lunar_month_status_t out[LUNAR_SYSTEM_COUNT];
    lunar_month_status_all(jd, out);
    for (int i = 0; i < LUNAR_SYSTEM_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, out[i].system);
        TEST_ASSERT_NOT_NULL(out[i].system_name);
        TEST_ASSERT_TRUE(out[i].approx_day_in_month >= 1);
        TEST_ASSERT_TRUE(out[i].approx_day_in_month <= 30);
    }
}

void test_month_status_near_new_moon(void)
{
    /* Near a new moon, Hebrew/Chinese/Buddhist should show day ~1 */
    /* Use a known new moon: 2024-01-11 */
    double jd = jd_from(2024, 1, 12);
    lunar_month_status_t s = lunar_month_status(jd, LUNAR_SYS_HEBREW);
    /* Should be early in the month */
    TEST_ASSERT_TRUE(s.approx_day_in_month <= 5);
}

/* ===================================================================
 * Alignment detection
 * =================================================================== */

void test_alignment_at_new_moon(void)
{
    /* Near a new moon, multiple lunar systems should start months */
    /* Hebrew, Chinese, Buddhist all start near new moon (phase ~0) */
    /* So near a new moon, at least 3 should align within 2 days */
    double jd = jd_from(2024, 1, 12); /* day after new moon */
    int count = lunar_systems_aligned(jd, 2.0);
    /* At least Hebrew, Chinese, Buddhist share new-moon start */
    TEST_ASSERT_TRUE(count >= 3);
}

void test_alignment_mid_month(void)
{
    /* Mid-month: fewer alignments expected (maybe 0, depends on tolerance) */
    double jd = jd_from(2024, 3, 25);
    int count = lunar_systems_aligned(jd, 1.0);
    /* Could be 0 to 5, just verify it doesn't crash and is in range */
    TEST_ASSERT_TRUE(count >= 0 && count <= LUNAR_SYSTEM_COUNT);
}

void test_alignment_tight_tolerance(void)
{
    /* With 0 tolerance, few or no systems should align */
    double jd = jd_from(2024, 7, 10);
    int count = lunar_systems_aligned(jd, 0.0);
    TEST_ASSERT_TRUE(count >= 0 && count <= LUNAR_SYSTEM_COUNT);
}

/* ===================================================================
 * Edge cases
 * =================================================================== */

void test_xiu_from_longitude_exact_boundary(void)
{
    /* Use start_deg of Xiu 0 exactly */
    lunar_mansion_t x0 = lunar_xiu_get(0);
    lunar_mansion_t r = lunar_xiu_from_longitude(x0.start_deg);
    TEST_ASSERT_EQUAL_INT(0, r.id);
}

void test_manzil_from_longitude_exact_boundary(void)
{
    lunar_mansion_t m0 = lunar_manzil_get(0);
    lunar_mansion_t r = lunar_manzil_from_longitude(m0.start_deg);
    TEST_ASSERT_EQUAL_INT(0, r.id);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Xiu (12 tests) */
    RUN_TEST(test_xiu_count);
    RUN_TEST(test_xiu_get_first);
    RUN_TEST(test_xiu_get_last);
    RUN_TEST(test_xiu_get_invalid_returns_minus1);
    RUN_TEST(test_xiu_name_valid);
    RUN_TEST(test_xiu_name_invalid);
    RUN_TEST(test_xiu_from_longitude_horn);
    RUN_TEST(test_xiu_from_longitude_wraps);
    RUN_TEST(test_xiu_spans_cover_360);
    RUN_TEST(test_xiu_from_longitude_negative);
    RUN_TEST(test_xiu_from_longitude_exact_boundary);

    /* Manzil (8 tests) */
    RUN_TEST(test_manzil_count);
    RUN_TEST(test_manzil_get_first);
    RUN_TEST(test_manzil_get_last);
    RUN_TEST(test_manzil_get_invalid);
    RUN_TEST(test_manzil_name_first);
    RUN_TEST(test_manzil_name_invalid);
    RUN_TEST(test_manzil_from_longitude_aries);
    RUN_TEST(test_manzil_spans_cover_360);
    RUN_TEST(test_manzil_from_longitude_wraps);
    RUN_TEST(test_manzil_from_longitude_exact_boundary);

    /* Lunation number (4 tests) */
    RUN_TEST(test_lunation_number_reference);
    RUN_TEST(test_lunation_number_modern);
    RUN_TEST(test_lunation_number_before_epoch);
    RUN_TEST(test_lunation_number_increases);

    /* Lunation context (2 tests) */
    RUN_TEST(test_lunation_context);
    RUN_TEST(test_lunation_number_matches);

    /* Next/prev phase (5 tests) */
    RUN_TEST(test_next_phase_new_moon);
    RUN_TEST(test_next_phase_full_moon);
    RUN_TEST(test_next_phase_first_quarter);
    RUN_TEST(test_prev_phase_new_moon);
    RUN_TEST(test_prev_phase_full_moon);

    /* System names (5 tests) */
    RUN_TEST(test_system_name_hebrew);
    RUN_TEST(test_system_name_islamic);
    RUN_TEST(test_system_name_chinese);
    RUN_TEST(test_system_name_buddhist);
    RUN_TEST(test_system_name_hindu);

    /* Month status (4 tests) */
    RUN_TEST(test_month_status_hebrew);
    RUN_TEST(test_month_status_islamic);
    RUN_TEST(test_month_status_all);
    RUN_TEST(test_month_status_near_new_moon);

    /* Alignment (3 tests) */
    RUN_TEST(test_alignment_at_new_moon);
    RUN_TEST(test_alignment_mid_month);
    RUN_TEST(test_alignment_tight_tolerance);

    return UNITY_END();
}
