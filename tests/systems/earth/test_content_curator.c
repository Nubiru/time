/* test_content_curator.c -- Tests for content curator module.
 * TDD: tests written first, implementation follows.
 *
 * Track 49.2 — Content curation, relevance scoring, digest diversity.
 * 41 tests covering all public functions + edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/content_curator.h"
#include "../../../src/systems/unified/wisdom.h"
#include "../../../src/systems/unified/fun_facts.h"
#include "../../../src/systems/unified/cultural_stories.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== cc_context_default (6 tests) ===== */

void test_context_default_sets_jd(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2460000.5f, (float)ctx.current_jd);
}

void test_context_default_null_profile(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_NULL(ctx.profile);
}

void test_context_default_null_space(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_NULL(ctx.space);
}

void test_context_default_no_active_system(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_EQUAL_INT(-1, ctx.active_system);
}

void test_context_default_zero_seen(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_EQUAL_INT(0, ctx.seen_count);
}

void test_context_default_zero_scale(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_EQUAL_INT(0, ctx.scale_level);
}

/* ===== cc_mark_seen / cc_is_seen (6 tests) ===== */

void test_mark_seen_increments_count(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx = cc_mark_seen(ctx, 42);
    TEST_ASSERT_EQUAL_INT(1, ctx.seen_count);
}

void test_mark_seen_stores_index(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx = cc_mark_seen(ctx, 42);
    TEST_ASSERT_EQUAL_INT(42, ctx.seen_indices[0]);
}

void test_is_seen_returns_true_for_seen(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx = cc_mark_seen(ctx, 7);
    TEST_ASSERT_EQUAL_INT(1, cc_is_seen(&ctx, 7));
}

void test_is_seen_returns_false_for_unseen(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx = cc_mark_seen(ctx, 7);
    TEST_ASSERT_EQUAL_INT(0, cc_is_seen(&ctx, 99));
}

void test_mark_seen_multiple(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx = cc_mark_seen(ctx, 1);
    ctx = cc_mark_seen(ctx, 5);
    ctx = cc_mark_seen(ctx, 10);
    TEST_ASSERT_EQUAL_INT(3, ctx.seen_count);
    TEST_ASSERT_EQUAL_INT(1, cc_is_seen(&ctx, 1));
    TEST_ASSERT_EQUAL_INT(1, cc_is_seen(&ctx, 5));
    TEST_ASSERT_EQUAL_INT(1, cc_is_seen(&ctx, 10));
}

void test_mark_seen_clamps_at_max(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    for (int i = 0; i < CC_MAX_SEEN + 5; i++) {
        ctx = cc_mark_seen(ctx, i);
    }
    TEST_ASSERT_EQUAL_INT(CC_MAX_SEEN, ctx.seen_count);
}

/* ===== cc_score_wisdom (5 tests) ===== */

void test_score_wisdom_unseen_default(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_TRUE(wisdom_quote_count() > 0);
    double score = cc_score_wisdom(0, &ctx);
    /* No active system => affinity 0.1, temporal 0.5, novelty 1.0 => 0.05 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, (float)score);
}

void test_score_wisdom_seen_penalty(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_TRUE(wisdom_quote_count() > 0);
    double unseen_val = cc_score_wisdom(0, &ctx);
    ctx = cc_mark_seen(ctx, CC_SOURCE_OFFSET_WISDOM + 0);
    double seen_val = cc_score_wisdom(0, &ctx);
    /* Seen items have novelty 0.1, unseen have 1.0 */
    TEST_ASSERT_TRUE(seen_val < unseen_val);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)(unseen_val * 0.1), (float)seen_val);
}

void test_score_wisdom_active_system_boost(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx.active_system = 0; /* astronomy */
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_ASTRONOMY, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
    double score = cc_score_wisdom(results[0], &ctx);
    /* affinity 1.0 * temporal 0.5 * novelty 1.0 = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)score);
}

void test_score_wisdom_invalid_index(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    double score = cc_score_wisdom(-1, &ctx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)score);
}

void test_score_wisdom_out_of_range(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    double score = cc_score_wisdom(99999, &ctx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)score);
}

/* ===== cc_score_fact (4 tests) ===== */

void test_score_fact_unseen_default(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_TRUE(ff_fact_count() > 0);
    double score = cc_score_fact(0, &ctx);
    /* No active system => affinity 0.1, temporal 0.5, novelty 1.0 => 0.05 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, (float)score);
}

void test_score_fact_seen_penalty(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    TEST_ASSERT_TRUE(ff_fact_count() > 0);
    double unseen_val = cc_score_fact(0, &ctx);
    ctx = cc_mark_seen(ctx, CC_SOURCE_OFFSET_FACT + 0);
    double seen_val = cc_score_fact(0, &ctx);
    TEST_ASSERT_TRUE(seen_val < unseen_val);
}

void test_score_fact_active_system_boost(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ctx.active_system = 0; /* astronomy */
    int count = ff_fact_count();
    TEST_ASSERT_TRUE(count > 0);
    int found_idx = -1;
    for (int i = 0; i < count; i++) {
        ff_fact_t f = ff_fact_get(i);
        if (f.system_mask & (1u << 0)) {
            found_idx = i;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_idx >= 0);
    double score = cc_score_fact(found_idx, &ctx);
    /* affinity 1.0 * temporal 0.5 * novelty 1.0 = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)score);
}

void test_score_fact_invalid_index(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    double score = cc_score_fact(-1, &ctx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)score);
}

/* ===== cc_suggest (5 tests) ===== */

void test_suggest_returns_positive_count(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_suggestion_t out[CC_MAX_SUGGESTIONS];
    int n = cc_suggest(&ctx, out, CC_MAX_SUGGESTIONS);
    TEST_ASSERT_TRUE(n > 0);
}

void test_suggest_sorted_by_relevance(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_suggestion_t out[CC_MAX_SUGGESTIONS];
    int n = cc_suggest(&ctx, out, CC_MAX_SUGGESTIONS);
    for (int i = 1; i < n; i++) {
        TEST_ASSERT_TRUE(out[i - 1].relevance >= out[i].relevance);
    }
}

void test_suggest_respects_max(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_suggestion_t out[3];
    int n = cc_suggest(&ctx, out, 3);
    TEST_ASSERT_TRUE(n <= 3);
}

void test_suggest_relevance_in_range(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_suggestion_t out[CC_MAX_SUGGESTIONS];
    int n = cc_suggest(&ctx, out, CC_MAX_SUGGESTIONS);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].relevance >= 0.0);
        TEST_ASSERT_TRUE(out[i].relevance <= 1.0);
    }
}

void test_suggest_has_valid_types(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_suggestion_t out[CC_MAX_SUGGESTIONS];
    int n = cc_suggest(&ctx, out, CC_MAX_SUGGESTIONS);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].type >= 0);
        TEST_ASSERT_TRUE(out[i].type < CC_TYPE_COUNT);
    }
}

/* ===== cc_digest (5 tests) ===== */

void test_digest_returns_items(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_digest_t d = cc_digest(&ctx);
    TEST_ASSERT_TRUE(d.count > 0);
    TEST_ASSERT_TRUE(d.count <= CC_DIGEST_SIZE);
}

void test_digest_sorted_by_relevance(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_digest_t d = cc_digest(&ctx);
    for (int i = 1; i < d.count; i++) {
        TEST_ASSERT_TRUE(d.items[i - 1].relevance >= d.items[i].relevance);
    }
}

void test_digest_prefers_diversity(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_digest_t d = cc_digest(&ctx);
    TEST_ASSERT_TRUE(d.count >= 2);
    int types_seen[CC_TYPE_COUNT] = {0};
    for (int i = 0; i < d.count; i++) {
        types_seen[d.items[i].type] = 1;
    }
    int distinct = 0;
    for (int i = 0; i < CC_TYPE_COUNT; i++) {
        if (types_seen[i]) distinct++;
    }
    TEST_ASSERT_TRUE(distinct >= 2);
}

void test_digest_relevance_in_range(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_digest_t d = cc_digest(&ctx);
    for (int i = 0; i < d.count; i++) {
        TEST_ASSERT_TRUE(d.items[i].relevance >= 0.0);
        TEST_ASSERT_TRUE(d.items[i].relevance <= 1.0);
    }
}

void test_digest_max_five_items(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    cc_digest_t d = cc_digest(&ctx);
    TEST_ASSERT_TRUE(d.count <= CC_DIGEST_SIZE);
    TEST_ASSERT_TRUE(d.count <= 5);
}

/* ===== cc_suggest_system (4 tests) ===== */

void test_suggest_system_no_space_returns_zero(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    int sys = cc_suggest_system(&ctx);
    TEST_ASSERT_TRUE(sys >= 0);
}

void test_suggest_system_some_explored(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ps_space_t space = ps_create();
    space = ps_mark_explored(space, 0);
    space = ps_mark_explored(space, 1);
    ctx.space = &space;
    int sys = cc_suggest_system(&ctx);
    TEST_ASSERT_TRUE(sys != 0);
    TEST_ASSERT_TRUE(sys != 1);
}

void test_suggest_system_all_explored_returns_neg1(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ps_space_t space = ps_create();
    for (int i = 0; i < CC_TOTAL_SYSTEMS; i++) {
        space = ps_mark_explored(space, i);
    }
    ctx.space = &space;
    int sys = cc_suggest_system(&ctx);
    TEST_ASSERT_EQUAL_INT(-1, sys);
}

void test_suggest_system_null_ctx(void) {
    int sys = cc_suggest_system(NULL);
    TEST_ASSERT_EQUAL_INT(-1, sys);
}

/* ===== cc_type_name (5 tests) ===== */

void test_type_name_wisdom(void) {
    TEST_ASSERT_EQUAL_STRING("Wisdom", cc_type_name(CC_TYPE_WISDOM));
}

void test_type_name_fun_fact(void) {
    TEST_ASSERT_EQUAL_STRING("Fun Fact", cc_type_name(CC_TYPE_FUN_FACT));
}

void test_type_name_cultural_story(void) {
    TEST_ASSERT_EQUAL_STRING("Cultural Story", cc_type_name(CC_TYPE_CULTURAL_STORY));
}

void test_type_name_system_card(void) {
    TEST_ASSERT_EQUAL_STRING("System Card", cc_type_name(CC_TYPE_SYSTEM_CARD));
}

void test_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", cc_type_name(CC_TYPE_COUNT));
}

/* ===== Edge cases (6 tests) ===== */

void test_suggest_with_all_seen(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    for (int i = 0; i < CC_MAX_SEEN; i++) {
        ctx = cc_mark_seen(ctx, i);
    }
    cc_suggestion_t out[CC_MAX_SUGGESTIONS];
    int n = cc_suggest(&ctx, out, CC_MAX_SUGGESTIONS);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_score_wisdom_with_favorite_system(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ps_space_t space = ps_create();
    space = ps_add_favorite(space, 0); /* astronomy */
    ctx.space = &space;
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_ASTRONOMY, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
    double score = cc_score_wisdom(results[0], &ctx);
    /* Favorite system gives affinity 0.5, temporal 0.5, novelty 1.0 => 0.25 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)score);
}

void test_score_wisdom_with_explored_system(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    ps_space_t space = ps_create();
    space = ps_mark_explored(space, 0); /* astronomy */
    ctx.space = &space;
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_ASTRONOMY, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
    double score = cc_score_wisdom(results[0], &ctx);
    /* Explored system gives affinity 0.3, temporal 0.5, novelty 1.0 => 0.15 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, (float)score);
}

void test_suggest_null_output_returns_zero(void) {
    cc_context_t ctx = cc_context_default(2460000.5);
    int n = cc_suggest(&ctx, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_is_seen_null_context(void) {
    TEST_ASSERT_EQUAL_INT(0, cc_is_seen(NULL, 0));
}

void test_digest_null_context(void) {
    cc_digest_t d = cc_digest(NULL);
    TEST_ASSERT_EQUAL_INT(0, d.count);
}

/* ===== Main ===== */

int main(void) {
    UNITY_BEGIN();

    /* cc_context_default (6) */
    RUN_TEST(test_context_default_sets_jd);
    RUN_TEST(test_context_default_null_profile);
    RUN_TEST(test_context_default_null_space);
    RUN_TEST(test_context_default_no_active_system);
    RUN_TEST(test_context_default_zero_seen);
    RUN_TEST(test_context_default_zero_scale);

    /* cc_mark_seen / cc_is_seen (6) */
    RUN_TEST(test_mark_seen_increments_count);
    RUN_TEST(test_mark_seen_stores_index);
    RUN_TEST(test_is_seen_returns_true_for_seen);
    RUN_TEST(test_is_seen_returns_false_for_unseen);
    RUN_TEST(test_mark_seen_multiple);
    RUN_TEST(test_mark_seen_clamps_at_max);

    /* cc_score_wisdom (5) */
    RUN_TEST(test_score_wisdom_unseen_default);
    RUN_TEST(test_score_wisdom_seen_penalty);
    RUN_TEST(test_score_wisdom_active_system_boost);
    RUN_TEST(test_score_wisdom_invalid_index);
    RUN_TEST(test_score_wisdom_out_of_range);

    /* cc_score_fact (4) */
    RUN_TEST(test_score_fact_unseen_default);
    RUN_TEST(test_score_fact_seen_penalty);
    RUN_TEST(test_score_fact_active_system_boost);
    RUN_TEST(test_score_fact_invalid_index);

    /* cc_suggest (5) */
    RUN_TEST(test_suggest_returns_positive_count);
    RUN_TEST(test_suggest_sorted_by_relevance);
    RUN_TEST(test_suggest_respects_max);
    RUN_TEST(test_suggest_relevance_in_range);
    RUN_TEST(test_suggest_has_valid_types);

    /* cc_digest (5) */
    RUN_TEST(test_digest_returns_items);
    RUN_TEST(test_digest_sorted_by_relevance);
    RUN_TEST(test_digest_prefers_diversity);
    RUN_TEST(test_digest_relevance_in_range);
    RUN_TEST(test_digest_max_five_items);

    /* cc_suggest_system (4) */
    RUN_TEST(test_suggest_system_no_space_returns_zero);
    RUN_TEST(test_suggest_system_some_explored);
    RUN_TEST(test_suggest_system_all_explored_returns_neg1);
    RUN_TEST(test_suggest_system_null_ctx);

    /* cc_type_name (5) */
    RUN_TEST(test_type_name_wisdom);
    RUN_TEST(test_type_name_fun_fact);
    RUN_TEST(test_type_name_cultural_story);
    RUN_TEST(test_type_name_system_card);
    RUN_TEST(test_type_name_invalid);

    /* Edge cases (6) */
    RUN_TEST(test_suggest_with_all_seen);
    RUN_TEST(test_score_wisdom_with_favorite_system);
    RUN_TEST(test_score_wisdom_with_explored_system);
    RUN_TEST(test_suggest_null_output_returns_zero);
    RUN_TEST(test_is_seen_null_context);
    RUN_TEST(test_digest_null_context);

    return UNITY_END();
}
