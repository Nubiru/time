/* test_user_context.c -- User Context module tests
 * TDD RED phase: tests written before implementation.
 * Tests pure user-context tracking for contextual personalization. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/user_context.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== uc_default ===== */

void test_default_zeroes_session(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.session.action_count);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, ctx.session.duration_sec);
    TEST_ASSERT_EQUAL_INT(-1, ctx.session.dominant_system);
    TEST_ASSERT_EQUAL_INT(-1, ctx.session.dominant_view);
    TEST_ASSERT_EQUAL_INT(-1, ctx.session.dominant_scale);
}

void test_default_zeroes_birth(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.birth_kin);
    TEST_ASSERT_EQUAL_INT(0, ctx.birth_seal);
    TEST_ASSERT_EQUAL_INT(0, ctx.birth_tone);
    TEST_ASSERT_EQUAL_INT(0, ctx.birth_hexagram);
    TEST_ASSERT_EQUAL_INT(0, ctx.birth_sign);
    TEST_ASSERT_EQUAL_INT(0, ctx.birth_gate);
}

void test_default_history_empty(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.history_count);
    TEST_ASSERT_EQUAL_INT(0, ctx.history_head);
}

void test_default_affinity_zero(void)
{
    uc_context_t ctx = uc_default();
    for (int i = 0; i < UC_MAX_SYSTEMS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, ctx.system_affinity[i]);
    }
}

void test_default_view_and_scale(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.current_view);
    TEST_ASSERT_EQUAL_INT(0, ctx.current_scale);
    TEST_ASSERT_EQUAL_UINT(0, ctx.active_systems);
}

/* ===== uc_set_birth ===== */

void test_set_birth_extracts_kin(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    TEST_ASSERT_EQUAL_INT(bp.tzolkin.kin, ctx.birth_kin);
}

void test_set_birth_extracts_seal(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    TEST_ASSERT_EQUAL_INT(bp.tzolkin.seal, ctx.birth_seal);
}

void test_set_birth_extracts_tone(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    TEST_ASSERT_EQUAL_INT(bp.tzolkin.tone, ctx.birth_tone);
}

void test_set_birth_extracts_hexagram(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    TEST_ASSERT_EQUAL_INT(bp.iching.king_wen, ctx.birth_hexagram);
}

void test_set_birth_extracts_sun_sign(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    TEST_ASSERT_EQUAL_INT(bp.astrology.sun_sign, ctx.birth_sign);
}

void test_set_birth_extracts_hd_gate(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    TEST_ASSERT_EQUAL_INT(bp.astrology.hd_sun_gate, ctx.birth_gate);
}

/* ===== uc_update — view change ===== */

void test_update_view_change(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_VIEW_CHANGE, 2, 1.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_EQUAL_INT(2, ctx.current_view);
    TEST_ASSERT_EQUAL_INT(1, ctx.session.action_count);
}

void test_update_view_change_tracks_time(void)
{
    uc_context_t ctx = uc_default();
    /* Start at view 0, timestamp 0 */
    uc_action_t a1 = { UC_ACT_VIEW_CHANGE, 0, 0.0 };
    ctx = uc_update(ctx, a1);
    /* Switch to view 1 at t=10 → view 0 gets 10 seconds */
    uc_action_t a2 = { UC_ACT_VIEW_CHANGE, 1, 10.0 };
    ctx = uc_update(ctx, a2);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 10.0, ctx.session.view_time[0]);
}

/* ===== uc_update — scale change ===== */

void test_update_scale_change(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_SCALE_CHANGE, 3, 1.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_EQUAL_INT(3, ctx.current_scale);
}

void test_update_scale_change_tracks_time(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t a1 = { UC_ACT_SCALE_CHANGE, 0, 0.0 };
    ctx = uc_update(ctx, a1);
    uc_action_t a2 = { UC_ACT_SCALE_CHANGE, 2, 5.0 };
    ctx = uc_update(ctx, a2);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5.0, ctx.session.scale_time[0]);
}

/* ===== uc_update — system toggle ===== */

void test_update_system_toggle_bumps_affinity(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_SYSTEM_TOGGLE, 3, 1.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_TRUE(ctx.system_affinity[3] > 0.0);
}

void test_update_system_toggle_tracks_system_time(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t a1 = { UC_ACT_SYSTEM_TOGGLE, 2, 0.0 };
    ctx = uc_update(ctx, a1);
    uc_action_t a2 = { UC_ACT_SYSTEM_TOGGLE, 5, 7.0 };
    ctx = uc_update(ctx, a2);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 7.0, ctx.session.system_time[2]);
}

/* ===== uc_update — history ring buffer ===== */

void test_history_records_actions(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_CARD_VIEW, 1, 5.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_EQUAL_INT(1, ctx.history_count);
    TEST_ASSERT_EQUAL_INT(UC_ACT_CARD_VIEW, ctx.history[0].type);
    TEST_ASSERT_EQUAL_INT(1, ctx.history[0].value);
}

void test_history_wraps_around(void)
{
    uc_context_t ctx = uc_default();
    /* Fill history past capacity */
    for (int i = 0; i < UC_MAX_HISTORY + 5; i++) {
        uc_action_t act = { UC_ACT_FACT_READ, i, (double)i };
        ctx = uc_update(ctx, act);
    }
    TEST_ASSERT_EQUAL_INT(UC_MAX_HISTORY, ctx.history_count);
    /* head should have wrapped around */
    TEST_ASSERT_EQUAL_INT(5, ctx.history_head);
}

/* ===== Dominant computation ===== */

void test_dominant_system_returns_max_time(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t a1 = { UC_ACT_SYSTEM_TOGGLE, 2, 0.0 };
    ctx = uc_update(ctx, a1);
    uc_action_t a2 = { UC_ACT_SYSTEM_TOGGLE, 5, 3.0 };
    ctx = uc_update(ctx, a2);
    uc_action_t a3 = { UC_ACT_SYSTEM_TOGGLE, 2, 20.0 };
    ctx = uc_update(ctx, a3);
    /* system 5 had 17s (3→20), system 2 had 3s (0→3) */
    TEST_ASSERT_EQUAL_INT(5, uc_dominant_system(&ctx));
}

void test_dominant_system_neg1_when_empty(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_EQUAL_INT(-1, uc_dominant_system(&ctx));
}

void test_dominant_view(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t a1 = { UC_ACT_VIEW_CHANGE, 1, 0.0 };
    ctx = uc_update(ctx, a1);
    uc_action_t a2 = { UC_ACT_VIEW_CHANGE, 3, 2.0 };
    ctx = uc_update(ctx, a2);
    uc_action_t a3 = { UC_ACT_VIEW_CHANGE, 1, 15.0 };
    ctx = uc_update(ctx, a3);
    /* view 3 had 13s (2→15), view 1 had 2s (0→2) */
    TEST_ASSERT_EQUAL_INT(3, uc_dominant_view(&ctx));
}

/* ===== uc_top_systems ===== */

void test_top_systems_returns_sorted(void)
{
    uc_context_t ctx = uc_default();
    /* Build up some affinities by toggling systems */
    for (int i = 0; i < 5; i++) {
        uc_action_t act = { UC_ACT_SYSTEM_TOGGLE, 3, (double)i };
        ctx = uc_update(ctx, act);
    }
    uc_action_t act2 = { UC_ACT_SYSTEM_TOGGLE, 7, 5.0 };
    ctx = uc_update(ctx, act2);

    int ids[3];
    int count = uc_top_systems(&ctx, ids, 3);
    TEST_ASSERT_TRUE(count >= 1);
    /* System 3 was toggled 5 times, should be top */
    TEST_ASSERT_EQUAL_INT(3, ids[0]);
}

void test_top_systems_empty_context(void)
{
    uc_context_t ctx = uc_default();
    int ids[3];
    int count = uc_top_systems(&ctx, ids, 3);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== uc_is_exploring / uc_is_focused ===== */

void test_is_exploring_with_varied_actions(void)
{
    uc_context_t ctx = uc_default();
    /* Generate many distinct view/scale changes */
    for (int i = 0; i < 10; i++) {
        uc_action_t act = { (i % 2 == 0) ? UC_ACT_VIEW_CHANGE : UC_ACT_SCALE_CHANGE,
                            i % 5, (double)i };
        ctx = uc_update(ctx, act);
    }
    TEST_ASSERT_TRUE(uc_is_exploring(&ctx));
}

void test_is_focused_with_same_actions(void)
{
    uc_context_t ctx = uc_default();
    /* All same view and scale */
    for (int i = 0; i < 10; i++) {
        uc_action_t act = { UC_ACT_CARD_VIEW, 0, (double)i };
        ctx = uc_update(ctx, act);
    }
    TEST_ASSERT_TRUE(uc_is_focused(&ctx));
}

void test_is_focused_empty_history(void)
{
    uc_context_t ctx = uc_default();
    /* Empty = focused (not exploring) */
    TEST_ASSERT_TRUE(uc_is_focused(&ctx));
}

void test_is_exploring_empty_history(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_FALSE(uc_is_exploring(&ctx));
}

/* ===== uc_to_wisdom_context ===== */

void test_to_wisdom_context_maps_view(void)
{
    uc_context_t ctx = uc_default();
    ctx.current_view = 2; /* should map to WE_VIEW_TZOLKIN */
    we_context_t wc = uc_to_wisdom_context(&ctx);
    TEST_ASSERT_EQUAL_INT(WE_VIEW_TZOLKIN, wc.view);
}

void test_to_wisdom_context_maps_scale(void)
{
    uc_context_t ctx = uc_default();
    ctx.current_scale = 3; /* WE_SCALE_COSMIC */
    we_context_t wc = uc_to_wisdom_context(&ctx);
    TEST_ASSERT_EQUAL_INT(WE_SCALE_COSMIC, wc.scale);
}

void test_to_wisdom_context_maps_systems(void)
{
    uc_context_t ctx = uc_default();
    ctx.active_systems = 0x07; /* astronomy + astrology + tzolkin */
    we_context_t wc = uc_to_wisdom_context(&ctx);
    TEST_ASSERT_EQUAL_UINT(0x07, wc.active_systems);
}

void test_to_wisdom_context_maps_birth(void)
{
    uc_context_t ctx = uc_default();
    ctx.birth_seal = 5;
    ctx.birth_tone = 8;
    we_context_t wc = uc_to_wisdom_context(&ctx);
    TEST_ASSERT_EQUAL_INT(5, wc.user_seal);
    TEST_ASSERT_EQUAL_INT(8, wc.user_tone);
}

void test_to_wisdom_context_convergence_zero(void)
{
    uc_context_t ctx = uc_default();
    we_context_t wc = uc_to_wisdom_context(&ctx);
    TEST_ASSERT_EQUAL_INT(0, wc.convergence_score);
}

/* ===== uc_to_facts_context ===== */

void test_to_facts_context_maps_view(void)
{
    uc_context_t ctx = uc_default();
    ctx.current_view = 1;
    ff_context_t fc = uc_to_facts_context(&ctx);
    TEST_ASSERT_EQUAL_INT(1, fc.view);
}

void test_to_facts_context_maps_scale(void)
{
    uc_context_t ctx = uc_default();
    ctx.current_scale = 2;
    ff_context_t fc = uc_to_facts_context(&ctx);
    TEST_ASSERT_EQUAL_INT(2, fc.scale);
}

void test_to_facts_context_maps_systems(void)
{
    uc_context_t ctx = uc_default();
    ctx.active_systems = 0xFF;
    ff_context_t fc = uc_to_facts_context(&ctx);
    TEST_ASSERT_EQUAL_UINT(0xFF, fc.active_systems);
}

void test_to_facts_context_maps_seal(void)
{
    uc_context_t ctx = uc_default();
    ctx.birth_seal = 12;
    ff_context_t fc = uc_to_facts_context(&ctx);
    TEST_ASSERT_EQUAL_INT(12, fc.user_seal);
}

void test_to_facts_context_convergence_zero(void)
{
    uc_context_t ctx = uc_default();
    ff_context_t fc = uc_to_facts_context(&ctx);
    TEST_ASSERT_EQUAL_INT(0, fc.convergence_active);
}

/* ===== uc_system_time ===== */

void test_system_time_returns_correct(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t a1 = { UC_ACT_SYSTEM_TOGGLE, 4, 0.0 };
    ctx = uc_update(ctx, a1);
    uc_action_t a2 = { UC_ACT_SYSTEM_TOGGLE, 6, 8.0 };
    ctx = uc_update(ctx, a2);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 8.0, uc_system_time(&ctx, 4));
}

void test_system_time_invalid_id(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, uc_system_time(&ctx, -1));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, uc_system_time(&ctx, UC_MAX_SYSTEMS));
}

/* ===== uc_session_duration ===== */

void test_session_duration(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t a1 = { UC_ACT_VIEW_CHANGE, 0, 0.0 };
    ctx = uc_update(ctx, a1);
    uc_action_t a2 = { UC_ACT_CARD_VIEW, 1, 25.0 };
    ctx = uc_update(ctx, a2);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25.0, uc_session_duration(&ctx));
}

/* ===== uc_reset_session ===== */

void test_reset_session_clears_session(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_VIEW_CHANGE, 2, 10.0 };
    ctx = uc_update(ctx, act);
    ctx = uc_reset_session(ctx);
    TEST_ASSERT_EQUAL_INT(0, ctx.session.action_count);
    TEST_ASSERT_EQUAL_INT(0, ctx.history_count);
    TEST_ASSERT_EQUAL_INT(0, ctx.history_head);
}

void test_reset_session_keeps_birth(void)
{
    birth_profile_t bp = bp_compute(1990, 7, 26);
    uc_context_t ctx = uc_default();
    ctx = uc_set_birth(ctx, &bp);
    uc_action_t act = { UC_ACT_VIEW_CHANGE, 2, 10.0 };
    ctx = uc_update(ctx, act);
    ctx = uc_reset_session(ctx);
    /* Birth data should survive reset */
    TEST_ASSERT_EQUAL_INT(bp.tzolkin.kin, ctx.birth_kin);
    TEST_ASSERT_EQUAL_INT(bp.tzolkin.seal, ctx.birth_seal);
}

void test_reset_session_keeps_affinity(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_SYSTEM_TOGGLE, 5, 1.0 };
    ctx = uc_update(ctx, act);
    double aff_before = ctx.system_affinity[5];
    ctx = uc_reset_session(ctx);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, aff_before, ctx.system_affinity[5]);
}

/* ===== uc_action_name ===== */

void test_action_name_view_change(void)
{
    const char *name = uc_action_name(UC_ACT_VIEW_CHANGE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_action_name_all_valid(void)
{
    for (int i = 0; i < UC_ACT_COUNT; i++) {
        const char *name = uc_action_name((uc_action_type_t)i);
        TEST_ASSERT_NOT_NULL(name);
    }
}

void test_action_name_invalid(void)
{
    const char *name = uc_action_name(UC_ACT_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===== uc_action_count ===== */

void test_action_count_tracks(void)
{
    uc_context_t ctx = uc_default();
    TEST_ASSERT_EQUAL_INT(0, uc_action_count(&ctx));
    uc_action_t act = { UC_ACT_CARD_VIEW, 0, 1.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_EQUAL_INT(1, uc_action_count(&ctx));
    ctx = uc_update(ctx, act);
    TEST_ASSERT_EQUAL_INT(2, uc_action_count(&ctx));
}

/* ===== Affinity decay ===== */

void test_affinity_clamped_to_one(void)
{
    uc_context_t ctx = uc_default();
    /* Toggle same system many times */
    for (int i = 0; i < 100; i++) {
        uc_action_t act = { UC_ACT_SYSTEM_TOGGLE, 0, (double)i };
        ctx = uc_update(ctx, act);
    }
    TEST_ASSERT_TRUE(ctx.system_affinity[0] <= 1.0);
}

void test_affinity_decays_on_other_actions(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t toggle = { UC_ACT_SYSTEM_TOGGLE, 3, 0.0 };
    ctx = uc_update(ctx, toggle);
    double aff_after_toggle = ctx.system_affinity[3];
    /* Several non-toggle actions should decay affinity */
    for (int i = 0; i < 20; i++) {
        uc_action_t act = { UC_ACT_CARD_VIEW, 0, (double)(i + 1) };
        ctx = uc_update(ctx, act);
    }
    TEST_ASSERT_TRUE(ctx.system_affinity[3] < aff_after_toggle);
}

/* ===== uc_update — view clamping ===== */

void test_update_view_clamps(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_VIEW_CHANGE, UC_MAX_VIEWS + 5, 1.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_TRUE(ctx.current_view < UC_MAX_VIEWS);
}

void test_update_scale_clamps(void)
{
    uc_context_t ctx = uc_default();
    uc_action_t act = { UC_ACT_SCALE_CHANGE, UC_MAX_SCALES + 5, 1.0 };
    ctx = uc_update(ctx, act);
    TEST_ASSERT_TRUE(ctx.current_scale < UC_MAX_SCALES);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();
    /* uc_default */
    RUN_TEST(test_default_zeroes_session);
    RUN_TEST(test_default_zeroes_birth);
    RUN_TEST(test_default_history_empty);
    RUN_TEST(test_default_affinity_zero);
    RUN_TEST(test_default_view_and_scale);
    /* uc_set_birth */
    RUN_TEST(test_set_birth_extracts_kin);
    RUN_TEST(test_set_birth_extracts_seal);
    RUN_TEST(test_set_birth_extracts_tone);
    RUN_TEST(test_set_birth_extracts_hexagram);
    RUN_TEST(test_set_birth_extracts_sun_sign);
    RUN_TEST(test_set_birth_extracts_hd_gate);
    /* uc_update */
    RUN_TEST(test_update_view_change);
    RUN_TEST(test_update_view_change_tracks_time);
    RUN_TEST(test_update_scale_change);
    RUN_TEST(test_update_scale_change_tracks_time);
    RUN_TEST(test_update_system_toggle_bumps_affinity);
    RUN_TEST(test_update_system_toggle_tracks_system_time);
    RUN_TEST(test_history_records_actions);
    RUN_TEST(test_history_wraps_around);
    /* Dominant */
    RUN_TEST(test_dominant_system_returns_max_time);
    RUN_TEST(test_dominant_system_neg1_when_empty);
    RUN_TEST(test_dominant_view);
    /* uc_top_systems */
    RUN_TEST(test_top_systems_returns_sorted);
    RUN_TEST(test_top_systems_empty_context);
    /* Exploring / focused */
    RUN_TEST(test_is_exploring_with_varied_actions);
    RUN_TEST(test_is_focused_with_same_actions);
    RUN_TEST(test_is_focused_empty_history);
    RUN_TEST(test_is_exploring_empty_history);
    /* uc_to_wisdom_context */
    RUN_TEST(test_to_wisdom_context_maps_view);
    RUN_TEST(test_to_wisdom_context_maps_scale);
    RUN_TEST(test_to_wisdom_context_maps_systems);
    RUN_TEST(test_to_wisdom_context_maps_birth);
    RUN_TEST(test_to_wisdom_context_convergence_zero);
    /* uc_to_facts_context */
    RUN_TEST(test_to_facts_context_maps_view);
    RUN_TEST(test_to_facts_context_maps_scale);
    RUN_TEST(test_to_facts_context_maps_systems);
    RUN_TEST(test_to_facts_context_maps_seal);
    RUN_TEST(test_to_facts_context_convergence_zero);
    /* uc_system_time */
    RUN_TEST(test_system_time_returns_correct);
    RUN_TEST(test_system_time_invalid_id);
    /* uc_session_duration */
    RUN_TEST(test_session_duration);
    /* uc_reset_session */
    RUN_TEST(test_reset_session_clears_session);
    RUN_TEST(test_reset_session_keeps_birth);
    RUN_TEST(test_reset_session_keeps_affinity);
    /* uc_action_name */
    RUN_TEST(test_action_name_view_change);
    RUN_TEST(test_action_name_all_valid);
    RUN_TEST(test_action_name_invalid);
    /* uc_action_count */
    RUN_TEST(test_action_count_tracks);
    /* Affinity */
    RUN_TEST(test_affinity_clamped_to_one);
    RUN_TEST(test_affinity_decays_on_other_actions);
    /* Clamping */
    RUN_TEST(test_update_view_clamps);
    RUN_TEST(test_update_scale_clamps);
    return UNITY_END();
}
