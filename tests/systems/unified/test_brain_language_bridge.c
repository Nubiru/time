/* test_brain_language_bridge.c — Tests for brain → language bridge.
 *
 * Verifies template slot generation, convergence adjectives,
 * wisdom display signals, and slot accessors. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_language_bridge.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Convergence adjective
 * =================================================================== */

void test_adj_ordinary_for_low(void) {
    TEST_ASSERT_EQUAL_STRING("ordinary", br_lang_convergence_adj(0.0));
    TEST_ASSERT_EQUAL_STRING("ordinary", br_lang_convergence_adj(0.2));
}

void test_adj_notable_at_point_three(void) {
    TEST_ASSERT_EQUAL_STRING("notable", br_lang_convergence_adj(0.3));
    TEST_ASSERT_EQUAL_STRING("notable", br_lang_convergence_adj(0.4));
}

void test_adj_significant_at_point_five(void) {
    TEST_ASSERT_EQUAL_STRING("significant", br_lang_convergence_adj(0.5));
    TEST_ASSERT_EQUAL_STRING("significant", br_lang_convergence_adj(0.6));
}

void test_adj_rare_at_point_seven(void) {
    TEST_ASSERT_EQUAL_STRING("rare", br_lang_convergence_adj(0.7));
    TEST_ASSERT_EQUAL_STRING("rare", br_lang_convergence_adj(0.8));
}

void test_adj_extraordinary_at_point_nine(void) {
    TEST_ASSERT_EQUAL_STRING("extraordinary", br_lang_convergence_adj(0.9));
    TEST_ASSERT_EQUAL_STRING("extraordinary", br_lang_convergence_adj(1.0));
}

/* ===================================================================
 * Slot names
 * =================================================================== */

void test_slot_name_headline(void) {
    TEST_ASSERT_EQUAL_STRING("headline",
        br_lang_slot_name(BR_LANG_SLOT_HEADLINE));
}

void test_slot_name_thread_type(void) {
    TEST_ASSERT_EQUAL_STRING("thread_type",
        br_lang_slot_name(BR_LANG_SLOT_THREAD_TYPE));
}

void test_slot_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", br_lang_slot_name(BR_LANG_SLOT_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", br_lang_slot_name(-1));
}

/* ===================================================================
 * Slot value accessor
 * =================================================================== */

void test_slot_value_empty_context(void) {
    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    TEST_ASSERT_EQUAL_STRING("",
        br_lang_slot_value(&ctx, BR_LANG_SLOT_HEADLINE));
}

void test_slot_value_null_context(void) {
    TEST_ASSERT_EQUAL_STRING("",
        br_lang_slot_value(NULL, BR_LANG_SLOT_HEADLINE));
}

void test_slot_value_invalid_type(void) {
    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    TEST_ASSERT_EQUAL_STRING("",
        br_lang_slot_value(&ctx, BR_LANG_SLOT_COUNT));
}

/* ===================================================================
 * Wisdom display signal
 * =================================================================== */

void test_show_wisdom_above_threshold(void) {
    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.convergence_strength = 0.8;
    TEST_ASSERT_EQUAL_INT(1, br_lang_should_show_wisdom(&ctx, 0.6));
}

void test_show_wisdom_below_threshold(void) {
    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.convergence_strength = 0.3;
    TEST_ASSERT_EQUAL_INT(0, br_lang_should_show_wisdom(&ctx, 0.6));
}

void test_show_wisdom_at_threshold(void) {
    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.convergence_strength = 0.6;
    TEST_ASSERT_EQUAL_INT(1, br_lang_should_show_wisdom(&ctx, 0.6));
}

void test_show_wisdom_null_context(void) {
    TEST_ASSERT_EQUAL_INT(0, br_lang_should_show_wisdom(NULL, 0.6));
}

/* ===================================================================
 * Full result → context conversion
 * =================================================================== */

void test_from_result_null_zeroes(void) {
    br_lang_context_t ctx;
    memset(&ctx, 0xFF, sizeof(ctx));
    br_lang_from_result(NULL, NULL, &ctx);
    TEST_ASSERT_EQUAL_INT(0, ctx.slot_count);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, ctx.convergence_strength);
}

void test_from_result_null_ctx_no_crash(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    br_lang_from_result(&result, NULL, NULL);
}

void test_from_result_fills_convergence_adj(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.convergence_strength = 0.85;

    br_lang_context_t ctx;
    br_lang_from_result(&result, NULL, &ctx);

    const char *adj = br_lang_slot_value(&ctx, BR_LANG_SLOT_CONVERGENCE_ADJ);
    TEST_ASSERT_EQUAL_STRING("rare", adj);
}

void test_from_result_fills_system_count(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.insight_count = 2;
    result.insights[0].system_count = 3;
    result.insights[1].system_count = 2;

    br_lang_context_t ctx;
    br_lang_from_result(&result, NULL, &ctx);

    const char *count = br_lang_slot_value(&ctx, BR_LANG_SLOT_SYSTEM_COUNT);
    TEST_ASSERT_EQUAL_STRING("5 systems", count);
}

void test_from_result_singular_system(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.insight_count = 1;
    result.insights[0].system_count = 1;

    br_lang_context_t ctx;
    br_lang_from_result(&result, NULL, &ctx);

    const char *count = br_lang_slot_value(&ctx, BR_LANG_SLOT_SYSTEM_COUNT);
    TEST_ASSERT_EQUAL_STRING("1 system", count);
}

void test_from_result_with_narrative(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.convergence_strength = 0.75;
    result.insight_count = 1;
    result.insights[0].type = BR_INSIGHT_CONVERGENCE;
    result.insights[0].system_count = 3;
    strncpy(result.insights[0].headline, "Test headline",
            BR_HEADLINE_MAX - 1);
    result.top_insight_index = 0;

    br_narrative_t narr;
    memset(&narr, 0, sizeof(narr));
    strncpy(narr.headline, "Narrative headline", BR_HEADLINE_MAX - 1);
    strncpy(narr.thread_type, "convergence", BR_THREAD_MAX - 1);
    strncpy(narr.summary, "A day of alignment", BR_SUMMARY_MAX - 1);
    narr.wisdom_id = 42;

    br_lang_context_t ctx;
    br_lang_from_result(&result, &narr, &ctx);

    /* Narrative headline overrides insight headline */
    TEST_ASSERT_EQUAL_STRING("Narrative headline",
        br_lang_slot_value(&ctx, BR_LANG_SLOT_HEADLINE));
    TEST_ASSERT_EQUAL_STRING("convergence",
        br_lang_slot_value(&ctx, BR_LANG_SLOT_THREAD_TYPE));
    TEST_ASSERT_EQUAL_INT(42, ctx.wisdom_id);
    TEST_ASSERT_EQUAL_INT(1, ctx.show_wisdom);
}

void test_from_result_wisdom_not_shown_for_low(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));
    result.convergence_strength = 0.3;

    br_lang_context_t ctx;
    br_lang_from_result(&result, NULL, &ctx);

    TEST_ASSERT_EQUAL_INT(0, ctx.show_wisdom);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Convergence adjective */
    RUN_TEST(test_adj_ordinary_for_low);
    RUN_TEST(test_adj_notable_at_point_three);
    RUN_TEST(test_adj_significant_at_point_five);
    RUN_TEST(test_adj_rare_at_point_seven);
    RUN_TEST(test_adj_extraordinary_at_point_nine);

    /* Slot names */
    RUN_TEST(test_slot_name_headline);
    RUN_TEST(test_slot_name_thread_type);
    RUN_TEST(test_slot_name_invalid);

    /* Slot value accessor */
    RUN_TEST(test_slot_value_empty_context);
    RUN_TEST(test_slot_value_null_context);
    RUN_TEST(test_slot_value_invalid_type);

    /* Wisdom display */
    RUN_TEST(test_show_wisdom_above_threshold);
    RUN_TEST(test_show_wisdom_below_threshold);
    RUN_TEST(test_show_wisdom_at_threshold);
    RUN_TEST(test_show_wisdom_null_context);

    /* Full conversion */
    RUN_TEST(test_from_result_null_zeroes);
    RUN_TEST(test_from_result_null_ctx_no_crash);
    RUN_TEST(test_from_result_fills_convergence_adj);
    RUN_TEST(test_from_result_fills_system_count);
    RUN_TEST(test_from_result_singular_system);
    RUN_TEST(test_from_result_with_narrative);
    RUN_TEST(test_from_result_wisdom_not_shown_for_low);

    return UNITY_END();
}
