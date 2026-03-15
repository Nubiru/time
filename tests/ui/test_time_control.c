#include "../unity/unity.h"
#include "../../src/ui/time_control.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Known reference: JD 2451545.0 = Jan 1.5 2000 (noon UTC) */
static const double JD_J2000 = 2451545.0;

/* ---- 1: tc_speed_count ---- */

void test_speed_count_returns_six(void)
{
    TEST_ASSERT_EQUAL_INT(6, tc_speed_count());
}

/* ---- 2-4: tc_speed_get ---- */

void test_speed_get_0_is_paused(void)
{
    tc_speed_t sp = tc_speed_get(0);
    TEST_ASSERT_EQUAL_STRING("Paused", sp.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, sp.multiplier);
}

void test_speed_get_1_is_realtime(void)
{
    tc_speed_t sp = tc_speed_get(1);
    TEST_ASSERT_EQUAL_STRING("Real-time", sp.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, sp.multiplier);
}

void test_speed_get_4_is_day_per_sec(void)
{
    tc_speed_t sp = tc_speed_get(4);
    TEST_ASSERT_EQUAL_STRING("Day/sec", sp.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 86400.0, sp.multiplier);
}

/* ---- 5-7: tc_speed_find_by_key ---- */

void test_speed_find_by_key_0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tc_speed_find_by_key(0));
}

void test_speed_find_by_key_3_returns_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, tc_speed_find_by_key(3));
}

void test_speed_find_by_key_9_returns_neg1(void)
{
    TEST_ASSERT_EQUAL_INT(-1, tc_speed_find_by_key(9));
}

/* ---- 8-9: tc_speed_name ---- */

void test_speed_name_1_is_realtime(void)
{
    TEST_ASSERT_EQUAL_STRING("Real-time", tc_speed_name(1.0));
}

void test_speed_name_999_is_custom(void)
{
    TEST_ASSERT_EQUAL_STRING("Custom", tc_speed_name(999.0));
}

/* ---- 10: tc_state_init ---- */

void test_state_init_defaults(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, s.speed);
    TEST_ASSERT_EQUAL_INT(TC_DIR_FORWARD, s.direction);
    TEST_ASSERT_EQUAL_INT(1, s.is_live);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000, s.jd);
    TEST_ASSERT_EQUAL_INT(1, s.speed_index);
}

/* ---- 11: tc_set_speed ---- */

void test_set_speed_to_preset_3(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed(s, 3);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 3600.0, s.speed);
    TEST_ASSERT_EQUAL_INT(3, s.speed_index);
}

/* ---- 12: tc_set_speed_by_key ---- */

void test_set_speed_by_key_4(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed_by_key(s, 4);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 86400.0, s.speed);
}

/* ---- 13: tc_set_speed_by_key 0 pauses ---- */

void test_set_speed_by_key_0_pauses(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed_by_key(s, 0);
    TEST_ASSERT_EQUAL_INT(TC_DIR_PAUSED, s.direction);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, s.speed);
}

/* ---- 14: tc_toggle_pause from running ---- */

void test_toggle_pause_from_running(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed(s, 3); /* 3600x */
    s = tc_toggle_pause(s);
    TEST_ASSERT_EQUAL_INT(TC_DIR_PAUSED, s.direction);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, s.speed);
}

/* ---- 15: tc_toggle_pause from paused restores speed ---- */

void test_toggle_pause_from_paused_restores(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed(s, 3); /* 3600x */
    s = tc_toggle_pause(s);
    s = tc_toggle_pause(s); /* Resume */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 3600.0, s.speed);
    TEST_ASSERT_EQUAL_INT(TC_DIR_FORWARD, s.direction);
}

/* ---- 16: tc_reverse ---- */

void test_reverse_flips_direction(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_reverse(s);
    TEST_ASSERT_EQUAL_INT(TC_DIR_REVERSE, s.direction);
}

/* ---- 17: tc_reverse twice returns to original ---- */

void test_reverse_twice_restores(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_reverse(s);
    s = tc_reverse(s);
    TEST_ASSERT_EQUAL_INT(TC_DIR_FORWARD, s.direction);
}

/* ---- 18: tc_go_live ---- */

void test_go_live_restores_defaults(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed(s, 4);
    s = tc_reverse(s);
    double new_jd = JD_J2000 + 100.0;
    s = tc_go_live(s, new_jd);
    TEST_ASSERT_EQUAL_INT(1, s.is_live);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, s.speed);
    TEST_ASSERT_EQUAL_INT(TC_DIR_FORWARD, s.direction);
    TEST_ASSERT_FLOAT_WITHIN(0.001, new_jd, s.jd);
}

/* ---- 19: tc_step forward ---- */

void test_step_forward_increases_jd(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_step(s, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000 + 1.0, s.jd);
}

/* ---- 20: tc_step backward ---- */

void test_step_backward_decreases_jd(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_step(s, -1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000 - 1.0, s.jd);
}

/* ---- 21: tc_step disables live ---- */

void test_step_disables_live(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    TEST_ASSERT_EQUAL_INT(1, s.is_live);
    s = tc_step(s, 1.0);
    TEST_ASSERT_EQUAL_INT(0, s.is_live);
}

/* ---- 22: tc_advance ---- */

void test_advance_changes_jd(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0; /* Must be non-live for advance to work */
    s = tc_set_speed(s, 1); /* 1x real-time */
    /* Advance by 86400 seconds (1 day) at 1x */
    s = tc_advance(s, 86400.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000 + 1.0, s.jd);
}

/* ---- 23: tc_advance when paused ---- */

void test_advance_paused_no_change(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0;
    s = tc_toggle_pause(s);
    s = tc_advance(s, 86400.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000, s.jd);
}

/* ---- 24: tc_advance when reverse ---- */

void test_advance_reverse_decreases_jd(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0;
    s = tc_reverse(s);
    /* Advance by 86400 seconds at 1x reverse */
    s = tc_advance(s, 86400.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000 - 1.0, s.jd);
}

/* ---- 25: tc_is_paused ---- */

void test_is_paused_correct(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    TEST_ASSERT_EQUAL_INT(0, tc_is_paused(s));
    s = tc_toggle_pause(s);
    TEST_ASSERT_EQUAL_INT(1, tc_is_paused(s));
}

/* ---- 26: tc_is_reverse ---- */

void test_is_reverse_correct(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    TEST_ASSERT_EQUAL_INT(0, tc_is_reverse(s));
    s = tc_reverse(s);
    TEST_ASSERT_EQUAL_INT(1, tc_is_reverse(s));
}

/* ---- 27: tc_is_live ---- */

void test_is_live_correct(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    TEST_ASSERT_EQUAL_INT(1, tc_is_live(s));
    s = tc_step(s, 1.0);
    TEST_ASSERT_EQUAL_INT(0, tc_is_live(s));
}

/* ---- 28: tc_format speed_display for paused ---- */

void test_format_paused_shows_paused(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_toggle_pause(s);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("PAUSED", d.speed_display);
}

/* ---- 29: tc_format speed_display for 3600x ---- */

void test_format_speed_3600x(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0;
    s = tc_set_speed(s, 3); /* 3600x */
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("3600x", d.speed_display);
}

/* ---- 30: tc_format direction_icon forward ---- */

void test_format_direction_icon_forward(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING(">", d.direction_icon);
}

/* ---- 31: tc_format direction_icon paused ---- */

void test_format_direction_icon_paused(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_toggle_pause(s);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("||", d.direction_icon);
}

/* ---- 32: tc_format direction_icon reverse ---- */

void test_format_direction_icon_reverse(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_reverse(s);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("<", d.direction_icon);
}

/* ---- 33: tc_format_speed writes to buffer ---- */

void test_format_speed_writes_buffer(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0;
    s = tc_set_speed(s, 2); /* 60x */
    char buf[32];
    int n = tc_format_speed(s, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("60x", buf);
    TEST_ASSERT_TRUE(n > 0);
}

/* ---- 34: tc_direction_icon all three ---- */

void test_direction_icon_all_three(void)
{
    TEST_ASSERT_EQUAL_STRING(">",  tc_direction_icon(TC_DIR_FORWARD));
    TEST_ASSERT_EQUAL_STRING("||", tc_direction_icon(TC_DIR_PAUSED));
    TEST_ASSERT_EQUAL_STRING("<",  tc_direction_icon(TC_DIR_REVERSE));
}

/* ---- 35: tc_format date for J2000 ---- */

void test_format_date_j2000(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("2000-01-01", d.date);
    TEST_ASSERT_EQUAL_STRING("12:00:00", d.time);
}

/* ---- 36: tc_format day name for J2000 (Saturday) ---- */

void test_format_day_name_j2000(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("Saturday", d.day_name);
}

/* ---- 37: speed_get out of range returns unknown ---- */

void test_speed_get_out_of_range(void)
{
    tc_speed_t sp = tc_speed_get(-1);
    TEST_ASSERT_EQUAL_STRING("Unknown", sp.name);
    sp = tc_speed_get(99);
    TEST_ASSERT_EQUAL_STRING("Unknown", sp.name);
}

/* ---- 38: set_speed invalid index no change ---- */

void test_set_speed_invalid_index_no_change(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    double original_speed = s.speed;
    s = tc_set_speed(s, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001, original_speed, s.speed);
    s = tc_state_init(JD_J2000);
    s = tc_set_speed(s, 99);
    TEST_ASSERT_FLOAT_WITHIN(0.001, original_speed, s.speed);
}

/* ---- 39: set_speed_by_key invalid key no change ---- */

void test_set_speed_by_key_invalid_no_change(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    double original_speed = s.speed;
    s = tc_set_speed_by_key(s, 7);
    TEST_ASSERT_FLOAT_WITHIN(0.001, original_speed, s.speed);
}

/* ---- 40: advance when live has no effect ---- */

void test_advance_live_no_effect(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    /* is_live = 1 by default */
    s = tc_advance(s, 86400.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000, s.jd);
}

/* ---- 41: reverse sets is_live to 0 ---- */

void test_reverse_disables_live(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    TEST_ASSERT_EQUAL_INT(1, s.is_live);
    s = tc_reverse(s);
    TEST_ASSERT_EQUAL_INT(0, s.is_live);
}

/* ---- 42: format speed real-time when live ---- */

void test_format_speed_realtime_when_live(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    tc_display_t d = tc_format(s);
    TEST_ASSERT_EQUAL_STRING("Real-time", d.speed_display);
}

/* ---- 43: reverse while paused updates saved_direction ---- */

void test_reverse_while_paused(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_toggle_pause(s);
    s = tc_reverse(s);
    /* Should still be paused */
    TEST_ASSERT_EQUAL_INT(TC_DIR_PAUSED, s.direction);
    /* Unpause should go reverse */
    s = tc_toggle_pause(s);
    TEST_ASSERT_EQUAL_INT(TC_DIR_REVERSE, s.direction);
}

/* ---- 44: speed preset 5 is 10 Days/sec ---- */

void test_speed_get_5_is_ten_days(void)
{
    tc_speed_t sp = tc_speed_get(5);
    TEST_ASSERT_EQUAL_STRING("10 Days/sec", sp.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 864000.0, sp.multiplier);
}

/* ---- 45: advance with high speed ---- */

void test_advance_high_speed(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0;
    s = tc_set_speed(s, 4); /* 86400x = Day/sec */
    /* 1 second at Day/sec = 1 day */
    s = tc_advance(s, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000 + 1.0, s.jd);
}

/* ---- 46: tc_format_speed negative direction ---- */

void test_format_speed_reverse_shows_negative(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s.is_live = 0;
    s = tc_set_speed(s, 2); /* 60x */
    s = tc_reverse(s);
    char buf[32];
    tc_format_speed(s, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("-60x", buf);
}

/* ---- 47: tc_speed_name for 60.0 ---- */

void test_speed_name_60_is_fast(void)
{
    TEST_ASSERT_EQUAL_STRING("Fast", tc_speed_name(60.0));
}

/* ---- 48: tc_set_speed from paused to non-zero resumes forward ---- */

void test_set_speed_from_paused_resumes_forward(void)
{
    tc_state_t s = tc_state_init(JD_J2000);
    s = tc_set_speed(s, 0); /* Pause */
    TEST_ASSERT_EQUAL_INT(TC_DIR_PAUSED, s.direction);
    s = tc_set_speed(s, 2); /* Fast (60x) */
    TEST_ASSERT_EQUAL_INT(TC_DIR_FORWARD, s.direction);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 60.0, s.speed);
}

/* ---- Main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Speed presets: 1-9 */
    RUN_TEST(test_speed_count_returns_six);
    RUN_TEST(test_speed_get_0_is_paused);
    RUN_TEST(test_speed_get_1_is_realtime);
    RUN_TEST(test_speed_get_4_is_day_per_sec);
    RUN_TEST(test_speed_find_by_key_0_returns_0);
    RUN_TEST(test_speed_find_by_key_3_returns_3);
    RUN_TEST(test_speed_find_by_key_9_returns_neg1);
    RUN_TEST(test_speed_name_1_is_realtime);
    RUN_TEST(test_speed_name_999_is_custom);

    /* State init: 10 */
    RUN_TEST(test_state_init_defaults);

    /* Set speed: 11-13 */
    RUN_TEST(test_set_speed_to_preset_3);
    RUN_TEST(test_set_speed_by_key_4);
    RUN_TEST(test_set_speed_by_key_0_pauses);

    /* Pause/resume: 14-15 */
    RUN_TEST(test_toggle_pause_from_running);
    RUN_TEST(test_toggle_pause_from_paused_restores);

    /* Reverse: 16-17 */
    RUN_TEST(test_reverse_flips_direction);
    RUN_TEST(test_reverse_twice_restores);

    /* Go live: 18 */
    RUN_TEST(test_go_live_restores_defaults);

    /* Step: 19-21 */
    RUN_TEST(test_step_forward_increases_jd);
    RUN_TEST(test_step_backward_decreases_jd);
    RUN_TEST(test_step_disables_live);

    /* Advance: 22-24 */
    RUN_TEST(test_advance_changes_jd);
    RUN_TEST(test_advance_paused_no_change);
    RUN_TEST(test_advance_reverse_decreases_jd);

    /* Query: 25-27 */
    RUN_TEST(test_is_paused_correct);
    RUN_TEST(test_is_reverse_correct);
    RUN_TEST(test_is_live_correct);

    /* Format display: 28-36 */
    RUN_TEST(test_format_paused_shows_paused);
    RUN_TEST(test_format_speed_3600x);
    RUN_TEST(test_format_direction_icon_forward);
    RUN_TEST(test_format_direction_icon_paused);
    RUN_TEST(test_format_direction_icon_reverse);
    RUN_TEST(test_format_speed_writes_buffer);
    RUN_TEST(test_direction_icon_all_three);
    RUN_TEST(test_format_date_j2000);
    RUN_TEST(test_format_day_name_j2000);

    /* Edge cases: 37-39 */
    RUN_TEST(test_speed_get_out_of_range);
    RUN_TEST(test_set_speed_invalid_index_no_change);
    RUN_TEST(test_set_speed_by_key_invalid_no_change);

    /* Advanced: 40-48 */
    RUN_TEST(test_advance_live_no_effect);
    RUN_TEST(test_reverse_disables_live);
    RUN_TEST(test_format_speed_realtime_when_live);
    RUN_TEST(test_reverse_while_paused);
    RUN_TEST(test_speed_get_5_is_ten_days);
    RUN_TEST(test_advance_high_speed);
    RUN_TEST(test_format_speed_reverse_shows_negative);
    RUN_TEST(test_speed_name_60_is_fast);
    RUN_TEST(test_set_speed_from_paused_resumes_forward);

    return UNITY_END();
}
