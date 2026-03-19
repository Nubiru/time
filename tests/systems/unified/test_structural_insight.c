/* test_structural_insight.c — Tests for cross-system structural insight database
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/structural_insight.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== si_all_count ========== */

static void test_all_count_at_least_30(void)
{
    TEST_ASSERT_GREATER_OR_EQUAL(30, si_all_count());
}

/* ========== si_query — specific pairs ========== */

static void test_query_kabbalah_human_design(void)
{
    si_result_t r = si_query(TS_SYS_KABBALAH, TS_SYS_HUMAN_DESIGN);
    TEST_ASSERT_GREATER_OR_EQUAL(2, r.count);
    /* Should contain "The Tree and the Graph" + "22 Paths, 36 Channels" */
    int found_tree = 0;
    int found_paths = 0;
    for (int i = 0; i < r.count; i++) {
        if (strcmp(r.entries[i].title, "The Tree and the Graph") == 0)
            found_tree = 1;
        if (strcmp(r.entries[i].title, "22 Paths, 36 Channels") == 0)
            found_paths = 1;
    }
    TEST_ASSERT_TRUE(found_tree);
    TEST_ASSERT_TRUE(found_paths);
}

static void test_query_iching_human_design(void)
{
    si_result_t r = si_query(TS_SYS_ICHING, TS_SYS_HUMAN_DESIGN);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
    TEST_ASSERT_EQUAL_STRING("The 64-fold Mirror", r.entries[0].title);
}

static void test_query_iching_tzolkin(void)
{
    si_result_t r = si_query(TS_SYS_ICHING, TS_SYS_TZOLKIN);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
}

static void test_query_hindu_buddhist(void)
{
    si_result_t r = si_query(TS_SYS_HINDU, TS_SYS_BUDDHIST);
    TEST_ASSERT_GREATER_OR_EQUAL(2, r.count);
}

static void test_query_hebrew_islamic(void)
{
    si_result_t r = si_query(TS_SYS_HEBREW, TS_SYS_ISLAMIC);
    TEST_ASSERT_GREATER_OR_EQUAL(2, r.count);
}

static void test_query_astrology_kabbalah(void)
{
    si_result_t r = si_query(TS_SYS_ASTROLOGY, TS_SYS_KABBALAH);
    TEST_ASSERT_GREATER_OR_EQUAL(2, r.count);
}

/* ========== si_query — commutativity ========== */

static void test_query_commutative_kabbalah_hd(void)
{
    si_result_t r1 = si_query(TS_SYS_KABBALAH, TS_SYS_HUMAN_DESIGN);
    si_result_t r2 = si_query(TS_SYS_HUMAN_DESIGN, TS_SYS_KABBALAH);
    TEST_ASSERT_EQUAL_INT(r1.count, r2.count);
}

static void test_query_commutative_iching_tzolkin(void)
{
    si_result_t r1 = si_query(TS_SYS_ICHING, TS_SYS_TZOLKIN);
    si_result_t r2 = si_query(TS_SYS_TZOLKIN, TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(r1.count, r2.count);
}

static void test_query_commutative_entries_match(void)
{
    si_result_t r1 = si_query(TS_SYS_HINDU, TS_SYS_BUDDHIST);
    si_result_t r2 = si_query(TS_SYS_BUDDHIST, TS_SYS_HINDU);
    TEST_ASSERT_EQUAL_INT(r1.count, r2.count);
    /* Same titles in same order */
    for (int i = 0; i < r1.count; i++) {
        TEST_ASSERT_EQUAL_STRING(r1.entries[i].title, r2.entries[i].title);
    }
}

/* ========== si_query — no pair ========== */

static void test_query_no_pair_returns_zero(void)
{
    /* Gregorian-Geological: no insight defined */
    si_result_t r = si_query(TS_SYS_GREGORIAN, TS_SYS_GEOLOGICAL);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

/* ========== si_query — invalid system ========== */

static void test_query_invalid_system_returns_zero(void)
{
    si_result_t r = si_query(-1, TS_SYS_KABBALAH);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

static void test_query_huge_system_returns_zero(void)
{
    si_result_t r = si_query(9999, TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

/* ========== si_get — valid indices ========== */

static void test_get_first(void)
{
    si_entry_t e = si_get(0);
    TEST_ASSERT_NOT_EQUAL(-1, e.system_a);
    TEST_ASSERT_TRUE(strlen(e.title) > 0);
    TEST_ASSERT_TRUE(strlen(e.insight) > 0);
}

static void test_get_last(void)
{
    int n = si_all_count();
    si_entry_t e = si_get(n - 1);
    TEST_ASSERT_NOT_EQUAL(-1, e.system_a);
    TEST_ASSERT_TRUE(strlen(e.title) > 0);
}

/* ========== si_get — invalid index ========== */

static void test_get_negative_invalid(void)
{
    si_entry_t e = si_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.system_a);
    TEST_ASSERT_EQUAL_STRING("?", e.title);
}

static void test_get_too_large(void)
{
    si_entry_t e = si_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, e.system_a);
    TEST_ASSERT_EQUAL_STRING("?", e.title);
}

/* ========== si_get — all entries valid ========== */

static void test_get_all_entries_have_content(void)
{
    int n = si_all_count();
    for (int i = 0; i < n; i++) {
        si_entry_t e = si_get(i);
        TEST_ASSERT_TRUE_MESSAGE(strlen(e.title) > 0, "empty title");
        TEST_ASSERT_TRUE_MESSAGE(strlen(e.insight) > 0, "empty insight");
        TEST_ASSERT_TRUE_MESSAGE(e.depth >= 1 && e.depth <= 3, "depth out of range");
    }
}

static void test_all_entries_depth_in_range(void)
{
    int n = si_all_count();
    for (int i = 0; i < n; i++) {
        si_entry_t e = si_get(i);
        TEST_ASSERT_GREATER_OR_EQUAL(1, e.depth);
        TEST_ASSERT_LESS_OR_EQUAL(3, e.depth);
    }
}

/* ========== si_get — no duplicate titles within same pair ========== */

static void test_no_duplicate_titles_per_pair(void)
{
    int n = si_all_count();
    for (int i = 0; i < n; i++) {
        si_entry_t ei = si_get(i);
        for (int j = i + 1; j < n; j++) {
            si_entry_t ej = si_get(j);
            /* Same pair? */
            int same_pair = (ei.system_a == ej.system_a && ei.system_b == ej.system_b) ||
                            (ei.system_a == ej.system_b && ei.system_b == ej.system_a);
            if (same_pair) {
                TEST_ASSERT_NOT_EQUAL_MESSAGE(0, strcmp(ei.title, ej.title),
                                               "duplicate title within same pair");
            }
        }
    }
}

/* ========== si_pairs_with ========== */

static void test_pairs_with_kabbalah(void)
{
    int partners[16];
    int count = si_pairs_with(TS_SYS_KABBALAH, partners, 16);
    TEST_ASSERT_GREATER_OR_EQUAL(4, count);
    /* Should include HUMAN_DESIGN, ASTROLOGY, HEBREW, ICHING */
    int found_hd = 0, found_astro = 0, found_heb = 0, found_ic = 0;
    for (int i = 0; i < count; i++) {
        if (partners[i] == TS_SYS_HUMAN_DESIGN) found_hd = 1;
        if (partners[i] == TS_SYS_ASTROLOGY) found_astro = 1;
        if (partners[i] == TS_SYS_HEBREW) found_heb = 1;
        if (partners[i] == TS_SYS_ICHING) found_ic = 1;
    }
    TEST_ASSERT_TRUE(found_hd);
    TEST_ASSERT_TRUE(found_astro);
    TEST_ASSERT_TRUE(found_heb);
    TEST_ASSERT_TRUE(found_ic);
}

static void test_pairs_with_geological(void)
{
    int partners[16];
    int count = si_pairs_with(TS_SYS_GEOLOGICAL, partners, 16);
    TEST_ASSERT_GREATER_OR_EQUAL(2, count);
    /* Should include COSMIC and BUDDHIST */
    int found_cosmic = 0, found_buddhist = 0;
    for (int i = 0; i < count; i++) {
        if (partners[i] == TS_SYS_COSMIC) found_cosmic = 1;
        if (partners[i] == TS_SYS_BUDDHIST) found_buddhist = 1;
    }
    TEST_ASSERT_TRUE(found_cosmic);
    TEST_ASSERT_TRUE(found_buddhist);
}

static void test_pairs_with_null_returns_count(void)
{
    int count = si_pairs_with(TS_SYS_KABBALAH, NULL, 0);
    TEST_ASSERT_GREATER_OR_EQUAL(4, count);
}

static void test_pairs_with_invalid_system(void)
{
    int partners[8];
    int count = si_pairs_with(-1, partners, 8);
    TEST_ASSERT_EQUAL_INT(0, count);
}

static void test_pairs_with_small_buffer(void)
{
    int partners[2];
    int count = si_pairs_with(TS_SYS_KABBALAH, partners, 2);
    /* Should cap at buffer size */
    TEST_ASSERT_LESS_OR_EQUAL(2, count);
    TEST_ASSERT_GREATER_OR_EQUAL(2, count);
}

/* ========== si_depth_name ========== */

static void test_depth_name_surface(void)
{
    TEST_ASSERT_EQUAL_STRING("Surface", si_depth_name(1));
}

static void test_depth_name_intermediate(void)
{
    TEST_ASSERT_EQUAL_STRING("Intermediate", si_depth_name(2));
}

static void test_depth_name_deep(void)
{
    TEST_ASSERT_EQUAL_STRING("Deep", si_depth_name(3));
}

static void test_depth_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", si_depth_name(0));
}

static void test_depth_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", si_depth_name(-5));
}

static void test_depth_name_invalid_four(void)
{
    TEST_ASSERT_EQUAL_STRING("?", si_depth_name(4));
}

/* ========== Extended system insights ========== */

static void test_query_coptic_egyptian(void)
{
    si_result_t r = si_query(TS_SYS_COPTIC, SI_SYS_EGYPTIAN);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
}

static void test_query_persian_zoroastrian(void)
{
    si_result_t r = si_query(TS_SYS_PERSIAN, SI_SYS_ZOROASTRIAN);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
}

static void test_query_celtic_astrology(void)
{
    si_result_t r = si_query(SI_SYS_CELTIC, TS_SYS_ASTROLOGY);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
}

/* ========== Max insights per pair ========== */

static void test_max_insights_per_pair(void)
{
    /* No pair should exceed SI_MAX_INSIGHTS */
    int n = si_all_count();
    for (int i = 0; i < n; i++) {
        si_entry_t ei = si_get(i);
        si_result_t r = si_query(ei.system_a, ei.system_b);
        TEST_ASSERT_LESS_OR_EQUAL(SI_MAX_INSIGHTS, r.count);
    }
}

/* ========== Specific insight content spot checks ========== */

static void test_64_fold_mirror_content(void)
{
    si_result_t r = si_query(TS_SYS_ICHING, TS_SYS_HUMAN_DESIGN);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
    /* Should mention "64" in the insight text */
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (strstr(r.entries[i].insight, "64") != NULL) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

static void test_gregorian_tzolkin_content(void)
{
    si_result_t r = si_query(TS_SYS_GREGORIAN, TS_SYS_TZOLKIN);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
    TEST_ASSERT_EQUAL_STRING("365 and 260", r.entries[0].title);
}

static void test_chinese_hebrew_lunisolar(void)
{
    si_result_t r = si_query(TS_SYS_CHINESE, TS_SYS_HEBREW);
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
}

int main(void)
{
    UNITY_BEGIN();

    /* si_all_count */
    RUN_TEST(test_all_count_at_least_30);

    /* si_query — specific pairs */
    RUN_TEST(test_query_kabbalah_human_design);
    RUN_TEST(test_query_iching_human_design);
    RUN_TEST(test_query_iching_tzolkin);
    RUN_TEST(test_query_hindu_buddhist);
    RUN_TEST(test_query_hebrew_islamic);
    RUN_TEST(test_query_astrology_kabbalah);

    /* si_query — commutativity */
    RUN_TEST(test_query_commutative_kabbalah_hd);
    RUN_TEST(test_query_commutative_iching_tzolkin);
    RUN_TEST(test_query_commutative_entries_match);

    /* si_query — no pair / invalid */
    RUN_TEST(test_query_no_pair_returns_zero);
    RUN_TEST(test_query_invalid_system_returns_zero);
    RUN_TEST(test_query_huge_system_returns_zero);

    /* si_get — valid */
    RUN_TEST(test_get_first);
    RUN_TEST(test_get_last);

    /* si_get — invalid */
    RUN_TEST(test_get_negative_invalid);
    RUN_TEST(test_get_too_large);

    /* si_get — bulk validation */
    RUN_TEST(test_get_all_entries_have_content);
    RUN_TEST(test_all_entries_depth_in_range);
    RUN_TEST(test_no_duplicate_titles_per_pair);

    /* si_pairs_with */
    RUN_TEST(test_pairs_with_kabbalah);
    RUN_TEST(test_pairs_with_geological);
    RUN_TEST(test_pairs_with_null_returns_count);
    RUN_TEST(test_pairs_with_invalid_system);
    RUN_TEST(test_pairs_with_small_buffer);

    /* si_depth_name */
    RUN_TEST(test_depth_name_surface);
    RUN_TEST(test_depth_name_intermediate);
    RUN_TEST(test_depth_name_deep);
    RUN_TEST(test_depth_name_invalid_zero);
    RUN_TEST(test_depth_name_invalid_negative);
    RUN_TEST(test_depth_name_invalid_four);

    /* Extended system insights */
    RUN_TEST(test_query_coptic_egyptian);
    RUN_TEST(test_query_persian_zoroastrian);
    RUN_TEST(test_query_celtic_astrology);

    /* Max insights per pair */
    RUN_TEST(test_max_insights_per_pair);

    /* Content spot checks */
    RUN_TEST(test_64_fold_mirror_content);
    RUN_TEST(test_gregorian_tzolkin_content);
    RUN_TEST(test_chinese_hebrew_lunisolar);

    return UNITY_END();
}
