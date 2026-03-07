/* test_number_scanner.c — Cross-System Number Scanner tests
 * TDD RED phase: tests written before implementation.
 * Tests scanning for number appearances across registered time cycles. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/number_scanner.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ns_cycle_count / ns_cycle_get ===== */

void test_cycle_count_at_least_21(void)
{
    TEST_ASSERT_TRUE(ns_cycle_count() >= 21);
}

void test_cycle_get_valid(void)
{
    ns_cycle_t c = ns_cycle_get(0);
    TEST_ASSERT_NOT_NULL(c.system);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_NOT_NULL(c.unit);
    TEST_ASSERT_TRUE(c.length > 0.0);
}

void test_cycle_get_invalid_negative(void)
{
    ns_cycle_t c = ns_cycle_get(-1);
    TEST_ASSERT_NULL(c.system);
}

void test_cycle_get_invalid_out_of_range(void)
{
    ns_cycle_t c = ns_cycle_get(9999);
    TEST_ASSERT_NULL(c.system);
}

/* ===== ns_sacred_count / ns_sacred_get ===== */

void test_sacred_count_at_least_11(void)
{
    TEST_ASSERT_TRUE(ns_sacred_count() >= 11);
}

void test_sacred_get_valid(void)
{
    ns_sacred_t s = ns_sacred_get(0);
    TEST_ASSERT_TRUE(s.number > 0);
    TEST_ASSERT_NOT_NULL(s.name);
    TEST_ASSERT_NOT_NULL(s.description);
}

void test_sacred_get_invalid_negative(void)
{
    ns_sacred_t s = ns_sacred_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.number);
}

void test_sacred_get_invalid_out_of_range(void)
{
    ns_sacred_t s = ns_sacred_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, s.number);
}

/* ===== number_scan — core scanning ===== */

void test_scan_432_finds_kali_yuga(void)
{
    ns_scan_result_t r = number_scan(432);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Hindu") == 0 &&
            strcmp(r.matches[i].cycle_name, "Kali Yuga") == 0) {
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 999.0 && r.matches[i].ratio < 1001.0);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_432_finds_maha_yuga(void)
{
    ns_scan_result_t r = number_scan(432);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Hindu") == 0 &&
            strcmp(r.matches[i].cycle_name, "Maha-Yuga") == 0) {
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 9999.0 && r.matches[i].ratio < 10001.0);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_432_finds_buddhist_kalpa(void)
{
    ns_scan_result_t r = number_scan(432);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Buddhist") == 0 &&
            strcmp(r.matches[i].cycle_name, "Kalpa") == 0) {
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 9999999.0 && r.matches[i].ratio < 10000001.0);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_7_finds_week(void)
{
    ns_scan_result_t r = number_scan(7);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].cycle_name, "Week") == 0) {
            /* 7 % 7 == 0, factor check fires first: ratio = 1 */
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 0.99 && r.matches[i].ratio < 1.01);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_7_finds_hebrew_sabbatical(void)
{
    ns_scan_result_t r = number_scan(7);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Hebrew") == 0 &&
            strcmp(r.matches[i].cycle_name, "Sabbatical") == 0) {
            /* 7 % 7 == 0, factor check fires first: ratio = 1 */
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 0.99 && r.matches[i].ratio < 1.01);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_64_finds_iching(void)
{
    ns_scan_result_t r = number_scan(64);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "I Ching") == 0 &&
            strcmp(r.matches[i].cycle_name, "Hexagram count") == 0) {
            /* 64 % 64 == 0, factor check fires first: ratio = 1 */
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 0.99 && r.matches[i].ratio < 1.01);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_64_finds_human_design(void)
{
    ns_scan_result_t r = number_scan(64);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Human Design") == 0 &&
            strcmp(r.matches[i].cycle_name, "Gate count") == 0) {
            /* 64 % 64 == 0, factor check fires first: ratio = 1 */
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 0.99 && r.matches[i].ratio < 1.01);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_12_finds_zodiac(void)
{
    ns_scan_result_t r = number_scan(12);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Zodiac") == 0 &&
            strcmp(r.matches[i].cycle_name, "Signs") == 0) {
            /* 12 % 12 == 0, factor check fires first: ratio = 1 */
            TEST_ASSERT_EQUAL_INT(NS_REL_FACTOR, r.matches[i].relationship);
            TEST_ASSERT_TRUE(r.matches[i].ratio > 0.99 && r.matches[i].ratio < 1.01);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_1_is_factor_of_everything(void)
{
    ns_scan_result_t r = number_scan(1);
    /* 1 divides every integer cycle, so should have many factor matches */
    TEST_ASSERT_TRUE(r.match_count >= 10);
}

void test_scan_0_returns_no_matches(void)
{
    ns_scan_result_t r = number_scan(0);
    TEST_ASSERT_EQUAL_INT(0, r.match_count);
}

void test_scan_negative_returns_no_matches(void)
{
    ns_scan_result_t r = number_scan(-1);
    TEST_ASSERT_EQUAL_INT(0, r.match_count);
}

void test_scan_match_count_within_limit(void)
{
    ns_scan_result_t r = number_scan(1);
    TEST_ASSERT_TRUE(r.match_count <= NS_MAX_MATCHES);
}

void test_scan_has_descriptions(void)
{
    ns_scan_result_t r = number_scan(432);
    TEST_ASSERT_TRUE(r.match_count > 0);
    for (int i = 0; i < r.match_count; i++) {
        TEST_ASSERT_NOT_NULL(r.matches[i].description);
        TEST_ASSERT_NOT_NULL(r.matches[i].system_name);
        TEST_ASSERT_NOT_NULL(r.matches[i].cycle_name);
    }
}

void test_scan_relationship_values_valid(void)
{
    ns_scan_result_t r = number_scan(432);
    for (int i = 0; i < r.match_count; i++) {
        TEST_ASSERT_TRUE(r.matches[i].relationship >= NS_REL_FACTOR &&
                         r.matches[i].relationship <= NS_REL_SCALE);
    }
}

/* ===== coincidence_score ===== */

void test_coincidence_score_7_at_least_2(void)
{
    /* 7 appears in Week + Hebrew Sabbatical at minimum */
    int score = coincidence_score(7);
    TEST_ASSERT_TRUE(score >= 2);
}

void test_coincidence_score_64_at_least_2(void)
{
    /* 64 in I Ching + Human Design */
    int score = coincidence_score(64);
    TEST_ASSERT_TRUE(score >= 2);
}

void test_coincidence_score_large_prime_is_0(void)
{
    int score = coincidence_score(99991);
    TEST_ASSERT_EQUAL_INT(0, score);
}

void test_coincidence_score_0_is_0(void)
{
    int score = coincidence_score(0);
    TEST_ASSERT_EQUAL_INT(0, score);
}

void test_coincidence_score_matches_scan_result(void)
{
    ns_scan_result_t r = number_scan(12);
    int score = coincidence_score(12);
    TEST_ASSERT_EQUAL_INT(r.coincidence_score, score);
}

/* ===== factor_scan ===== */

void test_factor_scan_25920_has_12(void)
{
    int factors[32];
    int count = factor_scan(25920, factors, 32);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (factors[i] == 12) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_factor_scan_25920_has_72(void)
{
    int factors[32];
    int count = factor_scan(25920, factors, 32);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (factors[i] == 72) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_factor_scan_25920_has_360(void)
{
    int factors[32];
    int count = factor_scan(25920, factors, 32);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (factors[i] == 360) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_factor_scan_25920_has_432(void)
{
    int factors[32];
    int count = factor_scan(25920, factors, 32);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (factors[i] == 432) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_factor_scan_25920_no_7(void)
{
    /* 25920 / 7 = 3702.857... not integer */
    int factors[32];
    int count = factor_scan(25920, factors, 32);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (factors[i] == 7) { found = 1; break; }
    }
    TEST_ASSERT_FALSE(found);
}

void test_factor_scan_260_has_52(void)
{
    int factors[32];
    int count = factor_scan(260, factors, 32);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (factors[i] == 52) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_factor_scan_respects_out_max(void)
{
    int factors[2];
    int count = factor_scan(25920, factors, 2);
    TEST_ASSERT_TRUE(count <= 2);
}

void test_factor_scan_zero_returns_0(void)
{
    int factors[32];
    int count = factor_scan(0, factors, 32);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_factor_scan_negative_returns_0(void)
{
    int factors[32];
    int count = factor_scan(-100, factors, 32);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== ns_is_factor_of_cycle ===== */

void test_is_factor_of_cycle_260(void)
{
    /* 260 divides Calendar Round (18980 / 260 = 73) */
    TEST_ASSERT_TRUE(ns_is_factor_of_cycle(260));
}

void test_is_factor_of_cycle_7(void)
{
    /* 7 divides Week (7) and Hebrew Sabbatical (7) */
    TEST_ASSERT_TRUE(ns_is_factor_of_cycle(7));
}

void test_is_factor_of_cycle_large_prime(void)
{
    TEST_ASSERT_FALSE(ns_is_factor_of_cycle(99991));
}

void test_is_factor_of_cycle_0(void)
{
    TEST_ASSERT_FALSE(ns_is_factor_of_cycle(0));
}

/* ===== ns_is_sacred_multiple ===== */

void test_is_sacred_multiple_144(void)
{
    /* 144 = 12 x 12, or 72 x 2, or 36 x 4 */
    TEST_ASSERT_TRUE(ns_is_sacred_multiple(144));
}

void test_is_sacred_multiple_24(void)
{
    /* 24 = 12 x 2 */
    TEST_ASSERT_TRUE(ns_is_sacred_multiple(24));
}

void test_is_sacred_multiple_7(void)
{
    /* 7 is itself a sacred number, so 7 % 7 == 0 */
    TEST_ASSERT_TRUE(ns_is_sacred_multiple(7));
}

void test_is_sacred_multiple_prime(void)
{
    /* 97 is prime and not a sacred number */
    TEST_ASSERT_FALSE(ns_is_sacred_multiple(97));
}

void test_is_sacred_multiple_0(void)
{
    /* 0 % anything == 0, but we should guard against this */
    TEST_ASSERT_FALSE(ns_is_sacred_multiple(0));
}

/* ===== ns_scale_check ===== */

void test_scale_check_432(void)
{
    /* 432 * 1000 = 432000 = Kali Yuga */
    int count = ns_scale_check(432);
    TEST_ASSERT_TRUE(count >= 1);
}

void test_scale_check_large_prime(void)
{
    int count = ns_scale_check(99991);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_scale_check_0(void)
{
    int count = ns_scale_check(0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== Scan result metadata ===== */

void test_scan_coincidence_score_populated(void)
{
    ns_scan_result_t r = number_scan(7);
    TEST_ASSERT_TRUE(r.coincidence_score >= 2);
}

void test_scan_432_scale_appearances(void)
{
    ns_scan_result_t r = number_scan(432);
    TEST_ASSERT_TRUE(r.scale_appearances >= 1);
}

/* ===== Data integrity ===== */

void test_all_cycles_have_system(void)
{
    int count = ns_cycle_count();
    for (int i = 0; i < count; i++) {
        ns_cycle_t c = ns_cycle_get(i);
        TEST_ASSERT_NOT_NULL(c.system);
        TEST_ASSERT_NOT_NULL(c.name);
        TEST_ASSERT_NOT_NULL(c.unit);
    }
}

void test_all_sacred_have_name(void)
{
    int count = ns_sacred_count();
    for (int i = 0; i < count; i++) {
        ns_sacred_t s = ns_sacred_get(i);
        TEST_ASSERT_TRUE(s.number > 0);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.description);
    }
}

void test_scan_260_finds_calendar_round(void)
{
    ns_scan_result_t r = number_scan(260);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].cycle_name, "Sacred calendar") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_scan_60_finds_sexagenary(void)
{
    ns_scan_result_t r = number_scan(60);
    int found = 0;
    for (int i = 0; i < r.match_count; i++) {
        if (strcmp(r.matches[i].system_name, "Chinese") == 0 &&
            strcmp(r.matches[i].cycle_name, "Sexagenary cycle") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Cycle registry */
    RUN_TEST(test_cycle_count_at_least_21);
    RUN_TEST(test_cycle_get_valid);
    RUN_TEST(test_cycle_get_invalid_negative);
    RUN_TEST(test_cycle_get_invalid_out_of_range);

    /* Sacred number registry */
    RUN_TEST(test_sacred_count_at_least_11);
    RUN_TEST(test_sacred_get_valid);
    RUN_TEST(test_sacred_get_invalid_negative);
    RUN_TEST(test_sacred_get_invalid_out_of_range);

    /* number_scan — core */
    RUN_TEST(test_scan_432_finds_kali_yuga);
    RUN_TEST(test_scan_432_finds_maha_yuga);
    RUN_TEST(test_scan_432_finds_buddhist_kalpa);
    RUN_TEST(test_scan_7_finds_week);
    RUN_TEST(test_scan_7_finds_hebrew_sabbatical);
    RUN_TEST(test_scan_64_finds_iching);
    RUN_TEST(test_scan_64_finds_human_design);
    RUN_TEST(test_scan_12_finds_zodiac);
    RUN_TEST(test_scan_1_is_factor_of_everything);
    RUN_TEST(test_scan_0_returns_no_matches);
    RUN_TEST(test_scan_negative_returns_no_matches);
    RUN_TEST(test_scan_match_count_within_limit);
    RUN_TEST(test_scan_has_descriptions);
    RUN_TEST(test_scan_relationship_values_valid);

    /* coincidence_score */
    RUN_TEST(test_coincidence_score_7_at_least_2);
    RUN_TEST(test_coincidence_score_64_at_least_2);
    RUN_TEST(test_coincidence_score_large_prime_is_0);
    RUN_TEST(test_coincidence_score_0_is_0);
    RUN_TEST(test_coincidence_score_matches_scan_result);

    /* factor_scan */
    RUN_TEST(test_factor_scan_25920_has_12);
    RUN_TEST(test_factor_scan_25920_has_72);
    RUN_TEST(test_factor_scan_25920_has_360);
    RUN_TEST(test_factor_scan_25920_has_432);
    RUN_TEST(test_factor_scan_25920_no_7);
    RUN_TEST(test_factor_scan_260_has_52);
    RUN_TEST(test_factor_scan_respects_out_max);
    RUN_TEST(test_factor_scan_zero_returns_0);
    RUN_TEST(test_factor_scan_negative_returns_0);

    /* ns_is_factor_of_cycle */
    RUN_TEST(test_is_factor_of_cycle_260);
    RUN_TEST(test_is_factor_of_cycle_7);
    RUN_TEST(test_is_factor_of_cycle_large_prime);
    RUN_TEST(test_is_factor_of_cycle_0);

    /* ns_is_sacred_multiple */
    RUN_TEST(test_is_sacred_multiple_144);
    RUN_TEST(test_is_sacred_multiple_24);
    RUN_TEST(test_is_sacred_multiple_7);
    RUN_TEST(test_is_sacred_multiple_prime);
    RUN_TEST(test_is_sacred_multiple_0);

    /* ns_scale_check */
    RUN_TEST(test_scale_check_432);
    RUN_TEST(test_scale_check_large_prime);
    RUN_TEST(test_scale_check_0);

    /* Scan result metadata */
    RUN_TEST(test_scan_coincidence_score_populated);
    RUN_TEST(test_scan_432_scale_appearances);

    /* Data integrity */
    RUN_TEST(test_all_cycles_have_system);
    RUN_TEST(test_all_sacred_have_name);
    RUN_TEST(test_scan_260_finds_calendar_round);
    RUN_TEST(test_scan_60_finds_sexagenary);

    return UNITY_END();
}
