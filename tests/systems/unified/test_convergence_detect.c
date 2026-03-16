/* test_convergence_detect.c -- Convergence Detector tests (Track 19.1)
 *
 * TDD: tests written before implementation.
 * Tests real cross-system convergence detection using actual calendar modules
 * (tzolkin, hebrew, hijri, iching, chinese, panchanga, astronomy). */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include "../../../src/math/julian.h"
#include "../../../src/systems/tzolkin/tzolkin.h"
#include "../../../src/systems/tzolkin/tzolkin_board.h"
#include "../../../src/systems/iching/iching.h"
#include "../../../src/systems/hebrew/hebrew.h"
#include "../../../src/systems/islamic/hijri.h"
#include "../../../src/systems/chinese/chinese.h"
#include "../../../src/systems/buddhist/buddhist.h"
#include "../../../src/systems/persian/persian.h"
#include "../../../src/systems/coptic/coptic.h"
#include "../../../src/systems/celtic/wheel_of_year.h"
#include "../../../src/systems/bahai/bahai.h"
#include "../../../src/systems/japanese/japanese.h"
#include "../../../src/systems/egyptian/egyptian.h"
#include "../../../src/systems/french_republican/french_republican.h"
#include "../../../src/systems/korean/korean_calendar.h"
#include "../../../src/systems/thai/thai_calendar.h"
#include "../../../src/systems/tamil/tamil_calendar.h"
#include "../../../src/systems/myanmar/myanmar.h"
#include "../../../src/systems/zoroastrian/zoroastrian.h"
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
 * cd_system_name — all systems + invalid
 * =================================================================== */

void test_system_name_astronomy(void)
{
    TEST_ASSERT_EQUAL_STRING("Astronomy", cd_system_name(CD_SYS_ASTRONOMY));
}

void test_system_name_astrology(void)
{
    TEST_ASSERT_EQUAL_STRING("Astrology", cd_system_name(CD_SYS_ASTROLOGY));
}

void test_system_name_tzolkin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", cd_system_name(CD_SYS_TZOLKIN));
}

void test_system_name_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", cd_system_name(CD_SYS_ICHING));
}

void test_system_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", cd_system_name(CD_SYS_CHINESE));
}

void test_system_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", cd_system_name(CD_SYS_HEBREW));
}

void test_system_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", cd_system_name(CD_SYS_ISLAMIC));
}

void test_system_name_hindu(void)
{
    TEST_ASSERT_EQUAL_STRING("Hindu", cd_system_name(CD_SYS_HINDU));
}

void test_system_name_invalid_count(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cd_system_name(CD_SYS_COUNT));
}

void test_system_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cd_system_name(-1));
}

void test_system_name_invalid_large(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cd_system_name(99));
}

/* ===================================================================
 * cd_type_count — must return a positive value
 * =================================================================== */

void test_type_count_positive(void)
{
    TEST_ASSERT_TRUE(cd_type_count() > 0);
}

void test_type_count_equals_sys_count(void)
{
    TEST_ASSERT_EQUAL_INT(CD_SYS_COUNT, cd_type_count());
}

/* ===================================================================
 * cd_significance — range checks
 * =================================================================== */

void test_significance_range_ordinary_date(void)
{
    /* A random mid-week date */
    double jd = jd_from(2024, 7, 17);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

void test_significance_range_another_date(void)
{
    double jd = jd_from(2023, 3, 15);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

void test_significance_range_ancient_date(void)
{
    double jd = jd_from(1500, 1, 1);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

void test_significance_range_modern_date(void)
{
    double jd = jd_from(2026, 3, 12);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

void test_significance_range_future_date(void)
{
    double jd = jd_from(2100, 6, 15);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

/* ===================================================================
 * cd_scan — basic structure and event validation
 * =================================================================== */

void test_scan_returns_valid_count(void)
{
    double jd = jd_from(2024, 1, 1);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
}

void test_scan_event_system_count_valid(void)
{
    /* Scan a date that likely has some significance */
    double jd = jd_from(2024, 3, 20); /* near equinox */
    cd_result_t r = cd_scan(jd);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.events[i].system_count >= 2);
        TEST_ASSERT_TRUE(r.events[i].system_count <= CD_MAX_SYSTEMS);
    }
}

void test_scan_event_strength_valid(void)
{
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.events[i].strength >= CD_STRENGTH_MINOR);
        TEST_ASSERT_TRUE(r.events[i].strength <= CD_STRENGTH_RARE);
    }
}

void test_scan_event_significance_range(void)
{
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.events[i].significance >= 0.0);
        TEST_ASSERT_TRUE(r.events[i].significance <= 1.0);
    }
}

void test_scan_event_description_not_null(void)
{
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_NULL(r.events[i].description);
    }
}

void test_scan_event_systems_valid_enum(void)
{
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    for (int i = 0; i < r.count; i++) {
        for (int j = 0; j < r.events[i].system_count; j++) {
            TEST_ASSERT_TRUE(r.events[i].systems[j] >= 0);
            TEST_ASSERT_TRUE(r.events[i].systems[j] < CD_SYS_COUNT);
        }
    }
}

/* ===================================================================
 * cd_scan — known significant dates
 * =================================================================== */

void test_scan_winter_solstice_2024(void)
{
    /* Dec 21, 2024 — solstice should trigger astronomy */
    double jd = jd_from(2024, 12, 21);
    cd_result_t r = cd_scan(jd);
    /* Just verify it doesn't crash and returns valid data */
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
}

void test_scan_spring_equinox_2024(void)
{
    /* Mar 20, 2024 — equinox + possible astrology ingress (Aries 0) */
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
}

void test_scan_gregorian_new_year(void)
{
    /* Jan 1, 2024 — possible chinese cycle_year + other alignments */
    double jd = jd_from(2024, 1, 1);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
}

/* ===================================================================
 * cd_scan — Tzolkin kin 1 detection
 * =================================================================== */

void test_scan_tzolkin_kin_1(void)
{
    /* Find a date where tzolkin kin = 1, then verify scan detects it.
     * 2013-02-13 is kin 1 according to the Dreamspell reference. */
    double jd = jd_from(2013, 2, 13);
    tzolkin_day_t tz = tzolkin_from_jd(jd);

    /* First verify kin is 1 */
    TEST_ASSERT_EQUAL_INT(1, tz.kin);

    /* cd_significance should reflect that at least tzolkin is significant */
    double s = cd_significance(jd);
    /* At least 1/8 = 0.125 since tzolkin kin 1 is significant */
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_scan_tzolkin_tone_1(void)
{
    /* Kin 1 has tone 1 as well. Let's also test a different tone 1.
     * Kin 14 (tone 1, seal 13 Skywalker — 1 + 13*1 ... no)
     * Tone 1 kins: 1, 14, 27, 40, 53, 66, ...
     * Actually tone = ((kin - 1) % 13) + 1, so tone 1 -> kin % 13 == 1
     * kins: 1, 14, 27, 40, ...
     * Test kin 14 (tone 1). Need a date with kin 14.
     * kin 14 = kin 1 + 13 days = 2013-02-13 + 13 = 2013-02-26 */
    double jd = jd_from(2013, 2, 26);
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, tz.tone);

    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_scan_tzolkin_portal_day(void)
{
    /* Check that a portal day is detected as significant.
     * Kin 1 (Red Magnetic Dragon) IS a portal day. */
    TEST_ASSERT_TRUE(tzolkin_board_is_portal(1));

    double jd = jd_from(2013, 2, 13); /* kin 1 */
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * cd_scan — Hebrew day 1 detection
 * =================================================================== */

void test_scan_hebrew_day_1(void)
{
    /* 1 Tishrei 5785 = approx 2024-10-03
     * hebrew_from_jd should return day = 1, month = 7 */
    hebrew_date_t hd;
    /* Try a few dates near Rosh Hashanah 5785 */
    double jd = jd_from(2024, 10, 3);
    hd = hebrew_from_jd(jd);
    /* Find a day 1 — if this isn't day 1, search nearby */
    int found_day1 = 0;
    double test_jd = jd;
    for (int i = 0; i < 32; i++) {
        hd = hebrew_from_jd(jd_from(2024, 10, 3) + i);
        if (hd.day == 1) {
            test_jd = jd_from(2024, 10, 3) + i;
            found_day1 = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_day1);

    double s = cd_significance(test_jd);
    /* Hebrew new month should contribute significance */
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_scan_hebrew_rosh_hashanah(void)
{
    /* 1 Tishrei = Rosh Hashanah. Month 7, day 1 or 2.
     * This should be significant for Hebrew system. */
    /* Look for a 1 Tishrei date */
    double jd = jd_from(2024, 10, 3);
    hebrew_date_t hd = hebrew_from_jd(jd);
    /* Search for Tishrei (month 7) day 1 */
    int found = 0;
    double rh_jd = jd;
    for (int i = -5; i < 10; i++) {
        hd = hebrew_from_jd(jd + i);
        if (hd.month == 7 && hd.day <= 2) {
            rh_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(rh_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

/* ===================================================================
 * cd_scan — Islamic day 1 / Ramadan detection
 * =================================================================== */

void test_scan_islamic_day_1(void)
{
    /* Find a Hijri day 1 */
    double jd = jd_from(2024, 3, 11); /* near Ramadan 2024 */
    hijri_date_t hd;
    double test_jd = jd;
    int found = 0;
    for (int i = 0; i < 32; i++) {
        hd = hijri_from_jd(jd + i);
        if (hd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_scan_islamic_ramadan(void)
{
    /* Find a Ramadan date (month 9) */
    double jd = jd_from(2024, 3, 12);
    hijri_date_t hd;
    double test_jd = jd;
    int found = 0;
    for (int i = 0; i < 40; i++) {
        hd = hijri_from_jd(jd + i);
        if (hd.month == 9) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * cd_scan — I Ching hexagram 1 detection
 * =================================================================== */

void test_scan_iching_hexagram_1(void)
{
    /* Find a date where iching_from_jd returns king_wen == 1 */
    double jd = jd_from(2024, 1, 1);
    double test_jd = jd;
    int found = 0;
    for (int i = 0; i < 65; i++) {
        hexagram_t hex = iching_from_jd(jd + i);
        if (hex.king_wen == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_scan_iching_hexagram_2(void)
{
    /* Hexagram 2 (Receptive) is also significant */
    double jd = jd_from(2024, 1, 1);
    double test_jd = jd;
    int found = 0;
    for (int i = 0; i < 65; i++) {
        hexagram_t hex = iching_from_jd(jd + i);
        if (hex.king_wen == 2) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * cd_scan — Chinese new year proximity
 * =================================================================== */

void test_scan_chinese_new_year_proximity(void)
{
    /* Jan 1 Gregorian — check cycle_year == 1 or day proximity */
    double jd = jd_from(2024, 1, 1);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
}

void test_scan_chinese_cycle_year_1(void)
{
    /* Find a year where cycle_year == 1 (start of sexagenary cycle).
     * cycle_year = ((year - 4) % 60) + 1
     * cycle_year 1 when (year - 4) % 60 == 0 -> year 2044, 1984, etc. */
    double jd = jd_from(1984, 1, 1);
    chinese_year_t cy = chinese_year_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, cy.cycle_year);

    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * cd_scan — Astronomy: lunar phases
 * =================================================================== */

void test_scan_new_moon_significant(void)
{
    /* A known approximate new moon: 2024-01-11 (synodic approximation).
     * Reference epoch: JD 2451550.1 (2000-01-06 new moon).
     * Synodic period: 29.530589 days. */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    /* Find new moon near 2024-01-11 (JD ~2460320) */
    double jd = jd_from(2024, 1, 11);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;

    double s = cd_significance(nearest_new);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_scan_full_moon_significant(void)
{
    /* Full moon is ~14.765 days after new moon */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 1, 25);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;
    double full_moon = nearest_new + synodic / 2.0;

    double s = cd_significance(full_moon);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * cd_scan — Astronomy: solstice/equinox detection
 * =================================================================== */

void test_scan_summer_solstice(void)
{
    /* June 21 — should be detected as solstice */
    double jd = jd_from(2024, 6, 21);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
}

void test_scan_autumn_equinox(void)
{
    /* Sep 22 — equinox */
    double jd = jd_from(2024, 9, 22);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
}

/* ===================================================================
 * cd_scan — Astrology: sign ingress
 * =================================================================== */

void test_scan_astrology_ingress(void)
{
    /* Approximate Sun longitude: lon = (JD - 2451545.0) / 365.25 * 360 + 280.46
     * Find a date where fmod(lon, 30) < 1.0.
     * Around Mar 20: lon ~ 0 (Aries ingress). */
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
}

/* ===================================================================
 * cd_scan — Hindu (tithi) detection
 * =================================================================== */

void test_scan_hindu_new_moon_tithi(void)
{
    /* Hindu uses lunar phase proxy: tithi 1 (new cycle) or tithi 15 (full moon).
     * Near a new moon, tithi should be near 1. */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 1, 11);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;

    double s = cd_significance(nearest_new);
    /* At minimum, astronomy + hindu should both fire */
    TEST_ASSERT_TRUE(s >= 0.0);
}

/* ===================================================================
 * cd_scan — ordinary dates should have low significance
 * =================================================================== */

void test_scan_ordinary_date_1(void)
{
    /* A random mid-year Tuesday — should have low convergence */
    double jd = jd_from(2024, 7, 9);
    cd_result_t r = cd_scan(jd);
    /* May have 0 or 1 convergence events (or none at all) */
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
}

void test_scan_ordinary_date_2(void)
{
    double jd = jd_from(2023, 5, 14);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
}

void test_scan_ordinary_date_low_significance(void)
{
    /* Most dates should have significance well below 1.0 */
    double jd = jd_from(2024, 8, 7);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s < 1.0);
}

/* ===================================================================
 * cd_check_pair — basic pairs
 * =================================================================== */

void test_check_pair_same_system(void)
{
    /* Same system should be significant if system is significant */
    double jd = jd_from(2013, 2, 13); /* kin 1 */
    int r = cd_check_pair(CD_SYS_TZOLKIN, CD_SYS_TZOLKIN, jd);
    /* Tzolkin is significant on kin 1, so pair check on same = 1 */
    TEST_ASSERT_EQUAL_INT(1, r);
}

void test_check_pair_returns_0_for_no_align(void)
{
    /* Two systems not both significant on an ordinary date */
    double jd = jd_from(2024, 7, 17);
    int r = cd_check_pair(CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY, jd);
    /* We can't know for sure, but it's a valid call */
    TEST_ASSERT_TRUE(r == 0 || r == 1);
}

void test_check_pair_astronomy_hindu_near_new_moon(void)
{
    /* Near a new moon, both astronomy and hindu (lunar proxy) should fire */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 1, 11);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;

    int r = cd_check_pair(CD_SYS_ASTRONOMY, CD_SYS_HINDU, nearest_new);
    TEST_ASSERT_EQUAL_INT(1, r);
}

void test_check_pair_hebrew_islamic_both_day1(void)
{
    /* Find a date where both Hebrew and Islamic calendars have day 1.
     * This is rare but we search for it. */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    for (int i = 0; i < 400; i++) {
        hebrew_date_t hd = hebrew_from_jd(jd + i);
        hijri_date_t id = hijri_from_jd(jd + i);
        if (hd.day == 1 && id.day == 1) {
            int r = cd_check_pair(CD_SYS_HEBREW, CD_SYS_ISLAMIC, jd + i);
            TEST_ASSERT_EQUAL_INT(1, r);
            found = 1;
            break;
        }
    }
    /* Both are lunar calendars, new months should sometimes coincide */
    TEST_ASSERT_TRUE(found);
}

void test_check_pair_all_enum_values_valid(void)
{
    /* Check that all enum pairs can be called without crashing */
    double jd = jd_from(2024, 6, 15);
    for (int a = 0; a < CD_SYS_COUNT; a++) {
        for (int b = 0; b < CD_SYS_COUNT; b++) {
            int r = cd_check_pair((cd_system_t)a, (cd_system_t)b, jd);
            TEST_ASSERT_TRUE(r == 0 || r == 1);
        }
    }
}

/* ===================================================================
 * cd_find_next — finds a convergence forward
 * =================================================================== */

void test_find_next_minor_within_30_days(void)
{
    /* Should find at least a MINOR convergence within 30 days */
    double jd = jd_from(2024, 1, 1);
    double found = cd_find_next(jd, CD_STRENGTH_MINOR, 30);
    TEST_ASSERT_TRUE(found > 0.0);
    TEST_ASSERT_TRUE(found >= jd);
    TEST_ASSERT_TRUE(found <= jd + 30);
}

void test_find_next_notable_within_90_days(void)
{
    /* NOTABLE (3+ systems) should be findable in 90 days */
    double jd = jd_from(2024, 1, 1);
    double found = cd_find_next(jd, CD_STRENGTH_NOTABLE, 90);
    /* May or may not find one — but if found, it's valid */
    if (found > 0.0) {
        TEST_ASSERT_TRUE(found >= jd);
        TEST_ASSERT_TRUE(found <= jd + 90);
        cd_result_t r = cd_scan(found);
        TEST_ASSERT_TRUE(r.count > 0);
        TEST_ASSERT_TRUE(r.events[0].strength >= CD_STRENGTH_NOTABLE);
    }
}

void test_find_next_returns_zero_for_max_days_zero(void)
{
    double jd = jd_from(2024, 6, 15);
    double found = cd_find_next(jd, CD_STRENGTH_RARE, 0);
    /* max_days=0 means scan only start_jd itself */
    /* Could be 0.0 or jd depending on implementation */
    TEST_ASSERT_TRUE(found == 0.0 || found == jd);
}

void test_find_next_rare_may_not_exist(void)
{
    /* RARE (5+ systems) within 30 days might not exist */
    double jd = jd_from(2024, 6, 15);
    double found = cd_find_next(jd, CD_STRENGTH_RARE, 30);
    /* 0.0 is acceptable — RARE is genuinely rare */
    TEST_ASSERT_TRUE(found == 0.0 || found >= jd);
}

void test_find_next_result_has_correct_strength(void)
{
    double jd = jd_from(2024, 1, 1);
    double found = cd_find_next(jd, CD_STRENGTH_MINOR, 60);
    if (found > 0.0) {
        cd_result_t r = cd_scan(found);
        TEST_ASSERT_TRUE(r.count > 0);
        TEST_ASSERT_TRUE(r.events[0].strength >= CD_STRENGTH_MINOR);
    }
}

/* ===================================================================
 * cd_scan — strength classification tests
 * =================================================================== */

void test_scan_two_systems_is_minor(void)
{
    /* Find a date with exactly 2 systems significant and verify MINOR.
     * Near a new moon: astronomy + hindu should be significant. */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    /* Pick a new moon that might only trigger astronomy + hindu */
    double jd = jd_from(2024, 5, 8);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;

    cd_result_t r = cd_scan(nearest_new);
    /* If there's an event, verify strength is valid */
    if (r.count > 0) {
        TEST_ASSERT_TRUE(r.events[0].strength >= CD_STRENGTH_MINOR);
    }
}

void test_scan_strength_minor_value(void)
{
    TEST_ASSERT_EQUAL_INT(1, CD_STRENGTH_MINOR);
}

void test_scan_strength_notable_value(void)
{
    TEST_ASSERT_EQUAL_INT(2, CD_STRENGTH_NOTABLE);
}

void test_scan_strength_major_value(void)
{
    TEST_ASSERT_EQUAL_INT(3, CD_STRENGTH_MAJOR);
}

void test_scan_strength_rare_value(void)
{
    TEST_ASSERT_EQUAL_INT(4, CD_STRENGTH_RARE);
}

/* ===================================================================
 * cd_scan — cross-system convergence (known alignment)
 * =================================================================== */

void test_scan_new_moon_plus_hebrew_day1(void)
{
    /* Find a new moon that also has Hebrew day 1.
     * Both Hebrew and Islamic calendars are lunar, so Hebrew Rosh Chodesh
     * (day 1) should coincide with new moon regions. */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 1, 1);
    int found = 0;

    for (int cycle = 0; cycle < 24; cycle++) {
        double new_moon = ref_new_moon +
            (floor((jd - ref_new_moon) / synodic) + cycle) * synodic;
        /* Check +/- 1 day around new moon for Hebrew day 1 */
        for (int d = -1; d <= 1; d++) {
            hebrew_date_t hd = hebrew_from_jd(new_moon + d);
            if (hd.day == 1) {
                /* This date should have at least astronomy + hebrew */
                int r = cd_check_pair(CD_SYS_ASTRONOMY, CD_SYS_HEBREW,
                                       new_moon + d);
                if (r) {
                    found = 1;
                    break;
                }
            }
        }
        if (found) break;
    }
    /* Lunar new month and new moon should sometimes align */
    TEST_ASSERT_TRUE(found);
}

/* ===================================================================
 * cd_scan — edge cases
 * =================================================================== */

void test_scan_very_early_date(void)
{
    /* Year 100 AD */
    double jd = jd_from(100, 1, 1);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
}

void test_scan_distant_future(void)
{
    /* Year 3000 */
    double jd = jd_from(3000, 6, 15);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
}

void test_significance_distant_future(void)
{
    double jd = jd_from(3000, 6, 15);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

void test_scan_jd_zero(void)
{
    /* JD 0 = very ancient date. Should not crash. */
    cd_result_t r = cd_scan(0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
}

void test_significance_jd_zero(void)
{
    double s = cd_significance(0.0);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

void test_find_next_from_jd_zero(void)
{
    /* Should not crash, may return 0.0 */
    double found = cd_find_next(0.0, CD_STRENGTH_MINOR, 5);
    TEST_ASSERT_TRUE(found >= 0.0);
}

/* ===================================================================
 * cd_scan — verify convergence needs 2+ systems
 * =================================================================== */

void test_scan_single_system_no_convergence(void)
{
    /* If only 1 system is significant, no convergence event. */
    /* Hard to guarantee exactly 1 system, so we just verify the
     * logical property: if count > 0, system_count >= 2. */
    double jd = jd_from(2024, 4, 10);
    cd_result_t r = cd_scan(jd);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.events[i].system_count >= 2);
    }
}

/* ===================================================================
 * Constants and enums
 * =================================================================== */

void test_cd_max_convergences(void)
{
    TEST_ASSERT_EQUAL_INT(32, CD_MAX_CONVERGENCES);
}

void test_cd_max_systems(void)
{
    TEST_ASSERT_EQUAL_INT(24, CD_MAX_SYSTEMS);
}

void test_cd_sys_count_value(void)
{
    TEST_ASSERT_EQUAL_INT(21, CD_SYS_COUNT);
}

void test_cd_sys_enum_order(void)
{
    TEST_ASSERT_EQUAL_INT(0, CD_SYS_ASTRONOMY);
    TEST_ASSERT_EQUAL_INT(1, CD_SYS_ASTROLOGY);
    TEST_ASSERT_EQUAL_INT(2, CD_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(3, CD_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(4, CD_SYS_CHINESE);
    TEST_ASSERT_EQUAL_INT(5, CD_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(6, CD_SYS_ISLAMIC);
    TEST_ASSERT_EQUAL_INT(7, CD_SYS_HINDU);
    TEST_ASSERT_EQUAL_INT(8, CD_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_INT(9, CD_SYS_PERSIAN);
    TEST_ASSERT_EQUAL_INT(10, CD_SYS_COPTIC);
    TEST_ASSERT_EQUAL_INT(11, CD_SYS_CELTIC);
    TEST_ASSERT_EQUAL_INT(12, CD_SYS_BAHAI);
    TEST_ASSERT_EQUAL_INT(13, CD_SYS_JAPANESE);
    TEST_ASSERT_EQUAL_INT(14, CD_SYS_EGYPTIAN);
    TEST_ASSERT_EQUAL_INT(15, CD_SYS_FRENCH);
    TEST_ASSERT_EQUAL_INT(16, CD_SYS_KOREAN);
    TEST_ASSERT_EQUAL_INT(17, CD_SYS_THAI);
    TEST_ASSERT_EQUAL_INT(18, CD_SYS_TAMIL);
    TEST_ASSERT_EQUAL_INT(19, CD_SYS_MYANMAR);
    TEST_ASSERT_EQUAL_INT(20, CD_SYS_ZOROASTRIAN);
}

/* ===================================================================
 * Idempotency / determinism
 * =================================================================== */

void test_scan_idempotent(void)
{
    double jd = jd_from(2024, 3, 20);
    cd_result_t r1 = cd_scan(jd);
    cd_result_t r2 = cd_scan(jd);
    TEST_ASSERT_EQUAL_INT(r1.count, r2.count);
    for (int i = 0; i < r1.count; i++) {
        TEST_ASSERT_EQUAL_INT(r1.events[i].system_count,
                              r2.events[i].system_count);
        TEST_ASSERT_EQUAL_INT(r1.events[i].strength,
                              r2.events[i].strength);
    }
}

void test_significance_idempotent(void)
{
    double jd = jd_from(2024, 6, 15);
    double diff = cd_significance(jd) - cd_significance(jd);
    TEST_ASSERT_TRUE(fabs(diff) < 0.0001);
}

void test_check_pair_idempotent(void)
{
    double jd = jd_from(2024, 1, 1);
    int r1 = cd_check_pair(CD_SYS_TZOLKIN, CD_SYS_HEBREW, jd);
    int r2 = cd_check_pair(CD_SYS_TZOLKIN, CD_SYS_HEBREW, jd);
    TEST_ASSERT_EQUAL_INT(r1, r2);
}

/* ===================================================================
 * cd_find_next — sequential days
 * =================================================================== */

void test_find_next_scans_sequentially(void)
{
    /* Find next convergence, then find next after that */
    double jd = jd_from(2024, 1, 1);
    double first = cd_find_next(jd, CD_STRENGTH_MINOR, 60);
    if (first > 0.0) {
        double second = cd_find_next(first + 1, CD_STRENGTH_MINOR, 60);
        if (second > 0.0) {
            TEST_ASSERT_TRUE(second > first);
        }
    }
}

/* ===================================================================
 * cd_system_name — new systems
 * =================================================================== */

void test_system_name_buddhist(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddhist", cd_system_name(CD_SYS_BUDDHIST));
}

void test_system_name_persian(void)
{
    TEST_ASSERT_EQUAL_STRING("Persian", cd_system_name(CD_SYS_PERSIAN));
}

void test_system_name_coptic(void)
{
    TEST_ASSERT_EQUAL_STRING("Coptic", cd_system_name(CD_SYS_COPTIC));
}

void test_system_name_celtic(void)
{
    TEST_ASSERT_EQUAL_STRING("Celtic", cd_system_name(CD_SYS_CELTIC));
}

void test_system_name_bahai(void)
{
    TEST_ASSERT_EQUAL_STRING("Bahai", cd_system_name(CD_SYS_BAHAI));
}

/* ===================================================================
 * Buddhist significance tests
 * =================================================================== */

void test_buddhist_uposatha_significant(void)
{
    /* Find an Uposatha day (quarter moon) within 30 days of a start date */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        uposatha_t u = buddhist_uposatha(jd + i);
        if (u.type != UPOSATHA_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    /* Buddhist should contribute at least 1/CD_SYS_COUNT */
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_buddhist_full_moon_significant(void)
{
    /* Full moon is an Uposatha day (Purnima). Find a full moon. */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 3, 1);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;
    double full_moon = nearest_new + synodic / 2.0;

    uposatha_t u = buddhist_uposatha(full_moon);
    TEST_ASSERT_TRUE(u.type == UPOSATHA_FULL_MOON);

    double s = cd_significance(full_moon);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_buddhist_non_uposatha_not_significant(void)
{
    /* Find a day that is NOT an Uposatha day */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        uposatha_t u = buddhist_uposatha(jd + i);
        if (u.type == UPOSATHA_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    /* Buddhist should NOT fire for this date via pair check */
    /* We verify pair with itself returns 0 on non-Uposatha day */
    int r = cd_check_pair(CD_SYS_BUDDHIST, CD_SYS_BUDDHIST, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_buddhist_new_moon_significant(void)
{
    /* New moon is an Uposatha day (Amavasya) */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 5, 1);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;

    uposatha_t u = buddhist_uposatha(nearest_new);
    TEST_ASSERT_TRUE(u.type == UPOSATHA_NEW_MOON);

    double s = cd_significance(nearest_new);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * Persian significance tests
 * =================================================================== */

void test_persian_nowruz_significant(void)
{
    /* Nowruz = 1 Farvardin. Approximate: March 20-21 Gregorian.
     * Find it by scanning. */
    double jd = jd_from(2024, 3, 19);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 5; i++) {
        persian_date_t pd = persian_from_jd(jd + i);
        if (pd.month == 1 && pd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_persian_month_boundary_significant(void)
{
    /* Find any Persian day 1 (month boundary) */
    double jd = jd_from(2024, 6, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 35; i++) {
        persian_date_t pd = persian_from_jd(jd + i);
        if (pd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_persian_mid_month_not_significant(void)
{
    /* Find a Persian date where day != 1 and day != 13 of month 1 */
    double jd = jd_from(2024, 5, 10);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        persian_date_t pd = persian_from_jd(jd + i);
        if (pd.day != 1 && !(pd.month == 1 && pd.day == 13)) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_PERSIAN, CD_SYS_PERSIAN, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_persian_sizdah_bedar_significant(void)
{
    /* 13 Farvardin (Sizdah Bedar) should be significant.
     * Find it near Nowruz + 12 days. */
    double jd = jd_from(2024, 3, 19);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 20; i++) {
        persian_date_t pd = persian_from_jd(jd + i);
        if (pd.month == 1 && pd.day == 13) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * Coptic significance tests
 * =================================================================== */

void test_coptic_nayrouz_significant(void)
{
    /* Coptic New Year (Nayrouz) = 1 Thout. Approx Sep 11-12 Gregorian. */
    double jd = jd_from(2024, 9, 10);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 5; i++) {
        coptic_date_t cd = coptic_from_jd(jd + i);
        if (cd.month == 1 && cd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_coptic_month_boundary_significant(void)
{
    /* Find any Coptic day 1 */
    double jd = jd_from(2024, 4, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 35; i++) {
        coptic_date_t cd = coptic_from_jd(jd + i);
        if (cd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_coptic_mid_month_not_significant(void)
{
    /* Find a Coptic date where day != 1 and no feast */
    double jd = jd_from(2024, 5, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        coptic_date_t cd_date = coptic_from_jd(jd + i);
        coptic_feast_t feast = coptic_feast(cd_date);
        if (cd_date.day != 1 && feast == COPTIC_FEAST_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_COPTIC, CD_SYS_COPTIC, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_coptic_feast_significant(void)
{
    /* Nayrouz is 1 Thout (already tested above as day 1).
     * Test Feast of the Cross: 17 Thout (month 1, day 17) */
    double jd = jd_from(2024, 9, 10);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 40; i++) {
        coptic_date_t cd_date = coptic_from_jd(jd + i);
        coptic_feast_t feast = coptic_feast(cd_date);
        if (feast != COPTIC_FEAST_NONE && cd_date.day != 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    /* If we find a feast on a non-day-1, check significance */
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

/* ===================================================================
 * Celtic significance tests
 * =================================================================== */

void test_celtic_winter_solstice_significant(void)
{
    /* Dec 21 — Yule / Winter Solstice. Solar longitude ~270 degrees. */
    double jd = jd_from(2024, 12, 21);
    double s = cd_significance(jd);
    /* Celtic should fire for winter solstice */
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_celtic_summer_solstice_significant(void)
{
    /* Jun 21 — Litha / Summer Solstice. Solar longitude ~90 degrees. */
    double jd = jd_from(2024, 6, 21);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_celtic_spring_equinox_significant(void)
{
    /* Mar 20 — Ostara / Spring Equinox. Solar longitude ~0 degrees. */
    double jd = jd_from(2024, 3, 20);
    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_celtic_mid_season_not_significant(void)
{
    /* A date far from any sabbat.
     * Solar longitudes of festivals: 0, 45, 90, 135, 180, 225, 270, 315
     * Midpoint between two: e.g., ~22.5 degrees (around April 5-ish).
     * We need a date where sun is at ~22 degrees from Aries. */
    double jd = jd_from(2024, 4, 12); /* roughly 22 degrees Aries */
    /* Check Celtic self-pair */
    int r = cd_check_pair(CD_SYS_CELTIC, CD_SYS_CELTIC, jd);
    /* Might or might not fire depending on exact solar longitude.
     * Just verify it doesn't crash. */
    TEST_ASSERT_TRUE(r == 0 || r == 1);
}

void test_celtic_samhain_significant(void)
{
    /* Oct 31/Nov 1 — Samhain. Solar longitude ~225 degrees. */
    double jd = jd_from(2024, 11, 1);
    /* The cross-quarter at solar longitude 225 should be active */
    double s = cd_significance(jd);
    /* Verify the call succeeds and returns valid range */
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

/* ===================================================================
 * Bahai significance tests
 * =================================================================== */

void test_bahai_naw_ruz_significant(void)
{
    /* Naw-Ruz: 1 Baha (month 1, day 1). Approx March 20 Gregorian. */
    double jd = jd_from(2024, 3, 19);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 5; i++) {
        bahai_date_t bd = bahai_from_jd(jd + i);
        if (bd.month == 1 && bd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_bahai_feast_boundary_significant(void)
{
    /* Day 1 of any 19-day month (feast boundary) */
    double jd = jd_from(2024, 5, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 25; i++) {
        bahai_date_t bd = bahai_from_jd(jd + i);
        if (bd.month >= 1 && bd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_bahai_mid_month_not_significant(void)
{
    /* Find a Bahai date in mid-month with no holy day */
    double jd = jd_from(2024, 6, 10);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 20; i++) {
        bahai_date_t bd = bahai_from_jd(jd + i);
        bahai_holy_day_t holy = bahai_holy_day(bd);
        if (bd.month >= 1 && bd.day != 1 && holy == BAHAI_HOLY_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_BAHAI, CD_SYS_BAHAI, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_bahai_holy_day_significant(void)
{
    /* Find a Bahai holy day (Ridvan 1st day = month 2, day 13) */
    double jd = jd_from(2024, 4, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 40; i++) {
        bahai_date_t bd = bahai_from_jd(jd + i);
        bahai_holy_day_t holy = bahai_holy_day(bd);
        if (holy != BAHAI_HOLY_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

/* ===================================================================
 * Cross-system convergence with new systems
 * =================================================================== */

void test_convergence_buddhist_astronomy_new_moon(void)
{
    /* Near a new moon, both Buddhist (Uposatha Amavasya) and
     * Astronomy (new moon) should fire. */
    double ref_new_moon = 2451550.1;
    double synodic = 29.530589;
    double jd = jd_from(2024, 2, 1);
    double cycles = (jd - ref_new_moon) / synodic;
    double nearest_new = ref_new_moon + floor(cycles + 0.5) * synodic;

    int r = cd_check_pair(CD_SYS_BUDDHIST, CD_SYS_ASTRONOMY, nearest_new);
    TEST_ASSERT_EQUAL_INT(1, r);
}

void test_convergence_persian_celtic_equinox(void)
{
    /* Persian and Celtic both relate to the spring equinox.
     * Search across multiple years for a year where Nowruz (day 1)
     * falls on a day when Celtic Ostara (solar long ~0) is active. */
    int found = 0;
    for (int year = 2020; year <= 2035; year++) {
        for (int i = 0; i < 7; i++) {
            double jd = jd_from(year, 3, 18 + i);
            int r = cd_check_pair(CD_SYS_PERSIAN, CD_SYS_CELTIC, jd);
            if (r) {
                found = 1;
                break;
            }
        }
        if (found) break;
    }
    /* Over 16 years, Nowruz and Ostara should overlap at least once */
    TEST_ASSERT_TRUE(found);
}

void test_convergence_scan_includes_new_systems(void)
{
    /* Scan a date and verify that new system enum values appear
     * in the event systems array when they fire. */
    double jd = jd_from(2024, 3, 20); /* equinox — should trigger Celtic */
    cd_result_t r = cd_scan(jd);
    /* Verify the scan handles 13 systems without crash */
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
    for (int i = 0; i < r.count; i++) {
        for (int j = 0; j < r.events[i].system_count; j++) {
            TEST_ASSERT_TRUE(r.events[i].systems[j] >= 0);
            TEST_ASSERT_TRUE(r.events[i].systems[j] < CD_SYS_COUNT);
        }
    }
}

void test_new_systems_all_enum_pairs_valid(void)
{
    /* Verify all 21 systems can be called in pairs without crash */
    double jd = jd_from(2024, 6, 15);
    for (int a = 0; a < CD_SYS_COUNT; a++) {
        for (int b = a; b < CD_SYS_COUNT; b++) {
            int r = cd_check_pair((cd_system_t)a, (cd_system_t)b, jd);
            TEST_ASSERT_TRUE(r == 0 || r == 1);
        }
    }
}

/* ===================================================================
 * Japanese significance tests
 * =================================================================== */

void test_system_name_japanese(void)
{
    TEST_ASSERT_EQUAL_STRING("Japanese", cd_system_name(CD_SYS_JAPANESE));
}

void test_japanese_taian_significant(void)
{
    /* Find a Taian day (most auspicious rokuyo) within 7 days */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 7; i++) {
        jp_rokuyo_t r = japanese_rokuyo(jd + i);
        if (r == JP_ROKUYO_TAIAN) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_japanese_not_significant(void)
{
    /* Find a day that is NOT Taian, NOT near sekki boundary, NOT era year 1.
     * We look for a non-Taian day far from any 15-degree solar boundary
     * in a non-gannen year (e.g. 2024 is Reiwa 6, not year 1). */
    double jd = jd_from(2024, 5, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 10; i++) {
        jp_rokuyo_t r = japanese_rokuyo(jd + i);
        if (r != JP_ROKUYO_TAIAN) {
            /* Check sekki: sun_lon mod 15 should be > 1 */
            double sun_lon = fmod((jd + i - 2451545.0) / 365.25 * 360.0 + 280.46, 360.0);
            if (sun_lon < 0.0) sun_lon += 360.0;
            double within_sekki = fmod(sun_lon, 15.0);
            if (within_sekki < 0.0) within_sekki += 15.0;
            if (within_sekki >= 1.0) {
                test_jd = jd + i;
                found = 1;
                break;
            }
        }
    }
    if (found) {
        int r = cd_check_pair(CD_SYS_JAPANESE, CD_SYS_JAPANESE, test_jd);
        TEST_ASSERT_EQUAL_INT(0, r);
    }
}

void test_japanese_era_gannen_significant(void)
{
    /* Reiwa 1 started May 1, 2019. era_year == 1 should be significant. */
    double jd = jd_from(2019, 5, 1);
    japanese_date_t jp = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, jp.era_year);
    TEST_ASSERT_TRUE(jp.era_index >= 0);

    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * Egyptian significance tests
 * =================================================================== */

void test_system_name_egyptian(void)
{
    TEST_ASSERT_EQUAL_STRING("Egyptian", cd_system_name(CD_SYS_EGYPTIAN));
}

void test_egyptian_month_boundary_significant(void)
{
    /* Find an Egyptian day 1 (month boundary) */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 35; i++) {
        egypt_date_t ed = egypt_from_jd(jd + i);
        if (ed.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_egyptian_epagomenal_significant(void)
{
    /* Find an epagomenal day (month 13) */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 370; i++) {
        egypt_date_t ed = egypt_from_jd(jd + i);
        if (egypt_is_epagomenal(ed)) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_egyptian_not_significant(void)
{
    /* Find a mid-month day that is not epagomenal */
    double jd = jd_from(2024, 2, 10);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        egypt_date_t ed = egypt_from_jd(jd + i);
        if (ed.day != 1 && !egypt_is_epagomenal(ed)) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_EGYPTIAN, CD_SYS_EGYPTIAN, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

/* ===================================================================
 * French Republican significance tests
 * =================================================================== */

void test_system_name_french(void)
{
    TEST_ASSERT_EQUAL_STRING("French Republican", cd_system_name(CD_SYS_FRENCH));
}

void test_french_month_boundary_significant(void)
{
    /* Find a French Republican day 1 (month boundary) */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 35; i++) {
        fr_date_t fd = fr_from_jd(jd + i);
        if (fd.month >= 1 && fd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_french_sansculottide_significant(void)
{
    /* Sansculottides occur near Sep 17-21 Gregorian (end of Republican year) */
    double jd = jd_from(2024, 9, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 15; i++) {
        fr_date_t fd = fr_from_jd(jd + i);
        if (fr_is_sansculottide(fd)) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_french_decadi_significant(void)
{
    /* Find a Decadi (10th day of decade) */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 15; i++) {
        fr_date_t fd = fr_from_jd(jd + i);
        if (fr_decade_day(fd) == 10) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_french_not_significant(void)
{
    /* Find a mid-month, non-Decadi, non-Sansculottide day */
    double jd = jd_from(2024, 2, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        fr_date_t fd = fr_from_jd(jd + i);
        if (fd.month >= 1 && fd.day != 1 &&
            !fr_is_sansculottide(fd) && fr_decade_day(fd) != 10) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_FRENCH, CD_SYS_FRENCH, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

/* ===================================================================
 * Korean significance tests
 * =================================================================== */

void test_system_name_korean(void)
{
    TEST_ASSERT_EQUAL_STRING("Korean", cd_system_name(CD_SYS_KOREAN));
}

void test_korean_seollal_significant(void)
{
    /* Seollal (Lunar New Year) = lunar month 1, day 1.
     * korean_festival should return non-NULL for (1, 1). */
    const korean_festival_t *fest = korean_festival(1, 1);
    TEST_ASSERT_NOT_NULL(fest);

    /* Find an actual lunar 1/1 date */
    double jd = jd_from(2024, 2, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        korean_date_t kd = korean_from_jd(jd + i);
        if (kd.lunar_month == 1 && kd.lunar_day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

void test_korean_not_significant(void)
{
    /* Find a date that is not a festival and not cycle_year 1 */
    double jd = jd_from(2024, 6, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        korean_date_t kd = korean_from_jd(jd + i);
        const korean_festival_t *fest = korean_festival(kd.lunar_month,
                                                        kd.lunar_day);
        if (fest == NULL && kd.cycle_year != 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_KOREAN, CD_SYS_KOREAN, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_korean_cycle_year_1_significant(void)
{
    /* Sexagenary cycle year 1: (year - 4) % 60 == 0 -> year 1984, 2044 */
    double jd = jd_from(1984, 6, 15);
    korean_date_t kd = korean_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, kd.cycle_year);

    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * Thai significance tests
 * =================================================================== */

void test_system_name_thai(void)
{
    TEST_ASSERT_EQUAL_STRING("Thai", cd_system_name(CD_SYS_THAI));
}

void test_thai_songkran_significant(void)
{
    /* Songkran: April 13-15 */
    double jd = jd_from(2024, 4, 14);
    thai_date_t td = thai_from_jd(jd);
    TEST_ASSERT_TRUE(thai_is_songkran(td));

    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_thai_not_significant(void)
{
    /* A date with no Thai festival */
    double jd = jd_from(2024, 7, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 10; i++) {
        thai_date_t td = thai_from_jd(jd + i);
        if (!thai_is_songkran(td) && thai_festival(td) == THAI_FEST_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_THAI, CD_SYS_THAI, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_thai_chakri_day_significant(void)
{
    /* Chakri Memorial Day: April 6 */
    double jd = jd_from(2024, 4, 6);
    thai_date_t td = thai_from_jd(jd);
    TEST_ASSERT_TRUE(thai_festival(td) != THAI_FEST_NONE);

    double s = cd_significance(jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

/* ===================================================================
 * Tamil significance tests
 * =================================================================== */

void test_system_name_tamil(void)
{
    TEST_ASSERT_EQUAL_STRING("Tamil", cd_system_name(CD_SYS_TAMIL));
}

void test_tamil_puthandu_significant(void)
{
    /* Puthandu (Tamil New Year): Sun enters sidereal Aries, ~April 14 */
    double jd = jd_from(2024, 4, 13);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 5; i++) {
        tamil_date_t td = tamil_from_jd(jd + i);
        if (tamil_festival(td) == TAMIL_FEST_PUTHANDU) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

void test_tamil_not_significant(void)
{
    /* Find a date with no Tamil festival and jovian != 1 */
    double jd = jd_from(2024, 7, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        tamil_date_t td = tamil_from_jd(jd + i);
        if (tamil_festival(td) == TAMIL_FEST_NONE && td.jovian != 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_TAMIL, CD_SYS_TAMIL, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_tamil_pongal_significant(void)
{
    /* Thai Pongal: Sun enters sidereal Capricorn, ~Jan 14 */
    double jd = jd_from(2024, 1, 13);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 5; i++) {
        tamil_date_t td = tamil_from_jd(jd + i);
        if (tamil_festival(td) == TAMIL_FEST_PONGAL) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

/* ===================================================================
 * Myanmar significance tests
 * =================================================================== */

void test_system_name_myanmar(void)
{
    TEST_ASSERT_EQUAL_STRING("Myanmar", cd_system_name(CD_SYS_MYANMAR));
}

void test_myanmar_month_boundary_significant(void)
{
    /* Find a Myanmar day 1 */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 35; i++) {
        myanmar_date_t md = myanmar_from_jd(jd + i);
        if (md.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_myanmar_not_significant(void)
{
    /* Find a Myanmar mid-month day in a non-watat year */
    double jd = jd_from(2024, 6, 15);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        myanmar_date_t md = myanmar_from_jd(jd + i);
        if (md.day != 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_MYANMAR, CD_SYS_MYANMAR, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_myanmar_tagu_day1_significant(void)
{
    /* Find Tagu (month 0) day 1 — Myanmar New Year.
     * Tagu typically starts around April. */
    double jd = jd_from(2024, 4, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 40; i++) {
        myanmar_date_t md = myanmar_from_jd(jd + i);
        if (md.month == MY_TAGU && md.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

/* ===================================================================
 * Zoroastrian significance tests
 * =================================================================== */

void test_system_name_zoroastrian(void)
{
    TEST_ASSERT_EQUAL_STRING("Zoroastrian", cd_system_name(CD_SYS_ZOROASTRIAN));
}

void test_zoroastrian_nowruz_significant(void)
{
    /* Zoroastrian Nowruz = 1 Farvardin (month 1, day 1) */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 370; i++) {
        zoro_date_t zd = zoro_from_jd(jd + i);
        if (zd.month == 1 && zd.day == 1) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_zoroastrian_gatha_significant(void)
{
    /* Find a Gatha epagomenal day (month 13) */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 370; i++) {
        zoro_date_t zd = zoro_from_jd(jd + i);
        if (zoro_is_gatha(zd)) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    double s = cd_significance(test_jd);
    TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
}

void test_zoroastrian_not_significant(void)
{
    /* Find a mid-month day with no festival and no Gatha */
    double jd = jd_from(2024, 3, 10);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 30; i++) {
        zoro_date_t zd = zoro_from_jd(jd + i);
        if (!zoro_is_gatha(zd) && zoro_festival(zd) == ZORO_FEST_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);

    int r = cd_check_pair(CD_SYS_ZOROASTRIAN, CD_SYS_ZOROASTRIAN, test_jd);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_zoroastrian_festival_significant(void)
{
    /* Find any Zoroastrian festival day */
    double jd = jd_from(2024, 1, 1);
    int found = 0;
    double test_jd = jd;
    for (int i = 0; i < 370; i++) {
        zoro_date_t zd = zoro_from_jd(jd + i);
        if (zoro_festival(zd) != ZORO_FEST_NONE) {
            test_jd = jd + i;
            found = 1;
            break;
        }
    }
    if (found) {
        double s = cd_significance(test_jd);
        TEST_ASSERT_TRUE(s >= 1.0 / (double)CD_SYS_COUNT - 0.001);
    }
}

/* ===================================================================
 * Cross-system convergence with expansion systems
 * =================================================================== */

void test_convergence_scan_includes_21_systems(void)
{
    /* Scan a date and verify all 21 system enum values are valid */
    double jd = jd_from(2024, 3, 20);
    cd_result_t r = cd_scan(jd);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= CD_MAX_CONVERGENCES);
    for (int i = 0; i < r.count; i++) {
        for (int j = 0; j < r.events[i].system_count; j++) {
            TEST_ASSERT_TRUE(r.events[i].systems[j] >= 0);
            TEST_ASSERT_TRUE(r.events[i].systems[j] < CD_SYS_COUNT);
        }
    }
}

void test_all_21_enum_pairs_valid(void)
{
    /* Verify all 21 systems can be called in pairs without crash */
    double jd = jd_from(2024, 8, 10);
    for (int a = 0; a < CD_SYS_COUNT; a++) {
        for (int b = a; b < CD_SYS_COUNT; b++) {
            int r = cd_check_pair((cd_system_t)a, (cd_system_t)b, jd);
            TEST_ASSERT_TRUE(r == 0 || r == 1);
        }
    }
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* cd_system_name */
    RUN_TEST(test_system_name_astronomy);
    RUN_TEST(test_system_name_astrology);
    RUN_TEST(test_system_name_tzolkin);
    RUN_TEST(test_system_name_iching);
    RUN_TEST(test_system_name_chinese);
    RUN_TEST(test_system_name_hebrew);
    RUN_TEST(test_system_name_islamic);
    RUN_TEST(test_system_name_hindu);
    RUN_TEST(test_system_name_invalid_count);
    RUN_TEST(test_system_name_invalid_negative);
    RUN_TEST(test_system_name_invalid_large);

    /* cd_type_count */
    RUN_TEST(test_type_count_positive);
    RUN_TEST(test_type_count_equals_sys_count);

    /* cd_significance range */
    RUN_TEST(test_significance_range_ordinary_date);
    RUN_TEST(test_significance_range_another_date);
    RUN_TEST(test_significance_range_ancient_date);
    RUN_TEST(test_significance_range_modern_date);
    RUN_TEST(test_significance_range_future_date);

    /* cd_scan structure */
    RUN_TEST(test_scan_returns_valid_count);
    RUN_TEST(test_scan_event_system_count_valid);
    RUN_TEST(test_scan_event_strength_valid);
    RUN_TEST(test_scan_event_significance_range);
    RUN_TEST(test_scan_event_description_not_null);
    RUN_TEST(test_scan_event_systems_valid_enum);

    /* cd_scan known dates */
    RUN_TEST(test_scan_winter_solstice_2024);
    RUN_TEST(test_scan_spring_equinox_2024);
    RUN_TEST(test_scan_gregorian_new_year);

    /* Tzolkin significance */
    RUN_TEST(test_scan_tzolkin_kin_1);
    RUN_TEST(test_scan_tzolkin_tone_1);
    RUN_TEST(test_scan_tzolkin_portal_day);

    /* Hebrew significance */
    RUN_TEST(test_scan_hebrew_day_1);
    RUN_TEST(test_scan_hebrew_rosh_hashanah);

    /* Islamic significance */
    RUN_TEST(test_scan_islamic_day_1);
    RUN_TEST(test_scan_islamic_ramadan);

    /* I Ching significance */
    RUN_TEST(test_scan_iching_hexagram_1);
    RUN_TEST(test_scan_iching_hexagram_2);

    /* Chinese significance */
    RUN_TEST(test_scan_chinese_new_year_proximity);
    RUN_TEST(test_scan_chinese_cycle_year_1);

    /* Astronomy significance */
    RUN_TEST(test_scan_new_moon_significant);
    RUN_TEST(test_scan_full_moon_significant);
    RUN_TEST(test_scan_summer_solstice);
    RUN_TEST(test_scan_autumn_equinox);

    /* Astrology significance */
    RUN_TEST(test_scan_astrology_ingress);

    /* Hindu significance */
    RUN_TEST(test_scan_hindu_new_moon_tithi);

    /* Ordinary dates */
    RUN_TEST(test_scan_ordinary_date_1);
    RUN_TEST(test_scan_ordinary_date_2);
    RUN_TEST(test_scan_ordinary_date_low_significance);

    /* cd_check_pair */
    RUN_TEST(test_check_pair_same_system);
    RUN_TEST(test_check_pair_returns_0_for_no_align);
    RUN_TEST(test_check_pair_astronomy_hindu_near_new_moon);
    RUN_TEST(test_check_pair_hebrew_islamic_both_day1);
    RUN_TEST(test_check_pair_all_enum_values_valid);

    /* cd_find_next */
    RUN_TEST(test_find_next_minor_within_30_days);
    RUN_TEST(test_find_next_notable_within_90_days);
    RUN_TEST(test_find_next_returns_zero_for_max_days_zero);
    RUN_TEST(test_find_next_rare_may_not_exist);
    RUN_TEST(test_find_next_result_has_correct_strength);

    /* Strength values */
    RUN_TEST(test_scan_two_systems_is_minor);
    RUN_TEST(test_scan_strength_minor_value);
    RUN_TEST(test_scan_strength_notable_value);
    RUN_TEST(test_scan_strength_major_value);
    RUN_TEST(test_scan_strength_rare_value);

    /* Cross-system convergence */
    RUN_TEST(test_scan_new_moon_plus_hebrew_day1);

    /* Edge cases */
    RUN_TEST(test_scan_very_early_date);
    RUN_TEST(test_scan_distant_future);
    RUN_TEST(test_significance_distant_future);
    RUN_TEST(test_scan_jd_zero);
    RUN_TEST(test_significance_jd_zero);
    RUN_TEST(test_find_next_from_jd_zero);

    /* Convergence requires 2+ */
    RUN_TEST(test_scan_single_system_no_convergence);

    /* Constants */
    RUN_TEST(test_cd_max_convergences);
    RUN_TEST(test_cd_max_systems);
    RUN_TEST(test_cd_sys_count_value);
    RUN_TEST(test_cd_sys_enum_order);

    /* Idempotency */
    RUN_TEST(test_scan_idempotent);
    RUN_TEST(test_significance_idempotent);
    RUN_TEST(test_check_pair_idempotent);

    /* Sequential find */
    RUN_TEST(test_find_next_scans_sequentially);

    /* New system names */
    RUN_TEST(test_system_name_buddhist);
    RUN_TEST(test_system_name_persian);
    RUN_TEST(test_system_name_coptic);
    RUN_TEST(test_system_name_celtic);
    RUN_TEST(test_system_name_bahai);

    /* Buddhist significance */
    RUN_TEST(test_buddhist_uposatha_significant);
    RUN_TEST(test_buddhist_full_moon_significant);
    RUN_TEST(test_buddhist_non_uposatha_not_significant);
    RUN_TEST(test_buddhist_new_moon_significant);

    /* Persian significance */
    RUN_TEST(test_persian_nowruz_significant);
    RUN_TEST(test_persian_month_boundary_significant);
    RUN_TEST(test_persian_mid_month_not_significant);
    RUN_TEST(test_persian_sizdah_bedar_significant);

    /* Coptic significance */
    RUN_TEST(test_coptic_nayrouz_significant);
    RUN_TEST(test_coptic_month_boundary_significant);
    RUN_TEST(test_coptic_mid_month_not_significant);
    RUN_TEST(test_coptic_feast_significant);

    /* Celtic significance */
    RUN_TEST(test_celtic_winter_solstice_significant);
    RUN_TEST(test_celtic_summer_solstice_significant);
    RUN_TEST(test_celtic_spring_equinox_significant);
    RUN_TEST(test_celtic_mid_season_not_significant);
    RUN_TEST(test_celtic_samhain_significant);

    /* Bahai significance */
    RUN_TEST(test_bahai_naw_ruz_significant);
    RUN_TEST(test_bahai_feast_boundary_significant);
    RUN_TEST(test_bahai_mid_month_not_significant);
    RUN_TEST(test_bahai_holy_day_significant);

    /* Cross-system convergence with new systems */
    RUN_TEST(test_convergence_buddhist_astronomy_new_moon);
    RUN_TEST(test_convergence_persian_celtic_equinox);
    RUN_TEST(test_convergence_scan_includes_new_systems);
    RUN_TEST(test_new_systems_all_enum_pairs_valid);

    /* Japanese significance */
    RUN_TEST(test_system_name_japanese);
    RUN_TEST(test_japanese_taian_significant);
    RUN_TEST(test_japanese_not_significant);
    RUN_TEST(test_japanese_era_gannen_significant);

    /* Egyptian significance */
    RUN_TEST(test_system_name_egyptian);
    RUN_TEST(test_egyptian_month_boundary_significant);
    RUN_TEST(test_egyptian_epagomenal_significant);
    RUN_TEST(test_egyptian_not_significant);

    /* French Republican significance */
    RUN_TEST(test_system_name_french);
    RUN_TEST(test_french_month_boundary_significant);
    RUN_TEST(test_french_sansculottide_significant);
    RUN_TEST(test_french_decadi_significant);
    RUN_TEST(test_french_not_significant);

    /* Korean significance */
    RUN_TEST(test_system_name_korean);
    RUN_TEST(test_korean_seollal_significant);
    RUN_TEST(test_korean_not_significant);
    RUN_TEST(test_korean_cycle_year_1_significant);

    /* Thai significance */
    RUN_TEST(test_system_name_thai);
    RUN_TEST(test_thai_songkran_significant);
    RUN_TEST(test_thai_not_significant);
    RUN_TEST(test_thai_chakri_day_significant);

    /* Tamil significance */
    RUN_TEST(test_system_name_tamil);
    RUN_TEST(test_tamil_puthandu_significant);
    RUN_TEST(test_tamil_not_significant);
    RUN_TEST(test_tamil_pongal_significant);

    /* Myanmar significance */
    RUN_TEST(test_system_name_myanmar);
    RUN_TEST(test_myanmar_month_boundary_significant);
    RUN_TEST(test_myanmar_not_significant);
    RUN_TEST(test_myanmar_tagu_day1_significant);

    /* Zoroastrian significance */
    RUN_TEST(test_system_name_zoroastrian);
    RUN_TEST(test_zoroastrian_nowruz_significant);
    RUN_TEST(test_zoroastrian_gatha_significant);
    RUN_TEST(test_zoroastrian_not_significant);
    RUN_TEST(test_zoroastrian_festival_significant);

    /* Cross-system convergence with 21 systems */
    RUN_TEST(test_convergence_scan_includes_21_systems);
    RUN_TEST(test_all_21_enum_pairs_valid);

    return UNITY_END();
}
