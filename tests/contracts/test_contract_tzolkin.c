/* test_contract_tzolkin.c — Exhaustive contract tests for tzolkin.h */

#include "../unity/unity.h"
#include "../../src/systems/tzolkin/tzolkin.h"
#include "../../src/math/julian.h"
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

void test_kin_range_sweep(void) {
    double jd_start = gregorian_to_jd(1900, 1, 1.0);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        tzolkin_day_t td = tzolkin_from_jd(jd_start + i * 36.5);
        if (td.kin < 1 || td.kin > 260) failures++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Kin out of [1,260]");
}

void test_seal_range_sweep(void) {
    double jd_start = gregorian_to_jd(1900, 1, 1.0);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        tzolkin_day_t td = tzolkin_from_jd(jd_start + i * 36.5);
        if (td.seal < 0 || td.seal > 19) failures++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Seal out of [0,19]");
}

void test_tone_range_sweep(void) {
    double jd_start = gregorian_to_jd(1900, 1, 1.0);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        tzolkin_day_t td = tzolkin_from_jd(jd_start + i * 36.5);
        if (td.tone < 1 || td.tone > 13) failures++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Tone out of [1,13]");
}

void test_anchor_2012_12_21(void) {
    tzolkin_day_t td = tzolkin_from_jd(gregorian_to_jd(2012, 12, 21.0));
    TEST_ASSERT_EQUAL_INT(207, td.kin);
}

void test_260_day_cycle(void) {
    double jd = gregorian_to_jd(2026, 3, 16.0);
    tzolkin_day_t td1 = tzolkin_from_jd(jd);
    tzolkin_day_t td2 = tzolkin_from_jd(jd + 260.0);
    TEST_ASSERT_EQUAL_INT(td1.kin, td2.kin);
    TEST_ASSERT_EQUAL_INT(td1.seal, td2.seal);
    TEST_ASSERT_EQUAL_INT(td1.tone, td2.tone);
}

void test_consecutive_kin_advance(void) {
    double jd = gregorian_to_jd(2026, 1, 1.0);
    int failures = 0;
    for (int i = 0; i < 260; i++) {
        tzolkin_day_t td1 = tzolkin_from_jd(jd + i);
        tzolkin_day_t td2 = tzolkin_from_jd(jd + i + 1);
        int expected_next = (td1.kin % 260) + 1;
        if (td2.kin != expected_next) failures++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Consecutive kin did not advance by 1");
}

void test_seal_name_valid_range(void) {
    for (int i = 0; i < 20; i++) {
        const char *name = tzolkin_seal_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_seal_name_invalid(void) {
    TEST_ASSERT_EQUAL_CHAR('?', tzolkin_seal_name(-1)[0]);
    TEST_ASSERT_EQUAL_CHAR('?', tzolkin_seal_name(20)[0]);
}

void test_tone_name_valid_range(void) {
    for (int i = 1; i <= 13; i++) {
        const char *name = tzolkin_tone_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_tone_name_invalid(void) {
    TEST_ASSERT_EQUAL_CHAR('?', tzolkin_tone_name(0)[0]);
    TEST_ASSERT_EQUAL_CHAR('?', tzolkin_tone_name(14)[0]);
}

void test_all_260_kin_appear(void) {
    double jd = gregorian_to_jd(2026, 1, 1.0);
    int seen[261] = {0};
    for (int i = 0; i < 260; i++) {
        tzolkin_day_t td = tzolkin_from_jd(jd + i);
        seen[td.kin] = 1;
    }
    int count = 0;
    for (int i = 1; i <= 260; i++) count += seen[i];
    TEST_ASSERT_EQUAL_INT(260, count);
}

void test_kin_seal_tone_consistency(void) {
    double jd = gregorian_to_jd(2026, 1, 1.0);
    int failures = 0;
    for (int i = 0; i < 260; i++) {
        tzolkin_day_t td = tzolkin_from_jd(jd + i);
        if (td.seal != (td.kin - 1) % 20) failures++;
        if (td.tone != ((td.kin - 1) % 13) + 1) failures++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Kin/seal/tone consistency violated");
}

void test_bce_dates_valid(void) {
    tzolkin_day_t td = tzolkin_from_jd(gregorian_to_jd(-500, 6, 15.0));
    TEST_ASSERT_TRUE(td.kin >= 1 && td.kin <= 260);
    TEST_ASSERT_TRUE(td.seal >= 0 && td.seal <= 19);
    TEST_ASSERT_TRUE(td.tone >= 1 && td.tone <= 13);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_kin_range_sweep);
    RUN_TEST(test_seal_range_sweep);
    RUN_TEST(test_tone_range_sweep);
    RUN_TEST(test_anchor_2012_12_21);
    RUN_TEST(test_260_day_cycle);
    RUN_TEST(test_consecutive_kin_advance);
    RUN_TEST(test_seal_name_valid_range);
    RUN_TEST(test_seal_name_invalid);
    RUN_TEST(test_tone_name_valid_range);
    RUN_TEST(test_tone_name_invalid);
    RUN_TEST(test_all_260_kin_appear);
    RUN_TEST(test_kin_seal_tone_consistency);
    RUN_TEST(test_bce_dates_valid);
    return UNITY_END();
}
