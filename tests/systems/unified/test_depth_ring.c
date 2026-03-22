/* test_depth_ring.c — Tests for depth ring (concentric timescale) mapping
 * Assigns each knowledge system to a concentric ring (0-5) by timescale.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/depth_ring.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== depth_ring_count ========== */

static void test_count_returns_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, depth_ring_count());
}

/* ========== depth_ring_for_system — RING_COSMIC (0) ========== */

static void test_geological_is_cosmic(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_COSMIC,
                          depth_ring_for_system(TS_SYS_GEOLOGICAL));
}

static void test_cosmic_is_cosmic(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_COSMIC,
                          depth_ring_for_system(TS_SYS_COSMIC));
}

/* ========== depth_ring_for_system — RING_AGE (1) ========== */

static void test_buddhist_is_age(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_AGE,
                          depth_ring_for_system(TS_SYS_BUDDHIST));
}

static void test_hindu_is_age(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_AGE,
                          depth_ring_for_system(TS_SYS_HINDU));
}

/* ========== depth_ring_for_system — RING_ERA (2) ========== */

static void test_kabbalah_is_era(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_ERA,
                          depth_ring_for_system(TS_SYS_KABBALAH));
}

static void test_human_design_is_era(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_ERA,
                          depth_ring_for_system(TS_SYS_HUMAN_DESIGN));
}

/* ========== depth_ring_for_system — RING_YEAR (3) ========== */

static void test_astrology_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_ASTROLOGY));
}

static void test_hebrew_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_HEBREW));
}

static void test_islamic_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_ISLAMIC));
}

static void test_coptic_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_COPTIC));
}

static void test_ethiopian_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_ETHIOPIAN));
}

static void test_persian_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_PERSIAN));
}

static void test_japanese_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_JAPANESE));
}

static void test_korean_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_KOREAN));
}

static void test_thai_is_year(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_YEAR,
                          depth_ring_for_system(TS_SYS_THAI));
}

/* ========== depth_ring_for_system — RING_CYCLE (4) ========== */

static void test_chinese_is_cycle(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_CYCLE,
                          depth_ring_for_system(TS_SYS_CHINESE));
}

static void test_iching_is_cycle(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_CYCLE,
                          depth_ring_for_system(TS_SYS_ICHING));
}

/* ========== depth_ring_for_system — RING_TODAY (5) ========== */

static void test_gregorian_is_today(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY,
                          depth_ring_for_system(TS_SYS_GREGORIAN));
}

static void test_tzolkin_is_today(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY,
                          depth_ring_for_system(TS_SYS_TZOLKIN));
}

static void test_haab_is_today(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY,
                          depth_ring_for_system(TS_SYS_HAAB));
}

/* ========== depth_ring_for_system — out of range ========== */

static void test_out_of_range_positive_returns_today(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, depth_ring_for_system(999));
}

static void test_out_of_range_negative_returns_today(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, depth_ring_for_system(-1));
}

static void test_out_of_range_count_returns_today(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY,
                          depth_ring_for_system(TS_SYS_COUNT));
}

/* ========== depth_ring_systems — each ring ========== */

static void test_systems_cosmic_count(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_COSMIC, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(2, count);
}

static void test_systems_cosmic_contains_geological(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_COSMIC, ids, TS_SYS_COUNT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (ids[i] == TS_SYS_GEOLOGICAL) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

static void test_systems_cosmic_contains_cosmic(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_COSMIC, ids, TS_SYS_COUNT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (ids[i] == TS_SYS_COSMIC) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

static void test_systems_age_count(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_AGE, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(3, count);
}

static void test_systems_era_count(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_ERA, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(3, count);
}

static void test_systems_year_count(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_YEAR, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(16, count);
}

static void test_systems_cycle_count(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_CYCLE, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(3, count);
}

static void test_systems_today_count(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_TODAY, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(8, count);
}

/* ========== depth_ring_systems — NULL out_ids ========== */

static void test_systems_null_returns_count_only(void)
{
    int count = depth_ring_systems(DEPTH_RING_YEAR, NULL, 0);
    TEST_ASSERT_EQUAL_INT(16, count);
}

static void test_systems_null_cosmic_returns_count(void)
{
    int count = depth_ring_systems(DEPTH_RING_COSMIC, NULL, 0);
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* ========== depth_ring_systems — invalid ring ========== */

static void test_systems_invalid_ring_returns_zero(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems((depth_ring_t)99, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(0, count);
}

static void test_systems_negative_ring_returns_zero(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems((depth_ring_t)-1, ids, TS_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ========== depth_ring_systems — max_out truncation ========== */

static void test_systems_max_out_limits_output(void)
{
    int ids[2];
    int count = depth_ring_systems(DEPTH_RING_YEAR, ids, 2);
    /* Should return total count (9) but only write 2 */
    TEST_ASSERT_EQUAL_INT(16, count);
}

/* ========== All 35 systems assigned ========== */

static void test_all_systems_assigned_to_some_ring(void)
{
    /* Every ts_system_t value 0..19 must map to a valid ring */
    for (int s = 0; s < TS_SYS_COUNT; s++) {
        depth_ring_t ring = depth_ring_for_system(s);
        TEST_ASSERT_TRUE_MESSAGE(ring >= DEPTH_RING_COSMIC && ring <= DEPTH_RING_TODAY,
                                 "System not assigned to a valid ring");
    }
}

static void test_total_systems_across_rings_equals_sys_count(void)
{
    int total = 0;
    for (int r = 0; r < DEPTH_RING_COUNT; r++) {
        total += depth_ring_systems((depth_ring_t)r, NULL, 0);
    }
    TEST_ASSERT_EQUAL_INT(TS_SYS_COUNT, total);
}

/* ========== depth_ring_name ========== */

static void test_name_cosmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmic", depth_ring_name(DEPTH_RING_COSMIC));
}

static void test_name_age(void)
{
    TEST_ASSERT_EQUAL_STRING("Age", depth_ring_name(DEPTH_RING_AGE));
}

static void test_name_era(void)
{
    TEST_ASSERT_EQUAL_STRING("Era", depth_ring_name(DEPTH_RING_ERA));
}

static void test_name_year(void)
{
    TEST_ASSERT_EQUAL_STRING("Year", depth_ring_name(DEPTH_RING_YEAR));
}

static void test_name_cycle(void)
{
    TEST_ASSERT_EQUAL_STRING("Cycle", depth_ring_name(DEPTH_RING_CYCLE));
}

static void test_name_today(void)
{
    TEST_ASSERT_EQUAL_STRING("Today", depth_ring_name(DEPTH_RING_TODAY));
}

static void test_name_invalid_returns_question_mark(void)
{
    TEST_ASSERT_EQUAL_STRING("?", depth_ring_name((depth_ring_t)99));
}

static void test_name_negative_returns_question_mark(void)
{
    TEST_ASSERT_EQUAL_STRING("?", depth_ring_name((depth_ring_t)-1));
}

/* ========== depth_ring_timescale ========== */

static void test_timescale_cosmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Billions of years",
                             depth_ring_timescale(DEPTH_RING_COSMIC));
}

static void test_timescale_age(void)
{
    TEST_ASSERT_EQUAL_STRING("Hundreds of thousands to millions of years",
                             depth_ring_timescale(DEPTH_RING_AGE));
}

static void test_timescale_era(void)
{
    TEST_ASSERT_EQUAL_STRING("Decades to centuries",
                             depth_ring_timescale(DEPTH_RING_ERA));
}

static void test_timescale_year(void)
{
    TEST_ASSERT_EQUAL_STRING("Annual cycles",
                             depth_ring_timescale(DEPTH_RING_YEAR));
}

static void test_timescale_cycle(void)
{
    TEST_ASSERT_EQUAL_STRING("Monthly and weekly cycles",
                             depth_ring_timescale(DEPTH_RING_CYCLE));
}

static void test_timescale_today(void)
{
    TEST_ASSERT_EQUAL_STRING("Today",
                             depth_ring_timescale(DEPTH_RING_TODAY));
}

static void test_timescale_invalid_returns_question_mark(void)
{
    TEST_ASSERT_EQUAL_STRING("?", depth_ring_timescale((depth_ring_t)99));
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* count */
    RUN_TEST(test_count_returns_6);

    /* ring assignments — COSMIC */
    RUN_TEST(test_geological_is_cosmic);
    RUN_TEST(test_cosmic_is_cosmic);

    /* ring assignments — AGE */
    RUN_TEST(test_buddhist_is_age);
    RUN_TEST(test_hindu_is_age);

    /* ring assignments — ERA */
    RUN_TEST(test_kabbalah_is_era);
    RUN_TEST(test_human_design_is_era);

    /* ring assignments — YEAR */
    RUN_TEST(test_astrology_is_year);
    RUN_TEST(test_hebrew_is_year);
    RUN_TEST(test_islamic_is_year);
    RUN_TEST(test_coptic_is_year);
    RUN_TEST(test_ethiopian_is_year);
    RUN_TEST(test_persian_is_year);
    RUN_TEST(test_japanese_is_year);
    RUN_TEST(test_korean_is_year);
    RUN_TEST(test_thai_is_year);

    /* ring assignments — CYCLE */
    RUN_TEST(test_chinese_is_cycle);
    RUN_TEST(test_iching_is_cycle);

    /* ring assignments — TODAY */
    RUN_TEST(test_gregorian_is_today);
    RUN_TEST(test_tzolkin_is_today);
    RUN_TEST(test_haab_is_today);

    /* out-of-range system_id */
    RUN_TEST(test_out_of_range_positive_returns_today);
    RUN_TEST(test_out_of_range_negative_returns_today);
    RUN_TEST(test_out_of_range_count_returns_today);

    /* depth_ring_systems per ring */
    RUN_TEST(test_systems_cosmic_count);
    RUN_TEST(test_systems_cosmic_contains_geological);
    RUN_TEST(test_systems_cosmic_contains_cosmic);
    RUN_TEST(test_systems_age_count);
    RUN_TEST(test_systems_era_count);
    RUN_TEST(test_systems_year_count);
    RUN_TEST(test_systems_cycle_count);
    RUN_TEST(test_systems_today_count);

    /* NULL out_ids */
    RUN_TEST(test_systems_null_returns_count_only);
    RUN_TEST(test_systems_null_cosmic_returns_count);

    /* invalid ring */
    RUN_TEST(test_systems_invalid_ring_returns_zero);
    RUN_TEST(test_systems_negative_ring_returns_zero);

    /* max_out truncation */
    RUN_TEST(test_systems_max_out_limits_output);

    /* all systems assigned */
    RUN_TEST(test_all_systems_assigned_to_some_ring);
    RUN_TEST(test_total_systems_across_rings_equals_sys_count);

    /* ring names */
    RUN_TEST(test_name_cosmic);
    RUN_TEST(test_name_age);
    RUN_TEST(test_name_era);
    RUN_TEST(test_name_year);
    RUN_TEST(test_name_cycle);
    RUN_TEST(test_name_today);
    RUN_TEST(test_name_invalid_returns_question_mark);
    RUN_TEST(test_name_negative_returns_question_mark);

    /* timescale strings */
    RUN_TEST(test_timescale_cosmic);
    RUN_TEST(test_timescale_age);
    RUN_TEST(test_timescale_era);
    RUN_TEST(test_timescale_year);
    RUN_TEST(test_timescale_cycle);
    RUN_TEST(test_timescale_today);
    RUN_TEST(test_timescale_invalid_returns_question_mark);

    return UNITY_END();
}
