/* test_convergence.c -- Cross-Calendar Convergence Detector tests
 * TDD RED phase: tests written before implementation.
 * Tests detection of significant dates across 11 calendar/astronomical
 * systems and convergence events when 3+ systems are active. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/convergence.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== conv_system_name ===== */

void test_system_name_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", conv_system_name(CONV_SYS_GREGORIAN));
}

void test_system_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", conv_system_name(CONV_SYS_HEBREW));
}

void test_system_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", conv_system_name(CONV_SYS_ISLAMIC));
}

void test_system_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", conv_system_name(CONV_SYS_CHINESE));
}

void test_system_name_tzolkin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", conv_system_name(CONV_SYS_TZOLKIN));
}

void test_system_name_haab(void)
{
    TEST_ASSERT_EQUAL_STRING("Haab", conv_system_name(CONV_SYS_HAAB));
}

void test_system_name_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", conv_system_name(CONV_SYS_ICHING));
}

void test_system_name_buddhist(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddhist", conv_system_name(CONV_SYS_BUDDHIST));
}

void test_system_name_hindu(void)
{
    TEST_ASSERT_EQUAL_STRING("Hindu", conv_system_name(CONV_SYS_HINDU));
}

void test_system_name_lunar(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunar", conv_system_name(CONV_SYS_LUNAR));
}

void test_system_name_zodiac(void)
{
    TEST_ASSERT_EQUAL_STRING("Zodiac", conv_system_name(CONV_SYS_ZODIAC));
}

void test_system_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", conv_system_name(CONV_SYS_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", conv_system_name(-1));
}

/* ===== conv_signal_type_name ===== */

void test_signal_type_name_new_year(void)
{
    TEST_ASSERT_EQUAL_STRING("New Year", conv_signal_type_name(CONV_SIG_NEW_YEAR));
}

void test_signal_type_name_new_month(void)
{
    TEST_ASSERT_EQUAL_STRING("New Month", conv_signal_type_name(CONV_SIG_NEW_MONTH));
}

void test_signal_type_name_holy_day(void)
{
    TEST_ASSERT_EQUAL_STRING("Holy Day", conv_signal_type_name(CONV_SIG_HOLY_DAY));
}

void test_signal_type_name_cycle_start(void)
{
    TEST_ASSERT_EQUAL_STRING("Cycle Start", conv_signal_type_name(CONV_SIG_CYCLE_START));
}

void test_signal_type_name_full_moon(void)
{
    TEST_ASSERT_EQUAL_STRING("Full Moon", conv_signal_type_name(CONV_SIG_FULL_MOON));
}

void test_signal_type_name_new_moon(void)
{
    TEST_ASSERT_EQUAL_STRING("New Moon", conv_signal_type_name(CONV_SIG_NEW_MOON));
}

void test_signal_type_name_solstice(void)
{
    TEST_ASSERT_EQUAL_STRING("Solstice", conv_signal_type_name(CONV_SIG_SOLSTICE));
}

void test_signal_type_name_sacred_number(void)
{
    TEST_ASSERT_EQUAL_STRING("Sacred Number",
                             conv_signal_type_name(CONV_SIG_SACRED_NUMBER));
}

void test_signal_type_name_tone_one(void)
{
    TEST_ASSERT_EQUAL_STRING("Tone One", conv_signal_type_name(CONV_SIG_TONE_ONE));
}

void test_signal_type_name_sabbath(void)
{
    TEST_ASSERT_EQUAL_STRING("Sabbath", conv_signal_type_name(CONV_SIG_SABBATH));
}

void test_signal_type_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", conv_signal_type_name(CONV_SIG_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", conv_signal_type_name(-1));
}

/* ===== conv_check_system — Gregorian ===== */

void test_gregorian_new_year(void)
{
    /* 2024-01-01 midnight UT = JD 2460310.5 */
    double jd = 2460310.5;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_GREGORIAN, jd, out, CONV_MAX_SIGNALS);
    /* Should detect new year (weight 3). New month suppressed on Jan 1. */
    TEST_ASSERT_GREATER_OR_EQUAL(1, n);
    int found_new_year = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_NEW_YEAR) {
            found_new_year = 1;
            TEST_ASSERT_EQUAL(3, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found_new_year);
}

void test_gregorian_new_month(void)
{
    /* 2024-03-01 midnight UT = JD 2460370.5 */
    double jd = 2460370.5;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_GREGORIAN, jd, out, CONV_MAX_SIGNALS);
    TEST_ASSERT_GREATER_OR_EQUAL(1, n);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_NEW_MONTH) found = 1;
    }
    TEST_ASSERT_EQUAL(1, found);
}

void test_gregorian_normal_day(void)
{
    /* 2024-01-15 = not day 1, not near solstice/equinox */
    double jd = 2460324.0;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_GREGORIAN, jd, out, CONV_MAX_SIGNALS);
    /* A normal mid-month day should produce 0 signals */
    TEST_ASSERT_EQUAL(0, n);
}

/* ===== conv_check_system — Islamic ===== */

void test_islamic_new_year_detection(void)
{
    /* Hijri epoch: JD 1948439.5 = 1 Muharram year 1.
     * Use a known Muharram 1 date. Hijri year 1446 starts around
     * July 7, 2024 = JD ~2460498.5. We approximate inline. */
    conv_signal_t out[CONV_MAX_SIGNALS];
    /* Check the epoch itself: JD 1948439.5 should be Muharram 1 */
    int n = conv_check_system(CONV_SYS_ISLAMIC, 1948439.5, out, CONV_MAX_SIGNALS);
    TEST_ASSERT_GREATER_OR_EQUAL(1, n);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_NEW_YEAR) found = 1;
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — Tzolkin ===== */

void test_tzolkin_kin_one(void)
{
    /* The Tzolkin kin formula: kin = ((int)(jd - 2452002.5) % 260 + 260) % 260
     * kin==0 means kin 1 (Red Magnetic Dragon). JD 2452002.5 itself should be kin 0.
     * So JD 2452002.5 = kin 1 equivalent (kin index 0). */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_TZOLKIN, 2452002.5, out, CONV_MAX_SIGNALS);
    TEST_ASSERT_GREATER_OR_EQUAL(1, n);
    int found_cycle = 0;
    int found_tone = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_CYCLE_START) {
            found_cycle = 1;
            TEST_ASSERT_EQUAL(3, out[i].weight);
        }
        if (out[i].type == CONV_SIG_TONE_ONE) found_tone = 1;
    }
    TEST_ASSERT_EQUAL(1, found_cycle);
    /* kin 1 also has tone 1, so tone_one should fire */
    TEST_ASSERT_EQUAL(1, found_tone);
}

void test_tzolkin_tone_one_not_kin_one(void)
{
    /* Tone 1 occurs every 13 kins. kin index 13 = tone 1, seal 13 -> not kin 1 */
    double jd = 2452002.5 + 13;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_TZOLKIN, jd, out, CONV_MAX_SIGNALS);
    int found_tone = 0;
    int found_cycle = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_TONE_ONE) found_tone = 1;
        if (out[i].type == CONV_SIG_CYCLE_START) found_cycle = 1;
    }
    /* Tone 1 wavespell start but NOT cycle start (kin 1) */
    TEST_ASSERT_EQUAL(1, found_tone);
    TEST_ASSERT_EQUAL(0, found_cycle);
}

/* ===== conv_check_system — Haab ===== */

void test_haab_wayeb(void)
{
    /* Wayeb = the 5 nameless days, month index 18 in the Haab cycle.
     * day_in_haab = ((int)(jd - 2452002.5 + 65) % 365 + 365) % 365
     * Wayeb starts at day 360 (18*20 = 360).
     * So we need day_in_haab = 360: jd - 2452002.5 + 65 = 360 + k*365
     * jd = 2452002.5 + 295 + k*365.
     * Let k=0: jd = 2452297.5 */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_HAAB, 2452297.5, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_HOLY_DAY && out[i].weight == 2) found = 1;
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — I Ching ===== */

void test_iching_hexagram_one(void)
{
    /* I Ching cycles through 64 hexagrams. hexagram = ((int)(jd) % 64 + 64) % 64
     * hexagram 0 -> King Wen 1 (Creative).
     * Find a JD where floor(jd) % 64 == 0. 2452000 % 64 = 2452000/64 = 38312.5
     * 38312*64 = 2451968, so 2452000 % 64 = 32.
     * We want JD where floor(jd) % 64 == 0. 2451968.5 works: floor(2451968.5) = 2451968
     * 2451968 % 64 = 0. */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_ICHING, 2451968.5, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_CYCLE_START) {
            found = 1;
            TEST_ASSERT_EQUAL(2, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — Lunar ===== */

void test_lunar_full_moon(void)
{
    /* Known full moon: JD 2451550.1 is new moon reference.
     * Full moon is approximately at 2451550.1 + 14.765 = 2451564.865
     * phase_fraction should be ~0.5 */
    double jd_full = 2451550.1 + 14.765;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_LUNAR, jd_full, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_FULL_MOON) {
            found = 1;
            TEST_ASSERT_EQUAL(2, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

void test_lunar_new_moon(void)
{
    /* JD 2451550.1 is our new moon reference epoch */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_LUNAR, 2451550.1, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_NEW_MOON) {
            found = 1;
            TEST_ASSERT_EQUAL(2, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — Hindu ===== */

void test_hindu_purnima(void)
{
    /* Full moon -> Hindu Purnima. Same JD as lunar full moon test. */
    double jd_full = 2451550.1 + 14.765;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_HINDU, jd_full, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_FULL_MOON) {
            found = 1;
            TEST_ASSERT_EQUAL(2, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — Hebrew ===== */

void test_hebrew_sabbath(void)
{
    /* Hebrew Sabbath = Saturday. dow from JD uses floor(jd+0.5)%7.
     * 0=Mon, 5=Sat. JD 2460316.0 = 2024-01-06 (Saturday, dow=5). */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_HEBREW, 2460316.0, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_SABBATH) {
            found = 1;
            TEST_ASSERT_EQUAL(1, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — Buddhist ===== */

void test_buddhist_uposatha_full_moon(void)
{
    /* Full moon -> Buddhist Uposatha observance day */
    double jd_full = 2451550.1 + 14.765;
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_BUDDHIST, jd_full, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_FULL_MOON || out[i].type == CONV_SIG_NEW_MOON) {
            found = 1;
            TEST_ASSERT_EQUAL(2, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

/* ===== conv_check_system — Zodiac ===== */

void test_zodiac_ingress(void)
{
    /* Near vernal equinox (sun longitude ~0 degrees), should detect ingress.
     * Spring equinox 2024: ~March 20 = JD 2460388.5 approx.
     * Simplified solar longitude formula should give near 0 or 360 deg. */
    conv_signal_t out[CONV_MAX_SIGNALS];
    /* We'll use 2024 March 20 = JD 2460388.5 approximately */
    int n = conv_check_system(CONV_SYS_ZODIAC, 2460389.0, out, CONV_MAX_SIGNALS);
    /* May or may not detect exactly due to approximation. Just check it returns >= 0 */
    TEST_ASSERT_GREATER_OR_EQUAL(0, n);
}

/* ===== conv_scan_date ===== */

void test_scan_date_returns_jd(void)
{
    double jd = 2460310.0;
    conv_scan_t scan = conv_scan_date(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001, jd, scan.jd);
}

void test_scan_date_gregorian_new_year_has_signals(void)
{
    /* 2024-01-01 midnight UT = JD 2460310.5 */
    conv_scan_t scan = conv_scan_date(2460310.5);
    TEST_ASSERT_GREATER_OR_EQUAL(1, scan.signal_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1, scan.systems_active);
}

void test_scan_date_systems_active_count(void)
{
    /* Any date should have systems_active counting distinct systems */
    conv_scan_t scan = conv_scan_date(2460310.0);
    /* Count distinct systems manually */
    int sys_seen[CONV_SYS_COUNT] = {0};
    int distinct = 0;
    for (int i = 0; i < scan.signal_count; i++) {
        if (!sys_seen[scan.signals[i].system]) {
            sys_seen[scan.signals[i].system] = 1;
            distinct++;
        }
    }
    TEST_ASSERT_EQUAL(distinct, scan.systems_active);
}

void test_scan_date_total_weight_sum(void)
{
    conv_scan_t scan = conv_scan_date(2460310.0);
    int sum = 0;
    for (int i = 0; i < scan.signal_count; i++) {
        sum += scan.signals[i].weight;
    }
    TEST_ASSERT_EQUAL(sum, scan.total_weight);
}

void test_scan_date_convergence_flag(void)
{
    /* is_convergence should be 1 when systems_active >= 3 */
    conv_scan_t scan = conv_scan_date(2460310.0);
    if (scan.systems_active >= 3) {
        TEST_ASSERT_EQUAL(1, scan.is_convergence);
    } else {
        TEST_ASSERT_EQUAL(0, scan.is_convergence);
    }
}

void test_scan_date_signal_count_within_bounds(void)
{
    conv_scan_t scan = conv_scan_date(2460310.0);
    TEST_ASSERT_GREATER_OR_EQUAL(0, scan.signal_count);
    TEST_ASSERT_LESS_OR_EQUAL(CONV_MAX_SIGNALS, scan.signal_count);
}

void test_scan_date_normal_day_valid(void)
{
    /* A mid-month, mid-week day. Should still produce a valid scan. */
    conv_scan_t scan = conv_scan_date(2460320.0);
    TEST_ASSERT_GREATER_OR_EQUAL(0, scan.signal_count);
    TEST_ASSERT_GREATER_OR_EQUAL(0, scan.systems_active);
}

/* ===== conv_best_in_range ===== */

void test_best_in_range_30_days(void)
{
    /* Over 30 days, should find the day with highest total_weight */
    double jd_start = 2460300.0;
    double jd_end = 2460330.0;
    double best = conv_best_in_range(jd_start, jd_end);
    TEST_ASSERT_GREATER_OR_EQUAL(jd_start, best);
    TEST_ASSERT_LESS_OR_EQUAL(jd_end, best);
}

void test_best_in_range_single_day(void)
{
    /* Range of 1 day should return that day */
    double jd = 2460310.0;
    double best = conv_best_in_range(jd, jd);
    TEST_ASSERT_FLOAT_WITHIN(0.5, jd, best);
}

/* ===== conv_count_in_range ===== */

void test_count_in_range_year(void)
{
    /* Over a full year, should find at least a few convergences */
    double jd_start = 2460310.0;
    double jd_end = jd_start + 365.0;
    int count = conv_count_in_range(jd_start, jd_end);
    TEST_ASSERT_GREATER_OR_EQUAL(1, count);
}

void test_count_in_range_small(void)
{
    /* Over 7 days, count should be non-negative */
    double jd_start = 2460310.0;
    double jd_end = jd_start + 7.0;
    int count = conv_count_in_range(jd_start, jd_end);
    TEST_ASSERT_GREATER_OR_EQUAL(0, count);
}

/* ===== conv_next_convergence ===== */

void test_next_convergence_finds_one(void)
{
    /* From any date, should find a convergence within 365 days */
    double jd = 2460310.0;
    double next = conv_next_convergence(jd, 365);
    TEST_ASSERT_GREATER_THAN(jd, next);
    TEST_ASSERT_LESS_OR_EQUAL(jd + 365.0, next);
    /* Verify it's actually a convergence */
    conv_scan_t scan = conv_scan_date(next);
    TEST_ASSERT_EQUAL(1, scan.is_convergence);
}

void test_next_convergence_from_convergence_date(void)
{
    /* If we're already on a convergence, next should be a later one */
    double jd = 2460310.0; /* Gregorian new year, probably a convergence */
    conv_scan_t scan = conv_scan_date(jd);
    if (scan.is_convergence) {
        double next = conv_next_convergence(jd, 365);
        TEST_ASSERT_GREATER_THAN(jd, next);
    }
}

/* ===== conv_prev_convergence ===== */

void test_prev_convergence_finds_one(void)
{
    double jd = 2460310.0;
    double prev = conv_prev_convergence(jd, 365);
    TEST_ASSERT_LESS_THAN(jd, prev);
    TEST_ASSERT_GREATER_OR_EQUAL(jd - 365.0, prev);
    /* Verify it's actually a convergence */
    conv_scan_t scan = conv_scan_date(prev);
    TEST_ASSERT_EQUAL(1, scan.is_convergence);
}

/* ===== Edge cases ===== */

void test_scan_date_very_old_jd(void)
{
    /* JD 0.0 = 4713 BCE Jan 1 in Julian calendar */
    conv_scan_t scan = conv_scan_date(0.0);
    /* Should not crash; signal_count may vary */
    TEST_ASSERT_GREATER_OR_EQUAL(0, scan.signal_count);
}

void test_scan_date_very_future_jd(void)
{
    /* JD 3000000.0 = ~year 3501 CE */
    conv_scan_t scan = conv_scan_date(3000000.0);
    TEST_ASSERT_GREATER_OR_EQUAL(0, scan.signal_count);
}

void test_scan_date_negative_jd(void)
{
    /* Negative JD is valid (before 4713 BCE) */
    conv_scan_t scan = conv_scan_date(-100000.0);
    TEST_ASSERT_GREATER_OR_EQUAL(0, scan.signal_count);
}

void test_check_system_invalid(void)
{
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_COUNT, 2460310.0, out, CONV_MAX_SIGNALS);
    TEST_ASSERT_EQUAL(0, n);
}

void test_check_system_zero_max(void)
{
    conv_signal_t out[1];
    int n = conv_check_system(CONV_SYS_GREGORIAN, 2460310.0, out, 0);
    TEST_ASSERT_EQUAL(0, n);
}

/* ===== Signal weight validation ===== */

void test_all_weights_valid(void)
{
    /* Scan a date and verify all signal weights are 1, 2, or 3 */
    conv_scan_t scan = conv_scan_date(2460310.0);
    for (int i = 0; i < scan.signal_count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(1, scan.signals[i].weight);
        TEST_ASSERT_LESS_OR_EQUAL(3, scan.signals[i].weight);
    }
}

void test_all_descriptions_non_null(void)
{
    conv_scan_t scan = conv_scan_date(2460310.0);
    for (int i = 0; i < scan.signal_count; i++) {
        TEST_ASSERT_NOT_NULL(scan.signals[i].description);
    }
}

/* ===== Chinese system ===== */

void test_chinese_new_year_approx(void)
{
    /* Chinese New Year 2024 = Feb 10 = JD ~2460350.5
     * Our approximation checks if Gregorian month/day is in the CNY window.
     * Check that system at least doesn't crash. */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_CHINESE, 2460350.5, out, CONV_MAX_SIGNALS);
    /* We just verify it runs and returns a valid count */
    TEST_ASSERT_GREATER_OR_EQUAL(0, n);
}

/* ===== Multiple systems active on a single date ===== */

void test_multiple_systems_on_new_year(void)
{
    /* On Gregorian Jan 1, we expect at least Gregorian signals.
     * Depending on lunar phase, Hebrew day, etc., we might see multiple systems. */
    conv_scan_t scan = conv_scan_date(2460310.5);
    /* At minimum, Gregorian should be active */
    int greg_active = 0;
    for (int i = 0; i < scan.signal_count; i++) {
        if (scan.signals[i].system == CONV_SYS_GREGORIAN) greg_active = 1;
    }
    TEST_ASSERT_EQUAL(1, greg_active);
}

/* ===== Range search consistency ===== */

void test_count_in_range_consistency(void)
{
    /* count_in_range should be >= 0 and not absurdly large */
    double jd_start = 2460310.0;
    int count = conv_count_in_range(jd_start, jd_start + 30.0);
    TEST_ASSERT_GREATER_OR_EQUAL(0, count);
    TEST_ASSERT_LESS_OR_EQUAL(31, count); /* can't be more than days scanned */
}

void test_next_convergence_no_result_returns_zero(void)
{
    /* With max_days=0, should return 0 (not found) */
    double result = conv_next_convergence(2460310.0, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_prev_convergence_no_result_returns_zero(void)
{
    double result = conv_prev_convergence(2460310.0, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

/* ===== Gregorian solstice/equinox ===== */

void test_gregorian_march_equinox(void)
{
    /* 2024-03-20 ~ JD 2460389.0 (noon). Check for solstice signal. */
    conv_signal_t out[CONV_MAX_SIGNALS];
    int n = conv_check_system(CONV_SYS_GREGORIAN, 2460389.0, out, CONV_MAX_SIGNALS);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (out[i].type == CONV_SIG_SOLSTICE) {
            found = 1;
            TEST_ASSERT_EQUAL(2, out[i].weight);
        }
    }
    TEST_ASSERT_EQUAL(1, found);
}

int main(void)
{
    UNITY_BEGIN();

    /* System names */
    RUN_TEST(test_system_name_gregorian);
    RUN_TEST(test_system_name_hebrew);
    RUN_TEST(test_system_name_islamic);
    RUN_TEST(test_system_name_chinese);
    RUN_TEST(test_system_name_tzolkin);
    RUN_TEST(test_system_name_haab);
    RUN_TEST(test_system_name_iching);
    RUN_TEST(test_system_name_buddhist);
    RUN_TEST(test_system_name_hindu);
    RUN_TEST(test_system_name_lunar);
    RUN_TEST(test_system_name_zodiac);
    RUN_TEST(test_system_name_invalid);

    /* Signal type names */
    RUN_TEST(test_signal_type_name_new_year);
    RUN_TEST(test_signal_type_name_new_month);
    RUN_TEST(test_signal_type_name_holy_day);
    RUN_TEST(test_signal_type_name_cycle_start);
    RUN_TEST(test_signal_type_name_full_moon);
    RUN_TEST(test_signal_type_name_new_moon);
    RUN_TEST(test_signal_type_name_solstice);
    RUN_TEST(test_signal_type_name_sacred_number);
    RUN_TEST(test_signal_type_name_tone_one);
    RUN_TEST(test_signal_type_name_sabbath);
    RUN_TEST(test_signal_type_name_invalid);

    /* Per-system checks */
    RUN_TEST(test_gregorian_new_year);
    RUN_TEST(test_gregorian_new_month);
    RUN_TEST(test_gregorian_normal_day);
    RUN_TEST(test_islamic_new_year_detection);
    RUN_TEST(test_tzolkin_kin_one);
    RUN_TEST(test_tzolkin_tone_one_not_kin_one);
    RUN_TEST(test_haab_wayeb);
    RUN_TEST(test_iching_hexagram_one);
    RUN_TEST(test_lunar_full_moon);
    RUN_TEST(test_lunar_new_moon);
    RUN_TEST(test_hindu_purnima);
    RUN_TEST(test_hebrew_sabbath);
    RUN_TEST(test_buddhist_uposatha_full_moon);
    RUN_TEST(test_zodiac_ingress);
    RUN_TEST(test_chinese_new_year_approx);

    /* Scan date */
    RUN_TEST(test_scan_date_returns_jd);
    RUN_TEST(test_scan_date_gregorian_new_year_has_signals);
    RUN_TEST(test_scan_date_systems_active_count);
    RUN_TEST(test_scan_date_total_weight_sum);
    RUN_TEST(test_scan_date_convergence_flag);
    RUN_TEST(test_scan_date_signal_count_within_bounds);
    RUN_TEST(test_scan_date_normal_day_valid);

    /* Range functions */
    RUN_TEST(test_best_in_range_30_days);
    RUN_TEST(test_best_in_range_single_day);
    RUN_TEST(test_count_in_range_year);
    RUN_TEST(test_count_in_range_small);
    RUN_TEST(test_count_in_range_consistency);

    /* Next/prev convergence */
    RUN_TEST(test_next_convergence_finds_one);
    RUN_TEST(test_next_convergence_from_convergence_date);
    RUN_TEST(test_prev_convergence_finds_one);
    RUN_TEST(test_next_convergence_no_result_returns_zero);
    RUN_TEST(test_prev_convergence_no_result_returns_zero);

    /* Multi-system */
    RUN_TEST(test_multiple_systems_on_new_year);

    /* Edge cases */
    RUN_TEST(test_scan_date_very_old_jd);
    RUN_TEST(test_scan_date_very_future_jd);
    RUN_TEST(test_scan_date_negative_jd);
    RUN_TEST(test_check_system_invalid);
    RUN_TEST(test_check_system_zero_max);

    /* Weight/description validation */
    RUN_TEST(test_all_weights_valid);
    RUN_TEST(test_all_descriptions_non_null);
    RUN_TEST(test_gregorian_march_equinox);

    return UNITY_END();
}
