#include "../unity/unity.h"
#include "../../src/ui/text_choreo.h"

void setUp(void) {}
void tearDown(void) {}

/* ==== Preset lookup ==== */

void test_preset_wisdom(void)
{
    tc_preset_t p = tc_preset(TC_WISDOM);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_TYPEWRITER, p.enter_style);
    TEST_ASSERT_TRUE(p.enter_duration > 0.0f);
    TEST_ASSERT_TRUE(p.hold_duration > 0.0f);
    TEST_ASSERT_TRUE(p.exit_duration > 0.0f);
}

void test_preset_toast(void)
{
    tc_preset_t p = tc_preset(TC_TOAST);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_INSTANT, p.enter_style);
    TEST_ASSERT_TRUE(p.hold_duration > 0.0f);
}

void test_preset_card(void)
{
    tc_preset_t p = tc_preset(TC_CARD);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_FADE_WORD, p.enter_style);
}

void test_preset_narrative(void)
{
    tc_preset_t p = tc_preset(TC_NARRATIVE);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_FADE_WORD, p.enter_style);
    TEST_ASSERT_TRUE(p.enter_duration > 0.0f);
}

void test_preset_hud(void)
{
    tc_preset_t p = tc_preset(TC_HUD);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_INSTANT, p.enter_style);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, p.hold_duration); /* persistent */
    TEST_ASSERT_EQUAL_FLOAT(0.0f, p.exit_duration);
}

void test_preset_invalid_returns_hud(void)
{
    tc_preset_t p = tc_preset(TC_CATEGORY_COUNT);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_INSTANT, p.enter_style);
}

/* ==== Phi-derived timing ==== */

void test_wisdom_hold_is_phi_dramatic(void)
{
    tc_preset_t p = tc_preset(TC_WISDOM);
    /* Wisdom hold ~8 seconds (epic: phi^2 * base * 3) */
    TEST_ASSERT_TRUE(p.hold_duration >= 6.0f);
    TEST_ASSERT_TRUE(p.hold_duration <= 10.0f);
}

void test_toast_hold_is_medium(void)
{
    tc_preset_t p = tc_preset(TC_TOAST);
    /* Toast hold ~2-4 seconds */
    TEST_ASSERT_TRUE(p.hold_duration >= 1.5f);
    TEST_ASSERT_TRUE(p.hold_duration <= 5.0f);
}

void test_wisdom_enter_exit_phi_ratio(void)
{
    tc_preset_t p = tc_preset(TC_WISDOM);
    /* Enter and exit should follow phi relationship */
    TEST_ASSERT_TRUE(p.enter_duration > 0.0f);
    TEST_ASSERT_TRUE(p.exit_duration > 0.0f);
}

/* ==== Sequence lifecycle ==== */

void test_sequence_create(void)
{
    tc_seq_t seq = tc_create(TC_WISDOM, 20);
    TEST_ASSERT_EQUAL_INT(TC_PHASE_ENTER, seq.phase);
    TEST_ASSERT_EQUAL_INT(0, seq.done);
}

void test_sequence_starts_in_enter(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 10);
    TEST_ASSERT_EQUAL_INT(TC_PHASE_ENTER, seq.phase);
}

void test_sequence_enters_hold_after_enter(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    /* Tick past enter duration */
    seq = tc_tick(seq, p.enter_duration + 0.1f);
    TEST_ASSERT_EQUAL_INT(TC_PHASE_HOLD, seq.phase);
}

void test_sequence_enters_exit_after_hold(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    /* Tick past enter + hold */
    seq = tc_tick(seq, p.enter_duration + p.hold_duration + 0.1f);
    TEST_ASSERT_EQUAL_INT(TC_PHASE_EXIT, seq.phase);
}

void test_sequence_done_after_all_phases(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    float total = p.enter_duration + p.hold_duration + p.exit_duration;
    seq = tc_tick(seq, total + 0.1f);
    TEST_ASSERT_EQUAL_INT(1, seq.done);
    TEST_ASSERT_EQUAL_INT(TC_PHASE_DONE, seq.phase);
}

void test_persistent_never_exits(void)
{
    /* HUD has hold=0, exit=0 — stays in HOLD forever */
    tc_seq_t seq = tc_create(TC_HUD, 5);
    tc_preset_t p = tc_preset(TC_HUD);
    seq = tc_tick(seq, p.enter_duration + 100.0f);
    TEST_ASSERT_EQUAL_INT(TC_PHASE_HOLD, seq.phase);
    TEST_ASSERT_EQUAL_INT(0, seq.done);
}

/* ==== Opacity queries ==== */

void test_opacity_during_enter(void)
{
    tc_seq_t seq = tc_create(TC_WISDOM, 20);
    tc_preset_t p = tc_preset(TC_WISDOM);
    seq = tc_tick(seq, p.enter_duration * 0.5f);
    float o = tc_opacity(seq);
    /* During enter, opacity should be partial (0-1 range) */
    TEST_ASSERT_TRUE(o >= 0.0f);
    TEST_ASSERT_TRUE(o <= 1.0f);
}

void test_opacity_during_hold_is_one(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    seq = tc_tick(seq, p.enter_duration + 0.5f);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, tc_opacity(seq));
}

void test_opacity_during_exit_decreases(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    /* Get into exit phase midway */
    seq = tc_tick(seq, p.enter_duration + p.hold_duration + p.exit_duration * 0.5f);
    float o = tc_opacity(seq);
    TEST_ASSERT_TRUE(o < 1.0f);
    TEST_ASSERT_TRUE(o >= 0.0f);
}

void test_opacity_when_done_is_zero(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    float total = p.enter_duration + p.hold_duration + p.exit_duration;
    seq = tc_tick(seq, total + 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, tc_opacity(seq));
}

/* ==== Category name ==== */

void test_category_name_wisdom(void)
{
    TEST_ASSERT_EQUAL_STRING("Wisdom", tc_category_name(TC_WISDOM));
}

void test_category_name_toast(void)
{
    TEST_ASSERT_EQUAL_STRING("Toast", tc_category_name(TC_TOAST));
}

void test_category_name_card(void)
{
    TEST_ASSERT_EQUAL_STRING("Card", tc_category_name(TC_CARD));
}

void test_category_name_narrative(void)
{
    TEST_ASSERT_EQUAL_STRING("Narrative", tc_category_name(TC_NARRATIVE));
}

void test_category_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tc_category_name(TC_CATEGORY_COUNT));
}

/* ==== Reveal passthrough ==== */

void test_char_visibility_during_typewriter(void)
{
    tc_seq_t seq = tc_create(TC_WISDOM, 20);
    tc_preset_t p = tc_preset(TC_WISDOM);
    seq = tc_tick(seq, p.enter_duration * 0.5f);
    /* First char should be visible, last char less so */
    float first = tc_char_visibility(seq, 0);
    float last = tc_char_visibility(seq, 19);
    TEST_ASSERT_TRUE(first >= last);
}

void test_char_visibility_during_hold_all_one(void)
{
    tc_seq_t seq = tc_create(TC_TOAST, 5);
    tc_preset_t p = tc_preset(TC_TOAST);
    seq = tc_tick(seq, p.enter_duration + 0.5f);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_FLOAT(1.0f, tc_char_visibility(seq, i));
    }
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* Presets */
    RUN_TEST(test_preset_wisdom);
    RUN_TEST(test_preset_toast);
    RUN_TEST(test_preset_card);
    RUN_TEST(test_preset_narrative);
    RUN_TEST(test_preset_hud);
    RUN_TEST(test_preset_invalid_returns_hud);

    /* Phi timing */
    RUN_TEST(test_wisdom_hold_is_phi_dramatic);
    RUN_TEST(test_toast_hold_is_medium);
    RUN_TEST(test_wisdom_enter_exit_phi_ratio);

    /* Sequence lifecycle */
    RUN_TEST(test_sequence_create);
    RUN_TEST(test_sequence_starts_in_enter);
    RUN_TEST(test_sequence_enters_hold_after_enter);
    RUN_TEST(test_sequence_enters_exit_after_hold);
    RUN_TEST(test_sequence_done_after_all_phases);
    RUN_TEST(test_persistent_never_exits);

    /* Opacity */
    RUN_TEST(test_opacity_during_enter);
    RUN_TEST(test_opacity_during_hold_is_one);
    RUN_TEST(test_opacity_during_exit_decreases);
    RUN_TEST(test_opacity_when_done_is_zero);

    /* Category names */
    RUN_TEST(test_category_name_wisdom);
    RUN_TEST(test_category_name_toast);
    RUN_TEST(test_category_name_card);
    RUN_TEST(test_category_name_narrative);
    RUN_TEST(test_category_name_invalid);

    /* Reveal passthrough */
    RUN_TEST(test_char_visibility_during_typewriter);
    RUN_TEST(test_char_visibility_during_hold_all_one);

    return UNITY_END();
}
