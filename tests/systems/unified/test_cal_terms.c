/* test_cal_terms.c -- Calendar Term Database tests.
 * Tests term retrieval, system-based filtering, kind-based filtering,
 * combined system+kind filtering, positional lookup, script validation,
 * and name lookups for enums.
 *
 * 115 terms: 12 Gregorian months + 20 Tzolkin seals + 13 Tzolkin tones
 * + 12 Hebrew months + 12 Islamic months + 10 Chinese stems
 * + 12 Chinese branches + 12 Hindu months + 12 Astrology signs. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/cal_terms.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== cal_term_count ===== */

void test_cal_term_total_count(void)
{
    TEST_ASSERT_EQUAL_INT(115, cal_term_count());
}

/* ===== cal_term_get ===== */

void test_cal_term_get_valid(void)
{
    cal_term_t t = cal_term_get(0);
    TEST_ASSERT_EQUAL_INT(0, t.id);
    TEST_ASSERT_NOT_NULL(t.native);
    TEST_ASSERT_NOT_NULL(t.transliterated);
    TEST_ASSERT_NOT_NULL(t.translated);
    TEST_ASSERT_NOT_NULL(t.script);
}

void test_cal_term_get_invalid(void)
{
    cal_term_t neg = cal_term_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, neg.id);
    cal_term_t big = cal_term_get(999);
    TEST_ASSERT_EQUAL_INT(-1, big.id);
}

/* ===== System counts ===== */

void test_gregorian_months_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_GREGORIAN, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(12, count);
}

void test_tzolkin_seals_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system_kind(CAL_SYS_TZOLKIN, CAL_KIND_DAY,
                                          results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(20, count);
}

void test_tzolkin_tones_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system_kind(CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,
                                          results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(13, count);
}

void test_hebrew_months_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_HEBREW, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(12, count);
    /* All Hebrew terms should be MONTH kind */
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(results[i]);
        TEST_ASSERT_EQUAL_INT(CAL_KIND_MONTH, t.kind);
    }
}

void test_islamic_months_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_ISLAMIC, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(12, count);
}

void test_chinese_stems_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system_kind(CAL_SYS_CHINESE, CAL_KIND_ELEMENT,
                                          results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(10, count);
}

void test_chinese_branches_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system_kind(CAL_SYS_CHINESE, CAL_KIND_SIGN,
                                          results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(12, count);
}

void test_hindu_months_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_HINDU, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(12, count);
}

void test_astrology_signs_count(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_ASTROLOGY, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(12, count);
}

/* ===== Positional lookups ===== */

void test_lookup_tzolkin_seal_1(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_TZOLKIN, CAL_KIND_DAY, 1);
    TEST_ASSERT_EQUAL_STRING("Imix", t.transliterated);
}

void test_lookup_tzolkin_seal_20(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_TZOLKIN, CAL_KIND_DAY, 20);
    TEST_ASSERT_EQUAL_STRING("Ahau", t.transliterated);
}

void test_lookup_hebrew_month_1(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_HEBREW, CAL_KIND_MONTH, 1);
    TEST_ASSERT_EQUAL_STRING("Tishrei", t.transliterated);
}

void test_lookup_islamic_month_9(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_ISLAMIC, CAL_KIND_MONTH, 9);
    TEST_ASSERT_EQUAL_STRING("Ramadan", t.transliterated);
}

void test_lookup_chinese_branch_1(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_CHINESE, CAL_KIND_SIGN, 1);
    TEST_ASSERT_EQUAL_STRING("Zi", t.transliterated);
}

void test_lookup_hindu_month_1(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_HINDU, CAL_KIND_MONTH, 1);
    TEST_ASSERT_EQUAL_STRING("Chaitra", t.transliterated);
}

void test_lookup_astrology_sign_1(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_ASTROLOGY, CAL_KIND_SIGN, 1);
    TEST_ASSERT_EQUAL_STRING("Aries", t.transliterated);
}

void test_lookup_invalid(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_TZOLKIN, CAL_KIND_DAY, 99);
    TEST_ASSERT_EQUAL_INT(-1, t.id);
}

/* ===== Field validation ===== */

void test_all_have_native(void)
{
    int count = cal_term_count();
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(i);
        TEST_ASSERT_NOT_NULL(t.native);
    }
}

void test_all_have_transliterated(void)
{
    int count = cal_term_count();
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(i);
        TEST_ASSERT_NOT_NULL(t.transliterated);
    }
}

void test_all_have_script(void)
{
    int count = cal_term_count();
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(i);
        TEST_ASSERT_NOT_NULL(t.script);
    }
}

/* ===== Script validation ===== */

void test_hebrew_script(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_HEBREW, results, CAL_TERM_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(results[i]);
        TEST_ASSERT_EQUAL_STRING("Hebr", t.script);
    }
}

void test_islamic_script(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_ISLAMIC, results, CAL_TERM_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(results[i]);
        TEST_ASSERT_EQUAL_STRING("Arab", t.script);
    }
}

void test_chinese_script(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_CHINESE, results, CAL_TERM_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(results[i]);
        TEST_ASSERT_EQUAL_STRING("Hans", t.script);
    }
}

void test_hindu_script(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_HINDU, results, CAL_TERM_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cal_term_t t = cal_term_get(results[i]);
        TEST_ASSERT_EQUAL_STRING("Deva", t.script);
    }
}

/* ===== Name lookups ===== */

void test_system_name_valid(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", cal_system_name(CAL_SYS_GREGORIAN));
}

void test_system_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cal_system_name((cal_system_t)99));
}

void test_kind_name_valid(void)
{
    TEST_ASSERT_EQUAL_STRING("Month", cal_kind_name(CAL_KIND_MONTH));
}

void test_kind_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cal_kind_name((cal_kind_t)99));
}

/* ===== by_kind totals ===== */

void test_by_kind_month_total(void)
{
    int results[64];
    int count = cal_terms_by_kind(CAL_KIND_MONTH, results, 64);
    /* 12 Gregorian + 12 Hebrew + 12 Islamic + 12 Hindu + 12 Astrology(no) = 60 */
    /* Actually: Gregorian(12) + Hebrew(12) + Islamic(12) + Hindu(12) = 48.
     * Astrology uses SIGN not MONTH. Total MONTH = 48? No, spec says 60.
     * 12+12+12+12+12 = 60: Greg+Hebrew+Islamic+Hindu+??? The 5th set of 12
     * months... wait, no. Let me recount: Gregorian(12 MONTH) + Hebrew(12 MONTH)
     * + Islamic(12 MONTH) + Hindu(12 MONTH) = 48. Astrology is SIGN(12).
     * That's only 48 months. But the spec says by_kind(MONTH)==60.
     * The spec counts 12+12+12+12+12 = 60, including all 5 MONTH sets.
     * But we only have 4 MONTH systems (Gregorian, Hebrew, Islamic, Hindu).
     * Astrology is SIGN. So MONTH total = 48. */
    TEST_ASSERT_EQUAL_INT(48, count);
}

void test_by_kind_sign_total(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_kind(CAL_KIND_SIGN, results, CAL_TERM_MAX_RESULTS);
    /* Chinese branches(12) + Astrology signs(12) = 24 */
    TEST_ASSERT_EQUAL_INT(24, count);
}

void test_by_kind_day_total(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_kind(CAL_KIND_DAY, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(20, count);
}

void test_by_kind_cycle_total(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_kind(CAL_KIND_CYCLE, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(13, count);
}

void test_by_kind_element_total(void)
{
    int results[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_kind(CAL_KIND_ELEMENT, results, CAL_TERM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(10, count);
}

/* ===== Edge cases ===== */

void test_by_system_null_results(void)
{
    int count = cal_terms_by_system(CAL_SYS_GREGORIAN, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_system_zero_max(void)
{
    int buf[CAL_TERM_MAX_RESULTS];
    int count = cal_terms_by_system(CAL_SYS_GREGORIAN, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== Tone content validation ===== */

void test_tone_1_translated(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_TZOLKIN, CAL_KIND_CYCLE, 1);
    TEST_ASSERT_EQUAL_STRING("Unity", t.translated);
}

void test_tone_13_translated(void)
{
    cal_term_t t = cal_term_lookup(CAL_SYS_TZOLKIN, CAL_KIND_CYCLE, 13);
    TEST_ASSERT_EQUAL_STRING("Presence", t.translated);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_cal_term_total_count);

    /* Get */
    RUN_TEST(test_cal_term_get_valid);
    RUN_TEST(test_cal_term_get_invalid);

    /* System counts */
    RUN_TEST(test_gregorian_months_count);
    RUN_TEST(test_tzolkin_seals_count);
    RUN_TEST(test_tzolkin_tones_count);
    RUN_TEST(test_hebrew_months_count);
    RUN_TEST(test_islamic_months_count);
    RUN_TEST(test_chinese_stems_count);
    RUN_TEST(test_chinese_branches_count);
    RUN_TEST(test_hindu_months_count);
    RUN_TEST(test_astrology_signs_count);

    /* Positional lookups */
    RUN_TEST(test_lookup_tzolkin_seal_1);
    RUN_TEST(test_lookup_tzolkin_seal_20);
    RUN_TEST(test_lookup_hebrew_month_1);
    RUN_TEST(test_lookup_islamic_month_9);
    RUN_TEST(test_lookup_chinese_branch_1);
    RUN_TEST(test_lookup_hindu_month_1);
    RUN_TEST(test_lookup_astrology_sign_1);
    RUN_TEST(test_lookup_invalid);

    /* Field validation */
    RUN_TEST(test_all_have_native);
    RUN_TEST(test_all_have_transliterated);
    RUN_TEST(test_all_have_script);

    /* Script validation */
    RUN_TEST(test_hebrew_script);
    RUN_TEST(test_islamic_script);
    RUN_TEST(test_chinese_script);
    RUN_TEST(test_hindu_script);

    /* Name lookups */
    RUN_TEST(test_system_name_valid);
    RUN_TEST(test_system_name_invalid);
    RUN_TEST(test_kind_name_valid);
    RUN_TEST(test_kind_name_invalid);

    /* by_kind totals */
    RUN_TEST(test_by_kind_month_total);
    RUN_TEST(test_by_kind_sign_total);
    RUN_TEST(test_by_kind_day_total);
    RUN_TEST(test_by_kind_cycle_total);
    RUN_TEST(test_by_kind_element_total);

    /* Edge cases */
    RUN_TEST(test_by_system_null_results);
    RUN_TEST(test_by_system_zero_max);

    /* Tone content */
    RUN_TEST(test_tone_1_translated);
    RUN_TEST(test_tone_13_translated);

    return UNITY_END();
}
