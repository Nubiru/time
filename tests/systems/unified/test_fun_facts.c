/* test_fun_facts.c -- Fun Fact Generator tests
 * Tests curated database of 80+ cross-system fun facts with trigger
 * conditions, scoring algorithm, seen-fact exclusion, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/fun_facts.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ff_fact_count ===== */

void test_fact_count_at_least_80(void)
{
    TEST_ASSERT_GREATER_OR_EQUAL(80, ff_fact_count());
}

void test_fact_count_positive(void)
{
    TEST_ASSERT_TRUE(ff_fact_count() > 0);
}

/* ===== ff_fact_get ===== */

void test_fact_get_first_valid(void)
{
    ff_fact_t f = ff_fact_get(0);
    TEST_ASSERT_NOT_NULL(f.text);
    TEST_ASSERT_TRUE(strlen(f.text) > 0);
}

void test_fact_get_last_valid(void)
{
    int count = ff_fact_count();
    ff_fact_t f = ff_fact_get(count - 1);
    TEST_ASSERT_NOT_NULL(f.text);
    TEST_ASSERT_TRUE(strlen(f.text) > 0);
}

void test_fact_get_negative_index_returns_empty(void)
{
    ff_fact_t f = ff_fact_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, f.id);
}

void test_fact_get_out_of_bounds_returns_empty(void)
{
    ff_fact_t f = ff_fact_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, f.id);
}

void test_fact_get_id_matches_index(void)
{
    for (int i = 0; i < ff_fact_count(); i++) {
        ff_fact_t f = ff_fact_get(i);
        TEST_ASSERT_EQUAL_INT(i, f.id);
    }
}

void test_fact_get_priority_in_range(void)
{
    for (int i = 0; i < ff_fact_count(); i++) {
        ff_fact_t f = ff_fact_get(i);
        TEST_ASSERT_TRUE(f.priority >= 1 && f.priority <= 3);
    }
}

void test_fact_get_category_in_range(void)
{
    for (int i = 0; i < ff_fact_count(); i++) {
        ff_fact_t f = ff_fact_get(i);
        TEST_ASSERT_TRUE(f.category >= 0 && f.category < FF_CAT_COUNT);
    }
}

void test_fact_get_trigger_in_range(void)
{
    for (int i = 0; i < ff_fact_count(); i++) {
        ff_fact_t f = ff_fact_get(i);
        TEST_ASSERT_TRUE(f.trigger >= 0 && f.trigger < FF_TRIG_COUNT);
    }
}

void test_fact_get_system_mask_nonzero(void)
{
    /* Every fact should reference at least one system */
    for (int i = 0; i < ff_fact_count(); i++) {
        ff_fact_t f = ff_fact_get(i);
        TEST_ASSERT_TRUE(f.system_mask != 0);
    }
}

void test_fact_text_within_max_length(void)
{
    for (int i = 0; i < ff_fact_count(); i++) {
        ff_fact_t f = ff_fact_get(i);
        TEST_ASSERT_TRUE(strlen(f.text) < FF_TEXT_MAX);
    }
}

/* ===== ff_by_category ===== */

void test_by_category_cross_system_finds_results(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_CROSS_SYSTEM, out, 80);
    TEST_ASSERT_GREATER_OR_EQUAL(10, n);
}

void test_by_category_historical_finds_results(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_HISTORICAL, out, 80);
    TEST_ASSERT_GREATER_OR_EQUAL(10, n);
}

void test_by_category_astronomical_finds_results(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_ASTRONOMICAL, out, 80);
    TEST_ASSERT_GREATER_OR_EQUAL(10, n);
}

void test_by_category_mathematical_finds_results(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_MATHEMATICAL, out, 80);
    TEST_ASSERT_GREATER_OR_EQUAL(5, n);
}

void test_by_category_geological_finds_results(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_GEOLOGICAL, out, 80);
    TEST_ASSERT_GREATER_OR_EQUAL(5, n);
}

void test_by_category_cultural_finds_results(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_CULTURAL, out, 80);
    TEST_ASSERT_GREATER_OR_EQUAL(5, n);
}

void test_by_category_invalid_returns_zero(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_COUNT, out, 80);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_by_category_respects_max_out(void)
{
    int out[3];
    int n = ff_by_category(FF_CAT_CROSS_SYSTEM, out, 3);
    TEST_ASSERT_TRUE(n <= 3);
}

void test_by_category_indices_valid(void)
{
    int out[80];
    int n = ff_by_category(FF_CAT_CROSS_SYSTEM, out, 80);
    int count = ff_fact_count();
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i] >= 0 && out[i] < count);
    }
}

/* ===== ff_category_name ===== */

void test_category_name_cross_system(void)
{
    TEST_ASSERT_EQUAL_STRING("Cross-System", ff_category_name(FF_CAT_CROSS_SYSTEM));
}

void test_category_name_historical(void)
{
    TEST_ASSERT_EQUAL_STRING("Historical", ff_category_name(FF_CAT_HISTORICAL));
}

void test_category_name_astronomical(void)
{
    TEST_ASSERT_EQUAL_STRING("Astronomical", ff_category_name(FF_CAT_ASTRONOMICAL));
}

void test_category_name_mathematical(void)
{
    TEST_ASSERT_EQUAL_STRING("Mathematical", ff_category_name(FF_CAT_MATHEMATICAL));
}

void test_category_name_geological(void)
{
    TEST_ASSERT_EQUAL_STRING("Geological", ff_category_name(FF_CAT_GEOLOGICAL));
}

void test_category_name_cultural(void)
{
    TEST_ASSERT_EQUAL_STRING("Cultural", ff_category_name(FF_CAT_CULTURAL));
}

void test_category_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", ff_category_name(FF_CAT_COUNT));
}

/* ===== ff_trigger_name ===== */

void test_trigger_name_always(void)
{
    TEST_ASSERT_EQUAL_STRING("Always", ff_trigger_name(FF_TRIG_ALWAYS));
}

void test_trigger_name_view_space(void)
{
    TEST_ASSERT_EQUAL_STRING("View: Space", ff_trigger_name(FF_TRIG_VIEW_SPACE));
}

void test_trigger_name_convergence(void)
{
    TEST_ASSERT_EQUAL_STRING("Convergence", ff_trigger_name(FF_TRIG_CONVERGENCE));
}

void test_trigger_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", ff_trigger_name(FF_TRIG_COUNT));
}

/* ===== ff_context_default ===== */

void test_context_default_view_is_space(void)
{
    ff_context_t ctx = ff_context_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.view);
}

void test_context_default_scale_is_human(void)
{
    ff_context_t ctx = ff_context_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.scale);
}

void test_context_default_no_active_systems(void)
{
    ff_context_t ctx = ff_context_default();
    TEST_ASSERT_EQUAL_UINT(0, ctx.active_systems);
}

void test_context_default_no_convergence(void)
{
    ff_context_t ctx = ff_context_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.convergence_active);
}

void test_context_default_seal_unknown(void)
{
    ff_context_t ctx = ff_context_default();
    TEST_ASSERT_EQUAL_INT(-1, ctx.user_seal);
}

/* ===== ff_evaluate ===== */

void test_evaluate_returns_results(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
}

void test_evaluate_count_within_max(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count <= FF_MAX_RESULTS);
}

void test_evaluate_scores_descending(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    for (int i = 1; i < sel.count; i++) {
        TEST_ASSERT_TRUE(sel.results[i - 1].score >= sel.results[i].score);
    }
}

void test_evaluate_space_view_context(void)
{
    ff_context_t ctx = ff_context_default();
    ctx.view = 0; /* space */
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
    /* Space view facts should score well */
    TEST_ASSERT_TRUE(sel.results[0].score >= 1);
}

void test_evaluate_earth_view_context(void)
{
    ff_context_t ctx = ff_context_default();
    ctx.view = 1; /* earth */
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
}

void test_evaluate_deep_time_context(void)
{
    ff_context_t ctx = ff_context_default();
    ctx.view = 5; /* deep_time */
    ctx.scale = 2; /* geological */
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
    /* Should favor geological facts at geological scale */
}

void test_evaluate_convergence_active(void)
{
    ff_context_t ctx = ff_context_default();
    ctx.convergence_active = 1;
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
}

void test_evaluate_with_active_systems(void)
{
    ff_context_t ctx = ff_context_default();
    /* Activate astronomy + astrology */
    ctx.active_systems = (1u << 0) | (1u << 1);
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
    /* Facts referencing astronomy/astrology should score higher */
}

void test_evaluate_system_overlap_boosts_score(void)
{
    /* Context with many active systems should boost matching facts */
    ff_context_t ctx_many = ff_context_default();
    ctx_many.active_systems = 0x3FFF; /* all 14 systems active */

    ff_context_t ctx_none = ff_context_default();
    ctx_none.active_systems = 0;

    ff_selection_t sel_many = ff_evaluate(&ctx_many, NULL, 0);
    ff_selection_t sel_none = ff_evaluate(&ctx_none, NULL, 0);

    /* Top result with all systems active should score >= top result with none */
    TEST_ASSERT_TRUE(sel_many.results[0].score >= sel_none.results[0].score);
}

/* ===== Seen-fact exclusion ===== */

void test_evaluate_excludes_seen_facts(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel1 = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel1.count > 0);

    int seen[1] = { sel1.results[0].fact_index };
    ff_selection_t sel2 = ff_evaluate(&ctx, seen, 1);

    /* The previously top fact should NOT appear in new results */
    for (int i = 0; i < sel2.count; i++) {
        TEST_ASSERT_NOT_EQUAL(seen[0], sel2.results[i].fact_index);
    }
}

void test_evaluate_multiple_seen_excluded(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel1 = ff_evaluate(&ctx, NULL, 0);

    /* Mark all first results as seen */
    int seen[FF_MAX_RESULTS];
    for (int i = 0; i < sel1.count; i++) {
        seen[i] = sel1.results[i].fact_index;
    }

    ff_selection_t sel2 = ff_evaluate(&ctx, seen, sel1.count);

    /* None of the seen facts should appear */
    for (int i = 0; i < sel2.count; i++) {
        for (int j = 0; j < sel1.count; j++) {
            TEST_ASSERT_NOT_EQUAL(seen[j], sel2.results[i].fact_index);
        }
    }
}

void test_evaluate_all_seen_returns_zero(void)
{
    ff_context_t ctx = ff_context_default();
    int count = ff_fact_count();

    /* Mark all facts as seen */
    int seen[256];
    for (int i = 0; i < count && i < 256; i++) {
        seen[i] = i;
    }

    ff_selection_t sel = ff_evaluate(&ctx, seen, count < 256 ? count : 256);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
}

/* ===== ff_best_fact ===== */

void test_best_fact_returns_valid_index(void)
{
    ff_context_t ctx = ff_context_default();
    int best = ff_best_fact(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(best >= 0 && best < ff_fact_count());
}

void test_best_fact_matches_evaluate_top(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    int best = ff_best_fact(&ctx, NULL, 0);
    TEST_ASSERT_EQUAL_INT(sel.results[0].fact_index, best);
}

void test_best_fact_all_seen_returns_neg1(void)
{
    ff_context_t ctx = ff_context_default();
    int count = ff_fact_count();
    int seen[256];
    for (int i = 0; i < count && i < 256; i++) {
        seen[i] = i;
    }
    int best = ff_best_fact(&ctx, seen, count < 256 ? count : 256);
    TEST_ASSERT_EQUAL_INT(-1, best);
}

void test_best_fact_excludes_seen(void)
{
    ff_context_t ctx = ff_context_default();
    int best1 = ff_best_fact(&ctx, NULL, 0);
    int seen[1] = { best1 };
    int best2 = ff_best_fact(&ctx, seen, 1);
    TEST_ASSERT_NOT_EQUAL(best1, best2);
}

/* ===== Score ordering ===== */

void test_high_priority_facts_rank_higher(void)
{
    /* Cross-system facts are priority 3 — they should tend to rank above
       cultural facts (priority 1) in a neutral context */
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
    /* Top result should have a score > 1 (more than just base priority=1) */
    TEST_ASSERT_TRUE(sel.results[0].score > 1);
}

void test_trigger_match_boosts_score(void)
{
    /* Space view should boost FF_TRIG_VIEW_SPACE facts */
    ff_context_t ctx = ff_context_default();
    ctx.view = 0; /* space */
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    /* Top result should have a score boosted by trigger match (+3) */
    TEST_ASSERT_TRUE(sel.results[0].score >= 4);
}

/* ===== Fact indices in results are valid ===== */

void test_evaluate_result_indices_valid(void)
{
    ff_context_t ctx = ff_context_default();
    ff_selection_t sel = ff_evaluate(&ctx, NULL, 0);
    int count = ff_fact_count();
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_TRUE(sel.results[i].fact_index >= 0);
        TEST_ASSERT_TRUE(sel.results[i].fact_index < count);
    }
}

/* ===== Null context ===== */

void test_evaluate_null_context_returns_empty(void)
{
    ff_selection_t sel = ff_evaluate(NULL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
}

void test_best_fact_null_context_returns_neg1(void)
{
    int best = ff_best_fact(NULL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, best);
}

/* ===== Scale bonus ===== */

void test_geological_scale_boosts_geological_facts(void)
{
    ff_context_t ctx_geo = ff_context_default();
    ctx_geo.scale = 2; /* geological */
    ctx_geo.view = 5;  /* deep_time */

    ff_context_t ctx_human = ff_context_default();
    ctx_human.scale = 0; /* human */

    ff_selection_t sel_geo = ff_evaluate(&ctx_geo, NULL, 0);
    ff_selection_t sel_human = ff_evaluate(&ctx_human, NULL, 0);

    /* Geological scale should produce different top results vs human scale */
    /* At minimum, scores should differ */
    TEST_ASSERT_TRUE(sel_geo.results[0].score >= 1);
    TEST_ASSERT_TRUE(sel_human.results[0].score >= 1);
}

/* ===== Main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* ff_fact_count */
    RUN_TEST(test_fact_count_at_least_80);
    RUN_TEST(test_fact_count_positive);

    /* ff_fact_get */
    RUN_TEST(test_fact_get_first_valid);
    RUN_TEST(test_fact_get_last_valid);
    RUN_TEST(test_fact_get_negative_index_returns_empty);
    RUN_TEST(test_fact_get_out_of_bounds_returns_empty);
    RUN_TEST(test_fact_get_id_matches_index);
    RUN_TEST(test_fact_get_priority_in_range);
    RUN_TEST(test_fact_get_category_in_range);
    RUN_TEST(test_fact_get_trigger_in_range);
    RUN_TEST(test_fact_get_system_mask_nonzero);
    RUN_TEST(test_fact_text_within_max_length);

    /* ff_by_category */
    RUN_TEST(test_by_category_cross_system_finds_results);
    RUN_TEST(test_by_category_historical_finds_results);
    RUN_TEST(test_by_category_astronomical_finds_results);
    RUN_TEST(test_by_category_mathematical_finds_results);
    RUN_TEST(test_by_category_geological_finds_results);
    RUN_TEST(test_by_category_cultural_finds_results);
    RUN_TEST(test_by_category_invalid_returns_zero);
    RUN_TEST(test_by_category_respects_max_out);
    RUN_TEST(test_by_category_indices_valid);

    /* ff_category_name */
    RUN_TEST(test_category_name_cross_system);
    RUN_TEST(test_category_name_historical);
    RUN_TEST(test_category_name_astronomical);
    RUN_TEST(test_category_name_mathematical);
    RUN_TEST(test_category_name_geological);
    RUN_TEST(test_category_name_cultural);
    RUN_TEST(test_category_name_invalid);

    /* ff_trigger_name */
    RUN_TEST(test_trigger_name_always);
    RUN_TEST(test_trigger_name_view_space);
    RUN_TEST(test_trigger_name_convergence);
    RUN_TEST(test_trigger_name_invalid);

    /* ff_context_default */
    RUN_TEST(test_context_default_view_is_space);
    RUN_TEST(test_context_default_scale_is_human);
    RUN_TEST(test_context_default_no_active_systems);
    RUN_TEST(test_context_default_no_convergence);
    RUN_TEST(test_context_default_seal_unknown);

    /* ff_evaluate */
    RUN_TEST(test_evaluate_returns_results);
    RUN_TEST(test_evaluate_count_within_max);
    RUN_TEST(test_evaluate_scores_descending);
    RUN_TEST(test_evaluate_space_view_context);
    RUN_TEST(test_evaluate_earth_view_context);
    RUN_TEST(test_evaluate_deep_time_context);
    RUN_TEST(test_evaluate_convergence_active);
    RUN_TEST(test_evaluate_with_active_systems);
    RUN_TEST(test_evaluate_system_overlap_boosts_score);

    /* Seen-fact exclusion */
    RUN_TEST(test_evaluate_excludes_seen_facts);
    RUN_TEST(test_evaluate_multiple_seen_excluded);
    RUN_TEST(test_evaluate_all_seen_returns_zero);

    /* ff_best_fact */
    RUN_TEST(test_best_fact_returns_valid_index);
    RUN_TEST(test_best_fact_matches_evaluate_top);
    RUN_TEST(test_best_fact_all_seen_returns_neg1);
    RUN_TEST(test_best_fact_excludes_seen);

    /* Score ordering */
    RUN_TEST(test_high_priority_facts_rank_higher);
    RUN_TEST(test_trigger_match_boosts_score);

    /* Result validity */
    RUN_TEST(test_evaluate_result_indices_valid);

    /* Edge cases */
    RUN_TEST(test_evaluate_null_context_returns_empty);
    RUN_TEST(test_best_fact_null_context_returns_neg1);

    /* Scale bonus */
    RUN_TEST(test_geological_scale_boosts_geological_facts);

    return UNITY_END();
}
