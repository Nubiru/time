/* test_aha_moments.c -- Tests for Aha Moments insight database.
 * Validates 50 pre-authored insight cards, trigger evaluation,
 * context matching, sorting, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/aha_moments.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== aha_count ===== */

void test_count_returns_50(void)
{
    TEST_ASSERT_EQUAL_INT(50, aha_count());
}

/* ===== aha_get — specific IDs ===== */

void test_get_id_0_the_same_moon(void)
{
    aha_entry_t e = aha_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_EQUAL_STRING("The Same Moon", e.title);
    TEST_ASSERT_TRUE(strlen(e.text) > 0);
    TEST_ASSERT_EQUAL_INT(AHA_TRIG_CONVERGENCE, e.trigger);
}

void test_get_id_14_number_in_your_chart(void)
{
    aha_entry_t e = aha_get(14);
    TEST_ASSERT_EQUAL_INT(14, e.id);
    TEST_ASSERT_EQUAL_STRING("The Number in Your Chart", e.title);
    TEST_ASSERT_TRUE(strlen(e.text) > 0);
    TEST_ASSERT_EQUAL_INT(AHA_TRIG_PERSONAL, e.trigger);
}

void test_get_id_25_solstice_agreement(void)
{
    aha_entry_t e = aha_get(25);
    TEST_ASSERT_EQUAL_INT(25, e.id);
    TEST_ASSERT_EQUAL_STRING("Solstice Agreement", e.title);
    TEST_ASSERT_TRUE(strlen(e.text) > 0);
    TEST_ASSERT_EQUAL_INT(AHA_TRIG_SEASONAL, e.trigger);
}

void test_get_id_35_binary_universe(void)
{
    aha_entry_t e = aha_get(35);
    TEST_ASSERT_EQUAL_INT(35, e.id);
    TEST_ASSERT_EQUAL_STRING("Binary Universe", e.title);
    TEST_ASSERT_TRUE(strlen(e.text) > 0);
    TEST_ASSERT_EQUAL_INT(AHA_TRIG_STRUCTURAL, e.trigger);
}

void test_get_id_49_red_thread(void)
{
    aha_entry_t e = aha_get(49);
    TEST_ASSERT_EQUAL_INT(49, e.id);
    TEST_ASSERT_EQUAL_STRING("The Red Thread", e.title);
    TEST_ASSERT_TRUE(strlen(e.text) > 0);
    TEST_ASSERT_EQUAL_INT(AHA_TRIG_ALWAYS, e.trigger);
}

/* ===== aha_get — loop all 50 ===== */

void test_get_all_50_title_nonempty(void)
{
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_TRUE_MESSAGE(strlen(e.title) > 0,
                                 "title should be non-empty");
    }
}

void test_get_all_50_text_nonempty(void)
{
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_TRUE_MESSAGE(strlen(e.text) > 0,
                                 "text should be non-empty");
    }
}

void test_get_all_50_trigger_in_range(void)
{
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_TRUE(e.trigger >= 0);
        TEST_ASSERT_TRUE(e.trigger < AHA_TRIG_COUNT);
    }
}

void test_get_all_50_depth_in_range(void)
{
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_TRUE_MESSAGE(e.depth >= 1 && e.depth <= 3,
                                 "depth should be 1, 2, or 3");
    }
}

void test_get_all_50_id_matches_index(void)
{
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_EQUAL_INT(i, e.id);
    }
}

/* ===== aha_get — invalid IDs ===== */

void test_get_negative_returns_invalid(void)
{
    aha_entry_t e = aha_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_EQUAL_STRING("?", e.title);
    TEST_ASSERT_EQUAL_STRING("?", e.text);
}

void test_get_50_returns_invalid(void)
{
    aha_entry_t e = aha_get(50);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_EQUAL_STRING("?", e.title);
    TEST_ASSERT_EQUAL_STRING("?", e.text);
}

void test_get_large_id_returns_invalid(void)
{
    aha_entry_t e = aha_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

/* ===== No duplicate IDs ===== */

void test_no_duplicate_ids(void)
{
    int seen[50] = {0};
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_TRUE(e.id >= 0 && e.id < 50);
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, seen[e.id],
                                      "duplicate id found");
        seen[e.id] = 1;
    }
}

/* ===== All IDs in range [0,49] ===== */

void test_all_ids_in_range(void)
{
    for (int i = 0; i < 50; i++) {
        aha_entry_t e = aha_get(i);
        TEST_ASSERT_TRUE(e.id >= 0);
        TEST_ASSERT_TRUE(e.id <= 49);
    }
}

/* ===== aha_evaluate — NULL context ===== */

void test_evaluate_null_returns_empty(void)
{
    aha_result_t r = aha_evaluate(NULL);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

/* ===== aha_evaluate — default context (all zeros) ===== */

void test_evaluate_default_matches_always(void)
{
    aha_context_t ctx = aha_default_context();
    aha_result_t r = aha_evaluate(&ctx);
    /* Should match the ALWAYS entries (IDs 45-49) */
    TEST_ASSERT_TRUE(r.count >= 5);
    /* Verify all ALWAYS IDs are present */
    int found[5] = {0};
    for (int i = 0; i < r.count; i++) {
        int idx = r.indices[i];
        if (idx >= 45 && idx <= 49) {
            found[idx - 45] = 1;
        }
    }
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, found[i],
                                      "ALWAYS entry not found");
    }
}

/* ===== aha_evaluate — convergence ===== */

void test_evaluate_convergence_score_3(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.convergence_score = 3;
    aha_result_t r = aha_evaluate(&ctx);
    /* Should include convergence entries with min_conv <= 3 */
    int found_conv = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_CONVERGENCE) {
            found_conv = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_conv);
}

/* ===== aha_evaluate — personal ===== */

void test_evaluate_personal_with_birth(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.has_birth = 1;
    aha_result_t r = aha_evaluate(&ctx);
    int found_personal = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_PERSONAL) {
            found_personal = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_personal);
}

/* ===== aha_evaluate — lunar ===== */

void test_evaluate_full_moon_matches_lunar(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.is_full_moon = 1;
    aha_result_t r = aha_evaluate(&ctx);
    int found_lunar = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_LUNAR) {
            found_lunar = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_lunar);
}

void test_evaluate_new_moon_matches_lunar(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.is_new_moon = 1;
    aha_result_t r = aha_evaluate(&ctx);
    int found_lunar = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_LUNAR) {
            found_lunar = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_lunar);
}

/* ===== aha_evaluate — seasonal ===== */

void test_evaluate_solstice_matches_seasonal(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.is_solstice = 1;
    aha_result_t r = aha_evaluate(&ctx);
    int found_seasonal = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_SEASONAL) {
            found_seasonal = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_seasonal);
}

void test_evaluate_equinox_matches_seasonal(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.is_equinox = 1;
    aha_result_t r = aha_evaluate(&ctx);
    int found_seasonal = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_SEASONAL) {
            found_seasonal = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_seasonal);
}

/* ===== aha_evaluate — deep time ===== */

void test_evaluate_deep_time_scale_5(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.scale_level = 5;
    aha_result_t r = aha_evaluate(&ctx);
    int found_deep = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_DEEP_TIME) {
            found_deep = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_deep);
}

/* ===== aha_evaluate — number trigger ===== */

void test_evaluate_number_with_3_systems(void)
{
    aha_context_t ctx = aha_default_context();
    /* Set 3 system bits */
    ctx.active_systems = (1u << 0) | (1u << 1) | (1u << 2);
    aha_result_t r = aha_evaluate(&ctx);
    int found_number = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_NUMBER) {
            found_number = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_number);
}

/* ===== aha_evaluate — structural trigger ===== */

void test_evaluate_structural_with_2_systems(void)
{
    aha_context_t ctx = aha_default_context();
    /* Set 2 system bits */
    ctx.active_systems = (1u << 0) | (1u << 1);
    aha_result_t r = aha_evaluate(&ctx);
    int found_structural = 0;
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        if (e.trigger == AHA_TRIG_STRUCTURAL) {
            found_structural = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_structural);
}

/* ===== aha_evaluate — sorting by depth descending ===== */

void test_evaluate_sorted_by_depth_descending(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.convergence_score = 5;
    ctx.has_birth = 1;
    ctx.is_full_moon = 1;
    ctx.is_solstice = 1;
    ctx.scale_level = 5;
    ctx.active_systems = 0x3FFF; /* all 14 systems */
    aha_result_t r = aha_evaluate(&ctx);
    TEST_ASSERT_TRUE(r.count > 1);
    for (int i = 1; i < r.count; i++) {
        aha_entry_t prev = aha_get(r.indices[i - 1]);
        aha_entry_t curr = aha_get(r.indices[i]);
        TEST_ASSERT_TRUE_MESSAGE(prev.depth >= curr.depth,
                                 "results not sorted by depth descending");
    }
}

/* ===== aha_evaluate — count capped at AHA_MAX_RESULTS ===== */

void test_evaluate_count_capped(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.convergence_score = 5;
    ctx.has_birth = 1;
    ctx.is_full_moon = 1;
    ctx.is_solstice = 1;
    ctx.scale_level = 5;
    ctx.active_systems = 0x3FFF;
    aha_result_t r = aha_evaluate(&ctx);
    TEST_ASSERT_TRUE(r.count <= AHA_MAX_RESULTS);
}

/* ===== aha_trigger_name ===== */

void test_trigger_name_convergence(void)
{
    const char *n = aha_trigger_name(AHA_TRIG_CONVERGENCE);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_TRUE(strlen(n) > 0);
}

void test_trigger_name_personal(void)
{
    const char *n = aha_trigger_name(AHA_TRIG_PERSONAL);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_TRUE(strlen(n) > 0);
}

void test_trigger_name_all_valid(void)
{
    for (int i = 0; i < AHA_TRIG_COUNT; i++) {
        const char *n = aha_trigger_name((aha_trigger_t)i);
        TEST_ASSERT_NOT_NULL(n);
        TEST_ASSERT_TRUE(strlen(n) > 0);
    }
}

void test_trigger_name_invalid(void)
{
    const char *n = aha_trigger_name(AHA_TRIG_COUNT);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("?", n);
}

void test_trigger_name_negative(void)
{
    const char *n = aha_trigger_name((aha_trigger_t)-1);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("?", n);
}

/* ===== aha_default_context ===== */

void test_default_context_all_zeros(void)
{
    aha_context_t ctx = aha_default_context();
    TEST_ASSERT_EQUAL_UINT(0, ctx.active_systems);
    TEST_ASSERT_EQUAL_INT(0, ctx.convergence_score);
    TEST_ASSERT_EQUAL_INT(0, ctx.has_birth);
    TEST_ASSERT_EQUAL_INT(0, ctx.is_full_moon);
    TEST_ASSERT_EQUAL_INT(0, ctx.is_new_moon);
    TEST_ASSERT_EQUAL_INT(0, ctx.is_solstice);
    TEST_ASSERT_EQUAL_INT(0, ctx.is_equinox);
    TEST_ASSERT_EQUAL_INT(0, ctx.scale_level);
}

/* ===== Trigger category coverage ===== */

void test_convergence_entries_exist(void)
{
    int count = 0;
    for (int i = 0; i < 50; i++) {
        if (aha_get(i).trigger == AHA_TRIG_CONVERGENCE) count++;
    }
    TEST_ASSERT_EQUAL_INT(10, count); /* IDs 0-9 */
}

void test_personal_entries_exist(void)
{
    int count = 0;
    for (int i = 0; i < 50; i++) {
        if (aha_get(i).trigger == AHA_TRIG_PERSONAL) count++;
    }
    TEST_ASSERT_EQUAL_INT(5, count); /* IDs 10-14 */
}

void test_always_entries_exist(void)
{
    int count = 0;
    for (int i = 0; i < 50; i++) {
        if (aha_get(i).trigger == AHA_TRIG_ALWAYS) count++;
    }
    TEST_ASSERT_EQUAL_INT(5, count); /* IDs 45-49 */
}

/* ===== aha_evaluate — no false matches with minimal context ===== */

void test_evaluate_no_convergence_without_score(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.convergence_score = 0;
    aha_result_t r = aha_evaluate(&ctx);
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        TEST_ASSERT_TRUE_MESSAGE(e.trigger != AHA_TRIG_CONVERGENCE,
                                 "convergence matched with score 0");
    }
}

void test_evaluate_no_personal_without_birth(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.has_birth = 0;
    aha_result_t r = aha_evaluate(&ctx);
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        TEST_ASSERT_TRUE_MESSAGE(e.trigger != AHA_TRIG_PERSONAL,
                                 "personal matched without birth");
    }
}

void test_evaluate_no_deep_time_at_scale_3(void)
{
    aha_context_t ctx = aha_default_context();
    ctx.scale_level = 3;
    aha_result_t r = aha_evaluate(&ctx);
    for (int i = 0; i < r.count; i++) {
        aha_entry_t e = aha_get(r.indices[i]);
        TEST_ASSERT_TRUE_MESSAGE(e.trigger != AHA_TRIG_DEEP_TIME,
                                 "deep_time matched at scale_level 3");
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* count */
    RUN_TEST(test_count_returns_50);

    /* get specific IDs */
    RUN_TEST(test_get_id_0_the_same_moon);
    RUN_TEST(test_get_id_14_number_in_your_chart);
    RUN_TEST(test_get_id_25_solstice_agreement);
    RUN_TEST(test_get_id_35_binary_universe);
    RUN_TEST(test_get_id_49_red_thread);

    /* get all 50 loop */
    RUN_TEST(test_get_all_50_title_nonempty);
    RUN_TEST(test_get_all_50_text_nonempty);
    RUN_TEST(test_get_all_50_trigger_in_range);
    RUN_TEST(test_get_all_50_depth_in_range);
    RUN_TEST(test_get_all_50_id_matches_index);

    /* get invalid */
    RUN_TEST(test_get_negative_returns_invalid);
    RUN_TEST(test_get_50_returns_invalid);
    RUN_TEST(test_get_large_id_returns_invalid);

    /* uniqueness */
    RUN_TEST(test_no_duplicate_ids);
    RUN_TEST(test_all_ids_in_range);

    /* evaluate NULL */
    RUN_TEST(test_evaluate_null_returns_empty);

    /* evaluate default -> ALWAYS */
    RUN_TEST(test_evaluate_default_matches_always);

    /* evaluate triggers */
    RUN_TEST(test_evaluate_convergence_score_3);
    RUN_TEST(test_evaluate_personal_with_birth);
    RUN_TEST(test_evaluate_full_moon_matches_lunar);
    RUN_TEST(test_evaluate_new_moon_matches_lunar);
    RUN_TEST(test_evaluate_solstice_matches_seasonal);
    RUN_TEST(test_evaluate_equinox_matches_seasonal);
    RUN_TEST(test_evaluate_deep_time_scale_5);
    RUN_TEST(test_evaluate_number_with_3_systems);
    RUN_TEST(test_evaluate_structural_with_2_systems);

    /* sorting & cap */
    RUN_TEST(test_evaluate_sorted_by_depth_descending);
    RUN_TEST(test_evaluate_count_capped);

    /* trigger names */
    RUN_TEST(test_trigger_name_convergence);
    RUN_TEST(test_trigger_name_personal);
    RUN_TEST(test_trigger_name_all_valid);
    RUN_TEST(test_trigger_name_invalid);
    RUN_TEST(test_trigger_name_negative);

    /* default context */
    RUN_TEST(test_default_context_all_zeros);

    /* category counts */
    RUN_TEST(test_convergence_entries_exist);
    RUN_TEST(test_personal_entries_exist);
    RUN_TEST(test_always_entries_exist);

    /* negative trigger tests */
    RUN_TEST(test_evaluate_no_convergence_without_score);
    RUN_TEST(test_evaluate_no_personal_without_birth);
    RUN_TEST(test_evaluate_no_deep_time_at_scale_3);

    return UNITY_END();
}
