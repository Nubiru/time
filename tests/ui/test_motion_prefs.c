#include "../unity/unity.h"
#include "ui/motion_prefs.h"

#define TOL 0.001f

void setUp(void) {}
void tearDown(void) {}

/* --- test_mp_create_defaults --- */
void test_mp_create_defaults(void)
{
    motion_prefs_t p = mp_create();
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, p.speed_scale);
    TEST_ASSERT_EQUAL_INT(0, p.reduced_motion);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.05f, p.min_duration);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, p.max_duration);
}

/* --- test_mp_set_speed_normal --- */
void test_mp_set_speed_normal(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, p.speed_scale);
}

/* --- test_mp_set_speed_fast --- */
void test_mp_set_speed_fast(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, p.speed_scale);
}

/* --- test_mp_set_speed_slow --- */
void test_mp_set_speed_slow(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, p.speed_scale);
}

/* --- test_mp_set_speed_clamped_min --- */
void test_mp_set_speed_clamped_min(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 0.01f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, MP_SPEED_MIN, p.speed_scale);
}

/* --- test_mp_set_speed_clamped_max --- */
void test_mp_set_speed_clamped_max(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 100.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, MP_SPEED_MAX, p.speed_scale);
}

/* --- test_mp_set_reduced_on --- */
void test_mp_set_reduced_on(void)
{
    motion_prefs_t p = mp_set_reduced(mp_create(), 1);
    TEST_ASSERT_EQUAL_INT(1, p.reduced_motion);
}

/* --- test_mp_set_reduced_off --- */
void test_mp_set_reduced_off(void)
{
    motion_prefs_t p = mp_set_reduced(mp_create(), 0);
    TEST_ASSERT_EQUAL_INT(0, p.reduced_motion);
}

/* --- test_mp_set_reduced_normalizes --- */
void test_mp_set_reduced_normalizes(void)
{
    motion_prefs_t p = mp_set_reduced(mp_create(), 5);
    TEST_ASSERT_EQUAL_INT(1, p.reduced_motion);
}

/* --- test_mp_set_bounds_valid --- */
void test_mp_set_bounds_valid(void)
{
    motion_prefs_t p = mp_set_bounds(mp_create(), 0.1f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.1f, p.min_duration);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, p.max_duration);
}

/* --- test_mp_set_bounds_negative_min --- */
void test_mp_set_bounds_negative_min(void)
{
    motion_prefs_t p = mp_set_bounds(mp_create(), -1.0f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, p.min_duration);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, p.max_duration);
}

/* --- test_mp_set_bounds_max_less_than_min --- */
void test_mp_set_bounds_max_less_than_min(void)
{
    motion_prefs_t p = mp_set_bounds(mp_create(), 2.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, p.min_duration);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, p.max_duration);
}

/* --- test_mp_apply_duration_normal --- */
void test_mp_apply_duration_normal(void)
{
    motion_prefs_t p = mp_create();
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, mp_apply_duration(p, 1.0f));
}

/* --- test_mp_apply_duration_fast --- */
void test_mp_apply_duration_fast(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, mp_apply_duration(p, 1.0f));
}

/* --- test_mp_apply_duration_slow --- */
void test_mp_apply_duration_slow(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, mp_apply_duration(p, 1.0f));
}

/* --- test_mp_apply_duration_clamped_min --- */
void test_mp_apply_duration_clamped_min(void)
{
    /* speed=10, base=0.1 -> 0.01, clamped to min_duration=0.05 */
    motion_prefs_t p = mp_set_speed(mp_create(), 10.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.05f, mp_apply_duration(p, 0.1f));
}

/* --- test_mp_apply_duration_clamped_max --- */
void test_mp_apply_duration_clamped_max(void)
{
    /* speed=0.1, base=4.0 -> 40.0, clamped to max_duration=5.0 */
    motion_prefs_t p = mp_set_speed(mp_create(), 0.1f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, mp_apply_duration(p, 4.0f));
}

/* --- test_mp_apply_duration_reduced --- */
void test_mp_apply_duration_reduced(void)
{
    motion_prefs_t p = mp_set_reduced(mp_create(), 1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, mp_apply_duration(p, 1.0f));
}

/* --- test_mp_apply_duration_zero_base --- */
void test_mp_apply_duration_zero_base(void)
{
    motion_prefs_t p = mp_create();
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, mp_apply_duration(p, 0.0f));
}

/* --- test_mp_apply_stiffness_fast --- */
void test_mp_apply_stiffness_fast(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 2.0f);
    /* base=100, speed=2 -> 100 * 4 = 400 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 400.0f, mp_apply_stiffness(p, 100.0f));
}

/* --- test_mp_apply_damping_fast --- */
void test_mp_apply_damping_fast(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 2.0f);
    /* base=10, speed=2 -> 10 * 2 = 20 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 20.0f, mp_apply_damping(p, 10.0f));
}

/* --- test_mp_should_animate_normal --- */
void test_mp_should_animate_normal(void)
{
    motion_prefs_t p = mp_create();
    TEST_ASSERT_EQUAL_INT(1, mp_should_animate(p));
}

/* --- test_mp_should_animate_reduced --- */
void test_mp_should_animate_reduced(void)
{
    motion_prefs_t p = mp_set_reduced(mp_create(), 1);
    TEST_ASSERT_EQUAL_INT(0, mp_should_animate(p));
}

/* --- test_mp_speed_returns_scale --- */
void test_mp_speed_returns_scale(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 3.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.5f, mp_speed(p));
}

/* --- test_mp_apply_delay_normal --- */
void test_mp_apply_delay_normal(void)
{
    motion_prefs_t p = mp_create();
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, mp_apply_delay(p, 0.5f));
}

/* --- test_mp_apply_delay_fast --- */
void test_mp_apply_delay_fast(void)
{
    motion_prefs_t p = mp_set_speed(mp_create(), 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, mp_apply_delay(p, 0.5f));
}

/* --- test_mp_apply_delay_reduced --- */
void test_mp_apply_delay_reduced(void)
{
    motion_prefs_t p = mp_set_reduced(mp_create(), 1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, mp_apply_delay(p, 0.5f));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mp_create_defaults);
    RUN_TEST(test_mp_set_speed_normal);
    RUN_TEST(test_mp_set_speed_fast);
    RUN_TEST(test_mp_set_speed_slow);
    RUN_TEST(test_mp_set_speed_clamped_min);
    RUN_TEST(test_mp_set_speed_clamped_max);
    RUN_TEST(test_mp_set_reduced_on);
    RUN_TEST(test_mp_set_reduced_off);
    RUN_TEST(test_mp_set_reduced_normalizes);
    RUN_TEST(test_mp_set_bounds_valid);
    RUN_TEST(test_mp_set_bounds_negative_min);
    RUN_TEST(test_mp_set_bounds_max_less_than_min);
    RUN_TEST(test_mp_apply_duration_normal);
    RUN_TEST(test_mp_apply_duration_fast);
    RUN_TEST(test_mp_apply_duration_slow);
    RUN_TEST(test_mp_apply_duration_clamped_min);
    RUN_TEST(test_mp_apply_duration_clamped_max);
    RUN_TEST(test_mp_apply_duration_reduced);
    RUN_TEST(test_mp_apply_duration_zero_base);
    RUN_TEST(test_mp_apply_stiffness_fast);
    RUN_TEST(test_mp_apply_damping_fast);
    RUN_TEST(test_mp_should_animate_normal);
    RUN_TEST(test_mp_should_animate_reduced);
    RUN_TEST(test_mp_speed_returns_scale);
    RUN_TEST(test_mp_apply_delay_normal);
    RUN_TEST(test_mp_apply_delay_fast);
    RUN_TEST(test_mp_apply_delay_reduced);
    return UNITY_END();
}
