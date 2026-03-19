/* test_brain_types.c — Tests for brain pipeline shared types.
 *
 * Validates type definitions, enum completeness, name lookups,
 * and struct size constraints for the brain intelligence layer. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_types.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Correlation type enum
 * =================================================================== */

void test_correlation_type_structural_is_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, BR_CORR_STRUCTURAL);
}

void test_correlation_type_count(void) {
    TEST_ASSERT_EQUAL_INT(4, BR_CORR_COUNT);
}

void test_correlation_type_values_sequential(void) {
    TEST_ASSERT_EQUAL_INT(0, BR_CORR_STRUCTURAL);
    TEST_ASSERT_EQUAL_INT(1, BR_CORR_HARMONIC);
    TEST_ASSERT_EQUAL_INT(2, BR_CORR_APPROXIMATE);
    TEST_ASSERT_EQUAL_INT(3, BR_CORR_COINCIDENTAL);
}

void test_correlation_type_name_structural(void) {
    TEST_ASSERT_EQUAL_STRING("Structural", br_correlation_type_name(BR_CORR_STRUCTURAL));
}

void test_correlation_type_name_harmonic(void) {
    TEST_ASSERT_EQUAL_STRING("Harmonic", br_correlation_type_name(BR_CORR_HARMONIC));
}

void test_correlation_type_name_approximate(void) {
    TEST_ASSERT_EQUAL_STRING("Approximate", br_correlation_type_name(BR_CORR_APPROXIMATE));
}

void test_correlation_type_name_coincidental(void) {
    TEST_ASSERT_EQUAL_STRING("Coincidental", br_correlation_type_name(BR_CORR_COINCIDENTAL));
}

void test_correlation_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", br_correlation_type_name(99));
    TEST_ASSERT_EQUAL_STRING("?", br_correlation_type_name(-1));
}

/* ===================================================================
 * Insight type enum
 * =================================================================== */

void test_insight_type_convergence_is_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, BR_INSIGHT_CONVERGENCE);
}

void test_insight_type_count(void) {
    TEST_ASSERT_EQUAL_INT(6, BR_INSIGHT_COUNT);
}

void test_insight_type_values_sequential(void) {
    TEST_ASSERT_EQUAL_INT(0, BR_INSIGHT_CONVERGENCE);
    TEST_ASSERT_EQUAL_INT(1, BR_INSIGHT_NUMBER);
    TEST_ASSERT_EQUAL_INT(2, BR_INSIGHT_CYCLE);
    TEST_ASSERT_EQUAL_INT(3, BR_INSIGHT_FESTIVAL);
    TEST_ASSERT_EQUAL_INT(4, BR_INSIGHT_ASTRONOMICAL);
    TEST_ASSERT_EQUAL_INT(5, BR_INSIGHT_QUIET);
}

void test_insight_type_name_convergence(void) {
    TEST_ASSERT_EQUAL_STRING("Convergence", br_insight_type_name(BR_INSIGHT_CONVERGENCE));
}

void test_insight_type_name_number(void) {
    TEST_ASSERT_EQUAL_STRING("Number", br_insight_type_name(BR_INSIGHT_NUMBER));
}

void test_insight_type_name_cycle(void) {
    TEST_ASSERT_EQUAL_STRING("Cycle", br_insight_type_name(BR_INSIGHT_CYCLE));
}

void test_insight_type_name_festival(void) {
    TEST_ASSERT_EQUAL_STRING("Festival", br_insight_type_name(BR_INSIGHT_FESTIVAL));
}

void test_insight_type_name_astronomical(void) {
    TEST_ASSERT_EQUAL_STRING("Astronomical", br_insight_type_name(BR_INSIGHT_ASTRONOMICAL));
}

void test_insight_type_name_quiet(void) {
    TEST_ASSERT_EQUAL_STRING("Quiet", br_insight_type_name(BR_INSIGHT_QUIET));
}

void test_insight_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", br_insight_type_name(99));
    TEST_ASSERT_EQUAL_STRING("?", br_insight_type_name(-1));
}

/* ===================================================================
 * Constants
 * =================================================================== */

void test_max_correlations_positive(void) {
    TEST_ASSERT_GREATER_THAN(0, BR_MAX_CORRELATIONS);
}

void test_max_insights_positive(void) {
    TEST_ASSERT_GREATER_THAN(0, BR_MAX_INSIGHTS);
}

void test_max_systems_per_insight_positive(void) {
    TEST_ASSERT_GREATER_THAN(0, BR_MAX_SYSTEMS);
}

void test_headline_max_reasonable(void) {
    TEST_ASSERT_GREATER_OR_EQUAL(64, BR_HEADLINE_MAX);
    TEST_ASSERT_LESS_OR_EQUAL(256, BR_HEADLINE_MAX);
}

void test_detail_max_reasonable(void) {
    TEST_ASSERT_GREATER_OR_EQUAL(128, BR_DETAIL_MAX);
    TEST_ASSERT_LESS_OR_EQUAL(512, BR_DETAIL_MAX);
}

void test_summary_max_reasonable(void) {
    TEST_ASSERT_GREATER_OR_EQUAL(256, BR_SUMMARY_MAX);
    TEST_ASSERT_LESS_OR_EQUAL(1024, BR_SUMMARY_MAX);
}

/* ===================================================================
 * Struct field access — correlation
 * =================================================================== */

void test_correlation_struct_zero_init(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    TEST_ASSERT_EQUAL_INT(0, c.system_a);
    TEST_ASSERT_EQUAL_INT(0, c.system_b);
    TEST_ASSERT_EQUAL_FLOAT(0.0, c.alignment_score);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, c.type);
    TEST_ASSERT_EQUAL_FLOAT(0.0, c.confidence);
}

void test_correlation_struct_field_assignment(void) {
    br_correlation_t c;
    c.system_a = 3;
    c.system_b = 7;
    c.alignment_score = 0.85;
    c.type = BR_CORR_HARMONIC;
    c.confidence = 0.95;
    TEST_ASSERT_EQUAL_INT(3, c.system_a);
    TEST_ASSERT_EQUAL_INT(7, c.system_b);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.85, c.alignment_score);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, c.type);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.95, c.confidence);
}

/* ===================================================================
 * Struct field access — insight
 * =================================================================== */

void test_insight_struct_zero_init(void) {
    br_insight_t i;
    memset(&i, 0, sizeof(i));
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_CONVERGENCE, i.type);
    TEST_ASSERT_EQUAL_INT(0, i.system_count);
    TEST_ASSERT_EQUAL_FLOAT(0.0, i.score);
}

void test_insight_headline_fits(void) {
    br_insight_t i;
    memset(&i, 0, sizeof(i));
    const char *text = "Seed Day: Tzolkin Kin 144 meets Ramadan night 18";
    strncpy(i.headline, text, BR_HEADLINE_MAX - 1);
    i.headline[BR_HEADLINE_MAX - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING(text, i.headline);
}

void test_insight_systems_array(void) {
    br_insight_t i;
    memset(&i, 0, sizeof(i));
    i.systems[0] = 2;
    i.systems[1] = 5;
    i.system_count = 2;
    TEST_ASSERT_EQUAL_INT(2, i.systems[0]);
    TEST_ASSERT_EQUAL_INT(5, i.systems[1]);
    TEST_ASSERT_EQUAL_INT(2, i.system_count);
}

/* ===================================================================
 * Struct field access — result
 * =================================================================== */

void test_result_struct_zero_init(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    TEST_ASSERT_EQUAL_INT(0, r.correlation_count);
    TEST_ASSERT_EQUAL_INT(0, r.insight_count);
    TEST_ASSERT_EQUAL_INT(0, r.top_insight_index);
    TEST_ASSERT_EQUAL_FLOAT(0.0, r.convergence_strength);
    TEST_ASSERT_EQUAL_FLOAT(0.0, r.jd);
}

void test_result_holds_correlations(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.correlations[0].system_a = 1;
    r.correlations[0].system_b = 2;
    r.correlations[0].alignment_score = 0.9;
    r.correlations[0].type = BR_CORR_STRUCTURAL;
    r.correlations[0].confidence = 1.0;
    r.correlation_count = 1;
    TEST_ASSERT_EQUAL_INT(1, r.correlation_count);
    TEST_ASSERT_EQUAL_INT(1, r.correlations[0].system_a);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.9, r.correlations[0].alignment_score);
}

void test_result_holds_insights(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.insights[0].type = BR_INSIGHT_FESTIVAL;
    r.insights[0].score = 0.75;
    strncpy(r.insights[0].headline, "Test headline", BR_HEADLINE_MAX - 1);
    r.insight_count = 1;
    r.top_insight_index = 0;
    TEST_ASSERT_EQUAL_INT(1, r.insight_count);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_FESTIVAL, r.insights[0].type);
    TEST_ASSERT_EQUAL_STRING("Test headline", r.insights[0].headline);
}

/* ===================================================================
 * Struct field access — daily narrative
 * =================================================================== */

void test_narrative_struct_zero_init(void) {
    br_narrative_t n;
    memset(&n, 0, sizeof(n));
    TEST_ASSERT_EQUAL_INT(0, n.system_count);
    TEST_ASSERT_EQUAL_INT(0, n.wisdom_id);
    TEST_ASSERT_EQUAL_FLOAT(0.0, n.confidence);
}

void test_narrative_headline_fits(void) {
    br_narrative_t n;
    memset(&n, 0, sizeof(n));
    const char *text = "Five traditions mark this as a day of new beginnings";
    strncpy(n.headline, text, BR_HEADLINE_MAX - 1);
    n.headline[BR_HEADLINE_MAX - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING(text, n.headline);
}

void test_narrative_summary_fits(void) {
    br_narrative_t n;
    memset(&n, 0, sizeof(n));
    const char *text = "Today, as Ramadan fasting continues across the Islamic world, "
                       "the Mayan Tzolkin marks Yellow Magnetic Seed — a day of new "
                       "beginnings. The Hebrew calendar counts toward Purim.";
    strncpy(n.summary, text, BR_SUMMARY_MAX - 1);
    n.summary[BR_SUMMARY_MAX - 1] = '\0';
    TEST_ASSERT_TRUE(strlen(n.summary) > 100);
}

void test_narrative_thread_type(void) {
    br_narrative_t n;
    memset(&n, 0, sizeof(n));
    strncpy(n.thread_type, "convergence", BR_THREAD_MAX - 1);
    n.thread_type[BR_THREAD_MAX - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING("convergence", n.thread_type);
}

/* ===================================================================
 * Utility functions
 * =================================================================== */

void test_correlation_type_count_fn(void) {
    TEST_ASSERT_EQUAL_INT(BR_CORR_COUNT, br_correlation_type_count());
}

void test_insight_type_count_fn(void) {
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_COUNT, br_insight_type_count());
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Correlation type enum */
    RUN_TEST(test_correlation_type_structural_is_zero);
    RUN_TEST(test_correlation_type_count);
    RUN_TEST(test_correlation_type_values_sequential);
    RUN_TEST(test_correlation_type_name_structural);
    RUN_TEST(test_correlation_type_name_harmonic);
    RUN_TEST(test_correlation_type_name_approximate);
    RUN_TEST(test_correlation_type_name_coincidental);
    RUN_TEST(test_correlation_type_name_invalid);

    /* Insight type enum */
    RUN_TEST(test_insight_type_convergence_is_zero);
    RUN_TEST(test_insight_type_count);
    RUN_TEST(test_insight_type_values_sequential);
    RUN_TEST(test_insight_type_name_convergence);
    RUN_TEST(test_insight_type_name_number);
    RUN_TEST(test_insight_type_name_cycle);
    RUN_TEST(test_insight_type_name_festival);
    RUN_TEST(test_insight_type_name_astronomical);
    RUN_TEST(test_insight_type_name_quiet);
    RUN_TEST(test_insight_type_name_invalid);

    /* Constants */
    RUN_TEST(test_max_correlations_positive);
    RUN_TEST(test_max_insights_positive);
    RUN_TEST(test_max_systems_per_insight_positive);
    RUN_TEST(test_headline_max_reasonable);
    RUN_TEST(test_detail_max_reasonable);
    RUN_TEST(test_summary_max_reasonable);

    /* Correlation struct */
    RUN_TEST(test_correlation_struct_zero_init);
    RUN_TEST(test_correlation_struct_field_assignment);

    /* Insight struct */
    RUN_TEST(test_insight_struct_zero_init);
    RUN_TEST(test_insight_headline_fits);
    RUN_TEST(test_insight_systems_array);

    /* Result struct */
    RUN_TEST(test_result_struct_zero_init);
    RUN_TEST(test_result_holds_correlations);
    RUN_TEST(test_result_holds_insights);

    /* Narrative struct */
    RUN_TEST(test_narrative_struct_zero_init);
    RUN_TEST(test_narrative_headline_fits);
    RUN_TEST(test_narrative_summary_fits);
    RUN_TEST(test_narrative_thread_type);

    /* Utility functions */
    RUN_TEST(test_correlation_type_count_fn);
    RUN_TEST(test_insight_type_count_fn);

    return UNITY_END();
}
