#include "../unity/unity.h"
#include "../../src/systems/tzolkin/tzolkin.h"
#include "../../src/math/julian.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== tzolkin_from_jd ===== */

void test_tzolkin_2012_dec_21(void) {
    /* 2012-12-21 = Kin 207 = 12 (tone) Blue Hand (seal 7) */
    double jd = gregorian_to_jd(2012, 12, 21.0);
    tzolkin_day_t kin = tzolkin_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(207, kin.kin);
    TEST_ASSERT_EQUAL_INT(12, kin.tone);
    TEST_ASSERT_EQUAL_INT(6, kin.seal); /* 0-indexed: Blue Hand = 6 */
}

void test_tzolkin_kin_range(void) {
    /* Kin must always be 1-260 */
    double jd = 2451545.0; /* J2000.0 */
    tzolkin_day_t kin = tzolkin_from_jd(jd);
    TEST_ASSERT_TRUE(kin.kin >= 1 && kin.kin <= 260);
    TEST_ASSERT_TRUE(kin.tone >= 1 && kin.tone <= 13);
    TEST_ASSERT_TRUE(kin.seal >= 0 && kin.seal <= 19);
}

void test_tzolkin_consecutive_days(void) {
    /* Consecutive non-leap days should give consecutive kins */
    double jd = gregorian_to_jd(2021, 6, 15.0); /* mid-year, no leap issue */
    tzolkin_day_t k1 = tzolkin_from_jd(jd);
    tzolkin_day_t k2 = tzolkin_from_jd(jd + 1.0);
    int expected = k1.kin % 260 + 1;
    TEST_ASSERT_EQUAL_INT(expected, k2.kin);
}

void test_tzolkin_260_cycle(void) {
    /* After 260 days, same kin should repeat (in non-leap-crossing span).
     * Use Mar 1, 2021 -> Nov 15, 2021 (260 days, no Feb 29). */
    double jd = gregorian_to_jd(2021, 3, 1.0);
    tzolkin_day_t k1 = tzolkin_from_jd(jd);
    tzolkin_day_t k2 = tzolkin_from_jd(jd + 260.0);
    TEST_ASSERT_EQUAL_INT(k1.kin, k2.kin);
    TEST_ASSERT_EQUAL_INT(k1.tone, k2.tone);
    TEST_ASSERT_EQUAL_INT(k1.seal, k2.seal);
}

void test_tzolkin_tone_cycles_13(void) {
    /* After 13 days, tone repeats (no Feb 29 crossing) */
    double jd = gregorian_to_jd(2021, 6, 1.0);
    tzolkin_day_t k1 = tzolkin_from_jd(jd);
    tzolkin_day_t k2 = tzolkin_from_jd(jd + 13.0);
    TEST_ASSERT_EQUAL_INT(k1.tone, k2.tone);
}

void test_tzolkin_seal_cycles_20(void) {
    /* After 20 days, seal repeats (no Feb 29 crossing) */
    double jd = gregorian_to_jd(2021, 6, 1.0);
    tzolkin_day_t k1 = tzolkin_from_jd(jd);
    tzolkin_day_t k2 = tzolkin_from_jd(jd + 20.0);
    TEST_ASSERT_EQUAL_INT(k1.seal, k2.seal);
}

/* ===== tzolkin_seal_name ===== */

void test_tzolkin_seal_name_dragon(void) {
    TEST_ASSERT_EQUAL_STRING("Red Dragon", tzolkin_seal_name(0));
}

void test_tzolkin_seal_name_sun(void) {
    TEST_ASSERT_EQUAL_STRING("Yellow Sun", tzolkin_seal_name(19));
}

void test_tzolkin_seal_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_seal_name(20));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_seal_name(-1));
}

/* ===== tzolkin_tone_name ===== */

void test_tzolkin_tone_name_magnetic(void) {
    TEST_ASSERT_EQUAL_STRING("Magnetic", tzolkin_tone_name(1));
}

void test_tzolkin_tone_name_cosmic(void) {
    TEST_ASSERT_EQUAL_STRING("Cosmic", tzolkin_tone_name(13));
}

void test_tzolkin_tone_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_tone_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_tone_name(14));
}

/* ===== Known date verification ===== */

void test_tzolkin_kin1_red_magnetic_dragon(void) {
    /* Kin 1 = tone 1 (Magnetic), seal 0 (Red Dragon) */
    /* 2013-02-13 is a known Kin 1 date (derived from 2012-12-21 = Kin 207) */
    double jd = gregorian_to_jd(2013, 2, 13.0);
    tzolkin_day_t kin = tzolkin_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, kin.kin);
    TEST_ASSERT_EQUAL_INT(1, kin.tone);
    TEST_ASSERT_EQUAL_INT(0, kin.seal);
}

void test_tzolkin_feb29_same_as_feb28(void) {
    /* Dreamspell: Feb 29 gets same kin as Feb 28 ("0.0 Hunab Ku") */
    double jd_28 = gregorian_to_jd(2020, 2, 28.0);
    double jd_29 = gregorian_to_jd(2020, 2, 29.0);
    double jd_mar1 = gregorian_to_jd(2020, 3, 1.0);
    tzolkin_day_t k28 = tzolkin_from_jd(jd_28);
    tzolkin_day_t k29 = tzolkin_from_jd(jd_29);
    tzolkin_day_t k_mar1 = tzolkin_from_jd(jd_mar1);
    /* Feb 29 = same kin as Feb 28 */
    TEST_ASSERT_EQUAL_INT(k28.kin, k29.kin);
    /* Mar 1 = Feb 28 + 1 (not + 2) */
    int expected = k28.kin % 260 + 1;
    TEST_ASSERT_EQUAL_INT(expected, k_mar1.kin);
}

int main(void) {
    UNITY_BEGIN();
    /* tzolkin_from_jd */
    RUN_TEST(test_tzolkin_2012_dec_21);
    RUN_TEST(test_tzolkin_kin_range);
    RUN_TEST(test_tzolkin_consecutive_days);
    RUN_TEST(test_tzolkin_260_cycle);
    RUN_TEST(test_tzolkin_tone_cycles_13);
    RUN_TEST(test_tzolkin_seal_cycles_20);
    /* seal names */
    RUN_TEST(test_tzolkin_seal_name_dragon);
    RUN_TEST(test_tzolkin_seal_name_sun);
    RUN_TEST(test_tzolkin_seal_name_invalid);
    /* tone names */
    RUN_TEST(test_tzolkin_tone_name_magnetic);
    RUN_TEST(test_tzolkin_tone_name_cosmic);
    RUN_TEST(test_tzolkin_tone_name_invalid);
    /* known dates */
    RUN_TEST(test_tzolkin_kin1_red_magnetic_dragon);
    /* leap day */
    RUN_TEST(test_tzolkin_feb29_same_as_feb28);
    return UNITY_END();
}
