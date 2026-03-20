/* test_system_correlation.c — Cross-system co-firing analysis tests (Track 26)
 *
 * TDD: tests for the 21x21 correlation matrix scanner.
 * Tests null safety, matrix properties, known relationships, ordering. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/system_correlation.h"
#include "../../../src/math/julian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Shared result — computed once, reused across tests.
 * 1 year scan (2026) is expensive, so we cache it. */
static sc_corr_result_t s_result;
static int s_computed = 0;

static void ensure_computed(void)
{
    if (!s_computed) {
        sc_corr_compute(2026, 1, &s_result);
        s_computed = 1;
    }
}

/* ===================================================================
 * Null safety
 * =================================================================== */

void test_compute_null_out(void)
{
    int ret = sc_corr_compute(2026, 1, NULL);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_compute_zero_years(void)
{
    sc_corr_result_t r;
    int ret = sc_corr_compute(2026, 0, &r);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_compute_negative_years(void)
{
    sc_corr_result_t r;
    int ret = sc_corr_compute(2026, -1, &r);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_top_pairs_null_result(void)
{
    sc_corr_pair_t pairs[5];
    int ret = sc_corr_top_pairs(NULL, pairs, 5);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_top_pairs_null_out(void)
{
    ensure_computed();
    int ret = sc_corr_top_pairs(&s_result, NULL, 5);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_top_pairs_zero_max(void)
{
    ensure_computed();
    sc_corr_pair_t pairs[1];
    int ret = sc_corr_top_pairs(&s_result, pairs, 0);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_frequency_null_result(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0,
        sc_corr_frequency(NULL, CD_SYS_ASTRONOMY, CD_SYS_ISLAMIC));
}

void test_frequency_invalid_system_a(void)
{
    ensure_computed();
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0,
        sc_corr_frequency(&s_result, (cd_system_t)-1, CD_SYS_ASTRONOMY));
}

void test_frequency_invalid_system_b(void)
{
    ensure_computed();
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0,
        sc_corr_frequency(&s_result, CD_SYS_ASTRONOMY, (cd_system_t)99));
}

void test_solo_null_result(void)
{
    int s = sc_corr_solo(NULL, CD_SYS_ASTRONOMY);
    TEST_ASSERT_EQUAL_INT(0, s);
}

void test_solo_invalid_system(void)
{
    ensure_computed();
    int s = sc_corr_solo(&s_result, (cd_system_t)-1);
    TEST_ASSERT_EQUAL_INT(0, s);
}

void test_solo_invalid_system_high(void)
{
    ensure_computed();
    int s = sc_corr_solo(&s_result, (cd_system_t)99);
    TEST_ASSERT_EQUAL_INT(0, s);
}

/* ===================================================================
 * Basic computation properties
 * =================================================================== */

void test_compute_returns_365_for_one_year(void)
{
    ensure_computed();
    TEST_ASSERT_EQUAL_INT(365, s_result.days_scanned);
}

void test_compute_years_recorded(void)
{
    ensure_computed();
    TEST_ASSERT_EQUAL_INT(1, s_result.years_scanned);
}

void test_matrix_values_non_negative(void)
{
    ensure_computed();
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        for (int j = 0; j < CD_SYS_COUNT; j++) {
            TEST_ASSERT_TRUE(s_result.matrix[i][j] >= 0.0);
        }
    }
}

void test_matrix_values_at_most_one(void)
{
    ensure_computed();
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        for (int j = 0; j < CD_SYS_COUNT; j++) {
            TEST_ASSERT_TRUE(s_result.matrix[i][j] <= 1.0);
        }
    }
}

void test_diagonal_is_zero(void)
{
    ensure_computed();
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, s_result.matrix[i][i]);
    }
}

void test_matrix_is_symmetric(void)
{
    ensure_computed();
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        for (int j = i + 1; j < CD_SYS_COUNT; j++) {
            TEST_ASSERT_FLOAT_WITHIN(1e-12,
                s_result.matrix[i][j],
                s_result.matrix[j][i]);
        }
    }
}

/* ===================================================================
 * Solo counts
 * =================================================================== */

void test_astronomy_fires_some_days(void)
{
    ensure_computed();
    /* Astronomy fires on new/full moon + solstice/equinox — should be >0 */
    int solo = sc_corr_solo(&s_result, CD_SYS_ASTRONOMY);
    TEST_ASSERT_TRUE(solo > 0);
}

void test_islamic_fires_some_days(void)
{
    ensure_computed();
    /* Islamic fires on new-month days + entire Ramadan — should be >0 */
    int solo = sc_corr_solo(&s_result, CD_SYS_ISLAMIC);
    TEST_ASSERT_TRUE(solo > 0);
}

void test_solo_count_bounded_by_days_scanned(void)
{
    ensure_computed();
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        TEST_ASSERT_TRUE(s_result.solo_count[i] <= s_result.days_scanned);
    }
}

/* ===================================================================
 * Frequency accessor matches matrix
 * =================================================================== */

void test_frequency_matches_matrix(void)
{
    ensure_computed();
    double f = sc_corr_frequency(&s_result, CD_SYS_ASTRONOMY, CD_SYS_ISLAMIC);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, s_result.matrix[CD_SYS_ASTRONOMY][CD_SYS_ISLAMIC], f);
}

void test_frequency_order_independent(void)
{
    ensure_computed();
    double f1 = sc_corr_frequency(&s_result, CD_SYS_HEBREW, CD_SYS_BUDDHIST);
    double f2 = sc_corr_frequency(&s_result, CD_SYS_BUDDHIST, CD_SYS_HEBREW);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, f1, f2);
}

/* ===================================================================
 * Top pairs
 * =================================================================== */

void test_top_pairs_returns_positive_count(void)
{
    ensure_computed();
    sc_corr_pair_t pairs[5];
    int n = sc_corr_top_pairs(&s_result, pairs, 5);
    TEST_ASSERT_TRUE(n > 0);
}

void test_top_pairs_descending_order(void)
{
    ensure_computed();
    sc_corr_pair_t pairs[10];
    int n = sc_corr_top_pairs(&s_result, pairs, 10);
    for (int i = 1; i < n; i++) {
        TEST_ASSERT_TRUE(pairs[i - 1].frequency >= pairs[i].frequency);
    }
}

void test_top_pairs_systems_valid(void)
{
    ensure_computed();
    sc_corr_pair_t pairs[5];
    int n = sc_corr_top_pairs(&s_result, pairs, 5);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(pairs[i].system_a >= 0);
        TEST_ASSERT_TRUE(pairs[i].system_a < CD_SYS_COUNT);
        TEST_ASSERT_TRUE(pairs[i].system_b >= 0);
        TEST_ASSERT_TRUE(pairs[i].system_b < CD_SYS_COUNT);
        TEST_ASSERT_TRUE(pairs[i].system_a < pairs[i].system_b);
    }
}

void test_top_pairs_frequency_positive(void)
{
    ensure_computed();
    sc_corr_pair_t pairs[5];
    int n = sc_corr_top_pairs(&s_result, pairs, 5);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(pairs[i].frequency > 0.0);
        TEST_ASSERT_TRUE(pairs[i].co_fire_days > 0);
    }
}

void test_top_pairs_respects_max(void)
{
    ensure_computed();
    sc_corr_pair_t pairs[2];
    int n = sc_corr_top_pairs(&s_result, pairs, 2);
    TEST_ASSERT_TRUE(n <= 2);
}

/* ===================================================================
 * Known structural relationships
 * =================================================================== */

void test_astronomy_hindu_co_fire(void)
{
    /* Both Astronomy and Hindu track lunar phases (new/full moon),
     * so they should co-fire on those days. */
    ensure_computed();
    double f = sc_corr_frequency(&s_result, CD_SYS_ASTRONOMY, CD_SYS_HINDU);
    TEST_ASSERT_TRUE(f > 0.0);
}

void test_astronomy_buddhist_co_fire(void)
{
    /* Buddhist Uposatha is tied to quarter-moon days, which overlap
     * with Astronomy's new/full moon detection. */
    ensure_computed();
    double f = sc_corr_frequency(&s_result, CD_SYS_ASTRONOMY, CD_SYS_BUDDHIST);
    TEST_ASSERT_TRUE(f > 0.0);
}

/* ===================================================================
 * main
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Null safety */
    RUN_TEST(test_compute_null_out);
    RUN_TEST(test_compute_zero_years);
    RUN_TEST(test_compute_negative_years);
    RUN_TEST(test_top_pairs_null_result);
    RUN_TEST(test_top_pairs_null_out);
    RUN_TEST(test_top_pairs_zero_max);
    RUN_TEST(test_frequency_null_result);
    RUN_TEST(test_frequency_invalid_system_a);
    RUN_TEST(test_frequency_invalid_system_b);
    RUN_TEST(test_solo_null_result);
    RUN_TEST(test_solo_invalid_system);
    RUN_TEST(test_solo_invalid_system_high);

    /* Computation properties */
    RUN_TEST(test_compute_returns_365_for_one_year);
    RUN_TEST(test_compute_years_recorded);
    RUN_TEST(test_matrix_values_non_negative);
    RUN_TEST(test_matrix_values_at_most_one);
    RUN_TEST(test_diagonal_is_zero);
    RUN_TEST(test_matrix_is_symmetric);

    /* Solo counts */
    RUN_TEST(test_astronomy_fires_some_days);
    RUN_TEST(test_islamic_fires_some_days);
    RUN_TEST(test_solo_count_bounded_by_days_scanned);

    /* Frequency accessor */
    RUN_TEST(test_frequency_matches_matrix);
    RUN_TEST(test_frequency_order_independent);

    /* Top pairs */
    RUN_TEST(test_top_pairs_returns_positive_count);
    RUN_TEST(test_top_pairs_descending_order);
    RUN_TEST(test_top_pairs_systems_valid);
    RUN_TEST(test_top_pairs_frequency_positive);
    RUN_TEST(test_top_pairs_respects_max);

    /* Known relationships */
    RUN_TEST(test_astronomy_hindu_co_fire);
    RUN_TEST(test_astronomy_buddhist_co_fire);

    return UNITY_END();
}
