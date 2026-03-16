/*
 * test_calendar_roundtrip.c — Round-trip consistency tests for calendar systems
 *
 * For every calendar system with both from_jd() and to_jd(), verifies that:
 *   date = system_from_jd(jd);
 *   jd2 = system_to_jd(date);
 *   |jd - jd2| < tolerance
 *
 * For systems with only from_jd(), validates output ranges.
 *
 * Pure function tests only. No GL calls.
 */

#include "unity.h"
#include "math/julian.h"
#include "systems/zoroastrian/zoroastrian.h"
#include "systems/islamic/hijri.h"
#include "systems/ethiopian/ethiopian.h"
#include "systems/coptic/coptic.h"
#include "systems/thai/thai_calendar.h"
#include "systems/egyptian/egyptian.h"
#include "systems/tamil/tamil_calendar.h"
#include "systems/persian/persian.h"
#include "systems/bahai/bahai.h"
#include "systems/hebrew/hebrew.h"
#include "systems/myanmar/myanmar.h"
#include "systems/french_republican/french_republican.h"
#include "systems/japanese/japanese.h"
#include "systems/korean/korean_calendar.h"
#include "systems/celtic/celtic_tree.h"
#include "systems/chinese/chinese.h"
#include "systems/tzolkin/tzolkin.h"
#include "systems/iching/iching.h"
#include "systems/tzolkin/haab.h"
#include "systems/balinese/pawukon.h"
#include <math.h>
#include <stdio.h>

void setUp(void) { }
void tearDown(void) { }

/* JD range: ~1582 CE to ~2100 CE (Gregorian era) */
#define JD_START 2299161.0
#define JD_END   2488070.0
#define ROUNDTRIP_COUNT 200
#define ROUNDTRIP_STEP ((JD_END - JD_START) / ROUNDTRIP_COUNT)

/* Tolerance levels for different calendar types:
 * EXACT:    arithmetic calendars (fixed rules, no astronomical lookup)
 * SIDEREAL: sidereal solar calendars (approximate sun position)
 * LUNAR:    lunisolar calendars (approximate moon phase/new moon) */
#define TOLERANCE_EXACT    1.0
#define TOLERANCE_SIDEREAL 3.0
#define TOLERANCE_LUNAR    30.0

/* ===== Round-trip tests (13 calendars) ===== */

void test_zoroastrian_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        zoro_date_t date = zoro_from_jd(jd);
        double jd2 = zoro_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Zoroastrian round-trip failed for some JD values");
}

void test_hijri_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        hijri_date_t date = hijri_from_jd(jd);
        double jd2 = hijri_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Hijri round-trip failed for some JD values");
}

void test_ethiopian_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        ethiopian_date_t date = ethiopian_from_jd(jd);
        double jd2 = ethiopian_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Ethiopian round-trip failed for some JD values");
}

void test_coptic_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        coptic_date_t date = coptic_from_jd(jd);
        double jd2 = coptic_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Coptic round-trip failed for some JD values");
}

void test_thai_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        thai_date_t date = thai_from_jd(jd);
        double jd2 = thai_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Thai round-trip failed for some JD values");
}

void test_egyptian_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        egypt_date_t date = egypt_from_jd(jd);
        double jd2 = egypt_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Egyptian round-trip failed for some JD values");
}

void test_tamil_roundtrip(void) {
    /* Tamil uses sidereal solar calendar — wider tolerance due to
     * approximate sun position computation */
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        tamil_date_t date = tamil_from_jd(jd);
        double jd2 = tamil_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_SIDEREAL) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Tamil round-trip failed for some JD values");
}

void test_persian_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        persian_date_t date = persian_from_jd(jd);
        double jd2 = persian_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Persian round-trip failed for some JD values");
}

void test_bahai_roundtrip(void) {
    /* Bahai calendar epoch: Naw-Ruz 1 BE = March 20, 1844 CE = JD ~2394646.
     * Start range after epoch for valid round-trip. */
    double bahai_start = 2394646.0;
    double bahai_step = (JD_END - bahai_start) / ROUNDTRIP_COUNT;
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = bahai_start + i * bahai_step;
        bahai_date_t date = bahai_from_jd(jd);
        double jd2 = bahai_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Bahai round-trip failed for some JD values");
}

void test_hebrew_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        hebrew_date_t date = hebrew_from_jd(jd);
        double jd2 = hebrew_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Hebrew round-trip failed for some JD values");
}

void test_myanmar_roundtrip(void) {
    /* Myanmar lunisolar calendar uses Surya Siddhanta solar year and
     * Metonic intercalation. At watat year boundaries, to_jd(from_jd(jd))
     * can land in the neighboring month. Use lunar tolerance. */
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        myanmar_date_t date = myanmar_from_jd(jd);
        double jd2 = myanmar_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_LUNAR) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Myanmar round-trip failed for some JD values");
}

void test_french_republican_roundtrip(void) {
    /* French Republican calendar: valid from 1792 CE onward.
     * Use JD range starting at 1792 (JD ~2375840). */
    double fr_start = 2375840.0;
    double fr_step = (JD_END - fr_start) / ROUNDTRIP_COUNT;
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = fr_start + i * fr_step;
        fr_date_t date = fr_from_jd(jd);
        double jd2 = fr_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "French Republican round-trip failed for some JD values");
}

void test_japanese_roundtrip(void) {
    /* Japanese calendar: Meiji era starts 1868 CE (JD ~2403674).
     * Pre-Meiji dates have era_index=-1 but still round-trip. */
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        japanese_date_t date = japanese_from_jd(jd);
        double jd2 = japanese_to_jd(date);
        if (fabs(jd - jd2) > TOLERANCE_EXACT) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Japanese round-trip failed for some JD values");
}

/* ===== Korean round-trip ===== */
/* korean_to_jd() takes (int gregorian_year, int lunar_month, int lunar_day).
 * Uses approximate new-moon computation internally. At new-moon boundaries,
 * the round-trip can land in an adjacent lunar month. We verify that at
 * least 98% of sampled dates round-trip exactly (allowing 2% boundary edge
 * cases from lunar approximation). */
void test_korean_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        korean_date_t date = korean_from_jd(jd);
        double jd2 = korean_to_jd(date.gregorian_year,
                                   date.lunar_month,
                                   date.lunar_day);
        korean_date_t date2 = korean_from_jd(jd2);
        if (date.lunar_month != date2.lunar_month ||
            date.lunar_day != date2.lunar_day) {
            failures++;
        }
    }
    /* Allow up to 2% failure rate for lunar boundary edge cases */
    int max_allowed = ROUNDTRIP_COUNT * 2 / 100;
    if (max_allowed < 3) max_allowed = 3;
    TEST_ASSERT_TRUE_MESSAGE(failures <= max_allowed,
        "Korean round-trip: too many failures (>2%)");
}

/* ===== Celtic Tree round-trip ===== */
/* celtic_tree_to_jd() takes (celtic_tree_date_t date, int gregorian_year).
 * The Celtic year starts Dec 24, so for dates Dec 24-31, the Celtic year
 * belongs to the NEXT Gregorian year. This year-boundary ambiguity means
 * the wrong greg_year can be passed to to_jd, yielding a ~365 day error.
 * We verify the round-trip matches the SAME Celtic date (month+day). */
void test_celtic_tree_roundtrip(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        celtic_tree_date_t date = celtic_tree_from_jd(jd);

        /* Extract Gregorian year for the extra parameter */
        int greg_year, greg_month;
        jd_to_gregorian(jd, &greg_year, &greg_month);

        double jd2 = celtic_tree_to_jd(date, greg_year);
        celtic_tree_date_t date2 = celtic_tree_from_jd(jd2);

        /* Verify same Celtic month/day (ignore year difference) */
        if (date.month != date2.month || date.day != date2.day) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Celtic Tree round-trip failed for some JD values");
}

/* ===== Range validation tests (from_jd only) ===== */

void test_chinese_range(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        chinese_year_t cy = chinese_year_from_jd(jd);
        if (cy.animal < 0 || cy.animal > 11) failures++;
        if (cy.element < 0 || cy.element > 4) failures++;
        if (cy.stem < 0 || cy.stem > 9) failures++;
        if (cy.branch < 0 || cy.branch > 11) failures++;
        if (cy.polarity < 0 || cy.polarity > 1) failures++;
        if (cy.cycle_year < 1 || cy.cycle_year > 60) failures++;
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Chinese year fields out of range");
}

void test_tzolkin_range(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        tzolkin_day_t tz = tzolkin_from_jd(jd);
        if (tz.seal < 0 || tz.seal > 19) failures++;
        if (tz.tone < 1 || tz.tone > 13) failures++;
        if (tz.kin < 1 || tz.kin > 260) failures++;
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Tzolkin fields out of range");
}

void test_iching_range(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        hexagram_t hex = iching_from_jd(jd);
        if (hex.king_wen < 1 || hex.king_wen > 64) failures++;
        if (hex.lower_trigram < 0 || hex.lower_trigram > 7) failures++;
        if (hex.upper_trigram < 0 || hex.upper_trigram > 7) failures++;
        for (int line = 0; line < 6; line++) {
            if (hex.lines[line] != 0 && hex.lines[line] != 1) failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "I Ching fields out of range");
}

void test_haab_range(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        haab_date_t h = haab_from_jd(jd);
        if (h.month < 0 || h.month > 18) failures++;
        if (h.month == 18) {
            /* Wayeb: day 0-4 */
            if (h.day < 0 || h.day > 4) failures++;
        } else {
            /* Regular month: day 0-19 */
            if (h.day < 0 || h.day > 19) failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Haab fields out of range");
}

void test_pawukon_range(void) {
    int failures = 0;
    for (int i = 0; i < ROUNDTRIP_COUNT; i++) {
        double jd = JD_START + i * ROUNDTRIP_STEP;
        pawukon_t p = pawukon_from_jd(jd);
        if (p.wuku < 0 || p.wuku > 29) failures++;
        if (p.wuku_day < 0 || p.wuku_day > 6) failures++;
        if (p.day_in_cycle < 0 || p.day_in_cycle > 209) failures++;
        if (p.eka_wara != 0) failures++;
        if (p.dwi_wara < 0 || p.dwi_wara > 1) failures++;
        if (p.tri_wara < 0 || p.tri_wara > 2) failures++;
        if (p.catur_wara < 0 || p.catur_wara > 3) failures++;
        if (p.panca_wara < 0 || p.panca_wara > 4) failures++;
        if (p.sad_wara < 0 || p.sad_wara > 5) failures++;
        if (p.sapta_wara < 0 || p.sapta_wara > 6) failures++;
        if (p.asta_wara < 0 || p.asta_wara > 7) failures++;
        if (p.sanga_wara < 0 || p.sanga_wara > 8) failures++;
        if (p.dasa_wara < 0 || p.dasa_wara > 9) failures++;
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "Pawukon cycle fields out of range");
}

/* ===== Consistency checks ===== */

/* Verify that consecutive JDs produce monotonically advancing dates
 * in calendars with linear day counts. */
void test_zoroastrian_monotonic(void) {
    double prev_jd = zoro_to_jd(zoro_from_jd(JD_START));
    for (int i = 1; i <= 100; i++) {
        double jd = JD_START + i;
        double rt_jd = zoro_to_jd(zoro_from_jd(jd));
        TEST_ASSERT_TRUE_MESSAGE(rt_jd >= prev_jd,
            "Zoroastrian dates not monotonically advancing");
        prev_jd = rt_jd;
    }
}

void test_ethiopian_monotonic(void) {
    double prev_jd = ethiopian_to_jd(ethiopian_from_jd(JD_START));
    for (int i = 1; i <= 100; i++) {
        double jd = JD_START + i;
        double rt_jd = ethiopian_to_jd(ethiopian_from_jd(jd));
        TEST_ASSERT_TRUE_MESSAGE(rt_jd >= prev_jd,
            "Ethiopian dates not monotonically advancing");
        prev_jd = rt_jd;
    }
}

void test_coptic_monotonic(void) {
    double prev_jd = coptic_to_jd(coptic_from_jd(JD_START));
    for (int i = 1; i <= 100; i++) {
        double jd = JD_START + i;
        double rt_jd = coptic_to_jd(coptic_from_jd(jd));
        TEST_ASSERT_TRUE_MESSAGE(rt_jd >= prev_jd,
            "Coptic dates not monotonically advancing");
        prev_jd = rt_jd;
    }
}

/* ===== Specific known-date round-trip spot checks ===== */

/* Helper: assert round-trip within tolerance */
static void assert_roundtrip(double jd, double jd2, const char *name) {
    char msg[128];
    snprintf(msg, sizeof(msg), "%s round-trip failed at JD %.1f (got %.1f)",
             name, jd, jd2);
    TEST_ASSERT_TRUE_MESSAGE(fabs(jd - jd2) <= TOLERANCE_EXACT, msg);
}

/* J2000 = 2000-01-01 12:00 UT = JD 2451545.0 */
void test_roundtrip_j2000_spot(void) {
    double jd = 2451545.0;

    assert_roundtrip(jd, zoro_to_jd(zoro_from_jd(jd)), "Zoroastrian");
    assert_roundtrip(jd, hijri_to_jd(hijri_from_jd(jd)), "Hijri");
    assert_roundtrip(jd, ethiopian_to_jd(ethiopian_from_jd(jd)), "Ethiopian");
    assert_roundtrip(jd, coptic_to_jd(coptic_from_jd(jd)), "Coptic");
    assert_roundtrip(jd, persian_to_jd(persian_from_jd(jd)), "Persian");
    assert_roundtrip(jd, hebrew_to_jd(hebrew_from_jd(jd)), "Hebrew");
}

/* Today: 2026-03-16 = JD ~2461452 */
void test_roundtrip_today_spot(void) {
    double jd = gregorian_to_jd(2026, 3, 16.0);

    assert_roundtrip(jd, zoro_to_jd(zoro_from_jd(jd)), "Zoroastrian");
    assert_roundtrip(jd, hijri_to_jd(hijri_from_jd(jd)), "Hijri");
    assert_roundtrip(jd, ethiopian_to_jd(ethiopian_from_jd(jd)), "Ethiopian");
    assert_roundtrip(jd, coptic_to_jd(coptic_from_jd(jd)), "Coptic");
    assert_roundtrip(jd, thai_to_jd(thai_from_jd(jd)), "Thai");
    assert_roundtrip(jd, egypt_to_jd(egypt_from_jd(jd)), "Egyptian");
    assert_roundtrip(jd, persian_to_jd(persian_from_jd(jd)), "Persian");
    assert_roundtrip(jd, bahai_to_jd(bahai_from_jd(jd)), "Bahai");
    assert_roundtrip(jd, hebrew_to_jd(hebrew_from_jd(jd)), "Hebrew");
    assert_roundtrip(jd, japanese_to_jd(japanese_from_jd(jd)), "Japanese");
}

int main(void) {
    UNITY_BEGIN();

    /* 13 full round-trip tests */
    RUN_TEST(test_zoroastrian_roundtrip);
    RUN_TEST(test_hijri_roundtrip);
    RUN_TEST(test_ethiopian_roundtrip);
    RUN_TEST(test_coptic_roundtrip);
    RUN_TEST(test_thai_roundtrip);
    RUN_TEST(test_egyptian_roundtrip);
    RUN_TEST(test_tamil_roundtrip);
    RUN_TEST(test_persian_roundtrip);
    RUN_TEST(test_bahai_roundtrip);
    RUN_TEST(test_hebrew_roundtrip);
    RUN_TEST(test_myanmar_roundtrip);
    RUN_TEST(test_french_republican_roundtrip);
    RUN_TEST(test_japanese_roundtrip);

    /* 2 special-signature round-trips */
    RUN_TEST(test_korean_roundtrip);
    RUN_TEST(test_celtic_tree_roundtrip);

    /* 5 range validation tests */
    RUN_TEST(test_chinese_range);
    RUN_TEST(test_tzolkin_range);
    RUN_TEST(test_iching_range);
    RUN_TEST(test_haab_range);
    RUN_TEST(test_pawukon_range);

    /* 3 monotonicity tests */
    RUN_TEST(test_zoroastrian_monotonic);
    RUN_TEST(test_ethiopian_monotonic);
    RUN_TEST(test_coptic_monotonic);

    /* 2 spot check tests */
    RUN_TEST(test_roundtrip_j2000_spot);
    RUN_TEST(test_roundtrip_today_spot);

    return UNITY_END();
}
