/* test_depth_temporal.c — Tests for temporal exploration of depth layers
 * Computes cycle recurrence, past/future state snapshots for each system.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/depth_temporal.h"

#include <string.h>
#include <math.h>
#include <stdio.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== dt_cycle_count ========== */

static void test_cycle_count_returns_35(void)
{
    TEST_ASSERT_EQUAL_INT(35, dt_cycle_count());
}

/* ========== dt_cycle_info — specific systems ========== */

static void test_cycle_info_tzolkin(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_TZOLKIN, c.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 260.0f, (float)c.cycle_days);
    TEST_ASSERT_NOT_NULL(c.description);
    TEST_ASSERT_NOT_NULL(strstr(c.description, "260"));
}

static void test_cycle_info_tzolkin_cycle_name(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_STRING("Tzolkin Sacred Cycle", c.cycle_name);
}

static void test_cycle_info_tzolkin_cycle_years(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_TZOLKIN);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.712f, (float)c.cycle_years);
}

static void test_cycle_info_iching(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ICHING, c.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 64.0f, (float)c.cycle_days);
}

static void test_cycle_info_iching_cycle_name(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_STRING("Hexagram Cycle", c.cycle_name);
}

static void test_cycle_info_hebrew(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HEBREW, c.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6939.75f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 19.0f, (float)c.cycle_years);
}

static void test_cycle_info_geological_no_recurrence(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_GEOLOGICAL);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GEOLOGICAL, c.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)c.cycle_years);
}

static void test_cycle_info_cosmic_no_recurrence(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_COSMIC);
    TEST_ASSERT_EQUAL_INT(TS_SYS_COSMIC, c.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)c.cycle_years);
}

static void test_cycle_info_gregorian(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_GREGORIAN);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 365.2425f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)c.cycle_years);
    TEST_ASSERT_EQUAL_STRING("Gregorian Year", c.cycle_name);
}

static void test_cycle_info_chinese(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_CHINESE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 21915.0f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, (float)c.cycle_years);
}

static void test_cycle_info_islamic(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_ISLAMIC);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10631.0f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, (float)c.cycle_years);
}

static void test_cycle_info_buddhist(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_BUDDHIST);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 29.53f, (float)c.cycle_days);
}

static void test_cycle_info_hindu(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_HINDU);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 29.53f, (float)c.cycle_days);
}

static void test_cycle_info_haab(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_HAAB);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 365.0f, (float)c.cycle_days);
}

static void test_cycle_info_persian(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_PERSIAN);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 365.2424f, (float)c.cycle_days);
}

static void test_cycle_info_japanese(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_JAPANESE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 4383.0f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 12.0f, (float)c.cycle_years);
}

static void test_cycle_info_korean(void)
{
    dt_cycle_t c = dt_cycle_info(TS_SYS_KOREAN);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 21915.0f, (float)c.cycle_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, (float)c.cycle_years);
}

/* ========== dt_cycle_info — all 20 systems loop ========== */

static void test_cycle_info_all_systems_have_nonempty_name(void)
{
    for (int i = 0; i < 20; i++) {
        dt_cycle_t c = dt_cycle_info(i);
        TEST_ASSERT_EQUAL_INT(i, c.system_id);
        TEST_ASSERT_NOT_NULL(c.cycle_name);
        TEST_ASSERT_TRUE(strlen(c.cycle_name) > 0);
    }
}

static void test_cycle_info_all_systems_have_nonempty_description(void)
{
    for (int i = 0; i < 20; i++) {
        dt_cycle_t c = dt_cycle_info(i);
        TEST_ASSERT_NOT_NULL(c.description);
        TEST_ASSERT_TRUE(strlen(c.description) > 0);
    }
}

/* ========== dt_cycle_info — invalid ========== */

static void test_cycle_info_invalid_positive(void)
{
    dt_cycle_t c = dt_cycle_info(35);
    TEST_ASSERT_EQUAL_INT(-1, c.system_id);
    TEST_ASSERT_EQUAL_STRING("?", c.cycle_name);
}

static void test_cycle_info_invalid_negative(void)
{
    dt_cycle_t c = dt_cycle_info(-1);
    TEST_ASSERT_EQUAL_INT(-1, c.system_id);
    TEST_ASSERT_EQUAL_STRING("?", c.cycle_name);
}

/* ========== dt_recurrence — basic ========== */

static void test_recurrence_tzolkin(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_TZOLKIN, 2460000.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2460260.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2459740.0f, (float)r.prev_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 260.0f, (float)r.days_until);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.days_since);
}

static void test_recurrence_iching(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_ICHING, 2460000.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2460064.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2459936.0f, (float)r.prev_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 64.0f, (float)r.days_until);
}

static void test_recurrence_geological_all_zeros(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_GEOLOGICAL, 2460000.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.prev_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.days_until);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.days_since);
    TEST_ASSERT_EQUAL_INT(0, r.occurrences_per_year);
}

static void test_recurrence_cosmic_all_zeros(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_COSMIC, 2460000.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.prev_jd);
    TEST_ASSERT_EQUAL_INT(0, r.occurrences_per_year);
}

static void test_recurrence_jd_zero_all_zeros(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_TZOLKIN, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.prev_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.days_until);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.days_since);
}

static void test_recurrence_jd_negative_all_zeros(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_TZOLKIN, -100.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.prev_jd);
}

/* ========== dt_recurrence — occurrences_per_year ========== */

static void test_recurrence_tzolkin_occurrences_per_year(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_TZOLKIN, 2460000.0);
    /* 365.2425 / 260.0 = ~1.4 -> 1 */
    TEST_ASSERT_EQUAL_INT(1, r.occurrences_per_year);
}

static void test_recurrence_iching_occurrences_per_year(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_ICHING, 2460000.0);
    /* 365.2425 / 64.0 = ~5.7 -> 5 */
    TEST_ASSERT_EQUAL_INT(5, r.occurrences_per_year);
}

static void test_recurrence_buddhist_occurrences_per_year(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_BUDDHIST, 2460000.0);
    /* 365.2425 / 29.53 = ~12.37 -> 12 */
    TEST_ASSERT_EQUAL_INT(12, r.occurrences_per_year);
}

static void test_recurrence_gregorian_occurrences_per_year(void)
{
    dt_recurrence_t r = dt_recurrence(TS_SYS_GREGORIAN, 2460000.0);
    /* 365.2425 / 365.2425 = 1 */
    TEST_ASSERT_EQUAL_INT(1, r.occurrences_per_year);
}

/* ========== dt_recurrence — invalid system ========== */

static void test_recurrence_invalid_system_all_zeros(void)
{
    dt_recurrence_t r = dt_recurrence(99, 2460000.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.next_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.prev_jd);
    TEST_ASSERT_EQUAL_INT(0, r.occurrences_per_year);
}

/* ========== dt_snapshot ========== */

static void test_snapshot_tzolkin_one_year_ago(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_TZOLKIN, 2460000.0, -1);
    TEST_ASSERT_EQUAL_INT(-1, s.years_offset);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_TRUE(strlen(s.state) > 0);
    TEST_ASSERT_NOT_NULL(strstr(s.state, "Kin"));
}

static void test_snapshot_tzolkin_forward(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_TZOLKIN, 2460000.0, 5);
    TEST_ASSERT_EQUAL_INT(5, s.years_offset);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_NOT_NULL(strstr(s.state, "Kin"));
}

static void test_snapshot_iching(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_ICHING, 2460000.0, -2);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_NOT_NULL(strstr(s.state, "Hexagram"));
}

static void test_snapshot_geological_deep_time(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_GEOLOGICAL, 2460000.0, -100);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_NOT_NULL(strstr(s.state, "Deep time"));
}

static void test_snapshot_cosmic_deep_time(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_COSMIC, 2460000.0, 50);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_NOT_NULL(strstr(s.state, "Deep time"));
}

static void test_snapshot_invalid_system(void)
{
    dt_snapshot_t s = dt_snapshot(99, 2460000.0, 0);
    TEST_ASSERT_EQUAL_INT(0, s.has_data);
}

static void test_snapshot_invalid_negative_system(void)
{
    dt_snapshot_t s = dt_snapshot(-1, 2460000.0, 0);
    TEST_ASSERT_EQUAL_INT(0, s.has_data);
}

static void test_snapshot_gregorian(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_GREGORIAN, 2460000.0, 3);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_TRUE(strlen(s.state) > 0);
}

static void test_snapshot_zero_offset(void)
{
    dt_snapshot_t s = dt_snapshot(TS_SYS_TZOLKIN, 2460000.0, 0);
    TEST_ASSERT_EQUAL_INT(0, s.years_offset);
    TEST_ASSERT_EQUAL_INT(1, s.has_data);
    TEST_ASSERT_NOT_NULL(strstr(s.state, "Kin"));
}

/* ========== dt_snapshot — Kin cycling within 1-260 range ========== */

static void test_snapshot_tzolkin_kin_in_range(void)
{
    /* Check that computed kin is always 1-260 across many offsets */
    for (int offset = -10; offset <= 10; offset++) {
        dt_snapshot_t s = dt_snapshot(TS_SYS_TZOLKIN, 2460000.0, offset);
        TEST_ASSERT_EQUAL_INT(1, s.has_data);
        /* Extract kin number from "Kin NNN" */
        int kin = 0;
        int matched = sscanf(s.state, "Kin %d", &kin);
        TEST_ASSERT_EQUAL_INT(1, matched);
        TEST_ASSERT_TRUE(kin >= 1 && kin <= 260);
    }
}

/* ========== dt_snapshot — Hexagram cycling within 1-64 range ========== */

static void test_snapshot_iching_hex_in_range(void)
{
    for (int offset = -10; offset <= 10; offset++) {
        dt_snapshot_t s = dt_snapshot(TS_SYS_ICHING, 2460000.0, offset);
        TEST_ASSERT_EQUAL_INT(1, s.has_data);
        int hex = 0;
        int matched = sscanf(s.state, "Hexagram %d", &hex);
        TEST_ASSERT_EQUAL_INT(1, matched);
        TEST_ASSERT_TRUE(hex >= 1 && hex <= 64);
    }
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* count */
    RUN_TEST(test_cycle_count_returns_35);

    /* cycle info — specific systems */
    RUN_TEST(test_cycle_info_tzolkin);
    RUN_TEST(test_cycle_info_tzolkin_cycle_name);
    RUN_TEST(test_cycle_info_tzolkin_cycle_years);
    RUN_TEST(test_cycle_info_iching);
    RUN_TEST(test_cycle_info_iching_cycle_name);
    RUN_TEST(test_cycle_info_hebrew);
    RUN_TEST(test_cycle_info_geological_no_recurrence);
    RUN_TEST(test_cycle_info_cosmic_no_recurrence);
    RUN_TEST(test_cycle_info_gregorian);
    RUN_TEST(test_cycle_info_chinese);
    RUN_TEST(test_cycle_info_islamic);
    RUN_TEST(test_cycle_info_buddhist);
    RUN_TEST(test_cycle_info_hindu);
    RUN_TEST(test_cycle_info_haab);
    RUN_TEST(test_cycle_info_persian);
    RUN_TEST(test_cycle_info_japanese);
    RUN_TEST(test_cycle_info_korean);

    /* cycle info — all systems loop */
    RUN_TEST(test_cycle_info_all_systems_have_nonempty_name);
    RUN_TEST(test_cycle_info_all_systems_have_nonempty_description);

    /* cycle info — invalid */
    RUN_TEST(test_cycle_info_invalid_positive);
    RUN_TEST(test_cycle_info_invalid_negative);

    /* recurrence — basic */
    RUN_TEST(test_recurrence_tzolkin);
    RUN_TEST(test_recurrence_iching);
    RUN_TEST(test_recurrence_geological_all_zeros);
    RUN_TEST(test_recurrence_cosmic_all_zeros);
    RUN_TEST(test_recurrence_jd_zero_all_zeros);
    RUN_TEST(test_recurrence_jd_negative_all_zeros);

    /* recurrence — occurrences per year */
    RUN_TEST(test_recurrence_tzolkin_occurrences_per_year);
    RUN_TEST(test_recurrence_iching_occurrences_per_year);
    RUN_TEST(test_recurrence_buddhist_occurrences_per_year);
    RUN_TEST(test_recurrence_gregorian_occurrences_per_year);

    /* recurrence — invalid */
    RUN_TEST(test_recurrence_invalid_system_all_zeros);

    /* snapshot */
    RUN_TEST(test_snapshot_tzolkin_one_year_ago);
    RUN_TEST(test_snapshot_tzolkin_forward);
    RUN_TEST(test_snapshot_iching);
    RUN_TEST(test_snapshot_geological_deep_time);
    RUN_TEST(test_snapshot_cosmic_deep_time);
    RUN_TEST(test_snapshot_invalid_system);
    RUN_TEST(test_snapshot_invalid_negative_system);
    RUN_TEST(test_snapshot_gregorian);
    RUN_TEST(test_snapshot_zero_offset);

    /* snapshot — range validation */
    RUN_TEST(test_snapshot_tzolkin_kin_in_range);
    RUN_TEST(test_snapshot_iching_hex_in_range);

    return UNITY_END();
}
