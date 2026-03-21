/* test_earth_fraction.c — Tests for awe-inspiring Earth/human fractions.
 * Validates constants, computed fractions, fact catalog, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/earth_fraction.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Earth age ===== */

void test_earth_age_approximately_4_54_billion(void) {
    double age = ef_earth_age_years();
    TEST_ASSERT_FLOAT_WITHIN(1.0e8, 4.54e9, age);
}

void test_earth_age_positive(void) {
    TEST_ASSERT_TRUE(ef_earth_age_years() > 0.0);
}

/* ===== Human age ===== */

void test_human_age_approximately_300000(void) {
    double age = ef_human_age_years();
    TEST_ASSERT_FLOAT_WITHIN(50000.0, 300000.0, age);
}

void test_human_age_positive(void) {
    TEST_ASSERT_TRUE(ef_human_age_years() > 0.0);
}

void test_human_age_much_less_than_earth_age(void) {
    TEST_ASSERT_TRUE(ef_human_age_years() < ef_earth_age_years() * 0.001);
}

/* ===== Human fraction ===== */

void test_human_fraction_less_than_one_percent(void) {
    double frac = ef_human_fraction();
    TEST_ASSERT_TRUE(frac < 1.0);
}

void test_human_fraction_less_than_0_01(void) {
    double frac = ef_human_fraction();
    TEST_ASSERT_TRUE(frac < 0.01);
}

void test_human_fraction_positive(void) {
    TEST_ASSERT_TRUE(ef_human_fraction() > 0.0);
}

void test_human_fraction_approximately_0_0066(void) {
    double frac = ef_human_fraction();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0066, frac);
}

void test_human_fraction_consistent_with_ages(void) {
    /* fraction% = human_age / earth_age * 100 */
    double expected = (ef_human_age_years() / ef_earth_age_years()) * 100.0;
    TEST_ASSERT_FLOAT_WITHIN(1.0e-10, expected, ef_human_fraction());
}

/* ===== Alive fraction ===== */

void test_alive_fraction_between_5_and_10_percent(void) {
    double pct = ef_alive_fraction();
    TEST_ASSERT_TRUE(pct >= 5.0);
    TEST_ASSERT_TRUE(pct <= 10.0);
}

void test_alive_fraction_approximately_7_4(void) {
    double pct = ef_alive_fraction();
    TEST_ASSERT_FLOAT_WITHIN(1.0, 7.4, pct);
}

void test_alive_fraction_consistent_with_population(void) {
    double expected = (ef_current_population() / ef_humans_ever_born()) * 100.0;
    TEST_ASSERT_FLOAT_WITHIN(1.0e-6, expected, ef_alive_fraction());
}

/* ===== Population ===== */

void test_humans_ever_born_greater_than_100_billion(void) {
    TEST_ASSERT_TRUE(ef_humans_ever_born() > 100.0e9);
}

void test_humans_ever_born_approximately_109_billion(void) {
    TEST_ASSERT_FLOAT_WITHIN(10.0e9, 109.0e9, ef_humans_ever_born());
}

void test_current_population_approximately_8_billion(void) {
    TEST_ASSERT_FLOAT_WITHIN(1.0e9, 8.1e9, ef_current_population());
}

void test_current_population_less_than_ever_born(void) {
    TEST_ASSERT_TRUE(ef_current_population() < ef_humans_ever_born());
}

void test_current_population_positive(void) {
    TEST_ASSERT_TRUE(ef_current_population() > 0.0);
}

/* ===== Clock analogy ===== */

void test_clock_analogy_less_than_10_seconds(void) {
    double sec = ef_clock_analogy_seconds();
    TEST_ASSERT_TRUE(sec < 10.0);
}

void test_clock_analogy_positive(void) {
    TEST_ASSERT_TRUE(ef_clock_analogy_seconds() > 0.0);
}

void test_clock_analogy_approximately_5_7_seconds(void) {
    double sec = ef_clock_analogy_seconds();
    TEST_ASSERT_FLOAT_WITHIN(1.0, 5.7, sec);
}

void test_clock_analogy_consistent_with_fraction(void) {
    /* seconds = (human_age / earth_age) * 86400 */
    double expected = (ef_human_age_years() / ef_earth_age_years()) * 86400.0;
    TEST_ASSERT_FLOAT_WITHIN(1.0e-6, expected, ef_clock_analogy_seconds());
}

/* ===== Fact catalog: count ===== */

void test_fact_count_at_least_8(void) {
    TEST_ASSERT_TRUE(ef_fact_count() >= 8);
}

void test_fact_count_positive(void) {
    TEST_ASSERT_TRUE(ef_fact_count() > 0);
}

void test_fact_count_reasonable_upper_bound(void) {
    TEST_ASSERT_TRUE(ef_fact_count() < 1000);
}

/* ===== Fact catalog: valid entries ===== */

void test_all_facts_have_name(void) {
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        TEST_ASSERT_NOT_NULL(f.fact_name);
        TEST_ASSERT_TRUE(strlen(f.fact_name) > 0);
    }
}

void test_all_facts_have_description(void) {
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        TEST_ASSERT_NOT_NULL(f.description);
        TEST_ASSERT_TRUE(strlen(f.description) > 10);
    }
}

void test_all_facts_have_unit(void) {
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        TEST_ASSERT_NOT_NULL(f.unit);
        TEST_ASSERT_TRUE(strlen(f.unit) > 0);
    }
}

void test_all_facts_have_visual_analogy(void) {
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        TEST_ASSERT_NOT_NULL(f.visual_analogy);
        TEST_ASSERT_TRUE(strlen(f.visual_analogy) > 10);
    }
}

void test_all_facts_have_nonzero_value(void) {
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        TEST_ASSERT_TRUE(f.value > 0.0);
    }
}

/* ===== Fact catalog: specific entries ===== */

void test_fact_0_is_earth_age(void) {
    earth_fraction_t f = ef_fact_get(0);
    TEST_ASSERT_NOT_NULL(f.fact_name);
    TEST_ASSERT_NOT_NULL(strstr(f.fact_name, "earth_age"));
}

void test_fact_earth_age_value(void) {
    earth_fraction_t f = ef_fact_get(0);
    TEST_ASSERT_FLOAT_WITHIN(1.0e8, 4.54e9, f.value);
}

void test_human_fraction_fact_exists(void) {
    int found = 0;
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "human_fraction")) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_alive_fraction_fact_exists(void) {
    int found = 0;
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "alive_fraction")) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_clock_analogy_fact_exists(void) {
    int found = 0;
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "clock")) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_ruler_analogy_fact_exists(void) {
    int found = 0;
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "ruler")) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== Boundary: out-of-range index ===== */

void test_fact_get_negative_index_returns_null_name(void) {
    earth_fraction_t f = ef_fact_get(-1);
    TEST_ASSERT_NULL(f.fact_name);
}

void test_fact_get_negative_index_returns_zero_value(void) {
    earth_fraction_t f = ef_fact_get(-1);
    TEST_ASSERT_FLOAT_WITHIN(1.0e-15, 0.0, f.value);
}

void test_fact_get_too_large_index_returns_null_name(void) {
    earth_fraction_t f = ef_fact_get(ef_fact_count());
    TEST_ASSERT_NULL(f.fact_name);
}

void test_fact_get_too_large_index_returns_zero_value(void) {
    earth_fraction_t f = ef_fact_get(ef_fact_count());
    TEST_ASSERT_FLOAT_WITHIN(1.0e-15, 0.0, f.value);
}

void test_fact_get_very_large_index(void) {
    earth_fraction_t f = ef_fact_get(999999);
    TEST_ASSERT_NULL(f.fact_name);
    TEST_ASSERT_NULL(f.description);
    TEST_ASSERT_NULL(f.unit);
    TEST_ASSERT_NULL(f.visual_analogy);
}

/* ===== Cross-consistency ===== */

void test_human_fraction_matches_fact_value(void) {
    double from_func = ef_human_fraction();
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "human_fraction")) {
            TEST_ASSERT_FLOAT_WITHIN(1.0e-10, from_func, f.value);
            return;
        }
    }
    TEST_FAIL_MESSAGE("human_fraction fact not found in catalog");
}

void test_alive_fraction_matches_fact_value(void) {
    double from_func = ef_alive_fraction();
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "alive_fraction")) {
            TEST_ASSERT_FLOAT_WITHIN(1.0e-10, from_func, f.value);
            return;
        }
    }
    TEST_FAIL_MESSAGE("alive_fraction fact not found in catalog");
}

void test_clock_seconds_matches_fact_value(void) {
    double from_func = ef_clock_analogy_seconds();
    int count = ef_fact_count();
    for (int i = 0; i < count; i++) {
        earth_fraction_t f = ef_fact_get(i);
        if (f.fact_name && strstr(f.fact_name, "clock")) {
            TEST_ASSERT_FLOAT_WITHIN(1.0e-10, from_func, f.value);
            return;
        }
    }
    TEST_FAIL_MESSAGE("clock_analogy fact not found in catalog");
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Earth age */
    RUN_TEST(test_earth_age_approximately_4_54_billion);
    RUN_TEST(test_earth_age_positive);

    /* Human age */
    RUN_TEST(test_human_age_approximately_300000);
    RUN_TEST(test_human_age_positive);
    RUN_TEST(test_human_age_much_less_than_earth_age);

    /* Human fraction */
    RUN_TEST(test_human_fraction_less_than_one_percent);
    RUN_TEST(test_human_fraction_less_than_0_01);
    RUN_TEST(test_human_fraction_positive);
    RUN_TEST(test_human_fraction_approximately_0_0066);
    RUN_TEST(test_human_fraction_consistent_with_ages);

    /* Alive fraction */
    RUN_TEST(test_alive_fraction_between_5_and_10_percent);
    RUN_TEST(test_alive_fraction_approximately_7_4);
    RUN_TEST(test_alive_fraction_consistent_with_population);

    /* Population */
    RUN_TEST(test_humans_ever_born_greater_than_100_billion);
    RUN_TEST(test_humans_ever_born_approximately_109_billion);
    RUN_TEST(test_current_population_approximately_8_billion);
    RUN_TEST(test_current_population_less_than_ever_born);
    RUN_TEST(test_current_population_positive);

    /* Clock analogy */
    RUN_TEST(test_clock_analogy_less_than_10_seconds);
    RUN_TEST(test_clock_analogy_positive);
    RUN_TEST(test_clock_analogy_approximately_5_7_seconds);
    RUN_TEST(test_clock_analogy_consistent_with_fraction);

    /* Fact catalog: count */
    RUN_TEST(test_fact_count_at_least_8);
    RUN_TEST(test_fact_count_positive);
    RUN_TEST(test_fact_count_reasonable_upper_bound);

    /* Fact catalog: valid entries */
    RUN_TEST(test_all_facts_have_name);
    RUN_TEST(test_all_facts_have_description);
    RUN_TEST(test_all_facts_have_unit);
    RUN_TEST(test_all_facts_have_visual_analogy);
    RUN_TEST(test_all_facts_have_nonzero_value);

    /* Fact catalog: specific entries */
    RUN_TEST(test_fact_0_is_earth_age);
    RUN_TEST(test_fact_earth_age_value);
    RUN_TEST(test_human_fraction_fact_exists);
    RUN_TEST(test_alive_fraction_fact_exists);
    RUN_TEST(test_clock_analogy_fact_exists);
    RUN_TEST(test_ruler_analogy_fact_exists);

    /* Boundary: out-of-range */
    RUN_TEST(test_fact_get_negative_index_returns_null_name);
    RUN_TEST(test_fact_get_negative_index_returns_zero_value);
    RUN_TEST(test_fact_get_too_large_index_returns_null_name);
    RUN_TEST(test_fact_get_too_large_index_returns_zero_value);
    RUN_TEST(test_fact_get_very_large_index);

    /* Cross-consistency */
    RUN_TEST(test_human_fraction_matches_fact_value);
    RUN_TEST(test_alive_fraction_matches_fact_value);
    RUN_TEST(test_clock_seconds_matches_fact_value);

    return UNITY_END();
}
