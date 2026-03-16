/*
 * test_calendar_pipeline.c — Integration tests for calendar system pipelines
 *
 * Verifies that all calendar systems produce consistent results when fed
 * the same Julian Day. Tests end-to-end: JD -> calendar conversion -> validation.
 *
 * Pure function tests only. No GL calls.
 */

#include "unity.h"
#include "math/julian.h"
#include "systems/gregorian/gregorian.h"
#include "systems/tzolkin/tzolkin.h"
#include "systems/chinese/chinese.h"
#include "systems/hebrew/hebrew.h"
#include "systems/islamic/hijri.h"
#include "systems/buddhist/buddhist.h"
#include "systems/persian/persian.h"
#include "systems/coptic/coptic.h"
#include "systems/ethiopian/ethiopian.h"
#include "systems/egyptian/egyptian.h"
#include "systems/japanese/japanese.h"
#include "systems/korean/korean_calendar.h"
#include "systems/thai/thai_calendar.h"
#include "systems/tamil/tamil_calendar.h"
#include "systems/myanmar/myanmar.h"
#include "systems/zoroastrian/zoroastrian.h"
#include "systems/bahai/bahai.h"
#include "systems/french_republican/french_republican.h"
#include "systems/unified/calendar_convert.h"
#include "systems/unified/convergence_detect.h"
#include <math.h>
#include <string.h>

#define J2000 2451545.0

/* 2026-03-16 12:00 UT */
#define JD_TODAY (gregorian_to_jd(2026, 3, 16.5))

/* Extreme past: 1 Jan 1 CE */
#define JD_YEAR_1 (gregorian_to_jd(1, 1, 1.5))

/* Extreme future: 1 Jan 9999 */
#define JD_YEAR_9999 (gregorian_to_jd(9999, 1, 1.5))

void setUp(void) { }
void tearDown(void) { }

/* ===== Test: all calendars valid at J2000 ===== */
void test_all_calendars_valid_at_j2000(void) {
    /* Gregorian: 2000-01-01 12:00 */
    int year, month;
    double day = jd_to_gregorian(J2000, &year, &month);
    TEST_ASSERT_EQUAL(2000, year);
    TEST_ASSERT_EQUAL(1, month);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.5, day);

    /* Tzolkin */
    tzolkin_day_t tz = tzolkin_from_jd(J2000);
    TEST_ASSERT_TRUE(tz.kin >= 1 && tz.kin <= 260);
    TEST_ASSERT_TRUE(tz.tone >= 1 && tz.tone <= 13);
    TEST_ASSERT_TRUE(tz.seal >= 0 && tz.seal <= 19);

    /* Chinese */
    chinese_year_t ch = chinese_year_from_jd(J2000);
    TEST_ASSERT_TRUE(ch.stem >= 0 && ch.stem <= 9);
    TEST_ASSERT_TRUE(ch.branch >= 0 && ch.branch <= 11);
    TEST_ASSERT_TRUE(ch.element >= 0 && ch.element <= 4);
    TEST_ASSERT_TRUE(ch.cycle_year >= 1 && ch.cycle_year <= 60);

    /* Hebrew */
    hebrew_date_t heb = hebrew_from_jd(J2000);
    TEST_ASSERT_TRUE(heb.year > 5000);
    TEST_ASSERT_TRUE(heb.month >= 1 && heb.month <= 13);
    TEST_ASSERT_TRUE(heb.day >= 1 && heb.day <= 30);

    /* Hijri */
    hijri_date_t hij = hijri_from_jd(J2000);
    TEST_ASSERT_TRUE(hij.year > 1400);
    TEST_ASSERT_TRUE(hij.month >= 1 && hij.month <= 12);
    TEST_ASSERT_TRUE(hij.day >= 1 && hij.day <= 30);

    /* Buddhist */
    int be = buddhist_era(2000);
    TEST_ASSERT_EQUAL(2543, be);

    /* Persian */
    persian_date_t per = persian_from_jd(J2000);
    TEST_ASSERT_TRUE(per.year > 1370);
    TEST_ASSERT_TRUE(per.month >= 1 && per.month <= 12);
    TEST_ASSERT_TRUE(per.day >= 1 && per.day <= 31);

    /* Coptic */
    coptic_date_t cop = coptic_from_jd(J2000);
    TEST_ASSERT_TRUE(cop.year > 1700);
    TEST_ASSERT_TRUE(cop.month >= 1 && cop.month <= 13);
    TEST_ASSERT_TRUE(cop.day >= 1 && cop.day <= 30);

    /* Ethiopian */
    ethiopian_date_t eth = ethiopian_from_jd(J2000);
    TEST_ASSERT_TRUE(eth.year > 1990);
    TEST_ASSERT_TRUE(eth.month >= 1 && eth.month <= 13);
    TEST_ASSERT_TRUE(eth.day >= 1 && eth.day <= 30);

    /* Egyptian */
    egypt_date_t egy = egypt_from_jd(J2000);
    TEST_ASSERT_TRUE(egy.year > 2700);
    TEST_ASSERT_TRUE(egy.month >= 1 && egy.month <= 13);
    TEST_ASSERT_TRUE(egy.day >= 1 && egy.day <= 30);

    /* Japanese */
    japanese_date_t jpn = japanese_from_jd(J2000);
    TEST_ASSERT_EQUAL(2000, jpn.greg_year);
    TEST_ASSERT_TRUE(jpn.era_index >= 0);

    /* Korean */
    korean_date_t kor = korean_from_jd(J2000);
    TEST_ASSERT_TRUE(kor.dangun_year > 4300);
    TEST_ASSERT_TRUE(kor.stem >= 0 && kor.stem <= 9);
    TEST_ASSERT_TRUE(kor.branch >= 0 && kor.branch <= 11);

    /* Thai */
    thai_date_t thai = thai_from_jd(J2000);
    TEST_ASSERT_EQUAL(2543, thai.be_year);
    TEST_ASSERT_TRUE(thai.month >= 1 && thai.month <= 12);

    /* Tamil */
    tamil_date_t tam = tamil_from_jd(J2000);
    TEST_ASSERT_TRUE(tam.year > 2000);
    TEST_ASSERT_TRUE(tam.month >= 1 && tam.month <= 12);
    TEST_ASSERT_TRUE(tam.day >= 1 && tam.day <= 32);

    /* Myanmar */
    myanmar_date_t mya = myanmar_from_jd(J2000);
    TEST_ASSERT_TRUE(mya.year > 1300);
    TEST_ASSERT_TRUE(mya.day >= 1 && mya.day <= 30);

    /* Zoroastrian */
    zoro_date_t zor = zoro_from_jd(J2000);
    TEST_ASSERT_TRUE(zor.year > 1300);
    TEST_ASSERT_TRUE(zor.month >= 1 && zor.month <= 13);
    TEST_ASSERT_TRUE(zor.day >= 1 && zor.day <= 30);

    /* Baha'i */
    bahai_date_t bah = bahai_from_jd(J2000);
    TEST_ASSERT_TRUE(bah.year > 150);
    TEST_ASSERT_TRUE(bah.month >= 0 && bah.month <= 19);

    /* French Republican */
    fr_date_t fr = fr_from_jd(J2000);
    TEST_ASSERT_TRUE(fr.year > 200);
    TEST_ASSERT_TRUE(fr.month >= 0 && fr.month <= 12);
}

/* ===== Test: all calendars valid at today ===== */
void test_all_calendars_valid_at_today(void) {
    double jd = JD_TODAY;

    tzolkin_day_t tz = tzolkin_from_jd(jd);
    TEST_ASSERT_TRUE(tz.kin >= 1 && tz.kin <= 260);

    chinese_year_t ch = chinese_year_from_jd(jd);
    TEST_ASSERT_TRUE(ch.stem >= 0 && ch.stem <= 9);

    hebrew_date_t heb = hebrew_from_jd(jd);
    TEST_ASSERT_TRUE(heb.year > 5780);

    hijri_date_t hij = hijri_from_jd(jd);
    TEST_ASSERT_TRUE(hij.year > 1445);

    persian_date_t per = persian_from_jd(jd);
    TEST_ASSERT_TRUE(per.year > 1400);

    coptic_date_t cop = coptic_from_jd(jd);
    TEST_ASSERT_TRUE(cop.year > 1740);

    ethiopian_date_t eth = ethiopian_from_jd(jd);
    TEST_ASSERT_TRUE(eth.year > 2010);

    japanese_date_t jpn = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL(2026, jpn.greg_year);
    TEST_ASSERT_EQUAL(JP_ERA_REIWA, (jp_era_t)jpn.era_index);

    korean_date_t kor = korean_from_jd(jd);
    TEST_ASSERT_TRUE(kor.dangun_year >= 4359);

    thai_date_t thai = thai_from_jd(jd);
    TEST_ASSERT_EQUAL(2569, thai.be_year);

    bahai_date_t bah = bahai_from_jd(jd);
    TEST_ASSERT_TRUE(bah.year > 180);

    fr_date_t fr = fr_from_jd(jd);
    TEST_ASSERT_TRUE(fr.year > 230);
}

/* ===== Test: Hebrew round-trip conversion ===== */
void test_hebrew_roundtrip(void) {
    hebrew_date_t heb = hebrew_from_jd(J2000);
    double jd_back = hebrew_to_jd(heb);
    /* Should round-trip to within 1 day */
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Hijri round-trip conversion ===== */
void test_hijri_roundtrip(void) {
    hijri_date_t hij = hijri_from_jd(J2000);
    double jd_back = hijri_to_jd(hij);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Persian round-trip conversion ===== */
void test_persian_roundtrip(void) {
    persian_date_t per = persian_from_jd(J2000);
    double jd_back = persian_to_jd(per);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Coptic round-trip conversion ===== */
void test_coptic_roundtrip(void) {
    coptic_date_t cop = coptic_from_jd(J2000);
    double jd_back = coptic_to_jd(cop);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Ethiopian round-trip conversion ===== */
void test_ethiopian_roundtrip(void) {
    ethiopian_date_t eth = ethiopian_from_jd(J2000);
    double jd_back = ethiopian_to_jd(eth);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Baha'i round-trip conversion ===== */
void test_bahai_roundtrip(void) {
    bahai_date_t bah = bahai_from_jd(J2000);
    double jd_back = bahai_to_jd(bah);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: French Republican round-trip conversion ===== */
void test_french_republican_roundtrip(void) {
    fr_date_t fr = fr_from_jd(J2000);
    double jd_back = fr_to_jd(fr);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Egyptian round-trip conversion ===== */
void test_egyptian_roundtrip(void) {
    egypt_date_t egy = egypt_from_jd(J2000);
    double jd_back = egypt_to_jd(egy);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: Zoroastrian round-trip conversion ===== */
void test_zoroastrian_roundtrip(void) {
    zoro_date_t zor = zoro_from_jd(J2000);
    double jd_back = zoro_to_jd(zor);
    TEST_ASSERT_FLOAT_WITHIN(1.0, J2000, jd_back);
}

/* ===== Test: calendars don't crash on extreme past date ===== */
void test_calendars_extreme_past(void) {
    double jd = JD_YEAR_1;

    /* These should not crash or produce NAN */
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    TEST_ASSERT_TRUE(tz.kin >= 1 && tz.kin <= 260);

    egypt_date_t egy = egypt_from_jd(jd);
    TEST_ASSERT_TRUE(egy.month >= 1 && egy.month <= 13);

    coptic_date_t cop = coptic_from_jd(jd);
    (void)cop; /* May produce unusual years, just must not crash */

    zoro_date_t zor = zoro_from_jd(jd);
    (void)zor;
}

/* ===== Test: calendars don't crash on extreme future date ===== */
void test_calendars_extreme_future(void) {
    double jd = JD_YEAR_9999;

    tzolkin_day_t tz = tzolkin_from_jd(jd);
    TEST_ASSERT_TRUE(tz.kin >= 1 && tz.kin <= 260);

    hebrew_date_t heb = hebrew_from_jd(jd);
    TEST_ASSERT_TRUE(heb.month >= 1 && heb.month <= 13);

    hijri_date_t hij = hijri_from_jd(jd);
    TEST_ASSERT_TRUE(hij.month >= 1 && hij.month <= 12);

    persian_date_t per = persian_from_jd(jd);
    TEST_ASSERT_TRUE(per.month >= 1 && per.month <= 12);

    egypt_date_t egy = egypt_from_jd(jd);
    TEST_ASSERT_TRUE(egy.month >= 1 && egy.month <= 13);
}

/* ===== Test: calendar_convert works for all systems ===== */
void test_calendar_convert_all_systems(void) {
    for (int sys = 0; sys < CAL_SYSTEM_COUNT; sys++) {
        TEST_ASSERT_TRUE(cal_system_valid((cal_system_t)sys));

        cal_date_t result = cal_convert((cal_system_t)sys, J2000);
        TEST_ASSERT_EQUAL(sys, result.system);

        /* Formatted string should be non-empty */
        TEST_ASSERT_TRUE(strlen(result.formatted) > 0);
    }
}

/* ===== Test: calendar_convert system names are valid ===== */
void test_calendar_convert_system_names(void) {
    for (int sys = 0; sys < CAL_SYSTEM_COUNT; sys++) {
        const char *name = cal_system_name((cal_system_t)sys);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
    TEST_ASSERT_EQUAL(CAL_SYSTEM_COUNT, cal_system_count());
}

/* ===== Test: convergence_detect runs without crashing ===== */
void test_convergence_detect_scan(void) {
    cd_result_t result = cd_scan(J2000);

    TEST_ASSERT_TRUE(result.count >= 0);
    TEST_ASSERT_TRUE(result.count <= CD_MAX_CONVERGENCES);

    for (int i = 0; i < result.count; i++) {
        TEST_ASSERT_TRUE(result.events[i].system_count > 0);
        TEST_ASSERT_TRUE(result.events[i].strength >= CD_STRENGTH_MINOR);
        TEST_ASSERT_TRUE(result.events[i].significance >= 0.0);
        TEST_ASSERT_TRUE(result.events[i].significance <= 1.0);
    }
}

/* ===== Test: convergence significance is valid ===== */
void test_convergence_significance(void) {
    double sig = cd_significance(J2000);
    TEST_ASSERT_TRUE(sig >= 0.0);
    TEST_ASSERT_TRUE(sig <= 1.0);
    TEST_ASSERT_FALSE(isnan(sig));
}

/* ===== Test: month/day ranges for each system ===== */
void test_month_day_ranges(void) {
    /* Hebrew: check month days are valid for a known year */
    hebrew_date_t heb = hebrew_from_jd(J2000);
    int months = hebrew_months_in_year(heb.year);
    TEST_ASSERT_TRUE(months == 12 || months == 13);
    for (int m = 1; m <= months; m++) {
        int days = hebrew_month_days(heb.year, m);
        TEST_ASSERT_TRUE(days >= 29 && days <= 30);
    }

    /* Hijri: odd months 30 days, even months 29 days (with exceptions) */
    hijri_date_t hij = hijri_from_jd(J2000);
    for (int m = 1; m <= 12; m++) {
        int days = hijri_month_days(hij.year, m);
        TEST_ASSERT_TRUE(days == 29 || days == 30);
    }

    /* Persian: first 6 months 31 days, next 5 months 30 days, last 29 or 30 */
    persian_date_t per = persian_from_jd(J2000);
    for (int m = 1; m <= 6; m++) {
        TEST_ASSERT_EQUAL(31, persian_month_days(per.year, m));
    }
    for (int m = 7; m <= 11; m++) {
        TEST_ASSERT_EQUAL(30, persian_month_days(per.year, m));
    }
    int last = persian_month_days(per.year, 12);
    TEST_ASSERT_TRUE(last == 29 || last == 30);

    /* Coptic: months 1-12 = 30 days, month 13 = 5 or 6 */
    coptic_date_t cop = coptic_from_jd(J2000);
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL(30, coptic_month_days(cop.year, m));
    }
    int epag = coptic_month_days(cop.year, 13);
    TEST_ASSERT_TRUE(epag == 5 || epag == 6);

    /* Egyptian: months 1-12 = 30 days, month 13 = 5 */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL(30, egypt_month_days(m));
    }
    TEST_ASSERT_EQUAL(5, egypt_month_days(13));

    /* Zoroastrian: months 1-12 = 30, month 13 = 5 */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL(30, zoro_month_days(m));
    }
    TEST_ASSERT_EQUAL(5, zoro_month_days(13));
}

/* ===== Test: Gregorian format works for J2000 ===== */
void test_gregorian_format_j2000(void) {
    char buf[64];
    gregorian_format_date(J2000, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2000-01-01", buf);

    gregorian_format_time(J2000, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("12:00:00", buf);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_all_calendars_valid_at_j2000);
    RUN_TEST(test_all_calendars_valid_at_today);
    RUN_TEST(test_hebrew_roundtrip);
    RUN_TEST(test_hijri_roundtrip);
    RUN_TEST(test_persian_roundtrip);
    RUN_TEST(test_coptic_roundtrip);
    RUN_TEST(test_ethiopian_roundtrip);
    RUN_TEST(test_bahai_roundtrip);
    RUN_TEST(test_french_republican_roundtrip);
    RUN_TEST(test_egyptian_roundtrip);
    RUN_TEST(test_zoroastrian_roundtrip);
    RUN_TEST(test_calendars_extreme_past);
    RUN_TEST(test_calendars_extreme_future);
    RUN_TEST(test_calendar_convert_all_systems);
    RUN_TEST(test_calendar_convert_system_names);
    RUN_TEST(test_convergence_detect_scan);
    RUN_TEST(test_convergence_significance);
    RUN_TEST(test_month_day_ranges);
    RUN_TEST(test_gregorian_format_j2000);

    return UNITY_END();
}
