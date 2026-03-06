#include "../unity/unity.h"
#include "../../src/ui/animation.h"

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Create: state=IDLE, current=from, elapsed=0, progress=0 */
void test_create(void)
{
    animation_t a = anim_create(10.0f, 50.0f, 2.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_EQUAL_INT(ANIM_IDLE, a.state);
    TEST_ASSERT_TRUE(a.current == 10.0f);
    TEST_ASSERT_TRUE(a.elapsed == 0.0f);
    TEST_ASSERT_TRUE(a.progress == 0.0f);
    TEST_ASSERT_TRUE(a.from == 10.0f);
    TEST_ASSERT_TRUE(a.to == 50.0f);
}

/* 2. Create delayed: delay set correctly */
void test_create_delayed(void)
{
    animation_t a = anim_create_delayed(0, 100, 1.0f, EASE_TYPE_LINEAR, 0.5f);
    TEST_ASSERT_TRUE(a.delay == 0.5f);
    TEST_ASSERT_EQUAL_INT(ANIM_IDLE, a.state);
}

/* 3. Start: IDLE -> RUNNING */
void test_start(void)
{
    animation_t a = anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    TEST_ASSERT_EQUAL_INT(ANIM_RUNNING, a.state);
}

/* 4. Pause: RUNNING -> PAUSED */
void test_pause(void)
{
    animation_t a = anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_pause(a);
    TEST_ASSERT_EQUAL_INT(ANIM_PAUSED, a.state);
}

/* 5. Reset: COMPLETE -> IDLE, current=from, elapsed=0 */
void test_reset(void)
{
    animation_t a = anim_create(5.0f, 10.0f, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 2.0f);
    TEST_ASSERT_EQUAL_INT(ANIM_COMPLETE, a.state);
    a = anim_reset(a);
    TEST_ASSERT_EQUAL_INT(ANIM_IDLE, a.state);
    TEST_ASSERT_TRUE(a.current == 5.0f);
    TEST_ASSERT_TRUE(a.elapsed == 0.0f);
}

/* 6. Restart: any -> RUNNING with elapsed=0 */
void test_restart(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 2.0f);
    TEST_ASSERT_EQUAL_INT(ANIM_COMPLETE, a.state);
    a = anim_restart(a);
    TEST_ASSERT_EQUAL_INT(ANIM_RUNNING, a.state);
    TEST_ASSERT_TRUE(a.elapsed == 0.0f);
}

/* 7. IDLE: tick does not advance */
void test_tick_idle(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_tick(a, 0.5f);
    TEST_ASSERT_TRUE(a.elapsed == 0.0f);
    TEST_ASSERT_EQUAL_INT(ANIM_IDLE, a.state);
}

/* 8. RUNNING: tick advances elapsed */
void test_tick_running(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 0.3f);
    TEST_ASSERT_TRUE(NEAR(0.3f, a.elapsed));
    TEST_ASSERT_EQUAL_INT(ANIM_RUNNING, a.state);
}

/* 9. PAUSED: tick does not advance */
void test_tick_paused(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 0.3f);
    a = anim_pause(a);
    float saved = a.elapsed;
    a = anim_tick(a, 0.5f);
    TEST_ASSERT_TRUE(a.elapsed == saved);
}

/* 10. COMPLETE: tick does not advance */
void test_tick_complete(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 2.0f);
    TEST_ASSERT_EQUAL_INT(ANIM_COMPLETE, a.state);
    float saved = a.elapsed;
    a = anim_tick(a, 1.0f);
    TEST_ASSERT_TRUE(a.elapsed == saved);
}

/* 11. Full duration: state=COMPLETE, current=to */
void test_full_duration(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 1.5f);
    TEST_ASSERT_EQUAL_INT(ANIM_COMPLETE, a.state);
    TEST_ASSERT_TRUE(NEAR(100.0f, a.current));
}

/* 12. Mid-animation: current between from and to */
void test_mid_animation(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    a = anim_tick(a, 0.5f);
    TEST_ASSERT_TRUE(a.current > 0.0f && a.current < 100.0f);
    TEST_ASSERT_TRUE(NEAR(50.0f, a.current));
}

/* 13. Linear at t=0.5: eased = 0.5 */
void test_easing_linear(void)
{
    float e = anim_apply_easing(EASE_TYPE_LINEAR, 0.5f);
    TEST_ASSERT_TRUE(NEAR(0.5f, e));
}

/* 14. In-quad at t=0.5: eased < 0.5 */
void test_easing_in_quad(void)
{
    float e = anim_apply_easing(EASE_TYPE_IN_QUAD, 0.5f);
    TEST_ASSERT_TRUE(e < 0.5f);
}

/* 15. Different easing types produce different midpoint values */
void test_easing_types_differ(void)
{
    float linear = anim_apply_easing(EASE_TYPE_LINEAR, 0.5f);
    float in_quad = anim_apply_easing(EASE_TYPE_IN_QUAD, 0.5f);
    float out_quad = anim_apply_easing(EASE_TYPE_OUT_QUAD, 0.5f);
    TEST_ASSERT_TRUE(linear != in_quad);
    TEST_ASSERT_TRUE(linear != out_quad);
    TEST_ASSERT_TRUE(in_quad != out_quad);
}

/* 16. During delay: current stays at from */
void test_delay_holds(void)
{
    animation_t a = anim_create_delayed(0, 100, 1.0f, EASE_TYPE_LINEAR, 0.5f);
    a = anim_start(a);
    a = anim_tick(a, 0.3f);
    TEST_ASSERT_TRUE(a.current == 0.0f);
    TEST_ASSERT_EQUAL_INT(ANIM_RUNNING, a.state);
}

/* 17. After delay: animation progresses normally */
void test_delay_then_progress(void)
{
    animation_t a = anim_create_delayed(0, 100, 1.0f, EASE_TYPE_LINEAR, 0.5f);
    a = anim_start(a);
    a = anim_tick(a, 1.0f);
    /* 0.5s delay + 0.5s animation: raw_t = 0.5, current ~ 50 */
    TEST_ASSERT_TRUE(a.current > 40.0f && a.current < 60.0f);
}

/* 18. Loop=1: resets and continues after completion */
void test_loop_resets(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_set_loop(a, 1);
    a = anim_start(a);
    a = anim_tick(a, 1.5f);
    TEST_ASSERT_EQUAL_INT(ANIM_RUNNING, a.state);
    TEST_ASSERT_TRUE(a.iteration > 0);
}

/* 19. Loop=2 (ping-pong): from/to swap, continues */
void test_pingpong(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_set_loop(a, 2);
    a = anim_start(a);

    /* First half */
    a = anim_tick(a, 0.5f);
    TEST_ASSERT_TRUE(a.current > 40.0f);

    /* Complete first cycle -> from/to swap */
    a = anim_tick(a, 0.6f);
    TEST_ASSERT_EQUAL_INT(ANIM_RUNNING, a.state);
    TEST_ASSERT_EQUAL_INT(1, a.iteration);

    /* Now reversed: from=100, to=0. Tick midway */
    a = anim_tick(a, 0.5f);
    TEST_ASSERT_TRUE(a.current < 60.0f);
}

/* 20. animation_value returns current */
void test_value(void)
{
    animation_t a = anim_create(10.0f, 20.0f, 1.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_TRUE(animation_value(a) == 10.0f);
    a = anim_start(a);
    a = anim_tick(a, 0.5f);
    TEST_ASSERT_TRUE(animation_value(a) == a.current);
}

/* 21. anim_is_complete true only when COMPLETE */
void test_is_complete(void)
{
    animation_t a = anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_FALSE(anim_is_complete(a));
    a = anim_start(a);
    TEST_ASSERT_FALSE(anim_is_complete(a));
    a = anim_tick(a, 2.0f);
    TEST_ASSERT_TRUE(anim_is_complete(a));
}

/* 22. anim_is_running true only when RUNNING */
void test_is_running(void)
{
    animation_t a = anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_FALSE(anim_is_running(a));
    a = anim_start(a);
    TEST_ASSERT_TRUE(anim_is_running(a));
    a = anim_pause(a);
    TEST_ASSERT_FALSE(anim_is_running(a));
}

/* 23. Empty sequence is complete */
void test_seq_empty(void)
{
    anim_sequence_t seq = anim_seq_create();
    TEST_ASSERT_TRUE(anim_seq_is_complete(seq));
}

/* 24. Sequence advances through animations in order */
void test_seq_advances(void)
{
    anim_sequence_t seq = anim_seq_create();
    seq = anim_seq_add(seq, anim_create(0, 10, 1.0f, EASE_TYPE_LINEAR));
    seq = anim_seq_add(seq, anim_create(10, 20, 1.0f, EASE_TYPE_LINEAR));

    seq = anim_seq_tick(seq, 0.5f);
    TEST_ASSERT_EQUAL_INT(0, seq.current_index);

    /* Complete first animation */
    seq = anim_seq_tick(seq, 0.6f);
    TEST_ASSERT_EQUAL_INT(1, seq.current_index);

    /* Complete second animation */
    seq = anim_seq_tick(seq, 1.1f);
    TEST_ASSERT_TRUE(anim_seq_is_complete(seq));
}

/* 25. Sequence value returns current animation's value */
void test_seq_value(void)
{
    anim_sequence_t seq = anim_seq_create();
    seq = anim_seq_add(seq, anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR));
    seq = anim_seq_tick(seq, 0.5f);
    float v = anim_seq_value(seq);
    TEST_ASSERT_TRUE(v > 40.0f && v < 60.0f);
}

/* 26. Purity: tick returns new struct, original unchanged */
void test_purity(void)
{
    animation_t a = anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR);
    a = anim_start(a);
    animation_t original = a;
    animation_t ticked = anim_tick(a, 0.5f);

    /* Original unchanged */
    TEST_ASSERT_TRUE(a.elapsed == original.elapsed);
    TEST_ASSERT_TRUE(a.current == original.current);

    /* Ticked is different */
    TEST_ASSERT_TRUE(ticked.elapsed != a.elapsed);
    TEST_ASSERT_TRUE(ticked.current != a.current);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_create_delayed);
    RUN_TEST(test_start);
    RUN_TEST(test_pause);
    RUN_TEST(test_reset);
    RUN_TEST(test_restart);
    RUN_TEST(test_tick_idle);
    RUN_TEST(test_tick_running);
    RUN_TEST(test_tick_paused);
    RUN_TEST(test_tick_complete);
    RUN_TEST(test_full_duration);
    RUN_TEST(test_mid_animation);
    RUN_TEST(test_easing_linear);
    RUN_TEST(test_easing_in_quad);
    RUN_TEST(test_easing_types_differ);
    RUN_TEST(test_delay_holds);
    RUN_TEST(test_delay_then_progress);
    RUN_TEST(test_loop_resets);
    RUN_TEST(test_pingpong);
    RUN_TEST(test_value);
    RUN_TEST(test_is_complete);
    RUN_TEST(test_is_running);
    RUN_TEST(test_seq_empty);
    RUN_TEST(test_seq_advances);
    RUN_TEST(test_seq_value);
    RUN_TEST(test_purity);
    return UNITY_END();
}
