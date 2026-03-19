#include "../unity/unity.h"
#include "../../src/render/view_state.h"

void setUp(void) {}
void tearDown(void) {}

/* --- vs_init: starts at Space View, no transition, LOD ULTRA --- */

static void test_vs_init(void) {
    view_state_t s = vs_init();
    TEST_ASSERT_EQUAL_INT(0, s.current_view);   /* VIEW_SPACE */
    TEST_ASSERT_EQUAL_INT(0, s.target_view);     /* same as current */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.transition_t);
    TEST_ASSERT_EQUAL_INT(0, s.focus_mode);      /* FOCUS_MODE_OVERVIEW */
    TEST_ASSERT_EQUAL_INT(0, s.lod_tier);        /* LOD_ULTRA */
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
}

/* --- vs_set_view: setting different view starts transition --- */

static void test_vs_set_view_starts_transition(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);  /* VIEW_EARTH */
    TEST_ASSERT_EQUAL_INT(0, s.current_view);    /* still at space */
    TEST_ASSERT_EQUAL_INT(1, s.target_view);     /* heading to earth */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.transition_t);
    TEST_ASSERT_EQUAL_INT(1, s.is_transitioning);
}

/* --- vs_set_view: setting same view is a no-op --- */

static void test_vs_set_view_same_noop(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 0);  /* VIEW_SPACE, already there */
    TEST_ASSERT_EQUAL_INT(0, s.current_view);
    TEST_ASSERT_EQUAL_INT(0, s.target_view);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.transition_t);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
}

/* --- vs_tick: transition_t advances with dt --- */

static void test_vs_tick_advances(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);  /* start transition */
    s = vs_tick(s, 0.5f);   /* half-ish of 1.618s */
    float expected = 0.5f / 1.618f;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected, s.transition_t);
    TEST_ASSERT_EQUAL_INT(1, s.is_transitioning);
}

/* --- vs_tick: transition completes after enough time --- */

static void test_vs_tick_completes(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 2);  /* VIEW_GALAXY */
    s = vs_tick(s, 2.0f);   /* more than 1.618s */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.transition_t);
    TEST_ASSERT_EQUAL_INT(2, s.current_view);    /* now at galaxy */
    TEST_ASSERT_EQUAL_INT(2, s.target_view);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
}

/* --- vs_tick: transition_t never exceeds 1.0 --- */

static void test_vs_tick_no_overshoot(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);
    s = vs_tick(s, 100.0f);  /* absurdly large dt */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.transition_t);
    TEST_ASSERT_TRUE(s.transition_t <= 1.0f);
}

/* --- vs_tick: no-op when not transitioning --- */

static void test_vs_tick_idle_noop(void) {
    view_state_t s = vs_init();
    view_state_t before = s;
    s = vs_tick(s, 1.0f);
    TEST_ASSERT_EQUAL_INT(before.current_view, s.current_view);
    TEST_ASSERT_EQUAL_INT(before.target_view, s.target_view);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, before.transition_t, s.transition_t);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
}

/* --- vs_set_focus: updates immediately --- */

static void test_vs_set_focus(void) {
    view_state_t s = vs_init();
    s = vs_set_focus(s, 3);  /* FOCUS_MODE_ICHING */
    TEST_ASSERT_EQUAL_INT(3, s.focus_mode);
}

/* --- vs_set_lod: updates immediately --- */

static void test_vs_set_lod(void) {
    view_state_t s = vs_init();
    s = vs_set_lod(s, 2);  /* LOD_MEDIUM */
    TEST_ASSERT_EQUAL_INT(2, s.lod_tier);
}

/* --- vs_schedule: space view matches ps_for_view(SPACE, LOD_ULTRA) --- */

static void test_vs_schedule_space(void) {
    view_state_t s = vs_init();
    pass_schedule_t sched = vs_schedule(&s);
    pass_schedule_t expected = ps_for_view(0, 0);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(expected.passes[i].enabled, sched.passes[i].enabled);
    }
}

/* --- vs_schedule: earth view matches ps_for_view(EARTH, LOD_ULTRA) --- */

static void test_vs_schedule_earth(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);
    s = vs_tick(s, 2.0f);  /* complete transition */
    pass_schedule_t sched = vs_schedule(&s);
    pass_schedule_t expected = ps_for_view(1, 0);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(expected.passes[i].enabled, sched.passes[i].enabled);
    }
}

/* --- vs_schedule: with focus mode applied --- */

static void test_vs_schedule_with_focus(void) {
    view_state_t s = vs_init();
    s = vs_set_focus(s, 1);  /* FOCUS_MODE_ASTROLOGY */
    pass_schedule_t sched = vs_schedule(&s);
    /* Zodiac should be full opacity, stars dimmed */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, sched.passes[PASS_ZODIAC].opacity_scale);
    TEST_ASSERT_TRUE(sched.passes[PASS_STARS].opacity_scale < 1.0f);
}

/* --- vs_blended_schedule: mid-transition interpolates opacity --- */

static void test_vs_blended_mid_transition(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);  /* space -> earth */
    s = vs_tick(s, 0.809f); /* ~50% through 1.618s */
    pass_schedule_t sched = vs_blended_schedule(&s);
    /* Zodiac pass: enabled in space (1.0), disabled in earth (0.0).
     * At ~50% transition, opacity should be roughly 0.5 */
    float zodiac_opacity = sched.passes[PASS_ZODIAC].opacity_scale;
    TEST_ASSERT_TRUE(zodiac_opacity > 0.1f);
    TEST_ASSERT_TRUE(zodiac_opacity < 0.9f);
}

/* --- vs_blended_schedule: when idle, equals normal schedule --- */

static void test_vs_blended_idle(void) {
    view_state_t s = vs_init();
    pass_schedule_t normal = vs_schedule(&s);
    pass_schedule_t blended = vs_blended_schedule(&s);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(normal.passes[i].enabled, blended.passes[i].enabled);
        TEST_ASSERT_FLOAT_WITHIN(0.001f,
            normal.passes[i].opacity_scale,
            blended.passes[i].opacity_scale);
    }
}

/* --- vs_is_transitioning: true during transition --- */

static void test_vs_is_transitioning(void) {
    view_state_t s = vs_init();
    TEST_ASSERT_EQUAL_INT(0, vs_is_transitioning(&s));
    s = vs_set_view(s, 2);
    TEST_ASSERT_EQUAL_INT(1, vs_is_transitioning(&s));
    s = vs_tick(s, 2.0f);
    TEST_ASSERT_EQUAL_INT(0, vs_is_transitioning(&s));
}

/* --- vs_transition_progress: returns 1.0 when idle --- */

static void test_vs_transition_progress_idle(void) {
    view_state_t s = vs_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, vs_transition_progress(&s));
}

/* --- vs_effective_view: before 50% shows current view --- */

static void test_vs_effective_view_early(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 2);  /* space -> galaxy */
    s = vs_tick(s, 0.3f);   /* well under 50% of 1.618s */
    TEST_ASSERT_EQUAL_INT(0, vs_effective_view(&s));  /* still space */
}

/* --- vs_effective_view: after 50% shows target view --- */

static void test_vs_effective_view_late(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 2);  /* space -> galaxy */
    s = vs_tick(s, 1.2f);   /* well past 50% of 1.618s */
    TEST_ASSERT_EQUAL_INT(2, vs_effective_view(&s));  /* galaxy */
}

/* --- vs_set_view during transition: redirects to new target --- */

static void test_vs_set_view_during_transition(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);  /* space -> earth */
    s = vs_tick(s, 0.3f);   /* partially through */
    s = vs_set_view(s, 2);  /* redirect to galaxy */
    TEST_ASSERT_EQUAL_INT(2, s.target_view);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.transition_t);
    TEST_ASSERT_EQUAL_INT(1, s.is_transitioning);
}

/* --- vs_set_focus preserves view state --- */

static void test_vs_set_focus_preserves_view(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);
    s = vs_tick(s, 2.0f);
    s = vs_set_focus(s, 5);  /* HD */
    TEST_ASSERT_EQUAL_INT(1, s.current_view);
    TEST_ASSERT_EQUAL_INT(5, s.focus_mode);
}

/* --- vs_set_lod preserves everything else --- */

static void test_vs_set_lod_preserves_state(void) {
    view_state_t s = vs_init();
    s = vs_set_focus(s, 2);
    s = vs_set_lod(s, 3);  /* LOD_LOW */
    TEST_ASSERT_EQUAL_INT(2, s.focus_mode);
    TEST_ASSERT_EQUAL_INT(3, s.lod_tier);
    TEST_ASSERT_EQUAL_INT(0, s.current_view);
}

/* --- vs_schedule with LOD: reduced passes at LOD_MEDIUM --- */

static void test_vs_schedule_lod_reduces_passes(void) {
    view_state_t s = vs_init();
    pass_schedule_t ultra = vs_schedule(&s);
    s = vs_set_lod(s, 2);  /* LOD_MEDIUM */
    pass_schedule_t medium = vs_schedule(&s);
    TEST_ASSERT_TRUE(medium.active_count <= ultra.active_count);
}

/* --- vs_tick with zero dt: no change --- */

static void test_vs_tick_zero_dt(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);
    float t0 = s.transition_t;
    s = vs_tick(s, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, t0, s.transition_t);
}

/* --- vs_tick with negative dt: treated as zero --- */

static void test_vs_tick_negative_dt(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);
    float t0 = s.transition_t;
    s = vs_tick(s, -1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, t0, s.transition_t);
}

/* --- vs_effective_view when idle: returns current --- */

static void test_vs_effective_view_idle(void) {
    view_state_t s = vs_init();
    TEST_ASSERT_EQUAL_INT(0, vs_effective_view(&s));
}

/* --- transition progress mid-flight --- */

static void test_vs_transition_progress_mid(void) {
    view_state_t s = vs_init();
    s = vs_set_view(s, 1);
    s = vs_tick(s, 0.809f);  /* ~50% of 1.618 */
    float p = vs_transition_progress(&s);
    TEST_ASSERT_TRUE(p > 0.3f);
    TEST_ASSERT_TRUE(p < 0.7f);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vs_init);
    RUN_TEST(test_vs_set_view_starts_transition);
    RUN_TEST(test_vs_set_view_same_noop);
    RUN_TEST(test_vs_tick_advances);
    RUN_TEST(test_vs_tick_completes);
    RUN_TEST(test_vs_tick_no_overshoot);
    RUN_TEST(test_vs_tick_idle_noop);
    RUN_TEST(test_vs_set_focus);
    RUN_TEST(test_vs_set_lod);
    RUN_TEST(test_vs_schedule_space);
    RUN_TEST(test_vs_schedule_earth);
    RUN_TEST(test_vs_schedule_with_focus);
    RUN_TEST(test_vs_blended_mid_transition);
    RUN_TEST(test_vs_blended_idle);
    RUN_TEST(test_vs_is_transitioning);
    RUN_TEST(test_vs_transition_progress_idle);
    RUN_TEST(test_vs_effective_view_early);
    RUN_TEST(test_vs_effective_view_late);
    RUN_TEST(test_vs_set_view_during_transition);
    RUN_TEST(test_vs_set_focus_preserves_view);
    RUN_TEST(test_vs_set_lod_preserves_state);
    RUN_TEST(test_vs_schedule_lod_reduces_passes);
    RUN_TEST(test_vs_tick_zero_dt);
    RUN_TEST(test_vs_tick_negative_dt);
    RUN_TEST(test_vs_effective_view_idle);
    RUN_TEST(test_vs_transition_progress_mid);
    return UNITY_END();
}
