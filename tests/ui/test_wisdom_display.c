/* test_wisdom_display.c -- Wisdom display trigger and formatting tests.
 * Tests trigger logic, animation phases, formatting, and state management. */

#include "../unity/unity.h"
#include "../../src/ui/wisdom_display.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Init ===== */

void test_init_hidden(void)
{
    wd_state_t s = wd_init();
    TEST_ASSERT_EQUAL_INT(-1, s.quote_index);
    TEST_ASSERT_EQUAL(WD_PHASE_HIDDEN, s.phase);
    TEST_ASSERT_EQUAL(WD_TRIGGER_NONE, s.trigger);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.opacity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.cooldown_remaining);
    TEST_ASSERT_EQUAL_INT(0, s.seen_count);
}

/* ===== Trigger conditions ===== */

void test_trigger_none_when_hidden_no_events(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_NONE,
                      wd_should_trigger(&s, &ctx, 0, 0, 0.0));
}

void test_trigger_convergence(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_CONVERGENCE,
                      wd_should_trigger(&s, &ctx, 1, 0, 0.0));
}

void test_trigger_view_change(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_VIEW_CHANGE,
                      wd_should_trigger(&s, &ctx, 0, 1, 0.0));
}

void test_trigger_idle(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_IDLE,
                      wd_should_trigger(&s, &ctx, 0, 0, 31.0));
}

void test_trigger_idle_exact_threshold(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_IDLE,
                      wd_should_trigger(&s, &ctx, 0, 0, 30.0));
}

void test_trigger_idle_below_threshold(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_NONE,
                      wd_should_trigger(&s, &ctx, 0, 0, 29.9));
}

void test_trigger_none_during_cooldown(void)
{
    wd_state_t s = wd_init();
    s.cooldown_remaining = 10.0;
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_NONE,
                      wd_should_trigger(&s, &ctx, 1, 1, 100.0));
}

void test_trigger_none_while_displaying(void)
{
    wd_state_t s = wd_init();
    s.phase = WD_PHASE_HOLD;
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_NONE,
                      wd_should_trigger(&s, &ctx, 1, 0, 0.0));
}

void test_convergence_priority_over_view_change(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_CONVERGENCE,
                      wd_should_trigger(&s, &ctx, 1, 1, 50.0));
}

void test_trigger_null_state(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL(WD_TRIGGER_NONE,
                      wd_should_trigger(NULL, &ctx, 1, 1, 100.0));
}

/* ===== Activation ===== */

void test_activate_selects_quote(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    int idx = wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_EQUAL_INT(idx, s.quote_index);
    TEST_ASSERT_EQUAL(WD_PHASE_FADE_IN, s.phase);
    TEST_ASSERT_EQUAL(WD_TRIGGER_CONVERGENCE, s.trigger);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.opacity);
}

void test_activate_adds_to_seen(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    int idx = wd_activate(&s, WD_TRIGGER_IDLE, &ctx);
    TEST_ASSERT_EQUAL_INT(1, s.seen_count);
    TEST_ASSERT_EQUAL_INT(idx, s.seen[0]);
}

void test_activate_null_state(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(-1, wd_activate(NULL, WD_TRIGGER_IDLE, &ctx));
}

void test_activate_null_ctx(void)
{
    wd_state_t s = wd_init();
    TEST_ASSERT_EQUAL_INT(-1, wd_activate(&s, WD_TRIGGER_IDLE, NULL));
}

void test_activate_multiple_adds_to_seen(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    /* Reset phase to allow another activation */
    s.phase = WD_PHASE_HIDDEN;
    s.quote_index = -1;
    wd_activate(&s, WD_TRIGGER_IDLE, &ctx);
    TEST_ASSERT_EQUAL_INT(2, s.seen_count);
}

/* ===== Update phases ===== */

void test_update_fade_in(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    wd_phase_t p = wd_update(&s, 0.75);
    TEST_ASSERT_EQUAL(WD_PHASE_FADE_IN, p);
    TEST_ASSERT_EQUAL(WD_PHASE_FADE_IN, s.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, (float)s.opacity);
}

void test_update_to_hold(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    wd_phase_t p = wd_update(&s, 1.6);
    TEST_ASSERT_EQUAL(WD_PHASE_HOLD, p);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)s.opacity);
}

void test_update_to_fade_out(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    wd_phase_t p = wd_update(&s, 10.0);
    TEST_ASSERT_EQUAL(WD_PHASE_FADE_OUT, p);
    TEST_ASSERT_TRUE(s.opacity < 1.0);
    TEST_ASSERT_TRUE(s.opacity >= 0.0);
}

void test_update_back_to_hidden(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    wd_phase_t p = wd_update(&s, 12.0);
    TEST_ASSERT_EQUAL(WD_PHASE_HIDDEN, p);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)s.opacity);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)WD_COOLDOWN, (float)s.cooldown_remaining);
    TEST_ASSERT_EQUAL_INT(-1, s.quote_index);
}

void test_update_null_state(void)
{
    wd_phase_t p = wd_update(NULL, 1.0);
    TEST_ASSERT_EQUAL(WD_PHASE_HIDDEN, p);
}

void test_cooldown_decreases(void)
{
    wd_state_t s = wd_init();
    s.cooldown_remaining = 10.0;
    wd_update(&s, 5.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.0f, (float)s.cooldown_remaining);
}

void test_cooldown_does_not_go_negative(void)
{
    wd_state_t s = wd_init();
    s.cooldown_remaining = 3.0;
    wd_update(&s, 10.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.cooldown_remaining);
}

void test_update_incremental_steps(void)
{
    /* Step through entire animation in small increments */
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_VIEW_CHANGE, &ctx);

    /* Small steps through fade in */
    for (int i = 0; i < 15; i++) {
        wd_update(&s, 0.1);
    }
    TEST_ASSERT_EQUAL(WD_PHASE_HOLD, s.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)s.opacity);

    /* Step through hold (need to pass 9.5s total to exit hold) */
    for (int i = 0; i < 81; i++) {
        wd_update(&s, 0.1);
    }
    TEST_ASSERT_EQUAL(WD_PHASE_FADE_OUT, s.phase);

    /* Step through fade out (need to pass 11.0s total) */
    for (int i = 0; i < 16; i++) {
        wd_update(&s, 0.1);
    }
    TEST_ASSERT_EQUAL(WD_PHASE_HIDDEN, s.phase);
}

/* ===== Formatting ===== */

void test_format_quote_when_active(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    const char *text = wd_format_quote(&s);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_GREATER_THAN(0, (int)strlen(text));
    /* Should contain em dash for attribution */
    TEST_ASSERT_NOT_NULL(strstr(text, "\xe2\x80\x94"));
}

void test_format_quote_when_hidden(void)
{
    wd_state_t s = wd_init();
    const char *text = wd_format_quote(&s);
    TEST_ASSERT_EQUAL_STRING("", text);
}

void test_format_quote_null_state(void)
{
    const char *text = wd_format_quote(NULL);
    TEST_ASSERT_EQUAL_STRING("", text);
}

void test_format_attribution_when_active(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_IDLE, &ctx);
    const char *attr = wd_format_attribution(&s);
    TEST_ASSERT_NOT_NULL(attr);
    TEST_ASSERT_GREATER_THAN(0, (int)strlen(attr));
    /* Should start with em dash */
    TEST_ASSERT_NOT_NULL(strstr(attr, "\xe2\x80\x94"));
}

void test_format_attribution_when_hidden(void)
{
    wd_state_t s = wd_init();
    const char *attr = wd_format_attribution(&s);
    TEST_ASSERT_EQUAL_STRING("", attr);
}

void test_format_attribution_null_state(void)
{
    const char *attr = wd_format_attribution(NULL);
    TEST_ASSERT_EQUAL_STRING("", attr);
}

/* ===== Visibility ===== */

void test_is_visible_when_hidden(void)
{
    wd_state_t s = wd_init();
    TEST_ASSERT_EQUAL_INT(0, wd_is_visible(&s));
}

void test_is_visible_when_displaying(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    wd_update(&s, 0.1);
    TEST_ASSERT_EQUAL_INT(1, wd_is_visible(&s));
}

void test_is_visible_null_state(void)
{
    TEST_ASSERT_EQUAL_INT(0, wd_is_visible(NULL));
}

/* ===== Name strings ===== */

void test_trigger_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("None", wd_trigger_name(WD_TRIGGER_NONE));
}

void test_trigger_name_convergence(void)
{
    TEST_ASSERT_EQUAL_STRING("Convergence",
                              wd_trigger_name(WD_TRIGGER_CONVERGENCE));
}

void test_trigger_name_view_change(void)
{
    TEST_ASSERT_EQUAL_STRING("View Change",
                              wd_trigger_name(WD_TRIGGER_VIEW_CHANGE));
}

void test_trigger_name_idle(void)
{
    TEST_ASSERT_EQUAL_STRING("Idle", wd_trigger_name(WD_TRIGGER_IDLE));
}

void test_trigger_name_kin_align(void)
{
    TEST_ASSERT_EQUAL_STRING("Kin Align",
                              wd_trigger_name(WD_TRIGGER_KIN_ALIGN));
}

void test_trigger_name_manual(void)
{
    TEST_ASSERT_EQUAL_STRING("Manual", wd_trigger_name(WD_TRIGGER_MANUAL));
}

void test_trigger_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wd_trigger_name(99));
}

void test_trigger_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wd_trigger_name(-1));
}

void test_phase_name_hidden(void)
{
    TEST_ASSERT_EQUAL_STRING("Hidden", wd_phase_name(WD_PHASE_HIDDEN));
}

void test_phase_name_fade_in(void)
{
    TEST_ASSERT_EQUAL_STRING("Fade In", wd_phase_name(WD_PHASE_FADE_IN));
}

void test_phase_name_hold(void)
{
    TEST_ASSERT_EQUAL_STRING("Hold", wd_phase_name(WD_PHASE_HOLD));
}

void test_phase_name_fade_out(void)
{
    TEST_ASSERT_EQUAL_STRING("Fade Out", wd_phase_name(WD_PHASE_FADE_OUT));
}

void test_phase_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wd_phase_name(99));
}

/* ===== Reset ===== */

void test_reset_seen(void)
{
    wd_state_t s = wd_init();
    we_context_t ctx = we_context_default();
    wd_activate(&s, WD_TRIGGER_CONVERGENCE, &ctx);
    TEST_ASSERT_EQUAL_INT(1, s.seen_count);
    wd_reset_seen(&s);
    TEST_ASSERT_EQUAL_INT(0, s.seen_count);
}

void test_reset_seen_null(void)
{
    /* Should not crash */
    wd_reset_seen(NULL);
}

/* ===== Constants ===== */

void test_timing_constants(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, (float)WD_FADE_IN_DURATION);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 8.0f, (float)WD_HOLD_DURATION);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, (float)WD_FADE_OUT_DURATION);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 11.0f, (float)WD_TOTAL_DURATION);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, (float)WD_IDLE_THRESHOLD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.0f, (float)WD_COOLDOWN);
}

void test_enum_counts(void)
{
    TEST_ASSERT_EQUAL_INT(6, WD_TRIGGER_COUNT);
    TEST_ASSERT_EQUAL_INT(4, WD_PHASE_COUNT);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Init */
    RUN_TEST(test_init_hidden);

    /* Trigger conditions */
    RUN_TEST(test_trigger_none_when_hidden_no_events);
    RUN_TEST(test_trigger_convergence);
    RUN_TEST(test_trigger_view_change);
    RUN_TEST(test_trigger_idle);
    RUN_TEST(test_trigger_idle_exact_threshold);
    RUN_TEST(test_trigger_idle_below_threshold);
    RUN_TEST(test_trigger_none_during_cooldown);
    RUN_TEST(test_trigger_none_while_displaying);
    RUN_TEST(test_convergence_priority_over_view_change);
    RUN_TEST(test_trigger_null_state);

    /* Activation */
    RUN_TEST(test_activate_selects_quote);
    RUN_TEST(test_activate_adds_to_seen);
    RUN_TEST(test_activate_null_state);
    RUN_TEST(test_activate_null_ctx);
    RUN_TEST(test_activate_multiple_adds_to_seen);

    /* Update phases */
    RUN_TEST(test_update_fade_in);
    RUN_TEST(test_update_to_hold);
    RUN_TEST(test_update_to_fade_out);
    RUN_TEST(test_update_back_to_hidden);
    RUN_TEST(test_update_null_state);
    RUN_TEST(test_cooldown_decreases);
    RUN_TEST(test_cooldown_does_not_go_negative);
    RUN_TEST(test_update_incremental_steps);

    /* Formatting */
    RUN_TEST(test_format_quote_when_active);
    RUN_TEST(test_format_quote_when_hidden);
    RUN_TEST(test_format_quote_null_state);
    RUN_TEST(test_format_attribution_when_active);
    RUN_TEST(test_format_attribution_when_hidden);
    RUN_TEST(test_format_attribution_null_state);

    /* Visibility */
    RUN_TEST(test_is_visible_when_hidden);
    RUN_TEST(test_is_visible_when_displaying);
    RUN_TEST(test_is_visible_null_state);

    /* Name strings */
    RUN_TEST(test_trigger_name_none);
    RUN_TEST(test_trigger_name_convergence);
    RUN_TEST(test_trigger_name_view_change);
    RUN_TEST(test_trigger_name_idle);
    RUN_TEST(test_trigger_name_kin_align);
    RUN_TEST(test_trigger_name_manual);
    RUN_TEST(test_trigger_name_invalid);
    RUN_TEST(test_trigger_name_negative);
    RUN_TEST(test_phase_name_hidden);
    RUN_TEST(test_phase_name_fade_in);
    RUN_TEST(test_phase_name_hold);
    RUN_TEST(test_phase_name_fade_out);
    RUN_TEST(test_phase_name_invalid);

    /* Reset */
    RUN_TEST(test_reset_seen);
    RUN_TEST(test_reset_seen_null);

    /* Constants */
    RUN_TEST(test_timing_constants);
    RUN_TEST(test_enum_counts);

    return UNITY_END();
}
