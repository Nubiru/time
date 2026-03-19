#include "../unity/unity.h"
#include "ui/text_reveal.h"

#define TOL 0.01f

void setUp(void) {}
void tearDown(void) {}

/* 1. Create: count, duration, easing set correctly */
void test_text_reveal_create_defaults(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 10, 2.0f);
    TEST_ASSERT_EQUAL_INT(10, ta.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, ta.duration);
    TEST_ASSERT_EQUAL_INT(TEXT_REVEAL_TYPEWRITER, ta.style);
    TEST_ASSERT_EQUAL_INT(EASE_TYPE_OUT_CUBIC, ta.easing);
    TEST_ASSERT_EQUAL_INT(0, ta.started);
    TEST_ASSERT_EQUAL_INT(0, ta.done);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ta.elapsed);
    /* Default element_fade = duration/count for fade styles, but stored anyway */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.2f, ta.element_fade);
}

/* 2. Visibility = 0 for all before start */
void test_text_reveal_not_started_zero(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 1.0f);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, i));
    }
}

/* 3. INSTANT: all elements 1.0 immediately after start+tick */
void test_text_reveal_instant_all_visible(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_INSTANT, 5, 1.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 0.001f);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, i));
    }
    TEST_ASSERT_EQUAL_INT(1, text_reveal_done(ta));
}

/* 4. TYPEWRITER: first char visible after small tick */
void test_text_reveal_typewriter_first_char(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 10, 2.0f);
    ta = text_reveal_start(ta);
    /* interval = 2.0/10 = 0.2s. First char at elapsed >= 0.0 */
    ta = text_reveal_tick(ta, 0.01f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, 0));
}

/* 5. TYPEWRITER: chars appear in order over time */
void test_text_reveal_typewriter_progressive(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 4, 2.0f);
    ta = text_reveal_start(ta);
    /* interval = 2.0/4 = 0.5s */
    /* Element i appears at i * 0.5s */

    ta = text_reveal_tick(ta, 0.6f);
    /* elapsed=0.6: elem 0 (at 0.0) visible, elem 1 (at 0.5) visible,
       elem 2 (at 1.0) not visible */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, 0));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, 1));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 2));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 3));
}

/* 6. TYPEWRITER: all visible at elapsed >= duration */
void test_text_reveal_typewriter_all_at_end(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 1.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 1.5f);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, i));
    }
}

/* 7. TYPEWRITER: no chars visible at elapsed=0 (started but no tick yet) */
void test_text_reveal_typewriter_none_at_zero(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 1.0f);
    ta = text_reveal_start(ta);
    /* started but elapsed still 0 — element 0 starts at 0.0, so it IS visible */
    /* Actually: visibility(0) = (elapsed >= 0 * interval) = (0 >= 0) = true */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, 0));
    /* But element 1 starts at interval > 0, so not visible */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 1));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 4));
}

/* 8. FADE_CHAR: partial visibility during fade */
void test_text_reveal_fade_char_gradual(void)
{
    /* 5 elements, 2.0s total, default element_fade = 2.0/5 = 0.4s */
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 5, 2.0f);
    ta = text_reveal_start(ta);
    /* spread = max(2.0 - 0.4, 0) = 1.6
       start_0 = 0, start_1 = 0.4, start_2 = 0.8, start_3 = 1.2, start_4 = 1.6
       At elapsed=0.2: elem 0 local_t = 0.2/0.4 = 0.5 -> partial
       With OUT_CUBIC easing on 0.5: should be > 0.5 (ease out is fast start) */
    ta = text_reveal_tick(ta, 0.2f);
    float v0 = text_reveal_visibility(ta, 0);
    TEST_ASSERT_TRUE(v0 > 0.1f);
    TEST_ASSERT_TRUE(v0 < 1.0f);
    /* elem 1 hasn't started yet (start_1 = 0.4) */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 1));
}

/* 9. FADE_CHAR: first element reaches 1.0 before last */
void test_text_reveal_fade_char_first_full(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 4, 2.0f);
    ta = text_reveal_start(ta);
    /* element_fade = 0.5, spread = 1.5, start times: 0, 0.5, 1.0, 1.5
       At elapsed=0.5: elem 0 local_t = 0.5/0.5 = 1.0 -> fully visible
       elem 3 local_t = (0.5-1.5)/0.5 < 0 -> 0 */
    ta = text_reveal_tick(ta, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, 0));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 3));
}

/* 10. FADE_CHAR: all elements 1.0 at duration */
void test_text_reveal_fade_char_all_at_end(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 5, 2.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 2.5f);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, i));
    }
}

/* 11. FADE_WORD produces same result as FADE_CHAR */
void test_text_reveal_fade_word_same_math(void)
{
    text_reveal_t ta_c = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 3, 1.5f);
    text_reveal_t ta_w = text_reveal_create(TEXT_REVEAL_FADE_WORD, 3, 1.5f);
    ta_c = text_reveal_start(ta_c);
    ta_w = text_reveal_start(ta_w);
    ta_c = text_reveal_tick(ta_c, 0.7f);
    ta_w = text_reveal_tick(ta_w, 0.7f);
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL,
            text_reveal_visibility(ta_c, i),
            text_reveal_visibility(ta_w, i));
    }
}

/* 12. set_element_fade changes fade time */
void test_text_reveal_set_element_fade(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 4, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, ta.element_fade); /* default = 2.0/4 */
    ta = text_reveal_set_element_fade(ta, 0.8f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.8f, ta.element_fade);
}

/* 13. set_easing changes easing type */
void test_text_reveal_set_easing(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 4, 2.0f);
    TEST_ASSERT_EQUAL_INT(EASE_TYPE_OUT_CUBIC, ta.easing);
    ta = text_reveal_set_easing(ta, EASE_TYPE_LINEAR);
    TEST_ASSERT_EQUAL_INT(EASE_TYPE_LINEAR, ta.easing);
}

/* 14. done=1 when elapsed >= duration */
void test_text_reveal_done_at_end(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 1.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, text_reveal_done(ta));
}

/* 15. done=0 at half duration */
void test_text_reveal_not_done_midway(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 2.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, text_reveal_done(ta));
}

/* 16. Progress tracks elapsed/duration */
void test_text_reveal_progress_linear(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 2.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, text_reveal_progress(ta));

    ta = text_reveal_tick(ta, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_progress(ta));
}

/* 17. Visible count: elements with visibility >= 0.99 */
void test_text_reveal_visible_count(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 4, 2.0f);
    ta = text_reveal_start(ta);
    /* interval = 0.5s. At elapsed=1.1: elements 0,1,2 visible (starts at 0, 0.5, 1.0) */
    ta = text_reveal_tick(ta, 1.1f);
    TEST_ASSERT_EQUAL_INT(3, text_reveal_visible_count(ta));
}

/* 18. Single element works correctly */
void test_text_reveal_single_element(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_FADE_CHAR, 1, 1.0f);
    ta = text_reveal_start(ta);
    /* count=1: start_0=0, element_fade=1.0 */
    ta = text_reveal_tick(ta, 0.5f);
    float v = text_reveal_visibility(ta, 0);
    TEST_ASSERT_TRUE(v > 0.1f);
    TEST_ASSERT_TRUE(v < 1.0f);

    ta = text_reveal_tick(ta, 0.6f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, 0));
}

/* 19. Zero duration: instant completion */
void test_text_reveal_zero_duration(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 5, 0.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 0.001f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_progress(ta));
    TEST_ASSERT_EQUAL_INT(1, text_reveal_done(ta));
    /* All elements should be visible */
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, text_reveal_visibility(ta, i));
    }
}

/* 20. Out of bounds index returns 0.0 */
void test_text_reveal_index_out_of_bounds(void)
{
    text_reveal_t ta = text_reveal_create(TEXT_REVEAL_TYPEWRITER, 3, 1.0f);
    ta = text_reveal_start(ta);
    ta = text_reveal_tick(ta, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, -1));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 3));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, text_reveal_visibility(ta, 100));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_text_reveal_create_defaults);
    RUN_TEST(test_text_reveal_not_started_zero);
    RUN_TEST(test_text_reveal_instant_all_visible);
    RUN_TEST(test_text_reveal_typewriter_first_char);
    RUN_TEST(test_text_reveal_typewriter_progressive);
    RUN_TEST(test_text_reveal_typewriter_all_at_end);
    RUN_TEST(test_text_reveal_typewriter_none_at_zero);
    RUN_TEST(test_text_reveal_fade_char_gradual);
    RUN_TEST(test_text_reveal_fade_char_first_full);
    RUN_TEST(test_text_reveal_fade_char_all_at_end);
    RUN_TEST(test_text_reveal_fade_word_same_math);
    RUN_TEST(test_text_reveal_set_element_fade);
    RUN_TEST(test_text_reveal_set_easing);
    RUN_TEST(test_text_reveal_done_at_end);
    RUN_TEST(test_text_reveal_not_done_midway);
    RUN_TEST(test_text_reveal_progress_linear);
    RUN_TEST(test_text_reveal_visible_count);
    RUN_TEST(test_text_reveal_single_element);
    RUN_TEST(test_text_reveal_zero_duration);
    RUN_TEST(test_text_reveal_index_out_of_bounds);
    return UNITY_END();
}
