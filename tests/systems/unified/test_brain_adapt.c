/* test_brain_adapt.c — Tests for context-adaptive insight ranking.
 *
 * Tests br_adapt reordering, seen cache, and affinity boosting. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_adapt.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helpers
 * =================================================================== */

static br_result_t make_two_insights(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));

    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Hebrew-Islamic", BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_HEBREW;
    r.insights[0].systems[1] = CD_SYS_ISLAMIC;
    r.insights[0].system_count = 2;
    r.insights[0].score = 0.3;

    r.insights[1].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[1].headline, "Tzolkin-Chinese", BR_HEADLINE_MAX - 1);
    r.insights[1].systems[0] = CD_SYS_TZOLKIN;
    r.insights[1].systems[1] = CD_SYS_CHINESE;
    r.insights[1].system_count = 2;
    r.insights[1].score = 0.5;

    r.insight_count = 2;
    r.top_insight_index = 1;
    return r;
}

static uc_context_t make_tzolkin_user(void) {
    uc_context_t ctx = uc_default();
    /* User has high Tzolkin affinity */
    ctx.system_affinity[CD_SYS_TZOLKIN] = 0.9;
    ctx.session.dominant_system = CD_SYS_TZOLKIN;
    return ctx;
}

static uc_context_t make_hebrew_user(void) {
    uc_context_t ctx = uc_default();
    /* User has high Hebrew affinity */
    ctx.system_affinity[CD_SYS_HEBREW] = 0.8;
    ctx.session.dominant_system = CD_SYS_HEBREW;
    return ctx;
}

/* ===================================================================
 * Seen cache
 * =================================================================== */

void test_seen_init_empty(void) {
    br_seen_cache_t cache = br_seen_init();
    TEST_ASSERT_EQUAL_INT(0, cache.count);
    TEST_ASSERT_EQUAL_INT(0, cache.head);
}

void test_seen_record_increments(void) {
    br_seen_cache_t cache = br_seen_init();
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));
    ins.type = BR_INSIGHT_CONVERGENCE;
    ins.systems[0] = CD_SYS_HEBREW;
    ins.system_count = 1;

    br_seen_record(&cache, &ins);
    TEST_ASSERT_EQUAL_INT(1, cache.count);
}

void test_seen_check_finds_recorded(void) {
    br_seen_cache_t cache = br_seen_init();
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));
    ins.type = BR_INSIGHT_CONVERGENCE;
    ins.systems[0] = CD_SYS_HEBREW;
    ins.system_count = 1;

    br_seen_record(&cache, &ins);
    TEST_ASSERT_EQUAL_INT(1, br_seen_check(&cache, &ins));
}

void test_seen_check_misses_unrecorded(void) {
    br_seen_cache_t cache = br_seen_init();
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));
    ins.type = BR_INSIGHT_FESTIVAL;
    ins.systems[0] = CD_SYS_THAI;
    ins.system_count = 1;

    TEST_ASSERT_EQUAL_INT(0, br_seen_check(&cache, &ins));
}

void test_seen_cache_wraps(void) {
    br_seen_cache_t cache = br_seen_init();
    br_insight_t ins;
    memset(&ins, 0, sizeof(ins));

    /* Fill cache beyond capacity */
    for (int i = 0; i < BR_SEEN_CACHE_SIZE + 5; i++) {
        ins.type = BR_INSIGHT_CONVERGENCE;
        ins.systems[0] = i;
        ins.system_count = 1;
        br_seen_record(&cache, &ins);
    }
    TEST_ASSERT_EQUAL_INT(BR_SEEN_CACHE_SIZE, cache.count);
}

/* ===================================================================
 * Insight hash
 * =================================================================== */

void test_hash_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_insight_hash(NULL));
}

void test_hash_different_types_differ(void) {
    br_insight_t a, b;
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    a.type = BR_INSIGHT_CONVERGENCE;
    b.type = BR_INSIGHT_FESTIVAL;
    TEST_ASSERT_NOT_EQUAL(br_insight_hash(&a), br_insight_hash(&b));
}

void test_hash_same_insight_same_hash(void) {
    br_insight_t a, b;
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    a.type = BR_INSIGHT_CONVERGENCE;
    a.systems[0] = CD_SYS_HEBREW;
    a.system_count = 1;
    b.type = BR_INSIGHT_CONVERGENCE;
    b.systems[0] = CD_SYS_HEBREW;
    b.system_count = 1;
    TEST_ASSERT_EQUAL_INT(br_insight_hash(&a), br_insight_hash(&b));
}

/* ===================================================================
 * br_adapt — no context (NULL)
 * =================================================================== */

void test_adapt_null_result(void) {
    br_seen_cache_t cache = br_seen_init();
    br_adapt(NULL, NULL, &cache); /* should not crash */
}

void test_adapt_no_context_sorts_by_score(void) {
    br_result_t r = make_two_insights();
    /* Insight 0 score=0.3, insight 1 score=0.5 */
    br_adapt(&r, NULL, NULL);
    /* After sort: insight with 0.5 should be first */
    TEST_ASSERT_EQUAL_FLOAT(0.5, r.insights[0].score);
    TEST_ASSERT_EQUAL_STRING("Tzolkin-Chinese", r.insights[0].headline);
}

void test_adapt_top_index_updated(void) {
    br_result_t r = make_two_insights();
    br_adapt(&r, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(0, r.top_insight_index);
}

/* ===================================================================
 * br_adapt — with user affinity
 * =================================================================== */

void test_adapt_hebrew_user_boosts_hebrew(void) {
    br_result_t r = make_two_insights();
    uc_context_t ctx = make_hebrew_user();
    /* Insight 0: Hebrew (score 0.3 + affinity boost ~0.4)
     * Insight 1: Tzolkin (score 0.5 + no boost) */
    br_adapt(&r, &ctx, NULL);
    /* Hebrew insight should be boosted above Tzolkin */
    TEST_ASSERT_EQUAL_STRING("Hebrew-Islamic", r.insights[0].headline);
}

void test_adapt_tzolkin_user_boosts_tzolkin(void) {
    br_result_t r = make_two_insights();
    uc_context_t ctx = make_tzolkin_user();
    br_adapt(&r, &ctx, NULL);
    /* Tzolkin already had higher score + affinity boost → stays first */
    TEST_ASSERT_EQUAL_STRING("Tzolkin-Chinese", r.insights[0].headline);
}

/* ===================================================================
 * br_adapt — with seen cache
 * =================================================================== */

void test_adapt_seen_insight_penalized(void) {
    br_result_t r = make_two_insights();
    br_seen_cache_t cache = br_seen_init();

    /* Mark the Tzolkin insight as seen */
    br_seen_record(&cache, &r.insights[1]);

    /* Tzolkin has score 0.5 - 0.3 penalty = 0.2
     * Hebrew has score 0.3 — should now rank higher */
    br_adapt(&r, NULL, &cache);
    TEST_ASSERT_EQUAL_STRING("Hebrew-Islamic", r.insights[0].headline);
}

/* ===================================================================
 * Single insight — no reordering needed
 * =================================================================== */

void test_adapt_single_insight_unchanged(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.insights[0].type = BR_INSIGHT_QUIET;
    r.insights[0].score = 0.0;
    r.insight_count = 1;
    r.top_insight_index = 0;

    br_adapt(&r, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(BR_INSIGHT_QUIET, r.insights[0].type);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Seen cache */
    RUN_TEST(test_seen_init_empty);
    RUN_TEST(test_seen_record_increments);
    RUN_TEST(test_seen_check_finds_recorded);
    RUN_TEST(test_seen_check_misses_unrecorded);
    RUN_TEST(test_seen_cache_wraps);

    /* Insight hash */
    RUN_TEST(test_hash_null_returns_zero);
    RUN_TEST(test_hash_different_types_differ);
    RUN_TEST(test_hash_same_insight_same_hash);

    /* Adapt — no context */
    RUN_TEST(test_adapt_null_result);
    RUN_TEST(test_adapt_no_context_sorts_by_score);
    RUN_TEST(test_adapt_top_index_updated);

    /* Adapt — user affinity */
    RUN_TEST(test_adapt_hebrew_user_boosts_hebrew);
    RUN_TEST(test_adapt_tzolkin_user_boosts_tzolkin);

    /* Adapt — seen cache */
    RUN_TEST(test_adapt_seen_insight_penalized);

    /* Single insight */
    RUN_TEST(test_adapt_single_insight_unchanged);

    return UNITY_END();
}
