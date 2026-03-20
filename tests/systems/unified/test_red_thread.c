/* test_red_thread.c — Tests for convergence narrative intelligence.
 *
 * Validates rt_compose, rt_compose_insight, rt_classify with
 * constructed br_result_t inputs. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/red_thread.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helpers: construct br_result_t for testing
 * =================================================================== */

static br_result_t make_empty_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insight_count = 0;
    r.top_insight_index = 0;
    r.convergence_strength = 0.0;
    return r;
}

static br_result_t make_single_system_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Solo system",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_ASTRONOMY;
    r.insights[0].system_count = 1;
    r.insights[0].score = 0.1;
    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.1;
    return r;
}

static br_result_t make_two_system_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Hebrew-Islamic new month",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_HEBREW;
    r.insights[0].systems[1] = CD_SYS_ISLAMIC;
    r.insights[0].system_count = 2;
    r.insights[0].score = 0.5;
    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.5;
    return r;
}

static br_result_t make_three_system_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Triple alignment",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_ASTRONOMY;
    r.insights[0].systems[1] = CD_SYS_PERSIAN;
    r.insights[0].systems[2] = CD_SYS_BAHAI;
    r.insights[0].system_count = 3;
    r.insights[0].score = 0.7;
    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.7;
    return r;
}

static br_result_t make_transmission_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "East Asian alignment",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_CHINESE;
    r.insights[0].systems[1] = CD_SYS_KOREAN;
    r.insights[0].system_count = 2;
    r.insights[0].score = 0.4;
    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.4;
    return r;
}

static br_result_t make_convergent_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Independent alignment",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_TZOLKIN;
    r.insights[0].systems[1] = CD_SYS_HEBREW;
    r.insights[0].system_count = 2;
    r.insights[0].score = 0.3;
    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.3;
    return r;
}

static br_result_t make_multi_insight_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;

    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Primary alignment",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_HEBREW;
    r.insights[0].systems[1] = CD_SYS_ISLAMIC;
    r.insights[0].systems[2] = CD_SYS_BUDDHIST;
    r.insights[0].system_count = 3;
    r.insights[0].score = 0.8;

    r.insights[1].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[1].headline, "Secondary alignment",
            BR_HEADLINE_MAX - 1);
    r.insights[1].systems[0] = CD_SYS_CHINESE;
    r.insights[1].systems[1] = CD_SYS_KOREAN;
    r.insights[1].system_count = 2;
    r.insights[1].score = 0.4;

    r.insight_count = 2;
    r.top_insight_index = 0;
    r.convergence_strength = 0.8;
    return r;
}

/* ===================================================================
 * Null safety
 * =================================================================== */

void test_compose_null_result_returns_zero(void) {
    rt_result_t out;
    TEST_ASSERT_EQUAL_INT(0, rt_compose(NULL, &out));
}

void test_compose_null_out_returns_zero(void) {
    br_result_t r = make_two_system_result();
    TEST_ASSERT_EQUAL_INT(0, rt_compose(&r, NULL));
}

void test_compose_both_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, rt_compose(NULL, NULL));
}

void test_compose_insight_null_result_returns_zero(void) {
    rt_result_t out;
    TEST_ASSERT_EQUAL_INT(0, rt_compose_insight(NULL, 0, &out));
}

void test_compose_insight_null_out_returns_zero(void) {
    br_result_t r = make_two_system_result();
    TEST_ASSERT_EQUAL_INT(0, rt_compose_insight(&r, 0, NULL));
}

void test_classify_null_returns_quiet(void) {
    TEST_ASSERT_EQUAL_STRING("quiet", rt_classify(NULL));
}

/* ===================================================================
 * Empty / quiet days
 * =================================================================== */

void test_compose_empty_result_produces_quiet(void) {
    br_result_t r = make_empty_result();
    rt_result_t out;
    int len = rt_compose(&r, &out);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "quiet"));
    TEST_ASSERT_EQUAL_INT(len, out.narrative_len);
}

void test_classify_empty_returns_quiet(void) {
    br_result_t r = make_empty_result();
    TEST_ASSERT_EQUAL_STRING("quiet", rt_classify(&r));
}

void test_compose_single_system_produces_quiet(void) {
    br_result_t r = make_single_system_result();
    rt_result_t out;
    int len = rt_compose(&r, &out);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "quiet"));
}

/* ===================================================================
 * Two-system convergence
 * =================================================================== */

void test_two_systems_contains_names(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "Hebrew"));
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "Islamic"));
}

void test_two_systems_contains_converge(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "converge"));
}

void test_two_systems_uses_and_not_comma(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* Two systems: "Hebrew and Islamic" not "Hebrew, and Islamic" */
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, " and "));
    TEST_ASSERT_NULL(strstr(out.narrative, ", and "));
}

void test_two_systems_narrative_len_positive(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    int len = rt_compose(&r, &out);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_EQUAL_INT(len, out.narrative_len);
}

void test_two_known_systems_has_explanation(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* Hebrew-Islamic is a known pair, so explanation should be included */
    TEST_ASSERT_EQUAL_INT(1, out.pairs_explained);
    TEST_ASSERT_EQUAL_INT(1, out.pairs_total);
}

void test_two_known_systems_confidence_one(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* 1 explained / 1 total = 1.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)out.confidence);
}

/* ===================================================================
 * Three-system convergence
 * =================================================================== */

void test_three_systems_uses_oxford_comma(void) {
    br_result_t r = make_three_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* Three systems: "Astronomy, Persian, and Bahai" */
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, ", and "));
}

void test_three_systems_pairs_total(void) {
    br_result_t r = make_three_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* 3 systems => C(3,2) = 3 pairs */
    TEST_ASSERT_EQUAL_INT(3, out.pairs_total);
}

void test_three_systems_contains_brief(void) {
    br_result_t r = make_three_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* Astronomy-Persian is a known pair with brief text */
    TEST_ASSERT_GREATER_THAN(0, out.pairs_explained);
    /* The narrative should contain the brief from the first found pair */
    TEST_ASSERT_GREATER_THAN(50, (int)strlen(out.narrative));
}

/* ===================================================================
 * rt_classify
 * =================================================================== */

void test_classify_shared_observation(void) {
    /* Hebrew-Islamic is BR_EXPLAIN_SHARED_SOURCE */
    br_result_t r = make_two_system_result();
    TEST_ASSERT_EQUAL_STRING("shared observation", rt_classify(&r));
}

void test_classify_cultural_transmission(void) {
    br_result_t r = make_transmission_result();
    TEST_ASSERT_EQUAL_STRING("cultural transmission", rt_classify(&r));
}

void test_classify_independent_discovery(void) {
    br_result_t r = make_convergent_result();
    TEST_ASSERT_EQUAL_STRING("independent discovery", rt_classify(&r));
}

void test_classify_quiet_for_single_system(void) {
    br_result_t r = make_single_system_result();
    /* Single system => 0 pairs => quiet */
    TEST_ASSERT_EQUAL_STRING("quiet", rt_classify(&r));
}

/* ===================================================================
 * rt_compose_insight — specific insight index
 * =================================================================== */

void test_compose_insight_invalid_index_returns_zero(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    TEST_ASSERT_EQUAL_INT(0, rt_compose_insight(&r, -1, &out));
    TEST_ASSERT_EQUAL_INT(0, rt_compose_insight(&r, 5, &out));
}

void test_compose_insight_secondary(void) {
    br_result_t r = make_multi_insight_result();
    rt_result_t out;
    int len = rt_compose_insight(&r, 1, &out);
    TEST_ASSERT_GREATER_THAN(0, len);
    /* Secondary insight has Chinese + Korean */
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "Chinese"));
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "Korean"));
}

void test_compose_insight_zeroes_output(void) {
    rt_result_t out;
    memset(&out, 0xFF, sizeof(out)); /* fill with junk */
    br_result_t r = make_two_system_result();
    rt_compose_insight(&r, 0, &out);
    /* Should be cleanly filled, not junk */
    TEST_ASSERT_TRUE(strlen(out.narrative) < RT_NARRATIVE_MAX);
    TEST_ASSERT_TRUE(out.narrative_len >= 0);
}

/* ===================================================================
 * Narrative content quality
 * =================================================================== */

void test_narrative_contains_classification(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* Should contain "This is shared observation." */
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "This is"));
}

void test_narrative_starts_with_today(void) {
    br_result_t r = make_two_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    TEST_ASSERT_EQUAL_INT(0, strncmp(out.narrative, "Today, ", 7));
}

void test_narrative_null_terminated(void) {
    br_result_t r = make_three_system_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* Verify the narrative is properly null-terminated */
    TEST_ASSERT_EQUAL_INT((int)strlen(out.narrative), out.narrative_len);
}

void test_compose_uses_top_insight(void) {
    br_result_t r = make_multi_insight_result();
    rt_result_t out;
    rt_compose(&r, &out);
    /* top_insight_index is 0 (Hebrew-Islamic-Buddhist) */
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "Hebrew"));
    TEST_ASSERT_NOT_NULL(strstr(out.narrative, "Islamic"));
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Null safety */
    RUN_TEST(test_compose_null_result_returns_zero);
    RUN_TEST(test_compose_null_out_returns_zero);
    RUN_TEST(test_compose_both_null_returns_zero);
    RUN_TEST(test_compose_insight_null_result_returns_zero);
    RUN_TEST(test_compose_insight_null_out_returns_zero);
    RUN_TEST(test_classify_null_returns_quiet);

    /* Empty / quiet */
    RUN_TEST(test_compose_empty_result_produces_quiet);
    RUN_TEST(test_classify_empty_returns_quiet);
    RUN_TEST(test_compose_single_system_produces_quiet);

    /* Two-system convergence */
    RUN_TEST(test_two_systems_contains_names);
    RUN_TEST(test_two_systems_contains_converge);
    RUN_TEST(test_two_systems_uses_and_not_comma);
    RUN_TEST(test_two_systems_narrative_len_positive);
    RUN_TEST(test_two_known_systems_has_explanation);
    RUN_TEST(test_two_known_systems_confidence_one);

    /* Three-system convergence */
    RUN_TEST(test_three_systems_uses_oxford_comma);
    RUN_TEST(test_three_systems_pairs_total);
    RUN_TEST(test_three_systems_contains_brief);

    /* Classification */
    RUN_TEST(test_classify_shared_observation);
    RUN_TEST(test_classify_cultural_transmission);
    RUN_TEST(test_classify_independent_discovery);
    RUN_TEST(test_classify_quiet_for_single_system);

    /* Specific insight index */
    RUN_TEST(test_compose_insight_invalid_index_returns_zero);
    RUN_TEST(test_compose_insight_secondary);
    RUN_TEST(test_compose_insight_zeroes_output);

    /* Narrative content quality */
    RUN_TEST(test_narrative_contains_classification);
    RUN_TEST(test_narrative_starts_with_today);
    RUN_TEST(test_narrative_null_terminated);
    RUN_TEST(test_compose_uses_top_insight);

    return UNITY_END();
}
