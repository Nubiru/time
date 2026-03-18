#include "../unity/unity.h"
#include "../../src/ui/touch_gestures.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- 1-3: tg_init defaults ---- */

void test_init_returns_zero_active_count(void)
{
    tg_state_t s = tg_init();
    TEST_ASSERT_EQUAL_INT(0, s.active_count);
}

void test_init_gesture_is_none(void)
{
    tg_state_t s = tg_init();
    TEST_ASSERT_EQUAL_INT(TG_NONE, s.last_gesture);
}

void test_init_last_tap_time_is_zero(void)
{
    tg_state_t s = tg_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.last_tap_time);
}

/* ---- 4-6: tg_touch_start tracking ---- */

void test_touch_start_increments_active_count(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    TEST_ASSERT_EQUAL_INT(1, s.active_count);
}

void test_touch_start_stores_position(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.3f, 0.7f, 100.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, s.points[0].start_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.7f, s.points[0].start_y);
}

void test_touch_start_stores_time(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 42.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 42.0f, (float)s.points[0].start_time);
}

/* ---- 7-8: multi-touch tracking by ID ---- */

void test_two_touch_points_tracked(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.2f, 0.3f, 100.0);
    s = tg_touch_start(s, 1, 0.8f, 0.7f, 100.0);
    TEST_ASSERT_EQUAL_INT(2, s.active_count);
}

void test_five_touch_points_tracked(void)
{
    tg_state_t s = tg_init();
    for (int i = 0; i < 5; i++) {
        s = tg_touch_start(s, i, 0.1f * (float)i, 0.5f, 100.0);
    }
    TEST_ASSERT_EQUAL_INT(5, s.active_count);
}

/* ---- 9: beyond max touch points ignored ---- */

void test_sixth_touch_ignored(void)
{
    tg_state_t s = tg_init();
    for (int i = 0; i < 6; i++) {
        s = tg_touch_start(s, i, 0.1f * (float)i, 0.5f, 100.0);
    }
    TEST_ASSERT_EQUAL_INT(5, s.active_count);
}

/* ---- 10-11: tg_touch_move ---- */

void test_touch_move_updates_current_position(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.6f, 0.7f, 110.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.6f, s.points[0].current_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.7f, s.points[0].current_y);
}

void test_touch_move_preserves_start_position(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.3f, 0.4f, 100.0);
    s = tg_touch_move(s, 0, 0.6f, 0.7f, 110.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, s.points[0].start_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.4f, s.points[0].start_y);
}

/* ---- 12-13: Single tap detection ---- */

void test_single_tap_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 200.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_TAP, g);
}

void test_tap_position_in_data(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.6f, 100.0);
    tg_touch_end(&s, 0, 0.51f, 0.61f, 200.0, &data);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.51f, data.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, data.y);
}

/* ---- 14-15: Tap thresholds ---- */

void test_tap_rejected_if_too_long(void)
{
    /* > 300ms should not be a tap */
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 500.0, &data);
    TEST_ASSERT_NOT_EQUAL(TG_TAP, g);
}

void test_tap_rejected_if_too_much_movement(void)
{
    /* > 10px (using 0.02 normalized ~ 10px on 500px screen) */
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.6f, 0.5f, 200.0, &data);
    /* 0.1 normalized = 50px on 500px screen, > TAP_MAX_MOVEMENT */
    TEST_ASSERT_NOT_EQUAL(TG_TAP, g);
}

/* ---- 16-17: Double tap detection ---- */

void test_double_tap_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* First tap */
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_touch_end(&s, 0, 0.5f, 0.5f, 200.0, &data);
    /* Second tap within 400ms of first tap end */
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 300.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 400.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_DOUBLE_TAP, g);
}

void test_double_tap_rejected_if_too_slow(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* First tap */
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_touch_end(&s, 0, 0.5f, 0.5f, 200.0, &data);
    /* Second tap after 400ms window */
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 700.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 800.0, &data);
    /* Should be single tap, not double */
    TEST_ASSERT_EQUAL_INT(TG_TAP, g);
}

/* ---- 18-19: Long press detection ---- */

void test_long_press_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g = tg_check_long_press(&s, 700.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_LONG_PRESS, g);
}

void test_long_press_not_if_too_short(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g = tg_check_long_press(&s, 400.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_NONE, g);
}

/* ---- 20: Long press rejected if moved too much ---- */

void test_long_press_rejected_if_moved(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.7f, 0.5f, 300.0);
    tg_gesture_t g = tg_check_long_press(&s, 700.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_NONE, g);
}

/* ---- 21-24: Swipe detection ---- */

void test_swipe_right_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Swipe right: large positive dx, small dy */
    s = tg_touch_start(s, 0, 0.1f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.3f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.3f, 0.5f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_SWIPE_RIGHT, g);
}

void test_swipe_left_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.3f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.3f, 0.5f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_SWIPE_LEFT, g);
}

void test_swipe_up_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Swipe up: large negative dy (up is negative in screen coords) */
    s = tg_touch_start(s, 0, 0.5f, 0.7f, 100.0);
    s = tg_touch_move(s, 0, 0.5f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_SWIPE_UP, g);
}

void test_swipe_down_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.3f, 100.0);
    s = tg_touch_move(s, 0, 0.5f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_SWIPE_DOWN, g);
}

/* ---- 25: Swipe velocity ---- */

void test_swipe_velocity_calculated(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* 0.2 normalized in 150ms */
    s = tg_touch_start(s, 0, 0.1f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.3f, 0.5f, 200.0);
    tg_touch_end(&s, 0, 0.3f, 0.5f, 250.0, &data);
    /* velocity = distance / time = 0.2 / 0.15 = 1.333... normalized/sec */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 1.333f, data.velocity);
}

/* ---- 26: Swipe delta stored ---- */

void test_swipe_delta_stored(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.1f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.3f, 0.5f, 200.0);
    tg_touch_end(&s, 0, 0.3f, 0.5f, 250.0, &data);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, data.dx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, data.dy);
}

/* ---- 27-28: Pinch detection ---- */

void test_pinch_out_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Two fingers start close, move apart */
    s = tg_touch_start(s, 0, 0.4f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.6f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.3f, 0.5f, 200.0);
    s = tg_touch_move(s, 1, 0.7f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.3f, 0.5f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_PINCH_OUT, g);
}

void test_pinch_in_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Two fingers start far, move closer */
    s = tg_touch_start(s, 0, 0.2f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.8f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.35f, 0.5f, 200.0);
    s = tg_touch_move(s, 1, 0.65f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.35f, 0.5f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_PINCH_IN, g);
}

/* ---- 29: Pinch scale factor ---- */

void test_pinch_scale_calculated(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Two fingers: initial distance 0.2, final distance 0.4 -> scale = 2.0 */
    s = tg_touch_start(s, 0, 0.4f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.6f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.3f, 0.5f, 200.0);
    s = tg_touch_move(s, 1, 0.7f, 0.5f, 200.0);
    tg_touch_end(&s, 0, 0.3f, 0.5f, 250.0, &data);
    /* scale = current_dist / start_dist = 0.4 / 0.2 = 2.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 2.0f, data.pinch_scale);
}

/* ---- 30-31: Two-finger rotation ---- */

void test_two_finger_rotation_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Two fingers rotate: start horizontal, end at 45 degrees */
    s = tg_touch_start(s, 0, 0.4f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.6f, 0.5f, 100.0);
    /* Rotate ~45 degrees clockwise */
    s = tg_touch_move(s, 0, 0.4293f, 0.5707f, 200.0);
    s = tg_touch_move(s, 1, 0.5707f, 0.4293f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.4293f, 0.5707f, 250.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_TWO_FINGER_ROTATE, g);
}

void test_rotation_angle_calculated(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    /* Two fingers: start horizontal (angle=0), rotate 90 degrees */
    s = tg_touch_start(s, 0, 0.4f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.6f, 0.5f, 100.0);
    /* Rotate 90 degrees: horizontal -> vertical */
    s = tg_touch_move(s, 0, 0.5f, 0.4f, 200.0);
    s = tg_touch_move(s, 1, 0.5f, 0.6f, 200.0);
    tg_touch_end(&s, 0, 0.5f, 0.4f, 250.0, &data);
    /* Angle should be ~PI/2 (90 degrees) */
    TEST_ASSERT_FLOAT_WITHIN(0.15f, 1.5708f, fabsf(data.rotate_angle));
}

/* ---- 32: Three-finger tap ---- */

void test_three_finger_tap_detected(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.3f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.5f, 0.5f, 100.0);
    s = tg_touch_start(s, 2, 0.7f, 0.5f, 100.0);
    /* End all three quickly with minimal movement */
    tg_touch_end(&s, 0, 0.3f, 0.5f, 200.0, &data);
    tg_touch_end(&s, 1, 0.5f, 0.5f, 210.0, &data);
    tg_gesture_t g = tg_touch_end(&s, 2, 0.7f, 0.5f, 220.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_THREE_FINGER_TAP, g);
}

/* ---- 33: Ambiguous input returns NONE ---- */

void test_ambiguous_input_returns_none(void)
{
    /* Hold for 350ms (too slow for tap, too fast for long press) with movement */
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    s = tg_touch_move(s, 0, 0.55f, 0.55f, 250.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.55f, 0.55f, 450.0, &data);
    /* Too long for tap (350ms > 300ms), too much movement for long press check,
     * not enough movement for swipe */
    TEST_ASSERT_EQUAL_INT(TG_NONE, g);
}

/* ---- 34-35: Gesture name strings ---- */

void test_gesture_name_none(void)
{
    const char *name = tg_gesture_name(TG_NONE);
    TEST_ASSERT_EQUAL_STRING("none", name);
}

void test_gesture_name_tap(void)
{
    const char *name = tg_gesture_name(TG_TAP);
    TEST_ASSERT_EQUAL_STRING("tap", name);
}

void test_gesture_name_double_tap(void)
{
    const char *name = tg_gesture_name(TG_DOUBLE_TAP);
    TEST_ASSERT_EQUAL_STRING("double_tap", name);
}

void test_gesture_name_long_press(void)
{
    const char *name = tg_gesture_name(TG_LONG_PRESS);
    TEST_ASSERT_EQUAL_STRING("long_press", name);
}

void test_gesture_name_swipe_left(void)
{
    const char *name = tg_gesture_name(TG_SWIPE_LEFT);
    TEST_ASSERT_EQUAL_STRING("swipe_left", name);
}

void test_gesture_name_pinch_in(void)
{
    const char *name = tg_gesture_name(TG_PINCH_IN);
    TEST_ASSERT_EQUAL_STRING("pinch_in", name);
}

void test_gesture_name_rotate(void)
{
    const char *name = tg_gesture_name(TG_TWO_FINGER_ROTATE);
    TEST_ASSERT_EQUAL_STRING("two_finger_rotate", name);
}

void test_gesture_name_three_finger_tap(void)
{
    const char *name = tg_gesture_name(TG_THREE_FINGER_TAP);
    TEST_ASSERT_EQUAL_STRING("three_finger_tap", name);
}

/* ---- 36-37: Edge cases ---- */

void test_zero_duration_touch_is_tap(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 100.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_TAP, g);
}

void test_same_position_start_end(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_touch_end(&s, 0, 0.5f, 0.5f, 200.0, &data);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, data.dx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, data.dy);
}

/* ---- 38: Touch end decrements count ---- */

void test_touch_end_decrements_count(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    s = tg_touch_start(s, 1, 0.6f, 0.6f, 100.0);
    TEST_ASSERT_EQUAL_INT(2, s.active_count);
    tg_touch_end(&s, 0, 0.5f, 0.5f, 200.0, &data);
    TEST_ASSERT_EQUAL_INT(1, s.active_count);
}

/* ---- 39: Long press position in data ---- */

void test_long_press_position_in_data(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.3f, 0.4f, 100.0);
    tg_check_long_press(&s, 700.0, &data);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, data.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.4f, data.y);
}

/* ---- 40: Move on unknown ID is safe ---- */

void test_move_unknown_id_safe(void)
{
    tg_state_t s = tg_init();
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    /* Move an ID that was never started -- should not crash */
    s = tg_touch_move(s, 99, 0.6f, 0.6f, 200.0);
    TEST_ASSERT_EQUAL_INT(1, s.active_count);
}

/* ---- 41: Long press not fired twice ---- */

void test_long_press_not_fired_twice(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.5f, 0.5f, 100.0);
    tg_gesture_t g1 = tg_check_long_press(&s, 700.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_LONG_PRESS, g1);
    /* Second check should return NONE (already fired) */
    tg_gesture_t g2 = tg_check_long_press(&s, 800.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_NONE, g2);
}

/* ---- 42: No active touches returns NONE for long press ---- */

void test_long_press_no_touches_returns_none(void)
{
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    tg_gesture_t g = tg_check_long_press(&s, 700.0, &data);
    TEST_ASSERT_EQUAL_INT(TG_NONE, g);
}

/* ---- 43: Swipe angle boundary ---- */

void test_diagonal_movement_not_swipe(void)
{
    /* 45 degree movement: should NOT be a swipe (> 30 degree from axis) */
    tg_state_t s = tg_init();
    tg_gesture_data_t data;
    s = tg_touch_start(s, 0, 0.3f, 0.3f, 100.0);
    s = tg_touch_move(s, 0, 0.5f, 0.5f, 200.0);
    tg_gesture_t g = tg_touch_end(&s, 0, 0.5f, 0.5f, 250.0, &data);
    TEST_ASSERT_NOT_EQUAL(TG_SWIPE_RIGHT, g);
    TEST_ASSERT_NOT_EQUAL(TG_SWIPE_DOWN, g);
}

int main(void)
{
    UNITY_BEGIN();

    /* init */
    RUN_TEST(test_init_returns_zero_active_count);
    RUN_TEST(test_init_gesture_is_none);
    RUN_TEST(test_init_last_tap_time_is_zero);

    /* touch start tracking */
    RUN_TEST(test_touch_start_increments_active_count);
    RUN_TEST(test_touch_start_stores_position);
    RUN_TEST(test_touch_start_stores_time);

    /* multi-touch */
    RUN_TEST(test_two_touch_points_tracked);
    RUN_TEST(test_five_touch_points_tracked);
    RUN_TEST(test_sixth_touch_ignored);

    /* touch move */
    RUN_TEST(test_touch_move_updates_current_position);
    RUN_TEST(test_touch_move_preserves_start_position);

    /* single tap */
    RUN_TEST(test_single_tap_detected);
    RUN_TEST(test_tap_position_in_data);
    RUN_TEST(test_tap_rejected_if_too_long);
    RUN_TEST(test_tap_rejected_if_too_much_movement);

    /* double tap */
    RUN_TEST(test_double_tap_detected);
    RUN_TEST(test_double_tap_rejected_if_too_slow);

    /* long press */
    RUN_TEST(test_long_press_detected);
    RUN_TEST(test_long_press_not_if_too_short);
    RUN_TEST(test_long_press_rejected_if_moved);

    /* swipe */
    RUN_TEST(test_swipe_right_detected);
    RUN_TEST(test_swipe_left_detected);
    RUN_TEST(test_swipe_up_detected);
    RUN_TEST(test_swipe_down_detected);
    RUN_TEST(test_swipe_velocity_calculated);
    RUN_TEST(test_swipe_delta_stored);

    /* pinch */
    RUN_TEST(test_pinch_out_detected);
    RUN_TEST(test_pinch_in_detected);
    RUN_TEST(test_pinch_scale_calculated);

    /* rotation */
    RUN_TEST(test_two_finger_rotation_detected);
    RUN_TEST(test_rotation_angle_calculated);

    /* three-finger tap */
    RUN_TEST(test_three_finger_tap_detected);

    /* ambiguous / edge cases */
    RUN_TEST(test_ambiguous_input_returns_none);
    RUN_TEST(test_gesture_name_none);
    RUN_TEST(test_gesture_name_tap);
    RUN_TEST(test_gesture_name_double_tap);
    RUN_TEST(test_gesture_name_long_press);
    RUN_TEST(test_gesture_name_swipe_left);
    RUN_TEST(test_gesture_name_pinch_in);
    RUN_TEST(test_gesture_name_rotate);
    RUN_TEST(test_gesture_name_three_finger_tap);
    RUN_TEST(test_zero_duration_touch_is_tap);
    RUN_TEST(test_same_position_start_end);
    RUN_TEST(test_touch_end_decrements_count);
    RUN_TEST(test_long_press_position_in_data);
    RUN_TEST(test_move_unknown_id_safe);
    RUN_TEST(test_long_press_not_fired_twice);
    RUN_TEST(test_long_press_no_touches_returns_none);
    RUN_TEST(test_diagonal_movement_not_swipe);

    return UNITY_END();
}
