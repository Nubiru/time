/* test_today_summary.c — Tests for the Today Summary module.
 * Validates ts_compute, ts_get_system, ts_significant_count,
 * ts_format_all, ts_format_brief, ts_system_name, ts_system_count. */

#include "unity.h"
#include "systems/unified/today_summary.h"
#include "math/julian.h"
#include <string.h>

/* J2000.0 epoch: 2000-01-01 12:00 UT */
#define JD_J2000 2451545.0

/* A known date: 2026-03-16 ~noon UT */
#define JD_20260316 2461322.0

/* A far past date: 1900-01-01 noon UT */
#define JD_19000101 2415020.5

/* A far future date: 2100-01-01 noon UT */
#define JD_21000101 2488069.5

void setUp(void) {}
void tearDown(void) {}

/* === ts_system_name tests === */

static void test_system_name_gregorian(void)
{
    const char *name = ts_system_name(TS_SYS_GREGORIAN);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Gregorian", name);
}

static void test_system_name_tzolkin(void)
{
    const char *name = ts_system_name(TS_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", name);
}

static void test_system_name_haab(void)
{
    const char *name = ts_system_name(TS_SYS_HAAB);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Haab", name);
}

static void test_system_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", ts_system_name(TS_SYS_CHINESE));
}

static void test_system_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", ts_system_name(TS_SYS_HEBREW));
}

static void test_system_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", ts_system_name(TS_SYS_ISLAMIC));
}

static void test_system_name_invalid(void)
{
    const char *name = ts_system_name((ts_system_t)-1);
    TEST_ASSERT_EQUAL_STRING("?", name);
}

static void test_system_name_all_valid(void)
{
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        const char *name = ts_system_name((ts_system_t)i);
        TEST_ASSERT_NOT_NULL(name);
        /* None should return "?" for valid system */
        TEST_ASSERT_TRUE(strcmp(name, "?") != 0);
    }
}

/* === ts_system_count tests === */

static void test_system_count(void)
{
    int count = ts_system_count();
    TEST_ASSERT_EQUAL_INT(TS_SYS_COUNT, count);
    TEST_ASSERT_TRUE(count >= 15); /* at least 15 systems */
}

/* === ts_compute basic tests === */

static void test_compute_j2000_returns_valid_summary(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000, s.jd);
    TEST_ASSERT_TRUE(s.entry_count > 0);
    TEST_ASSERT_TRUE(s.entry_count <= TS_MAX_SYSTEMS);
}

static void test_compute_gregorian_entry(void)
{
    /* J2000.0 = 2000-01-01 12:00 UT */
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GREGORIAN, e.system);
    /* date_str should contain "2000-01-01" */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "2000"));
}

static void test_compute_tzolkin_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_EQUAL_INT(TS_SYS_TZOLKIN, e.system);
    /* date_str should contain "Kin" */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "Kin"));
}

static void test_compute_haab_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_HAAB);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HAAB, e.system);
    /* Should have a date string */
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_chinese_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_CHINESE);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* J2000 = year 2000 => Dragon year */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "Dragon"));
}

static void test_compute_hebrew_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* Should have a year >= 5760 */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "57"));
}

static void test_compute_islamic_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_ISLAMIC);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* Hijri year around 1420 for J2000 */
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_buddhist_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* BE 2543 for year 2000 */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "2543"));
}

static void test_compute_hindu_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_HINDU);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_iching_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* Should mention a hexagram name */
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_coptic_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_COPTIC);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_ethiopian_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_ETHIOPIAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_persian_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_PERSIAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_japanese_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_JAPANESE);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* Should contain era name (Heisei for year 2000) */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "Heisei"));
}

static void test_compute_korean_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_KOREAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* Dangun year: 2000 + 2333 = 4333 (but before lunar new year, may be 4332).
     * Just verify the entry is active and has content. */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "Dangun"));
}

static void test_compute_thai_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_THAI);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* BE 2543 for year 2000 */
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "2543"));
}

static void test_compute_geological_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_GEOLOGICAL);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    /* Should mention Holocene or Phanerozoic */
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

static void test_compute_cosmic_entry(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, TS_SYS_COSMIC);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_TRUE(strlen(e.date_str) > 0);
}

/* === ts_get_system tests === */

static void test_get_system_not_found(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ts_entry_t e = ts_get_system(&s, (ts_system_t)99);
    TEST_ASSERT_EQUAL_INT(0, e.active);
}

/* === ts_significant_count tests === */

static void test_significant_count_threshold_0(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    int count = ts_significant_count(&s, 0);
    /* All active entries have significance >= 0 */
    TEST_ASSERT_EQUAL_INT(s.entry_count, count);
}

static void test_significant_count_threshold_high(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    int count = ts_significant_count(&s, 3);
    /* Very high threshold: few or none */
    TEST_ASSERT_TRUE(count >= 0);
    TEST_ASSERT_TRUE(count <= s.entry_count);
}

/* === ts_format_all tests === */

static void test_format_all_basic(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    char buf[2048];
    int len = ts_format_all(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    /* Should contain at least the Gregorian system name */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gregorian"));
}

static void test_format_all_small_buffer(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    char buf[32];
    int len = ts_format_all(&s, buf, sizeof(buf));
    /* Should not overflow, length <= buf_size - 1 */
    TEST_ASSERT_TRUE(len >= 0);
    TEST_ASSERT_TRUE((int)strlen(buf) < 32);
}

/* === ts_format_brief tests === */

static void test_format_brief_basic(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    char buf[512];
    int len = ts_format_brief(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    /* Should contain something meaningful */
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

/* === Convergence score tests === */

static void test_convergence_score_nonnegative(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    TEST_ASSERT_TRUE(s.convergence_score >= 0);
    TEST_ASSERT_TRUE(s.convergence_score <= 10);
}

/* === Headline tests === */

static void test_headline_populated(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    TEST_ASSERT_TRUE(strlen(s.headline) > 0);
}

/* === Edge case tests === */

static void test_compute_far_past(void)
{
    ts_summary_t s = ts_compute(JD_19000101);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_19000101, s.jd);
    TEST_ASSERT_TRUE(s.entry_count > 0);
    /* Gregorian should still work */
    ts_entry_t e = ts_get_system(&s, TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "1900"));
}

static void test_compute_far_future(void)
{
    ts_summary_t s = ts_compute(JD_21000101);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_21000101, s.jd);
    TEST_ASSERT_TRUE(s.entry_count > 0);
    ts_entry_t e = ts_get_system(&s, TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_NOT_NULL(strstr(e.date_str, "2100"));
}

/* === Significance level range test === */

static void test_significance_levels_in_range(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    for (int i = 0; i < s.entry_count; i++) {
        TEST_ASSERT_TRUE(s.entries[i].significance >= 0);
        TEST_ASSERT_TRUE(s.entries[i].significance <= 3);
    }
}

/* === Entry system IDs are correct === */

static void test_entry_system_ids_sequential(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    /* Each entry's system field should match its position */
    for (int i = 0; i < s.entry_count; i++) {
        TEST_ASSERT_EQUAL_INT(i, (int)s.entries[i].system);
    }
}

/* ===================================================================
 * Festival naming tests — Gregorian equinox/solstice
 * =================================================================== */

static void test_gregorian_spring_equinox(void)
{
    double jd = gregorian_to_jd(2026, 3, 20.0);
    ts_summary_t s = ts_compute(jd);
    ts_entry_t e = ts_get_system(&s, TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(1, e.active);
    TEST_ASSERT_EQUAL_INT(2, e.significance);
    TEST_ASSERT_NOT_NULL(strstr(e.note, "Spring Equinox"));
}

static void test_gregorian_summer_solstice(void)
{
    double jd = gregorian_to_jd(2026, 6, 21.0);
    ts_summary_t s = ts_compute(jd);
    ts_entry_t e = ts_get_system(&s, TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(2, e.significance);
    TEST_ASSERT_NOT_NULL(strstr(e.note, "Summer Solstice"));
}

static void test_gregorian_winter_solstice(void)
{
    double jd = gregorian_to_jd(2026, 12, 21.0);
    ts_summary_t s = ts_compute(jd);
    ts_entry_t e = ts_get_system(&s, TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(2, e.significance);
    TEST_ASSERT_NOT_NULL(strstr(e.note, "Winter Solstice"));
}

/* ===================================================================
 * Festival naming tests — Hebrew holidays
 * =================================================================== */

static void test_hebrew_pesach_named(void)
{
    /* Find 15 Nisan by scanning a year range */
    int found = 0;
    for (int d = 0; d < 400; d++) {
        double jd = gregorian_to_jd(2026, 1, 1.0) + (double)d;
        ts_summary_t s = ts_compute(jd);
        ts_entry_t e = ts_get_system(&s, TS_SYS_HEBREW);
        if (strstr(e.note, "Pesach")) {
            TEST_ASSERT_EQUAL_INT(2, e.significance);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

static void test_hebrew_sukkot_named(void)
{
    int found = 0;
    for (int d = 0; d < 400; d++) {
        double jd = gregorian_to_jd(2026, 1, 1.0) + (double)d;
        ts_summary_t s = ts_compute(jd);
        ts_entry_t e = ts_get_system(&s, TS_SYS_HEBREW);
        if (strstr(e.note, "Sukkot")) {
            TEST_ASSERT_EQUAL_INT(2, e.significance);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

static void test_hebrew_shavuot_named(void)
{
    int found = 0;
    for (int d = 0; d < 400; d++) {
        double jd = gregorian_to_jd(2026, 1, 1.0) + (double)d;
        ts_summary_t s = ts_compute(jd);
        ts_entry_t e = ts_get_system(&s, TS_SYS_HEBREW);
        if (strstr(e.note, "Shavuot")) {
            TEST_ASSERT_EQUAL_INT(2, e.significance);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===================================================================
 * Festival naming tests — Islamic holidays
 * =================================================================== */

static void test_islamic_eid_al_fitr_named(void)
{
    /* Scan for 1 Shawwal — should say "Eid al-Fitr" not "New month" */
    int found = 0;
    for (int d = 0; d < 400; d++) {
        double jd = gregorian_to_jd(2026, 1, 1.0) + (double)d;
        ts_summary_t s = ts_compute(jd);
        ts_entry_t e = ts_get_system(&s, TS_SYS_ISLAMIC);
        if (strstr(e.note, "Eid al-Fitr")) {
            TEST_ASSERT_EQUAL_INT(2, e.significance);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

static void test_islamic_eid_al_adha_named(void)
{
    int found = 0;
    for (int d = 0; d < 400; d++) {
        double jd = gregorian_to_jd(2026, 1, 1.0) + (double)d;
        ts_summary_t s = ts_compute(jd);
        ts_entry_t e = ts_get_system(&s, TS_SYS_ISLAMIC);
        if (strstr(e.note, "Eid al-Adha")) {
            TEST_ASSERT_EQUAL_INT(2, e.significance);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

static void test_islamic_mawlid_named(void)
{
    int found = 0;
    for (int d = 0; d < 400; d++) {
        double jd = gregorian_to_jd(2026, 1, 1.0) + (double)d;
        ts_summary_t s = ts_compute(jd);
        ts_entry_t e = ts_get_system(&s, TS_SYS_ISLAMIC);
        if (strstr(e.note, "Mawlid")) {
            TEST_ASSERT_EQUAL_INT(1, e.significance);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

int main(void)
{
    UNITY_BEGIN();

    /* System name tests (8) */
    RUN_TEST(test_system_name_gregorian);
    RUN_TEST(test_system_name_tzolkin);
    RUN_TEST(test_system_name_haab);
    RUN_TEST(test_system_name_chinese);
    RUN_TEST(test_system_name_hebrew);
    RUN_TEST(test_system_name_islamic);
    RUN_TEST(test_system_name_invalid);
    RUN_TEST(test_system_name_all_valid);

    /* System count (1) */
    RUN_TEST(test_system_count);

    /* Compute + entry tests (17) */
    RUN_TEST(test_compute_j2000_returns_valid_summary);
    RUN_TEST(test_compute_gregorian_entry);
    RUN_TEST(test_compute_tzolkin_entry);
    RUN_TEST(test_compute_haab_entry);
    RUN_TEST(test_compute_chinese_entry);
    RUN_TEST(test_compute_hebrew_entry);
    RUN_TEST(test_compute_islamic_entry);
    RUN_TEST(test_compute_buddhist_entry);
    RUN_TEST(test_compute_hindu_entry);
    RUN_TEST(test_compute_iching_entry);
    RUN_TEST(test_compute_coptic_entry);
    RUN_TEST(test_compute_ethiopian_entry);
    RUN_TEST(test_compute_persian_entry);
    RUN_TEST(test_compute_japanese_entry);
    RUN_TEST(test_compute_korean_entry);
    RUN_TEST(test_compute_thai_entry);
    RUN_TEST(test_compute_geological_entry);
    RUN_TEST(test_compute_cosmic_entry);

    /* ts_get_system (1) */
    RUN_TEST(test_get_system_not_found);

    /* Significant count (2) */
    RUN_TEST(test_significant_count_threshold_0);
    RUN_TEST(test_significant_count_threshold_high);

    /* Format tests (3) */
    RUN_TEST(test_format_all_basic);
    RUN_TEST(test_format_all_small_buffer);
    RUN_TEST(test_format_brief_basic);

    /* Convergence (1) */
    RUN_TEST(test_convergence_score_nonnegative);

    /* Headline (1) */
    RUN_TEST(test_headline_populated);

    /* Edge cases (2) */
    RUN_TEST(test_compute_far_past);
    RUN_TEST(test_compute_far_future);

    /* Data integrity (2) */
    RUN_TEST(test_significance_levels_in_range);
    RUN_TEST(test_entry_system_ids_sequential);

    /* Gregorian equinox/solstice naming (3) */
    RUN_TEST(test_gregorian_spring_equinox);
    RUN_TEST(test_gregorian_summer_solstice);
    RUN_TEST(test_gregorian_winter_solstice);

    /* Hebrew festival naming (3) */
    RUN_TEST(test_hebrew_pesach_named);
    RUN_TEST(test_hebrew_sukkot_named);
    RUN_TEST(test_hebrew_shavuot_named);

    /* Islamic festival naming (3) */
    RUN_TEST(test_islamic_eid_al_fitr_named);
    RUN_TEST(test_islamic_eid_al_adha_named);
    RUN_TEST(test_islamic_mawlid_named);

    return UNITY_END();
}
