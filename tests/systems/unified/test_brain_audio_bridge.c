/* test_brain_audio_bridge.c — Tests for brain → audio bridge.
 *
 * Verifies conversion of brain scan results into audio event parameters:
 * consonance, convergence density, event intensity, surprise factor. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_audio_bridge.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Correlation type → consonance mapping
 * =================================================================== */

void test_consonance_structural_is_one(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f,
        br_audio_correlation_consonance(BR_CORR_STRUCTURAL));
}

void test_consonance_harmonic(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.75f,
        br_audio_correlation_consonance(BR_CORR_HARMONIC));
}

void test_consonance_approximate(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f,
        br_audio_correlation_consonance(BR_CORR_APPROXIMATE));
}

void test_consonance_coincidental(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.25f,
        br_audio_correlation_consonance(BR_CORR_COINCIDENTAL));
}

void test_consonance_invalid_returns_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_audio_correlation_consonance(BR_CORR_COUNT));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_audio_correlation_consonance(-1));
}

/* ===================================================================
 * Insight type → intensity mapping
 * =================================================================== */

void test_intensity_convergence_high(void) {
    float v = br_audio_insight_intensity(BR_INSIGHT_CONVERGENCE);
    TEST_ASSERT_TRUE(v > 0.7f);
}

void test_intensity_festival_moderate(void) {
    float v = br_audio_insight_intensity(BR_INSIGHT_FESTIVAL);
    TEST_ASSERT_TRUE(v > 0.4f);
}

void test_intensity_quiet_low(void) {
    float v = br_audio_insight_intensity(BR_INSIGHT_QUIET);
    TEST_ASSERT_TRUE(v < 0.3f);
}

void test_intensity_invalid_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_audio_insight_intensity(BR_INSIGHT_COUNT));
}

/* ===================================================================
 * Convergence density mapping
 * =================================================================== */

void test_density_zero_strength(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_audio_convergence_density(0.0));
}

void test_density_full_strength(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f,
        br_audio_convergence_density(1.0));
}

void test_density_half_strength(void) {
    float v = br_audio_convergence_density(0.5);
    TEST_ASSERT_TRUE(v > 0.0f);
    TEST_ASSERT_TRUE(v < 1.0f);
}

void test_density_clamps_negative(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_audio_convergence_density(-0.5));
}

void test_density_clamps_over_one(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f,
        br_audio_convergence_density(1.5));
}

/* ===================================================================
 * Full result → audio event conversion
 * =================================================================== */

void test_from_result_null_result_zeroes(void) {
    br_audio_event_t evt;
    memset(&evt, 0xFF, sizeof(evt));
    br_audio_from_result(NULL, &evt);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, evt.consonance);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, evt.convergence_density);
    TEST_ASSERT_EQUAL_INT(0, evt.convergence_count);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, evt.event_intensity);
}

void test_from_result_empty_result(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    br_audio_event_t evt;
    br_audio_from_result(&result, &evt);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, evt.consonance);
    TEST_ASSERT_EQUAL_INT(0, evt.convergence_count);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_QUIET, evt.dominant_insight);
}

void test_from_result_with_correlations(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.correlation_count = 2;
    result.correlations[0].type = BR_CORR_STRUCTURAL;
    result.correlations[0].alignment_score = 0.9;
    result.correlations[1].type = BR_CORR_HARMONIC;
    result.correlations[1].alignment_score = 0.7;
    result.convergence_strength = 0.8;

    br_audio_event_t evt;
    br_audio_from_result(&result, &evt);

    /* Consonance should be average of structural (1.0) and harmonic (0.75) */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.875f, evt.consonance);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.8f, evt.convergence_density);
    TEST_ASSERT_TRUE(evt.event_intensity > 0.0f);
}

void test_from_result_with_insights(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.insight_count = 3;
    result.insights[0].type = BR_INSIGHT_CONVERGENCE;
    result.insights[0].score = 0.9;
    result.insights[0].system_count = 4;
    result.insights[1].type = BR_INSIGHT_FESTIVAL;
    result.insights[1].score = 0.6;
    result.insights[1].system_count = 2;
    result.insights[2].type = BR_INSIGHT_CYCLE;
    result.insights[2].score = 0.5;
    result.insights[2].system_count = 1;
    result.top_insight_index = 0;
    result.convergence_strength = 0.7;

    br_audio_event_t evt;
    br_audio_from_result(&result, &evt);

    TEST_ASSERT_EQUAL_INT(3, evt.convergence_count);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_CONVERGENCE, evt.dominant_insight);
    TEST_ASSERT_EQUAL_INT(7, evt.system_count); /* 4+2+1 unique max */
    TEST_ASSERT_TRUE(evt.event_intensity > 0.5f);
}

void test_from_result_null_out_no_crash(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    /* Should not crash */
    br_audio_from_result(&result, NULL);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Consonance mapping */
    RUN_TEST(test_consonance_structural_is_one);
    RUN_TEST(test_consonance_harmonic);
    RUN_TEST(test_consonance_approximate);
    RUN_TEST(test_consonance_coincidental);
    RUN_TEST(test_consonance_invalid_returns_zero);

    /* Intensity mapping */
    RUN_TEST(test_intensity_convergence_high);
    RUN_TEST(test_intensity_festival_moderate);
    RUN_TEST(test_intensity_quiet_low);
    RUN_TEST(test_intensity_invalid_zero);

    /* Density mapping */
    RUN_TEST(test_density_zero_strength);
    RUN_TEST(test_density_full_strength);
    RUN_TEST(test_density_half_strength);
    RUN_TEST(test_density_clamps_negative);
    RUN_TEST(test_density_clamps_over_one);

    /* Full conversion */
    RUN_TEST(test_from_result_null_result_zeroes);
    RUN_TEST(test_from_result_empty_result);
    RUN_TEST(test_from_result_with_correlations);
    RUN_TEST(test_from_result_with_insights);
    RUN_TEST(test_from_result_null_out_no_crash);

    return UNITY_END();
}
