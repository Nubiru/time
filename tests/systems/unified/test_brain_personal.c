/* test_brain_personal.c — Tests for birth moment integration.
 *
 * Tests br_personal_scan with constructed birth profiles and known dates. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_personal.h"
#include "../../../src/systems/unified/birth_profile.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helper: known birth date
 * =================================================================== */

/* Use a fixed birth date to create a profile */
static birth_profile_t test_birth(void) {
    /* 1990-07-15 — a known date we can verify */
    return bp_compute(1990, 7, 15);
}

/* JD for a Gregorian date (simplified — matches julian.h algorithm) */
static double jd_for(int y, int m, int d) {
    /* Standard Gregorian→JD conversion */
    int a = (14 - m) / 12;
    int y2 = y + 4800 - a;
    int m2 = m + 12 * a - 3;
    return (double)d + (153 * m2 + 2) / 5 + 365 * y2 + y2 / 4
           - y2 / 100 + y2 / 400 - 32045.0 + 0.5 - 0.5;
}

/* ===================================================================
 * Type names
 * =================================================================== */

void test_type_name_kin_return(void) {
    TEST_ASSERT_EQUAL_STRING("Kin Return",
                             br_personal_type_name(BR_PERSONAL_KIN_RETURN));
}

void test_type_name_zodiac(void) {
    TEST_ASSERT_EQUAL_STRING("Zodiac Match",
                             br_personal_type_name(BR_PERSONAL_ZODIAC_MATCH));
}

void test_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", br_personal_type_name(99));
}

/* ===================================================================
 * Null safety
 * =================================================================== */

void test_scan_null_birth(void) {
    br_personal_result_t out;
    TEST_ASSERT_EQUAL_INT(0, br_personal_scan(2460000.5, NULL, &out));
}

void test_scan_null_out(void) {
    birth_profile_t bp = test_birth();
    TEST_ASSERT_EQUAL_INT(0, br_personal_scan(2460000.5, &bp, NULL));
}

/* ===================================================================
 * Kin Return detection
 * =================================================================== */

void test_kin_return_on_birth_date(void) {
    birth_profile_t bp = test_birth();
    /* Scanning the birth date itself — Kin should match exactly */
    double jd = jd_for(1990, 7, 15);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);

    /* Should find Kin Return (and possibly tone+seal, but those are
     * suppressed when full Kin matches) */
    int found_kin = 0;
    for (int i = 0; i < out.count; i++) {
        if (out.events[i].type == BR_PERSONAL_KIN_RETURN) {
            found_kin = 1;
            TEST_ASSERT_EQUAL_FLOAT(1.0, out.events[i].score);
        }
    }
    TEST_ASSERT_TRUE(found_kin);
}

void test_kin_return_260_days_later(void) {
    birth_profile_t bp = test_birth();
    /* 260 days after birth — should be same Kin */
    double jd = jd_for(1990, 7, 15) + 260.0;
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);

    int found_kin = 0;
    for (int i = 0; i < out.count; i++) {
        if (out.events[i].type == BR_PERSONAL_KIN_RETURN) found_kin = 1;
    }
    TEST_ASSERT_TRUE(found_kin);
}

void test_kin_return_headline_contains_kin(void) {
    birth_profile_t bp = test_birth();
    double jd = jd_for(1990, 7, 15);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);

    for (int i = 0; i < out.count; i++) {
        if (out.events[i].type == BR_PERSONAL_KIN_RETURN) {
            TEST_ASSERT_NOT_NULL(strstr(out.events[i].headline, "Galactic"));
        }
    }
}

/* ===================================================================
 * No match on arbitrary date
 * =================================================================== */

void test_no_kin_return_next_day(void) {
    birth_profile_t bp = test_birth();
    /* 1 day after birth — different Kin */
    double jd = jd_for(1990, 7, 16);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);

    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_NOT_EQUAL(BR_PERSONAL_KIN_RETURN, out.events[i].type);
    }
}

/* ===================================================================
 * Result structure
 * =================================================================== */

void test_result_zeroed_on_no_match(void) {
    birth_profile_t bp = test_birth();
    br_personal_result_t out;
    /* An arbitrary far-future date — may or may not have matches */
    br_personal_scan(jd_for(2050, 6, 1), &bp, &out);
    TEST_ASSERT_TRUE(out.count >= 0);
    TEST_ASSERT_TRUE(out.count <= BR_MAX_PERSONAL);
}

void test_relevance_equals_max_score(void) {
    birth_profile_t bp = test_birth();
    double jd = jd_for(1990, 7, 15); /* birth date — max relevance */
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);
    TEST_ASSERT_EQUAL_FLOAT(1.0, out.relevance);
}

void test_relevance_in_valid_range(void) {
    birth_profile_t bp = test_birth();
    br_personal_result_t out;
    /* Arbitrary date — relevance should be in [0,1] */
    br_personal_scan(jd_for(2030, 3, 15), &bp, &out);
    TEST_ASSERT_TRUE(out.relevance >= 0.0);
    TEST_ASSERT_TRUE(out.relevance <= 1.0);
}

/* ===================================================================
 * Zodiac match
 * =================================================================== */

void test_zodiac_match_same_season(void) {
    birth_profile_t bp = test_birth(); /* July 15 → Cancer */
    /* Check another July date in a different year */
    double jd = jd_for(2025, 7, 10);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);

    int found_zodiac = 0;
    for (int i = 0; i < out.count; i++) {
        if (out.events[i].type == BR_PERSONAL_ZODIAC_MATCH) found_zodiac = 1;
    }
    TEST_ASSERT_TRUE(found_zodiac);
}

void test_zodiac_no_match_different_season(void) {
    birth_profile_t bp = test_birth(); /* July 15 → Cancer */
    /* January — Capricorn/Aquarius, not Cancer */
    double jd = jd_for(2025, 1, 15);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);

    int found_zodiac = 0;
    for (int i = 0; i < out.count; i++) {
        if (out.events[i].type == BR_PERSONAL_ZODIAC_MATCH) found_zodiac = 1;
    }
    TEST_ASSERT_FALSE(found_zodiac);
}

/* ===================================================================
 * Multiple events
 * =================================================================== */

void test_birth_date_has_multiple_events(void) {
    birth_profile_t bp = test_birth();
    double jd = jd_for(1990, 7, 15);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);
    /* On the birth date, Kin + zodiac + hebrew month should all match */
    TEST_ASSERT_GREATER_THAN(1, out.count);
}

void test_events_count_within_max(void) {
    birth_profile_t bp = test_birth();
    double jd = jd_for(1990, 7, 15);
    br_personal_result_t out;
    br_personal_scan(jd, &bp, &out);
    TEST_ASSERT_TRUE(out.count <= BR_MAX_PERSONAL);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Type names */
    RUN_TEST(test_type_name_kin_return);
    RUN_TEST(test_type_name_zodiac);
    RUN_TEST(test_type_name_invalid);

    /* Null safety */
    RUN_TEST(test_scan_null_birth);
    RUN_TEST(test_scan_null_out);

    /* Kin Return */
    RUN_TEST(test_kin_return_on_birth_date);
    RUN_TEST(test_kin_return_260_days_later);
    RUN_TEST(test_kin_return_headline_contains_kin);
    RUN_TEST(test_no_kin_return_next_day);

    /* Result structure */
    RUN_TEST(test_result_zeroed_on_no_match);
    RUN_TEST(test_relevance_equals_max_score);
    RUN_TEST(test_relevance_in_valid_range);

    /* Zodiac */
    RUN_TEST(test_zodiac_match_same_season);
    RUN_TEST(test_zodiac_no_match_different_season);

    /* Multiple events */
    RUN_TEST(test_birth_date_has_multiple_events);
    RUN_TEST(test_events_count_within_max);

    return UNITY_END();
}
