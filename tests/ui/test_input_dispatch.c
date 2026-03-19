#include "../../tests/unity/unity.h"
#include "../../src/ui/input_dispatch.h"

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Basic key dispatch
 * =================================================================== */

void test_id_dispatch_e_earth(void)
{
    id_result_t r = id_dispatch('E', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_VIEW_EARTH, r.action);
    TEST_ASSERT_EQUAL_INT(0, r.parameter);
}

void test_id_dispatch_esc_overview(void)
{
    id_result_t r = id_dispatch(27, 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_OVERVIEW, r.action);
}

void test_id_dispatch_space_pause(void)
{
    id_result_t r = id_dispatch(32, 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_PAUSE, r.action);
}

/* ===================================================================
 * Focus mode keys
 * =================================================================== */

void test_id_dispatch_a_astrology(void)
{
    id_result_t r = id_dispatch('A', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_ASTROLOGY, r.action);
}

void test_id_dispatch_k_kin(void)
{
    id_result_t r = id_dispatch('K', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_KIN, r.action);
}

void test_id_dispatch_i_iching(void)
{
    id_result_t r = id_dispatch('I', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_ICHING, r.action);
}

void test_id_dispatch_c_chinese(void)
{
    id_result_t r = id_dispatch('C', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_CHINESE, r.action);
}

void test_id_dispatch_d_hd(void)
{
    id_result_t r = id_dispatch('D', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_HD, r.action);
}

/* ===================================================================
 * Toggle keys
 * =================================================================== */

void test_id_dispatch_t_trails(void)
{
    id_result_t r = id_dispatch('T', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_TRAILS, r.action);
}

void test_id_dispatch_h_hud(void)
{
    id_result_t r = id_dispatch('H', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_HUD, r.action);
}

void test_id_dispatch_s_stars(void)
{
    id_result_t r = id_dispatch('S', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_STARS, r.action);
}

void test_id_dispatch_l_labels(void)
{
    id_result_t r = id_dispatch('L', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_LABELS, r.action);
}

void test_id_dispatch_g_grid(void)
{
    id_result_t r = id_dispatch('G', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_GRID, r.action);
}

/* ===================================================================
 * Time control keys
 * =================================================================== */

void test_id_dispatch_1_realtime(void)
{
    id_result_t r = id_dispatch('1', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_REALTIME, r.action);
}

void test_id_dispatch_2_fast(void)
{
    id_result_t r = id_dispatch('2', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_FAST, r.action);
}

void test_id_dispatch_3_faster(void)
{
    id_result_t r = id_dispatch('3', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_FASTER, r.action);
}

void test_id_dispatch_4_day_sec(void)
{
    id_result_t r = id_dispatch('4', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_DAY_SEC, r.action);
}

void test_id_dispatch_5_10day_sec(void)
{
    id_result_t r = id_dispatch('5', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_10DAY_SEC, r.action);
}

void test_id_dispatch_0_pause(void)
{
    id_result_t r = id_dispatch('0', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_PAUSE, r.action);
}

void test_id_dispatch_minus_reverse(void)
{
    id_result_t r = id_dispatch('-', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TIME_REVERSE, r.action);
}

/* ===================================================================
 * Scale jump (Shift + digit)
 * =================================================================== */

void test_id_dispatch_shift_0_scale(void)
{
    id_result_t r = id_dispatch('0', 1);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_SCALE_JUMP, r.action);
    TEST_ASSERT_EQUAL_INT(0, r.parameter);
}

void test_id_dispatch_shift_3_scale(void)
{
    id_result_t r = id_dispatch('3', 1);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_SCALE_JUMP, r.action);
    TEST_ASSERT_EQUAL_INT(3, r.parameter);
}

void test_id_dispatch_shift_6_scale(void)
{
    id_result_t r = id_dispatch('6', 1);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_SCALE_JUMP, r.action);
    TEST_ASSERT_EQUAL_INT(6, r.parameter);
}

/* ===================================================================
 * Unknown / no-op keys
 * =================================================================== */

void test_id_dispatch_unknown(void)
{
    id_result_t r = id_dispatch('Z', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_NONE, r.action);
    TEST_ASSERT_EQUAL_INT(0, r.parameter);
}

void test_id_dispatch_unknown_number(void)
{
    /* '7' is not a valid time key */
    id_result_t r = id_dispatch('7', 0);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_NONE, r.action);
}

/* ===================================================================
 * Case insensitivity
 * =================================================================== */

void test_id_dispatch_case_insensitive(void)
{
    TEST_ASSERT_EQUAL_INT(ID_ACTION_VIEW_EARTH,      id_dispatch('e', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_VIEW_EARTH,      id_dispatch('E', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_ASTROLOGY,  id_dispatch('a', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_KIN,        id_dispatch('k', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_ICHING,     id_dispatch('i', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_CHINESE,    id_dispatch('c', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_FOCUS_HD,         id_dispatch('d', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_TRAILS,    id_dispatch('t', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_HUD,       id_dispatch('h', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_STARS,     id_dispatch('s', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_LABELS,    id_dispatch('l', 0).action);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_GRID,      id_dispatch('g', 0).action);
}

/* ===================================================================
 * Action name
 * =================================================================== */

void test_id_action_name_valid(void)
{
    for (int i = 0; i < ID_ACTION_COUNT; i++) {
        const char *name = id_action_name((id_action_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

void test_id_action_name_none(void)
{
    const char *name = id_action_name(ID_ACTION_NONE);
    TEST_ASSERT_NOT_NULL(name);
}

void test_id_action_name_invalid(void)
{
    const char *name = id_action_name(ID_ACTION_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===================================================================
 * Key label
 * =================================================================== */

void test_id_key_label_valid(void)
{
    const char *label = id_key_label(ID_ACTION_VIEW_EARTH);
    TEST_ASSERT_NOT_NULL(label);
    TEST_ASSERT_TRUE(label[0] != '\0');
}

void test_id_key_label_space(void)
{
    const char *label = id_key_label(ID_ACTION_TOGGLE_PAUSE);
    TEST_ASSERT_NOT_NULL(label);
}

void test_id_key_label_invalid(void)
{
    const char *label = id_key_label(ID_ACTION_COUNT);
    TEST_ASSERT_NOT_NULL(label);
}

/* ===================================================================
 * Category checks
 * =================================================================== */

void test_id_is_focus(void)
{
    TEST_ASSERT_EQUAL_INT(1, id_is_focus_action(ID_ACTION_FOCUS_ASTROLOGY));
    TEST_ASSERT_EQUAL_INT(1, id_is_focus_action(ID_ACTION_FOCUS_KIN));
    TEST_ASSERT_EQUAL_INT(1, id_is_focus_action(ID_ACTION_FOCUS_ICHING));
    TEST_ASSERT_EQUAL_INT(1, id_is_focus_action(ID_ACTION_FOCUS_CHINESE));
    TEST_ASSERT_EQUAL_INT(1, id_is_focus_action(ID_ACTION_FOCUS_HD));
    TEST_ASSERT_EQUAL_INT(1, id_is_focus_action(ID_ACTION_FOCUS_OVERVIEW));
    TEST_ASSERT_EQUAL_INT(0, id_is_focus_action(ID_ACTION_TOGGLE_HUD));
    TEST_ASSERT_EQUAL_INT(0, id_is_focus_action(ID_ACTION_TIME_REALTIME));
    TEST_ASSERT_EQUAL_INT(0, id_is_focus_action(ID_ACTION_NONE));
}

void test_id_is_time(void)
{
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_REALTIME));
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_FAST));
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_FASTER));
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_DAY_SEC));
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_10DAY_SEC));
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_PAUSE));
    TEST_ASSERT_EQUAL_INT(1, id_is_time_action(ID_ACTION_TIME_REVERSE));
    TEST_ASSERT_EQUAL_INT(0, id_is_time_action(ID_ACTION_TOGGLE_HUD));
    TEST_ASSERT_EQUAL_INT(0, id_is_time_action(ID_ACTION_FOCUS_KIN));
    TEST_ASSERT_EQUAL_INT(0, id_is_time_action(ID_ACTION_NONE));
}

void test_id_is_toggle(void)
{
    TEST_ASSERT_EQUAL_INT(1, id_is_toggle_action(ID_ACTION_TOGGLE_TRAILS));
    TEST_ASSERT_EQUAL_INT(1, id_is_toggle_action(ID_ACTION_TOGGLE_HUD));
    TEST_ASSERT_EQUAL_INT(1, id_is_toggle_action(ID_ACTION_TOGGLE_STARS));
    TEST_ASSERT_EQUAL_INT(1, id_is_toggle_action(ID_ACTION_TOGGLE_LABELS));
    TEST_ASSERT_EQUAL_INT(1, id_is_toggle_action(ID_ACTION_TOGGLE_GRID));
    TEST_ASSERT_EQUAL_INT(1, id_is_toggle_action(ID_ACTION_TOGGLE_PAUSE));
    TEST_ASSERT_EQUAL_INT(0, id_is_toggle_action(ID_ACTION_FOCUS_KIN));
    TEST_ASSERT_EQUAL_INT(0, id_is_toggle_action(ID_ACTION_TIME_REALTIME));
    TEST_ASSERT_EQUAL_INT(0, id_is_toggle_action(ID_ACTION_NONE));
}

/* ===================================================================
 * Time speed multipliers
 * =================================================================== */

void test_id_time_speed_values(void)
{
    TEST_ASSERT_EQUAL_INT(1,      id_time_speed(ID_ACTION_TIME_REALTIME));
    TEST_ASSERT_EQUAL_INT(60,     id_time_speed(ID_ACTION_TIME_FAST));
    TEST_ASSERT_EQUAL_INT(3600,   id_time_speed(ID_ACTION_TIME_FASTER));
    TEST_ASSERT_EQUAL_INT(86400,  id_time_speed(ID_ACTION_TIME_DAY_SEC));
    TEST_ASSERT_EQUAL_INT(864000, id_time_speed(ID_ACTION_TIME_10DAY_SEC));
    TEST_ASSERT_EQUAL_INT(0,      id_time_speed(ID_ACTION_TIME_PAUSE));
    TEST_ASSERT_EQUAL_INT(-1,     id_time_speed(ID_ACTION_TIME_REVERSE));
}

void test_id_time_speed_non_time(void)
{
    TEST_ASSERT_EQUAL_INT(0, id_time_speed(ID_ACTION_NONE));
    TEST_ASSERT_EQUAL_INT(0, id_time_speed(ID_ACTION_TOGGLE_HUD));
    TEST_ASSERT_EQUAL_INT(0, id_time_speed(ID_ACTION_FOCUS_KIN));
}

/* ===================================================================
 * Shift does not affect letter keys
 * =================================================================== */

void test_id_dispatch_shift_letter_unchanged(void)
{
    /* Shift + letter should still dispatch the letter action */
    id_result_t r = id_dispatch('E', 1);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_VIEW_EARTH, r.action);

    r = id_dispatch('T', 1);
    TEST_ASSERT_EQUAL_INT(ID_ACTION_TOGGLE_TRAILS, r.action);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Basic key dispatch */
    RUN_TEST(test_id_dispatch_e_earth);
    RUN_TEST(test_id_dispatch_esc_overview);
    RUN_TEST(test_id_dispatch_space_pause);

    /* Focus mode keys */
    RUN_TEST(test_id_dispatch_a_astrology);
    RUN_TEST(test_id_dispatch_k_kin);
    RUN_TEST(test_id_dispatch_i_iching);
    RUN_TEST(test_id_dispatch_c_chinese);
    RUN_TEST(test_id_dispatch_d_hd);

    /* Toggle keys */
    RUN_TEST(test_id_dispatch_t_trails);
    RUN_TEST(test_id_dispatch_h_hud);
    RUN_TEST(test_id_dispatch_s_stars);
    RUN_TEST(test_id_dispatch_l_labels);
    RUN_TEST(test_id_dispatch_g_grid);

    /* Time control keys */
    RUN_TEST(test_id_dispatch_1_realtime);
    RUN_TEST(test_id_dispatch_2_fast);
    RUN_TEST(test_id_dispatch_3_faster);
    RUN_TEST(test_id_dispatch_4_day_sec);
    RUN_TEST(test_id_dispatch_5_10day_sec);
    RUN_TEST(test_id_dispatch_0_pause);
    RUN_TEST(test_id_dispatch_minus_reverse);

    /* Scale jump */
    RUN_TEST(test_id_dispatch_shift_0_scale);
    RUN_TEST(test_id_dispatch_shift_3_scale);
    RUN_TEST(test_id_dispatch_shift_6_scale);

    /* Unknown keys */
    RUN_TEST(test_id_dispatch_unknown);
    RUN_TEST(test_id_dispatch_unknown_number);

    /* Case insensitivity */
    RUN_TEST(test_id_dispatch_case_insensitive);

    /* Action name */
    RUN_TEST(test_id_action_name_valid);
    RUN_TEST(test_id_action_name_none);
    RUN_TEST(test_id_action_name_invalid);

    /* Key label */
    RUN_TEST(test_id_key_label_valid);
    RUN_TEST(test_id_key_label_space);
    RUN_TEST(test_id_key_label_invalid);

    /* Category checks */
    RUN_TEST(test_id_is_focus);
    RUN_TEST(test_id_is_time);
    RUN_TEST(test_id_is_toggle);

    /* Time speed */
    RUN_TEST(test_id_time_speed_values);
    RUN_TEST(test_id_time_speed_non_time);

    /* Shift + letter */
    RUN_TEST(test_id_dispatch_shift_letter_unchanged);

    return UNITY_END();
}
