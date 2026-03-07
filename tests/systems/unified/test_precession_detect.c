/* test_precession_detect.c -- Precession Encoder Detector tests
 * TDD RED phase: tests written before implementation.
 * Tests whether ancient time systems encode the axial precession cycle
 * (~25,772 years). Registers known cycles from multiple cultures, tests
 * each against precession via integer multiples, and generates a report
 * ranking systems by encoding accuracy. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/precession_detect.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants verification ===== */

void test_precession_cycle_years(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1, 25772.0, PRECESSION_CYCLE_YEARS);
}

void test_max_cycles_defined(void)
{
    TEST_ASSERT_TRUE(PRECESSION_MAX_CYCLES >= 9);
}

void test_max_matches_defined(void)
{
    TEST_ASSERT_TRUE(PRECESSION_MAX_MATCHES >= 9);
}

/* ===== pd_cycle_count ===== */

void test_cycle_count_minimum_nine(void)
{
    /* 9 cultures in the specification table */
    TEST_ASSERT_TRUE(pd_cycle_count() >= 9);
}

void test_cycle_count_within_max(void)
{
    TEST_ASSERT_TRUE(pd_cycle_count() <= PRECESSION_MAX_CYCLES);
}

/* ===== pd_cycle_get ===== */

void test_cycle_get_first_has_name(void)
{
    pd_cycle_t c = pd_cycle_get(0);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_TRUE(strlen(c.name) > 0);
}

void test_cycle_get_first_has_culture(void)
{
    pd_cycle_t c = pd_cycle_get(0);
    TEST_ASSERT_NOT_NULL(c.culture);
    TEST_ASSERT_TRUE(strlen(c.culture) > 0);
}

void test_cycle_get_first_has_positive_period(void)
{
    pd_cycle_t c = pd_cycle_get(0);
    TEST_ASSERT_TRUE(c.period_years > 0.0);
}

void test_cycle_get_out_of_range_returns_null_name(void)
{
    pd_cycle_t c = pd_cycle_get(999);
    TEST_ASSERT_NULL(c.name);
}

void test_cycle_get_negative_returns_null_name(void)
{
    pd_cycle_t c = pd_cycle_get(-1);
    TEST_ASSERT_NULL(c.name);
}

void test_cycle_get_mayan_exists(void)
{
    int count = pd_cycle_count();
    int found = 0;
    for (int i = 0; i < count; i++) {
        pd_cycle_t c = pd_cycle_get(i);
        if (strcmp(c.culture, "Mayan") == 0) {
            found = 1;
            TEST_ASSERT_FLOAT_WITHIN(0.01, 5125.36, c.period_years);
            TEST_ASSERT_EQUAL_INT(5, c.known_multiplier);
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, found);
}

void test_cycle_get_hindu_exists(void)
{
    int count = pd_cycle_count();
    int found = 0;
    for (int i = 0; i < count; i++) {
        pd_cycle_t c = pd_cycle_get(i);
        if (strcmp(c.culture, "Hindu") == 0) {
            found = 1;
            TEST_ASSERT_FLOAT_WITHIN(0.01, 2160.0, c.period_years);
            TEST_ASSERT_EQUAL_INT(12, c.known_multiplier);
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, found);
}

void test_cycle_get_islamic_exists(void)
{
    int count = pd_cycle_count();
    int found = 0;
    for (int i = 0; i < count; i++) {
        pd_cycle_t c = pd_cycle_get(i);
        if (strcmp(c.culture, "Islamic") == 0) {
            found = 1;
            TEST_ASSERT_FLOAT_WITHIN(0.01, 30.0, c.period_years);
            TEST_ASSERT_EQUAL_INT(859, c.known_multiplier);
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, found);
}

void test_cycle_get_all_have_positive_period(void)
{
    int count = pd_cycle_count();
    for (int i = 0; i < count; i++) {
        pd_cycle_t c = pd_cycle_get(i);
        TEST_ASSERT_TRUE(c.period_years > 0.0);
    }
}

void test_cycle_get_all_have_known_multiplier(void)
{
    /* All registered cycles have known multipliers > 0 */
    int count = pd_cycle_count();
    for (int i = 0; i < count; i++) {
        pd_cycle_t c = pd_cycle_get(i);
        TEST_ASSERT_TRUE(c.known_multiplier > 0);
    }
}

/* ===== pd_error ===== */

void test_error_exact_match(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, pd_error(25772.0));
}

void test_error_mayan_product(void)
{
    /* 5125.36 * 5 = 25626.8 */
    double err = pd_error(25626.8);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 0.56, err);
}

void test_error_hindu_product(void)
{
    /* 2160 * 12 = 25920 */
    double err = pd_error(25920.0);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 0.57, err);
}

void test_error_hebrew_product(void)
{
    /* 600 * 43 = 25800 */
    double err = pd_error(25800.0);
    TEST_ASSERT_FLOAT_WITHIN(0.02, 0.11, err);
}

void test_error_islamic_product(void)
{
    /* 30 * 859 = 25770 */
    double err = pd_error(25770.0);
    TEST_ASSERT_FLOAT_WITHIN(0.005, 0.01, err);
}

void test_error_always_positive(void)
{
    /* Even if product < precession */
    double err = pd_error(20000.0);
    TEST_ASSERT_TRUE(err > 0.0);
}

void test_error_symmetric(void)
{
    /* Same distance above and below */
    double above = pd_error(25772.0 + 100.0);
    double below = pd_error(25772.0 - 100.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, above, below);
}

/* ===== pd_best_multiplier ===== */

void test_best_multiplier_exact(void)
{
    /* 25772 itself: best mult is 1 */
    TEST_ASSERT_EQUAL_INT(1, pd_best_multiplier(25772.0));
}

void test_best_multiplier_mayan(void)
{
    TEST_ASSERT_EQUAL_INT(5, pd_best_multiplier(5125.36));
}

void test_best_multiplier_hindu(void)
{
    TEST_ASSERT_EQUAL_INT(12, pd_best_multiplier(2160.0));
}

void test_best_multiplier_chinese(void)
{
    /* 25772 / 60 = 429.53, round = 430 */
    int m = pd_best_multiplier(60.0);
    /* 60*429=25740 (err=32), 60*430=25800 (err=28) => 430 wins */
    TEST_ASSERT_EQUAL_INT(430, m);
}

void test_best_multiplier_islamic(void)
{
    /* 25772 / 30 = 859.07, round = 859 */
    int m = pd_best_multiplier(30.0);
    TEST_ASSERT_EQUAL_INT(859, m);
}

void test_best_multiplier_small_period(void)
{
    /* period=1.0 should give mult=round(25772)  but search 1-1000 */
    int m = pd_best_multiplier(1.0);
    /* Can only go up to 1000, so best is 1000 (25772-1000=24772 vs others) */
    /* Actually the closest to 25772 in range 1-1000 is 1000*1=1000... no.
     * Wait: 1.0 * m = m. We need m closest to 25772. Max m=1000. So best is 1000. */
    TEST_ASSERT_EQUAL_INT(1000, m);
}

/* ===== pd_test ===== */

void test_pd_test_mayan(void)
{
    pd_match_t m = pd_test(5125.36, 5, "Long Count Great Cycle", "Mayan");
    TEST_ASSERT_EQUAL_STRING("Long Count Great Cycle", m.cycle_name);
    TEST_ASSERT_EQUAL_STRING("Mayan", m.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5125.36, m.period_years);
    TEST_ASSERT_EQUAL_INT(5, m.multiplier);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25626.8, m.product_years);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 0.56, m.error_percent);
}

void test_pd_test_exact(void)
{
    pd_match_t m = pd_test(25772.0, 1, "Exact", "Test");
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 0.0, m.error_percent);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25772.0, m.product_years);
}

void test_pd_test_zero_multiplier_searches(void)
{
    /* multiplier=0 means auto-search for best */
    pd_match_t m = pd_test(5125.36, 0, "Auto", "Test");
    TEST_ASSERT_EQUAL_INT(5, m.multiplier);
    TEST_ASSERT_TRUE(m.error_percent < 1.0);
}

void test_pd_test_rank_default_zero(void)
{
    /* rank is not assigned by pd_test, should be 0 */
    pd_match_t m = pd_test(5125.36, 5, "Test", "Test");
    TEST_ASSERT_EQUAL_INT(0, m.rank);
}

void test_pd_test_stores_name(void)
{
    pd_match_t m = pd_test(60.0, 429, "Sexagenary cycle", "Chinese");
    TEST_ASSERT_EQUAL_STRING("Sexagenary cycle", m.cycle_name);
    TEST_ASSERT_EQUAL_STRING("Chinese", m.culture);
}

/* ===== pd_degrees_per_year ===== */

void test_degrees_per_year_value(void)
{
    double expected = 360.0 / 25772.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-8, expected, pd_degrees_per_year());
}

void test_degrees_per_year_approx(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.01397, pd_degrees_per_year());
}

/* ===== pd_years_per_degree ===== */

void test_years_per_degree_value(void)
{
    double expected = 25772.0 / 360.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, pd_years_per_degree());
}

void test_years_per_degree_approx_72(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.5, 71.589, pd_years_per_degree());
}

void test_years_per_degree_inverse(void)
{
    /* degrees_per_year * years_per_degree = 1.0 */
    double product = pd_degrees_per_year() * pd_years_per_degree();
    TEST_ASSERT_FLOAT_WITHIN(1e-8, 1.0, product);
}

/* ===== pd_report ===== */

void test_report_match_count(void)
{
    pd_report_t r = pd_report();
    TEST_ASSERT_TRUE(r.match_count >= 9);
}

void test_report_match_count_within_max(void)
{
    pd_report_t r = pd_report();
    TEST_ASSERT_TRUE(r.match_count <= PRECESSION_MAX_MATCHES);
}

void test_report_sorted_by_error(void)
{
    pd_report_t r = pd_report();
    for (int i = 1; i < r.match_count; i++) {
        TEST_ASSERT_TRUE(r.matches[i].error_percent >= r.matches[i - 1].error_percent);
    }
}

void test_report_ranks_assigned(void)
{
    pd_report_t r = pd_report();
    for (int i = 0; i < r.match_count; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, r.matches[i].rank);
    }
}

void test_report_best_error(void)
{
    pd_report_t r = pd_report();
    TEST_ASSERT_TRUE(r.best_error_percent >= 0.0);
    TEST_ASSERT_TRUE(r.best_error_percent < 1.0);
    /* best_error_percent should match first match */
    TEST_ASSERT_FLOAT_WITHIN(1e-6, r.matches[0].error_percent, r.best_error_percent);
}

void test_report_best_culture(void)
{
    pd_report_t r = pd_report();
    TEST_ASSERT_NOT_NULL(r.best_culture);
    TEST_ASSERT_TRUE(strlen(r.best_culture) > 0);
}

void test_report_islamic_best(void)
{
    /* Islamic 30-year Hijri cycle: 30*859=25770, error ~0.01% — should be rank 1 */
    pd_report_t r = pd_report();
    TEST_ASSERT_EQUAL_STRING("Islamic", r.best_culture);
}

void test_report_all_have_names(void)
{
    pd_report_t r = pd_report();
    for (int i = 0; i < r.match_count; i++) {
        TEST_ASSERT_NOT_NULL(r.matches[i].cycle_name);
        TEST_ASSERT_NOT_NULL(r.matches[i].culture);
    }
}

/* ===== pd_report_rank ===== */

void test_report_rank_first(void)
{
    pd_report_t r = pd_report();
    pd_match_t m = pd_report_rank(&r, 1);
    TEST_ASSERT_EQUAL_INT(1, m.rank);
    TEST_ASSERT_NOT_NULL(m.cycle_name);
}

void test_report_rank_last(void)
{
    pd_report_t r = pd_report();
    pd_match_t m = pd_report_rank(&r, r.match_count);
    TEST_ASSERT_EQUAL_INT(r.match_count, m.rank);
}

void test_report_rank_out_of_range(void)
{
    pd_report_t r = pd_report();
    pd_match_t m = pd_report_rank(&r, 0);
    TEST_ASSERT_EQUAL_INT(0, m.rank);
    TEST_ASSERT_NULL(m.cycle_name);
}

void test_report_rank_too_high(void)
{
    pd_report_t r = pd_report();
    pd_match_t m = pd_report_rank(&r, 999);
    TEST_ASSERT_EQUAL_INT(0, m.rank);
}

/* ===== pd_cultures_within ===== */

void test_cultures_within_loose(void)
{
    pd_report_t r = pd_report();
    int count = pd_cultures_within(&r, 5.0);
    /* All 9 cultures should be within 5% */
    TEST_ASSERT_TRUE(count >= 9);
}

void test_cultures_within_tight(void)
{
    pd_report_t r = pd_report();
    int count = pd_cultures_within(&r, 0.005);
    /* Only Islamic at ~0.01%... actually 0.01 > 0.005, so 0 */
    TEST_ASSERT_TRUE(count >= 0);
    TEST_ASSERT_TRUE(count < pd_cycle_count());
}

void test_cultures_within_zero(void)
{
    pd_report_t r = pd_report();
    int count = pd_cultures_within(&r, 0.0);
    /* No culture has exactly 0% error */
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_cultures_within_one_percent(void)
{
    pd_report_t r = pd_report();
    int count = pd_cultures_within(&r, 1.0);
    /* Mayan(0.56%), Hindu(0.57%), Hebrew(0.11%), Chinese(0.12%),
     * Greek(0.57%), Buddhist(0.57%), Islamic(0.01%), Babylonian(0.04%) = 8
     * Egyptian(2.04%) is out. So at least 8. */
    TEST_ASSERT_TRUE(count >= 7);
}

/* ===== Known encoding table validation ===== */

void test_babylonian_saros(void)
{
    /* 18.03 * 1430 = 25782.9, error ~0.04% */
    pd_match_t m = pd_test(18.03, 1430, "Saros cycle", "Babylonian");
    TEST_ASSERT_FLOAT_WITHIN(0.5, 25782.9, m.product_years);
    TEST_ASSERT_FLOAT_WITHIN(0.02, 0.04, m.error_percent);
}

void test_egyptian_sothic(void)
{
    /* 1461 * 18 = 26298, error ~2.04% */
    pd_match_t m = pd_test(1461.0, 18, "Sothic cycle", "Egyptian");
    TEST_ASSERT_FLOAT_WITHIN(0.01, 26298.0, m.product_years);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2.04, m.error_percent);
}

void test_greek_platonic_year(void)
{
    /* 25920 * 1 = 25920, error ~0.57% */
    pd_match_t m = pd_test(25920.0, 1, "Great Year (Plato)", "Greek");
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25920.0, m.product_years);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 0.57, m.error_percent);
}

void test_buddhist_kalpa(void)
{
    /* 4320 * 6 = 25920, error ~0.57% */
    pd_match_t m = pd_test(4320.0, 6, "Kalpa subdivision", "Buddhist");
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25920.0, m.product_years);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 0.57, m.error_percent);
}

int main(void)
{
    UNITY_BEGIN();

    /* Constants */
    RUN_TEST(test_precession_cycle_years);
    RUN_TEST(test_max_cycles_defined);
    RUN_TEST(test_max_matches_defined);

    /* pd_cycle_count */
    RUN_TEST(test_cycle_count_minimum_nine);
    RUN_TEST(test_cycle_count_within_max);

    /* pd_cycle_get */
    RUN_TEST(test_cycle_get_first_has_name);
    RUN_TEST(test_cycle_get_first_has_culture);
    RUN_TEST(test_cycle_get_first_has_positive_period);
    RUN_TEST(test_cycle_get_out_of_range_returns_null_name);
    RUN_TEST(test_cycle_get_negative_returns_null_name);
    RUN_TEST(test_cycle_get_mayan_exists);
    RUN_TEST(test_cycle_get_hindu_exists);
    RUN_TEST(test_cycle_get_islamic_exists);
    RUN_TEST(test_cycle_get_all_have_positive_period);
    RUN_TEST(test_cycle_get_all_have_known_multiplier);

    /* pd_error */
    RUN_TEST(test_error_exact_match);
    RUN_TEST(test_error_mayan_product);
    RUN_TEST(test_error_hindu_product);
    RUN_TEST(test_error_hebrew_product);
    RUN_TEST(test_error_islamic_product);
    RUN_TEST(test_error_always_positive);
    RUN_TEST(test_error_symmetric);

    /* pd_best_multiplier */
    RUN_TEST(test_best_multiplier_exact);
    RUN_TEST(test_best_multiplier_mayan);
    RUN_TEST(test_best_multiplier_hindu);
    RUN_TEST(test_best_multiplier_chinese);
    RUN_TEST(test_best_multiplier_islamic);
    RUN_TEST(test_best_multiplier_small_period);

    /* pd_test */
    RUN_TEST(test_pd_test_mayan);
    RUN_TEST(test_pd_test_exact);
    RUN_TEST(test_pd_test_zero_multiplier_searches);
    RUN_TEST(test_pd_test_rank_default_zero);
    RUN_TEST(test_pd_test_stores_name);

    /* pd_degrees_per_year */
    RUN_TEST(test_degrees_per_year_value);
    RUN_TEST(test_degrees_per_year_approx);

    /* pd_years_per_degree */
    RUN_TEST(test_years_per_degree_value);
    RUN_TEST(test_years_per_degree_approx_72);
    RUN_TEST(test_years_per_degree_inverse);

    /* pd_report */
    RUN_TEST(test_report_match_count);
    RUN_TEST(test_report_match_count_within_max);
    RUN_TEST(test_report_sorted_by_error);
    RUN_TEST(test_report_ranks_assigned);
    RUN_TEST(test_report_best_error);
    RUN_TEST(test_report_best_culture);
    RUN_TEST(test_report_islamic_best);
    RUN_TEST(test_report_all_have_names);

    /* pd_report_rank */
    RUN_TEST(test_report_rank_first);
    RUN_TEST(test_report_rank_last);
    RUN_TEST(test_report_rank_out_of_range);
    RUN_TEST(test_report_rank_too_high);

    /* pd_cultures_within */
    RUN_TEST(test_cultures_within_loose);
    RUN_TEST(test_cultures_within_tight);
    RUN_TEST(test_cultures_within_zero);
    RUN_TEST(test_cultures_within_one_percent);

    /* Known encoding table validation */
    RUN_TEST(test_babylonian_saros);
    RUN_TEST(test_egyptian_sothic);
    RUN_TEST(test_greek_platonic_year);
    RUN_TEST(test_buddhist_kalpa);

    return UNITY_END();
}
