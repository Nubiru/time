#include "../unity/unity.h"
#include "../../src/ui/toast_message.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- 1: toast_queue_init ---- */

void test_queue_init_count_is_zero(void)
{
    toast_queue_t q = toast_queue_init();
    TEST_ASSERT_EQUAL_INT(0, q.count);
}

/* ---- 2: toast_push count ---- */

void test_push_count_becomes_one(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Hello", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(1, q.count);
}

/* ---- 3: toast_push text matches ---- */

void test_push_text_matches(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Speed: 100x", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_STRING("Speed: 100x", q.items[0].text);
}

/* ---- 4: toast_push level matches ---- */

void test_push_level_matches(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_WARNING, 3.0f);
    TEST_ASSERT_EQUAL_INT(TOAST_WARNING, q.items[0].level);
}

/* ---- 5: toast_push active is 1 ---- */

void test_push_active_is_one(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(1, q.items[0].active);
}

/* ---- 6: toast_push opacity is 1.0 ---- */

void test_push_opacity_is_one(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, q.items[0].opacity);
}

/* ---- 7: toast_push multiple increases count ---- */

void test_push_multiple_increases_count(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "One", TOAST_INFO, 2.0f);
    q = toast_push(q, "Two", TOAST_INFO, 2.0f);
    q = toast_push(q, "Three", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(3, q.count);
}

/* ---- 8: toast_push when full drops oldest ---- */

void test_push_full_drops_oldest(void)
{
    toast_queue_t q = toast_queue_init();
    int i;
    for (i = 0; i < TOAST_MAX_QUEUE; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Toast %d", i);
        q = toast_push(q, buf, TOAST_INFO, 2.0f);
    }
    TEST_ASSERT_EQUAL_INT(TOAST_MAX_QUEUE, q.count);

    /* Push one more — oldest ("Toast 0") should be gone */
    q = toast_push(q, "Overflow", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(TOAST_MAX_QUEUE, q.count);
    TEST_ASSERT_EQUAL_STRING("Toast 1", q.items[0].text);
    TEST_ASSERT_EQUAL_STRING("Overflow", q.items[TOAST_MAX_QUEUE - 1].text);
}

/* ---- 9: toast_update elapsed increases ---- */

void test_update_elapsed_increases(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    q = toast_update(q, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, q.items[0].elapsed);
}

/* ---- 10: toast_update opacity stays 1.0 when not fading ---- */

void test_update_opacity_stays_full_when_not_fading(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    q = toast_update(q, 1.0f);  /* 1.0s elapsed, 1.0s remaining > 0.5 fade */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, q.items[0].opacity);
}

/* ---- 11: toast_update opacity decreases near end ---- */

void test_update_opacity_decreases_near_end(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    q = toast_update(q, 1.75f);  /* 0.25s remaining, in fade window */
    /* opacity = 0.25 / 0.5 = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, q.items[0].opacity);
}

/* ---- 12: toast_update expired toast removed ---- */

void test_update_expired_toast_removed(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    q = toast_update(q, 2.0f);  /* exactly at duration */
    TEST_ASSERT_EQUAL_INT(0, q.count);
}

/* ---- 13: toast_update with dt=0 no change ---- */

void test_update_dt_zero_no_change(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    q = toast_update(q, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, q.items[0].elapsed);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, q.items[0].opacity);
    TEST_ASSERT_EQUAL_INT(1, q.count);
}

/* ---- 14: toast_clear count becomes 0 ---- */

void test_clear_count_becomes_zero(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "One", TOAST_INFO, 2.0f);
    q = toast_push(q, "Two", TOAST_INFO, 2.0f);
    q = toast_clear(q);
    TEST_ASSERT_EQUAL_INT(0, q.count);
}

/* ---- 15: toast_newest returns most recent ---- */

void test_newest_returns_most_recent(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "First", TOAST_INFO, 2.0f);
    q = toast_push(q, "Second", TOAST_WARNING, 3.0f);
    toast_t t = toast_newest(q);
    TEST_ASSERT_EQUAL_STRING("Second", t.text);
    TEST_ASSERT_EQUAL_INT(TOAST_WARNING, t.level);
}

/* ---- 16: toast_newest on empty queue returns inactive ---- */

void test_newest_empty_queue_returns_inactive(void)
{
    toast_queue_t q = toast_queue_init();
    toast_t t = toast_newest(q);
    TEST_ASSERT_EQUAL_INT(0, t.active);
}

/* ---- 17: toast_active_count correct count ---- */

void test_active_count_correct(void)
{
    toast_queue_t q = toast_queue_init();
    TEST_ASSERT_EQUAL_INT(0, toast_active_count(q));
    q = toast_push(q, "One", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(1, toast_active_count(q));
    q = toast_push(q, "Two", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(2, toast_active_count(q));
}

/* ---- 18: toast_is_empty ---- */

void test_is_empty_true_when_empty(void)
{
    toast_queue_t q = toast_queue_init();
    TEST_ASSERT_EQUAL_INT(1, toast_is_empty(q));
}

void test_is_empty_false_when_not_empty(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(0, toast_is_empty(q));
}

/* ---- 19: toast_speed 3600 ---- */

void test_speed_3600_contains_3600x(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_speed(q, 3600.0);
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "3600x"));
}

/* ---- 20: toast_speed 0 PAUSED ---- */

void test_speed_zero_contains_paused(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_speed(q, 0.0);
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "PAUSED"));
}

/* ---- 21: toast_speed 1 Real-time ---- */

void test_speed_one_contains_realtime(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_speed(q, 1.0);
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "Real-time"));
}

/* ---- 22: toast_toggle on ---- */

void test_toggle_on_contains_on(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_toggle(q, "Stars", 1);
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "ON"));
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "Stars"));
}

/* ---- 23: toast_toggle off ---- */

void test_toggle_off_contains_off(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_toggle(q, "HUD", 0);
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "OFF"));
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "HUD"));
}

/* ---- 24: toast_view matches view name ---- */

void test_view_matches_name(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_view(q, "Earth View");
    TEST_ASSERT_EQUAL_STRING("Earth View", q.items[0].text);
}

/* ---- 25: toast_jump contains jumped to ---- */

void test_jump_contains_jumped_to(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_jump(q, "Jan 1, 2000");
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "Jumped to"));
    TEST_ASSERT_NOT_NULL(strstr(q.items[0].text, "Jan 1, 2000"));
}

/* ---- 26: toast_level_name returns non-NULL ---- */

void test_level_name_non_null_for_all(void)
{
    TEST_ASSERT_NOT_NULL(toast_level_name(TOAST_INFO));
    TEST_ASSERT_NOT_NULL(toast_level_name(TOAST_SUCCESS));
    TEST_ASSERT_NOT_NULL(toast_level_name(TOAST_WARNING));
}

void test_level_name_info_is_info(void)
{
    TEST_ASSERT_EQUAL_STRING("Info", toast_level_name(TOAST_INFO));
}

/* ---- 27: toast_default_duration INFO approx 2.0 ---- */

void test_default_duration_info_approx_2(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f,
                             toast_default_duration(TOAST_INFO));
}

/* ---- 28: toast_default_duration WARNING approx 3.0 ---- */

void test_default_duration_warning_approx_3(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.0f,
                             toast_default_duration(TOAST_WARNING));
}

/* ---- 29: Full lifecycle ---- */

void test_full_lifecycle(void)
{
    toast_queue_t q = toast_queue_init();

    /* Push a toast */
    q = toast_push(q, "Lifecycle", TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(1, q.count);
    TEST_ASSERT_EQUAL_INT(1, q.items[0].active);

    /* Update partially — still active */
    q = toast_update(q, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, q.count);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, q.items[0].opacity);

    /* Update into fade zone */
    q = toast_update(q, 0.75f);  /* elapsed=1.75, remaining=0.25 */
    TEST_ASSERT_EQUAL_INT(1, q.count);
    TEST_ASSERT_TRUE(q.items[0].opacity < 1.0f);
    TEST_ASSERT_TRUE(q.items[0].opacity > 0.0f);

    /* Update past expiry */
    q = toast_update(q, 0.5f);  /* elapsed=2.25 > duration=2.0 */
    TEST_ASSERT_EQUAL_INT(0, q.count);
}

/* ---- 30: Push with default duration (0) ---- */

void test_push_default_duration_uses_level_default(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Test", TOAST_WARNING, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.0f, q.items[0].duration);
}

/* ---- 31: toast_default_duration SUCCESS approx 2.5 ---- */

void test_default_duration_success_approx_2_5(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.5f,
                             toast_default_duration(TOAST_SUCCESS));
}

/* ---- 32: Multiple toasts with different durations expire correctly ---- */

void test_multiple_expire_independently(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, "Short", TOAST_INFO, 1.0f);
    q = toast_push(q, "Long", TOAST_INFO, 3.0f);
    TEST_ASSERT_EQUAL_INT(2, q.count);

    /* After 1.0s, short one should expire */
    q = toast_update(q, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, q.count);
    TEST_ASSERT_EQUAL_STRING("Long", q.items[0].text);
}

/* ---- 33: NULL text handled safely ---- */

void test_push_null_text_safe(void)
{
    toast_queue_t q = toast_queue_init();
    q = toast_push(q, NULL, TOAST_INFO, 2.0f);
    TEST_ASSERT_EQUAL_INT(1, q.count);
    TEST_ASSERT_EQUAL_STRING("", q.items[0].text);
}

int main(void)
{
    UNITY_BEGIN();

    /* Queue init */
    RUN_TEST(test_queue_init_count_is_zero);

    /* Push basics */
    RUN_TEST(test_push_count_becomes_one);
    RUN_TEST(test_push_text_matches);
    RUN_TEST(test_push_level_matches);
    RUN_TEST(test_push_active_is_one);
    RUN_TEST(test_push_opacity_is_one);
    RUN_TEST(test_push_multiple_increases_count);
    RUN_TEST(test_push_full_drops_oldest);

    /* Update */
    RUN_TEST(test_update_elapsed_increases);
    RUN_TEST(test_update_opacity_stays_full_when_not_fading);
    RUN_TEST(test_update_opacity_decreases_near_end);
    RUN_TEST(test_update_expired_toast_removed);
    RUN_TEST(test_update_dt_zero_no_change);

    /* Clear */
    RUN_TEST(test_clear_count_becomes_zero);

    /* Query */
    RUN_TEST(test_newest_returns_most_recent);
    RUN_TEST(test_newest_empty_queue_returns_inactive);
    RUN_TEST(test_active_count_correct);
    RUN_TEST(test_is_empty_true_when_empty);
    RUN_TEST(test_is_empty_false_when_not_empty);

    /* Convenience constructors */
    RUN_TEST(test_speed_3600_contains_3600x);
    RUN_TEST(test_speed_zero_contains_paused);
    RUN_TEST(test_speed_one_contains_realtime);
    RUN_TEST(test_toggle_on_contains_on);
    RUN_TEST(test_toggle_off_contains_off);
    RUN_TEST(test_view_matches_name);
    RUN_TEST(test_jump_contains_jumped_to);

    /* Display */
    RUN_TEST(test_level_name_non_null_for_all);
    RUN_TEST(test_level_name_info_is_info);
    RUN_TEST(test_default_duration_info_approx_2);
    RUN_TEST(test_default_duration_warning_approx_3);
    RUN_TEST(test_default_duration_success_approx_2_5);

    /* Lifecycle & edge cases */
    RUN_TEST(test_full_lifecycle);
    RUN_TEST(test_push_default_duration_uses_level_default);
    RUN_TEST(test_multiple_expire_independently);
    RUN_TEST(test_push_null_text_safe);

    return UNITY_END();
}
