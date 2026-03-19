/* test_brain_visual_bridge.c — Tests for brain → visual encoding bridge.
 *
 * Verifies correlation→link mapping, insight→highlight mapping,
 * scene intensity, and full frame conversion. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_visual_bridge.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Scene intensity
 * =================================================================== */

void test_scene_intensity_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_visual_scene_intensity(0.0));
}

void test_scene_intensity_one(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f,
        br_visual_scene_intensity(1.0));
}

void test_scene_intensity_clamps_negative(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
        br_visual_scene_intensity(-0.5));
}

void test_scene_intensity_clamps_over(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f,
        br_visual_scene_intensity(1.5));
}

void test_scene_intensity_half(void) {
    float v = br_visual_scene_intensity(0.5);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, v);
}

/* ===================================================================
 * Correlation → link
 * =================================================================== */

void test_link_null_correlation_zeroed(void) {
    br_visual_link_t link = br_visual_link_from_correlation(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, link.line_opacity);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, link.glow_strength);
}

void test_link_structural_full_glow(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    c.type = BR_CORR_STRUCTURAL;
    c.alignment_score = 0.9;
    c.confidence = 1.0;
    c.system_a = 0;
    c.system_b = 1;

    br_visual_link_t link = br_visual_link_from_correlation(&c);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, link.glow_strength);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.9f, link.line_opacity);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 4.0f, link.line_thickness); /* 1+3*1.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, link.color_warmth);
}

void test_link_coincidental_dim_glow(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    c.type = BR_CORR_COINCIDENTAL;
    c.alignment_score = 0.3;
    c.confidence = 0.2;

    br_visual_link_t link = br_visual_link_from_correlation(&c);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, link.glow_strength);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.3f, link.line_opacity);
}

void test_link_preserves_system_ids(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    c.system_a = 5;
    c.system_b = 12;

    br_visual_link_t link = br_visual_link_from_correlation(&c);
    TEST_ASSERT_EQUAL_INT(5, link.system_a);
    TEST_ASSERT_EQUAL_INT(12, link.system_b);
}

/* ===================================================================
 * Insight → visual highlight
 * =================================================================== */

void test_insight_null_zeroed(void) {
    br_visual_insight_t vis = br_visual_insight_from_brain(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vis.pulse_intensity);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vis.ring_glow);
}

void test_insight_convergence_high_glow(void) {
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));
    ins.type = BR_INSIGHT_CONVERGENCE;
    ins.score = 0.95;
    ins.systems[0] = 3;
    ins.system_count = 1;

    br_visual_insight_t vis = br_visual_insight_from_brain(&ins);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.9f, vis.ring_glow);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.95f, vis.pulse_intensity);
    TEST_ASSERT_EQUAL_INT(3, vis.highlight_system);
    TEST_ASSERT_TRUE(vis.background_shift > 0.0f);
}

void test_insight_quiet_minimal_glow(void) {
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));
    ins.type = BR_INSIGHT_QUIET;
    ins.score = 0.1;

    br_visual_insight_t vis = br_visual_insight_from_brain(&ins);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, vis.ring_glow);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vis.background_shift);
}

void test_insight_no_systems_highlight_minus_one(void) {
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));
    ins.type = BR_INSIGHT_NUMBER;
    ins.system_count = 0;

    br_visual_insight_t vis = br_visual_insight_from_brain(&ins);
    TEST_ASSERT_EQUAL_INT(-1, vis.highlight_system);
}

/* ===================================================================
 * Full frame conversion
 * =================================================================== */

void test_frame_null_result_zeroed(void) {
    br_visual_frame_t frame;
    memset(&frame, 0xFF, sizeof(frame));
    br_visual_from_result(NULL, &frame);
    TEST_ASSERT_EQUAL_INT(0, frame.link_count);
    TEST_ASSERT_EQUAL_INT(0, frame.insight_count);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, frame.overall_intensity);
}

void test_frame_null_frame_no_crash(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    br_visual_from_result(&result, NULL);
}

void test_frame_with_correlations_and_insights(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.correlation_count = 2;
    result.correlations[0].type = BR_CORR_STRUCTURAL;
    result.correlations[0].alignment_score = 0.9;
    result.correlations[0].confidence = 1.0;
    result.correlations[1].type = BR_CORR_HARMONIC;
    result.correlations[1].alignment_score = 0.6;
    result.correlations[1].confidence = 0.8;

    result.insight_count = 1;
    result.insights[0].type = BR_INSIGHT_CONVERGENCE;
    result.insights[0].score = 0.85;
    result.insights[0].systems[0] = 2;
    result.insights[0].system_count = 1;

    result.convergence_strength = 0.75;

    br_visual_frame_t frame;
    br_visual_from_result(&result, &frame);

    TEST_ASSERT_EQUAL_INT(2, frame.link_count);
    TEST_ASSERT_EQUAL_INT(1, frame.insight_count);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.75f, frame.overall_intensity);
    TEST_ASSERT_TRUE(frame.atmosphere_warmth > 0.0f);
}

void test_frame_empty_result(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));

    br_visual_frame_t frame;
    br_visual_from_result(&result, &frame);

    TEST_ASSERT_EQUAL_INT(0, frame.link_count);
    TEST_ASSERT_EQUAL_INT(0, frame.insight_count);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, frame.overall_intensity);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, frame.atmosphere_warmth);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Scene intensity */
    RUN_TEST(test_scene_intensity_zero);
    RUN_TEST(test_scene_intensity_one);
    RUN_TEST(test_scene_intensity_clamps_negative);
    RUN_TEST(test_scene_intensity_clamps_over);
    RUN_TEST(test_scene_intensity_half);

    /* Correlation → link */
    RUN_TEST(test_link_null_correlation_zeroed);
    RUN_TEST(test_link_structural_full_glow);
    RUN_TEST(test_link_coincidental_dim_glow);
    RUN_TEST(test_link_preserves_system_ids);

    /* Insight → highlight */
    RUN_TEST(test_insight_null_zeroed);
    RUN_TEST(test_insight_convergence_high_glow);
    RUN_TEST(test_insight_quiet_minimal_glow);
    RUN_TEST(test_insight_no_systems_highlight_minus_one);

    /* Full frame */
    RUN_TEST(test_frame_null_result_zeroed);
    RUN_TEST(test_frame_null_frame_no_crash);
    RUN_TEST(test_frame_with_correlations_and_insights);
    RUN_TEST(test_frame_empty_result);

    return UNITY_END();
}
