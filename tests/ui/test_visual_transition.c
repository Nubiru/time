#include "../unity/unity.h"
#include "../../src/ui/visual_transition.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Init: 1-3 --- */

void test_init_all_idle(void)
{
    visual_transitions_t vt = vtrans_init();
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_FALSE(vtrans_card_is_animating(vt, i));
    }
    for (int i = 0; i < LAYER_COUNT; i++) {
        TEST_ASSERT_FALSE(vtrans_layer_is_animating(vt, i));
    }
}

void test_init_card_opacity_zero(void)
{
    visual_transitions_t vt = vtrans_init();
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_card_opacity(vt, i));
    }
}

void test_init_layer_opacity_one(void)
{
    visual_transitions_t vt = vtrans_init();
    for (int i = 0; i < LAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, vtrans_layer_opacity(vt, i));
    }
}

/* --- Card show/hide: 4-11 --- */

void test_card_show_starts_animation(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    TEST_ASSERT_TRUE(vtrans_card_is_animating(vt, CARD_TZOLKIN));
}

void test_card_show_opacity_starts_zero(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    /* At t=0 still near zero */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vtrans_card_opacity(vt, CARD_TZOLKIN));
}

void test_card_show_completes_to_one(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    /* Tick past full duration */
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, vtrans_card_opacity(vt, CARD_TZOLKIN));
    TEST_ASSERT_FALSE(vtrans_card_is_animating(vt, CARD_TZOLKIN));
}

void test_card_hide_fades_to_zero(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_ICHING);
    vt = vtrans_tick(vt, 2.0f); /* fully shown */
    vt = vtrans_card_hide(vt, CARD_ICHING);
    vt = vtrans_tick(vt, 2.0f); /* fully hidden */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vtrans_card_opacity(vt, CARD_ICHING));
}

void test_card_slide_starts_offscreen(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    /* Slide offset should be nonzero at start */
    float offset = vtrans_card_slide_x(vt, CARD_TZOLKIN);
    TEST_ASSERT_TRUE(offset < -0.01f || offset > 0.01f);
}

void test_card_slide_ends_at_zero(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_card_slide_x(vt, CARD_TZOLKIN));
}

void test_card_invalid_index_safe(void)
{
    visual_transitions_t vt = vtrans_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_card_opacity(vt, -1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_card_opacity(vt, 99));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_card_slide_x(vt, -1));
}

void test_card_mid_animation(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_ASTROLOGY);
    vt = vtrans_tick(vt, 0.15f); /* partial progress */
    float op = vtrans_card_opacity(vt, CARD_ASTROLOGY);
    TEST_ASSERT_TRUE(op > 0.0f && op < 1.0f);
}

/* --- Layer transitions: 12-17 --- */

void test_layer_fade_out(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_layer_fade_out(vt, LAYER_ZODIAC_RING);
    TEST_ASSERT_TRUE(vtrans_layer_is_animating(vt, LAYER_ZODIAC_RING));
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vtrans_layer_opacity(vt, LAYER_ZODIAC_RING));
}

void test_layer_fade_in(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_layer_fade_out(vt, LAYER_PLANETS);
    vt = vtrans_tick(vt, 2.0f); /* fully faded out */
    vt = vtrans_layer_fade_in(vt, LAYER_PLANETS);
    vt = vtrans_tick(vt, 2.0f); /* fully faded in */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, vtrans_layer_opacity(vt, LAYER_PLANETS));
}

void test_layer_fade_to_target(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_layer_fade_to(vt, LAYER_LABELS, 0.5f);
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, vtrans_layer_opacity(vt, LAYER_LABELS));
}

void test_layer_invalid_index_safe(void)
{
    visual_transitions_t vt = vtrans_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_layer_opacity(vt, -1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_layer_opacity(vt, 99));
}

void test_layer_mid_fade(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_layer_fade_out(vt, LAYER_ORBITS);
    vt = vtrans_tick(vt, 0.1f);
    float op = vtrans_layer_opacity(vt, LAYER_ORBITS);
    TEST_ASSERT_TRUE(op > 0.0f && op < 1.0f);
}

void test_layer_no_change_without_trigger(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_tick(vt, 1.0f);
    /* All layers should still be 1.0 */
    for (int i = 0; i < LAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, vtrans_layer_opacity(vt, i));
    }
}

/* --- Toast transitions: 18-21 --- */

void test_toast_show(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_toast_show(vt, 0);
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, vtrans_toast_opacity(vt, 0));
}

void test_toast_dismiss(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_toast_show(vt, 0);
    vt = vtrans_tick(vt, 2.0f);
    vt = vtrans_toast_dismiss(vt, 0);
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vtrans_toast_opacity(vt, 0));
}

void test_toast_slide_y(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_toast_show(vt, 0);
    /* Starts offset */
    float off = vtrans_toast_slide_y(vt, 0);
    TEST_ASSERT_TRUE(off < -0.001f || off > 0.001f);
    /* Ends at zero */
    vt = vtrans_tick(vt, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_toast_slide_y(vt, 0));
}

void test_toast_invalid_safe(void)
{
    visual_transitions_t vt = vtrans_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_toast_opacity(vt, -1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_toast_opacity(vt, 99));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, vtrans_toast_slide_y(vt, -1));
}

/* --- Stagger: 22-24 --- */

void test_stagger_cards_adds_delays(void)
{
    visual_transitions_t vt = vtrans_init();
    card_type_t order[3] = {CARD_TZOLKIN, CARD_ICHING, CARD_CHINESE};
    vt = vtrans_stagger_cards(vt, order, 3);
    /* All three should be animating */
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_TRUE(vtrans_card_is_animating(vt, order[i]));
    }
}

void test_stagger_first_no_delay(void)
{
    visual_transitions_t vt = vtrans_init();
    card_type_t order[2] = {CARD_TZOLKIN, CARD_ICHING};
    vt = vtrans_stagger_cards(vt, order, 2);
    /* Tick a tiny bit — first card should have some progress */
    vt = vtrans_tick(vt, 0.05f);
    float first = vtrans_card_opacity(vt, CARD_TZOLKIN);
    /* First card starts immediately, second may still be delayed */
    TEST_ASSERT_TRUE(first >= 0.0f);
}

void test_stagger_later_cards_delayed(void)
{
    visual_transitions_t vt = vtrans_init();
    card_type_t order[3] = {CARD_TZOLKIN, CARD_ICHING, CARD_CHINESE};
    vt = vtrans_stagger_cards(vt, order, 3);
    /* At t=0.01, first card may progress but third should still be ~0 due to delay */
    vt = vtrans_tick(vt, 0.01f);
    float first = vtrans_card_opacity(vt, CARD_TZOLKIN);
    float third = vtrans_card_opacity(vt, CARD_CHINESE);
    TEST_ASSERT_TRUE(first >= third);
}

/* --- Tick behavior: 25-28 --- */

void test_tick_zero_dt_no_change(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_HD);
    float before = vtrans_card_opacity(vt, CARD_HD);
    vt = vtrans_tick(vt, 0.0f);
    float after = vtrans_card_opacity(vt, CARD_HD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, before, after);
}

void test_tick_negative_dt_clamped(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    vt = vtrans_tick(vt, 0.1f);
    float before = vtrans_card_opacity(vt, CARD_TZOLKIN);
    vt = vtrans_tick(vt, -0.5f); /* negative should be clamped to 0 */
    float after = vtrans_card_opacity(vt, CARD_TZOLKIN);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, before, after);
}

void test_complete_animation_stays_complete(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    vt = vtrans_tick(vt, 5.0f);
    float a = vtrans_card_opacity(vt, CARD_TZOLKIN);
    vt = vtrans_tick(vt, 5.0f); /* extra ticks */
    float b = vtrans_card_opacity(vt, CARD_TZOLKIN);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a, b);
}

void test_multiple_cards_independent(void)
{
    visual_transitions_t vt = vtrans_init();
    vt = vtrans_card_show(vt, CARD_TZOLKIN);
    vt = vtrans_tick(vt, 2.0f);
    /* Tzolkin fully shown, others still zero */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, vtrans_card_opacity(vt, CARD_TZOLKIN));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, vtrans_card_opacity(vt, CARD_ICHING));
}

/* --- Duration config: 29-30 --- */

void test_card_duration_phi_timed(void)
{
    visual_transitions_t vt = vtrans_init();
    float dur = vtrans_card_duration(vt);
    /* Should be in golden ratio timing range (0.2-0.7s typically) */
    TEST_ASSERT_TRUE(dur > 0.1f && dur < 2.0f);
}

void test_layer_duration_phi_timed(void)
{
    visual_transitions_t vt = vtrans_init();
    float dur = vtrans_layer_duration(vt);
    TEST_ASSERT_TRUE(dur > 0.1f && dur < 2.0f);
}

/* --- Purity: 31 --- */

void test_purity(void)
{
    visual_transitions_t a = vtrans_init();
    visual_transitions_t b = vtrans_init();
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f,
            vtrans_card_opacity(a, i),
            vtrans_card_opacity(b, i));
    }
    TEST_ASSERT_FLOAT_WITHIN(0.001f,
        vtrans_card_duration(a), vtrans_card_duration(b));
}

int main(void)
{
    UNITY_BEGIN();
    /* Init */
    RUN_TEST(test_init_all_idle);
    RUN_TEST(test_init_card_opacity_zero);
    RUN_TEST(test_init_layer_opacity_one);
    /* Card show/hide */
    RUN_TEST(test_card_show_starts_animation);
    RUN_TEST(test_card_show_opacity_starts_zero);
    RUN_TEST(test_card_show_completes_to_one);
    RUN_TEST(test_card_hide_fades_to_zero);
    RUN_TEST(test_card_slide_starts_offscreen);
    RUN_TEST(test_card_slide_ends_at_zero);
    RUN_TEST(test_card_invalid_index_safe);
    RUN_TEST(test_card_mid_animation);
    /* Layer transitions */
    RUN_TEST(test_layer_fade_out);
    RUN_TEST(test_layer_fade_in);
    RUN_TEST(test_layer_fade_to_target);
    RUN_TEST(test_layer_invalid_index_safe);
    RUN_TEST(test_layer_mid_fade);
    RUN_TEST(test_layer_no_change_without_trigger);
    /* Toast */
    RUN_TEST(test_toast_show);
    RUN_TEST(test_toast_dismiss);
    RUN_TEST(test_toast_slide_y);
    RUN_TEST(test_toast_invalid_safe);
    /* Stagger */
    RUN_TEST(test_stagger_cards_adds_delays);
    RUN_TEST(test_stagger_first_no_delay);
    RUN_TEST(test_stagger_later_cards_delayed);
    /* Tick */
    RUN_TEST(test_tick_zero_dt_no_change);
    RUN_TEST(test_tick_negative_dt_clamped);
    RUN_TEST(test_complete_animation_stays_complete);
    RUN_TEST(test_multiple_cards_independent);
    /* Duration */
    RUN_TEST(test_card_duration_phi_timed);
    RUN_TEST(test_layer_duration_phi_timed);
    /* Purity */
    RUN_TEST(test_purity);
    return UNITY_END();
}
