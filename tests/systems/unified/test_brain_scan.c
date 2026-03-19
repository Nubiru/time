/* test_brain_scan.c — Tests for brain scanner module.
 *
 * Tests br_process() with constructed cd_result_t inputs (pure transform)
 * and br_scan() with real Julian Days (integration). */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_scan.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helpers: construct cd_result_t for testing
 * =================================================================== */

static cd_result_t make_empty(void) {
    cd_result_t cd;
    memset(&cd, 0, sizeof(cd));
    return cd;
}

static cd_result_t make_single_event(void) {
    cd_result_t cd;
    memset(&cd, 0, sizeof(cd));

    cd.count = 1;
    cd.events[0].systems[0] = CD_SYS_TZOLKIN;
    cd.events[0].systems[1] = CD_SYS_HEBREW;
    cd.events[0].system_count = 2;
    cd.events[0].strength = CD_STRENGTH_MINOR;
    cd.events[0].description = "Tzolkin-Hebrew minor alignment";
    cd.events[0].significance = 0.3;

    return cd;
}

static cd_result_t make_major_convergence(void) {
    cd_result_t cd;
    memset(&cd, 0, sizeof(cd));

    cd.count = 2;

    /* Event 0: 4-system major convergence */
    cd.events[0].systems[0] = CD_SYS_TZOLKIN;
    cd.events[0].systems[1] = CD_SYS_HEBREW;
    cd.events[0].systems[2] = CD_SYS_ISLAMIC;
    cd.events[0].systems[3] = CD_SYS_CHINESE;
    cd.events[0].system_count = 4;
    cd.events[0].strength = CD_STRENGTH_MAJOR;
    cd.events[0].description = "Four traditions mark new month";
    cd.events[0].significance = 0.8;

    /* Event 1: 2-system minor */
    cd.events[1].systems[0] = CD_SYS_ASTRONOMY;
    cd.events[1].systems[1] = CD_SYS_CELTIC;
    cd.events[1].system_count = 2;
    cd.events[1].strength = CD_STRENGTH_MINOR;
    cd.events[1].description = "Astronomical-Celtic alignment";
    cd.events[1].significance = 0.2;

    return cd;
}

/* ===================================================================
 * br_process — null/empty input
 * =================================================================== */

void test_process_null_cd_gives_quiet(void) {
    br_result_t result;
    br_process(NULL, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(1, result.insight_count);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_QUIET, result.insights[0].type);
    TEST_ASSERT_EQUAL_FLOAT(0.0, result.insights[0].score);
}

void test_process_null_cd_sets_jd(void) {
    br_result_t result;
    br_process(NULL, 2460000.5, &result);
    TEST_ASSERT_EQUAL_FLOAT(2460000.5, result.jd);
}

void test_process_empty_cd_gives_quiet(void) {
    cd_result_t cd = make_empty();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(1, result.insight_count);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_QUIET, result.insights[0].type);
}

void test_process_empty_cd_zero_correlations(void) {
    cd_result_t cd = make_empty();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(0, result.correlation_count);
}

void test_process_empty_cd_zero_strength(void) {
    cd_result_t cd = make_empty();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_FLOAT(0.0, result.convergence_strength);
}

/* ===================================================================
 * br_process — single event
 * =================================================================== */

void test_process_single_event_one_insight(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(1, result.insight_count);
}

void test_process_single_event_type_convergence(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_CONVERGENCE, result.insights[0].type);
}

void test_process_single_event_headline(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_STRING("Tzolkin-Hebrew minor alignment",
                             result.insights[0].headline);
}

void test_process_single_event_systems(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(2, result.insights[0].system_count);
    TEST_ASSERT_EQUAL_INT(CD_SYS_TZOLKIN, result.insights[0].systems[0]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, result.insights[0].systems[1]);
}

void test_process_single_event_score(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_FLOAT(0.3, result.insights[0].score);
}

void test_process_single_event_one_correlation(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    /* 2 systems → C(2,2) = 1 pair */
    TEST_ASSERT_EQUAL_INT(1, result.correlation_count);
    TEST_ASSERT_EQUAL_INT(CD_SYS_TZOLKIN, result.correlations[0].system_a);
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, result.correlations[0].system_b);
}

void test_process_single_event_strength(void) {
    cd_result_t cd = make_single_event();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_FLOAT(0.25, result.convergence_strength);
}

/* ===================================================================
 * br_process — major convergence (multiple events)
 * =================================================================== */

void test_process_major_two_insights(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_INT(2, result.insight_count);
}

void test_process_major_top_insight_is_strongest(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    /* Event 0 is MAJOR (0.75), event 1 is MINOR (0.25) */
    TEST_ASSERT_EQUAL_INT(0, result.top_insight_index);
}

void test_process_major_strength(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    TEST_ASSERT_EQUAL_FLOAT(0.75, result.convergence_strength);
}

void test_process_major_correlations(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    /* Event 0: 4 systems → C(4,2) = 6 pairs
     * Event 1: 2 systems → C(2,2) = 1 pair
     * Total: 7 correlations */
    TEST_ASSERT_EQUAL_INT(7, result.correlation_count);
}

void test_process_major_first_correlation_pair(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    /* First pair from 4-system event: (TZOLKIN, HEBREW) */
    TEST_ASSERT_EQUAL_INT(CD_SYS_TZOLKIN, result.correlations[0].system_a);
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, result.correlations[0].system_b);
}

/* ===================================================================
 * br_strength_from_tier
 * =================================================================== */

void test_strength_minor(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.25, br_strength_from_tier(CD_STRENGTH_MINOR));
}

void test_strength_notable(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.50, br_strength_from_tier(CD_STRENGTH_NOTABLE));
}

void test_strength_major(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.75, br_strength_from_tier(CD_STRENGTH_MAJOR));
}

void test_strength_rare(void) {
    TEST_ASSERT_EQUAL_FLOAT(1.00, br_strength_from_tier(CD_STRENGTH_RARE));
}

void test_strength_invalid(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_strength_from_tier(0));
}

/* ===================================================================
 * br_active_count
 * =================================================================== */

void test_active_count_null(void) {
    TEST_ASSERT_EQUAL_INT(0, br_active_count(NULL, 0.0));
}

void test_active_count_all(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    /* Both insights score > 0.0 */
    TEST_ASSERT_EQUAL_INT(2, br_active_count(&result, 0.1));
}

void test_active_count_threshold(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    /* Only event 0 has significance 0.8, event 1 has 0.2 */
    TEST_ASSERT_EQUAL_INT(1, br_active_count(&result, 0.5));
}

/* ===================================================================
 * br_top_insight
 * =================================================================== */

void test_top_insight_null(void) {
    TEST_ASSERT_NULL(br_top_insight(NULL));
}

void test_top_insight_empty(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    TEST_ASSERT_NULL(br_top_insight(&result));
}

void test_top_insight_returns_best(void) {
    cd_result_t cd = make_major_convergence();
    br_result_t result;
    br_process(&cd, 2460000.5, &result);
    const br_insight_t *top = br_top_insight(&result);
    TEST_ASSERT_NOT_NULL(top);
    TEST_ASSERT_EQUAL_STRING("Four traditions mark new month", top->headline);
}

/* ===================================================================
 * br_scan integration (uses real cd_scan)
 * =================================================================== */

void test_scan_sets_jd(void) {
    br_result_t result;
    double jd = 2460000.5; /* 2023-02-24 */
    br_scan(jd, &result);
    TEST_ASSERT_EQUAL_FLOAT(jd, result.jd);
}

void test_scan_has_insights(void) {
    br_result_t result;
    br_scan(2460000.5, &result);
    /* Should always have at least one insight (even QUIET) */
    TEST_ASSERT_GREATER_THAN(0, result.insight_count);
}

void test_scan_top_insight_valid(void) {
    br_result_t result;
    br_scan(2460000.5, &result);
    TEST_ASSERT_TRUE(result.top_insight_index >= 0);
    TEST_ASSERT_TRUE(result.top_insight_index < result.insight_count);
}

void test_scan_strength_in_range(void) {
    br_result_t result;
    br_scan(2460000.5, &result);
    TEST_ASSERT_TRUE(result.convergence_strength >= 0.0);
    TEST_ASSERT_TRUE(result.convergence_strength <= 1.0);
}

void test_scan_correlations_valid(void) {
    br_result_t result;
    br_scan(2460000.5, &result);
    TEST_ASSERT_TRUE(result.correlation_count >= 0);
    TEST_ASSERT_TRUE(result.correlation_count <= BR_MAX_CORRELATIONS);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* br_process — null/empty */
    RUN_TEST(test_process_null_cd_gives_quiet);
    RUN_TEST(test_process_null_cd_sets_jd);
    RUN_TEST(test_process_empty_cd_gives_quiet);
    RUN_TEST(test_process_empty_cd_zero_correlations);
    RUN_TEST(test_process_empty_cd_zero_strength);

    /* br_process — single event */
    RUN_TEST(test_process_single_event_one_insight);
    RUN_TEST(test_process_single_event_type_convergence);
    RUN_TEST(test_process_single_event_headline);
    RUN_TEST(test_process_single_event_systems);
    RUN_TEST(test_process_single_event_score);
    RUN_TEST(test_process_single_event_one_correlation);
    RUN_TEST(test_process_single_event_strength);

    /* br_process — major convergence */
    RUN_TEST(test_process_major_two_insights);
    RUN_TEST(test_process_major_top_insight_is_strongest);
    RUN_TEST(test_process_major_strength);
    RUN_TEST(test_process_major_correlations);
    RUN_TEST(test_process_major_first_correlation_pair);

    /* br_strength_from_tier */
    RUN_TEST(test_strength_minor);
    RUN_TEST(test_strength_notable);
    RUN_TEST(test_strength_major);
    RUN_TEST(test_strength_rare);
    RUN_TEST(test_strength_invalid);

    /* br_active_count */
    RUN_TEST(test_active_count_null);
    RUN_TEST(test_active_count_all);
    RUN_TEST(test_active_count_threshold);

    /* br_top_insight */
    RUN_TEST(test_top_insight_null);
    RUN_TEST(test_top_insight_empty);
    RUN_TEST(test_top_insight_returns_best);

    /* br_scan integration */
    RUN_TEST(test_scan_sets_jd);
    RUN_TEST(test_scan_has_insights);
    RUN_TEST(test_scan_top_insight_valid);
    RUN_TEST(test_scan_strength_in_range);
    RUN_TEST(test_scan_correlations_valid);

    return UNITY_END();
}
