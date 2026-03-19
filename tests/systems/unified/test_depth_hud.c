/* test_depth_hud.c — Tests for HUD integration data module
 * Defines where each knowledge system appears in the HUD,
 * toggle keys, position, and format templates per depth tier.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/depth_hud.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== dh_system_count ========== */

static void test_system_count_returns_20(void)
{
    TEST_ASSERT_EQUAL_INT(20, dh_system_count());
}

/* ========== dh_position_count ========== */

static void test_position_count_returns_8(void)
{
    TEST_ASSERT_EQUAL_INT(8, dh_position_count());
}

/* ========== dh_system_spec — individual system checks ========== */

static void test_spec_gregorian(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GREGORIAN, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Gregorian", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('0', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_TOP_CENTER, s.position);
    TEST_ASSERT_EQUAL_INT(0, s.priority);
}

static void test_spec_hebrew(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HEBREW, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Hebrew", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('H', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_LEFT_PANEL, s.position);
    TEST_ASSERT_EQUAL_INT(3, s.priority);
}

static void test_spec_islamic(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_ISLAMIC);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ISLAMIC, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Islamic", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('I', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_LEFT_PANEL, s.position);
    TEST_ASSERT_EQUAL_INT(4, s.priority);
}

static void test_spec_buddhist(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_INT(TS_SYS_BUDDHIST, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Buddhist", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('B', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_LEFT_PANEL, s.position);
    TEST_ASSERT_EQUAL_INT(6, s.priority);
}

static void test_spec_hindu(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_HINDU);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HINDU, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Hindu/Vedic", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('V', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_LEFT_PANEL, s.position);
    TEST_ASSERT_EQUAL_INT(7, s.priority);
}

static void test_spec_kabbalah(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_KABBALAH);
    TEST_ASSERT_EQUAL_INT(TS_SYS_KABBALAH, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Kabbalah", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('Q', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_RIGHT_PANEL, s.position);
    TEST_ASSERT_EQUAL_INT(9, s.priority);
}

static void test_spec_iching(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ICHING, s.system_id);
    TEST_ASSERT_EQUAL_STRING("I Ching", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('X', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_TOP_RIGHT, s.position);
    TEST_ASSERT_EQUAL_INT(2, s.priority);
}

static void test_spec_tzolkin(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_TZOLKIN, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('T', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_TOP_RIGHT, s.position);
    TEST_ASSERT_EQUAL_INT(1, s.priority);
}

static void test_spec_astrology(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_ASTROLOGY);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ASTROLOGY, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Astrology", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('A', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_BOTTOM_LEFT, s.position);
    TEST_ASSERT_EQUAL_INT(1, s.priority);
}

static void test_spec_human_design(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_HUMAN_DESIGN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HUMAN_DESIGN, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Human Design", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('D', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_RIGHT_PANEL, s.position);
    TEST_ASSERT_EQUAL_INT(8, s.priority);
}

static void test_spec_chinese(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_CHINESE);
    TEST_ASSERT_EQUAL_INT(TS_SYS_CHINESE, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Chinese", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('C', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_BOTTOM_LEFT, s.position);
    TEST_ASSERT_EQUAL_INT(5, s.priority);
}

static void test_spec_haab(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_HAAB);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HAAB, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Haab", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('.', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_TOP_RIGHT, s.position);
    TEST_ASSERT_EQUAL_INT(12, s.priority);
}

static void test_spec_geological(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_GEOLOGICAL);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GEOLOGICAL, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Geological", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('G', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_BOTTOM_CENTER, s.position);
    TEST_ASSERT_EQUAL_INT(17, s.priority);
}

static void test_spec_cosmic(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_COSMIC);
    TEST_ASSERT_EQUAL_INT(TS_SYS_COSMIC, s.system_id);
    TEST_ASSERT_EQUAL_STRING("Cosmic", s.display_name);
    TEST_ASSERT_EQUAL_CHAR('/', s.toggle_key);
    TEST_ASSERT_EQUAL_INT(HUD_POS_BOTTOM_CENTER, s.position);
    TEST_ASSERT_EQUAL_INT(18, s.priority);
}

/* ========== dh_system_spec — loop: all 20 valid ========== */

static void test_all_20_systems_have_valid_names(void)
{
    for (int i = 0; i < 20; i++) {
        dh_system_spec_t s = dh_system_spec(i);
        TEST_ASSERT_EQUAL_INT(i, s.system_id);
        TEST_ASSERT_NOT_EQUAL(0, strcmp(s.display_name, "?"));
    }
}

/* ========== dh_system_spec — invalid ========== */

static void test_spec_invalid_20(void)
{
    dh_system_spec_t s = dh_system_spec(20);
    TEST_ASSERT_EQUAL_INT(-1, s.system_id);
    TEST_ASSERT_EQUAL_STRING("?", s.display_name);
}

static void test_spec_invalid_negative(void)
{
    dh_system_spec_t s = dh_system_spec(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.system_id);
    TEST_ASSERT_EQUAL_STRING("?", s.display_name);
}

/* ========== dh_system_by_key ========== */

static void test_key_H_is_hebrew(void)
{
    TEST_ASSERT_EQUAL_INT(TS_SYS_HEBREW, dh_system_by_key('H'));
}

static void test_key_I_is_islamic(void)
{
    TEST_ASSERT_EQUAL_INT(TS_SYS_ISLAMIC, dh_system_by_key('I'));
}

static void test_key_Q_is_kabbalah(void)
{
    TEST_ASSERT_EQUAL_INT(TS_SYS_KABBALAH, dh_system_by_key('Q'));
}

static void test_key_T_is_tzolkin(void)
{
    TEST_ASSERT_EQUAL_INT(TS_SYS_TZOLKIN, dh_system_by_key('T'));
}

static void test_key_X_is_iching(void)
{
    TEST_ASSERT_EQUAL_INT(TS_SYS_ICHING, dh_system_by_key('X'));
}

static void test_key_Z_not_found(void)
{
    TEST_ASSERT_EQUAL_INT(-1, dh_system_by_key('Z'));
}

static void test_key_nul_not_found(void)
{
    TEST_ASSERT_EQUAL_INT(-1, dh_system_by_key('\0'));
}

/* ========== dh_systems_at_position ========== */

static void test_left_panel_has_multiple_systems(void)
{
    int ids[24];
    int n = dh_systems_at_position(HUD_POS_LEFT_PANEL, ids, 24);
    /* Hebrew, Islamic, Buddhist, Hindu, Coptic, Ethiopian, Persian = 7 */
    TEST_ASSERT_EQUAL_INT(7, n);
}

static void test_top_right_has_systems(void)
{
    int ids[24];
    int n = dh_systems_at_position(HUD_POS_TOP_RIGHT, ids, 24);
    /* Tzolkin, Haab, I Ching = 3 */
    TEST_ASSERT_EQUAL_INT(3, n);
}

static void test_systems_at_position_null_out(void)
{
    int n = dh_systems_at_position(HUD_POS_LEFT_PANEL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(7, n);
}

static void test_systems_at_position_invalid(void)
{
    int ids[24];
    int n = dh_systems_at_position((hud_position_t)99, ids, 24);
    TEST_ASSERT_EQUAL_INT(0, n);
}

static void test_systems_at_position_max_limit(void)
{
    int ids[2];
    int n = dh_systems_at_position(HUD_POS_LEFT_PANEL, ids, 2);
    /* Should only fill 2 even though there are 7 */
    TEST_ASSERT_EQUAL_INT(2, n);
}

static void test_top_left_is_empty(void)
{
    int ids[24];
    int n = dh_systems_at_position(HUD_POS_TOP_LEFT, ids, 24);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ========== dh_position_name ========== */

static void test_position_name_top_left(void)
{
    TEST_ASSERT_EQUAL_STRING("Top Left", dh_position_name(HUD_POS_TOP_LEFT));
}

static void test_position_name_top_center(void)
{
    TEST_ASSERT_EQUAL_STRING("Top Center", dh_position_name(HUD_POS_TOP_CENTER));
}

static void test_position_name_top_right(void)
{
    TEST_ASSERT_EQUAL_STRING("Top Right", dh_position_name(HUD_POS_TOP_RIGHT));
}

static void test_position_name_bottom_left(void)
{
    TEST_ASSERT_EQUAL_STRING("Bottom Left", dh_position_name(HUD_POS_BOTTOM_LEFT));
}

static void test_position_name_bottom_center(void)
{
    TEST_ASSERT_EQUAL_STRING("Bottom Center", dh_position_name(HUD_POS_BOTTOM_CENTER));
}

static void test_position_name_bottom_right(void)
{
    TEST_ASSERT_EQUAL_STRING("Bottom Right", dh_position_name(HUD_POS_BOTTOM_RIGHT));
}

static void test_position_name_left_panel(void)
{
    TEST_ASSERT_EQUAL_STRING("Left Panel", dh_position_name(HUD_POS_LEFT_PANEL));
}

static void test_position_name_right_panel(void)
{
    TEST_ASSERT_EQUAL_STRING("Right Panel", dh_position_name(HUD_POS_RIGHT_PANEL));
}

static void test_position_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", dh_position_name((hud_position_t)99));
}

/* ========== No duplicate toggle keys ========== */

static void test_no_duplicate_toggle_keys(void)
{
    char keys[20];
    for (int i = 0; i < 20; i++) {
        dh_system_spec_t s = dh_system_spec(i);
        keys[i] = s.toggle_key;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            TEST_ASSERT_NOT_EQUAL_MESSAGE(keys[i], keys[j],
                "Duplicate toggle key found");
        }
    }
}

/* ========== All format templates non-empty ========== */

static void test_all_systems_have_format_t0(void)
{
    for (int i = 0; i < 20; i++) {
        dh_system_spec_t s = dh_system_spec(i);
        TEST_ASSERT_NOT_NULL(s.format_t0);
        TEST_ASSERT_TRUE(strlen(s.format_t0) > 0);
    }
}

static void test_all_systems_have_format_t1(void)
{
    for (int i = 0; i < 20; i++) {
        dh_system_spec_t s = dh_system_spec(i);
        TEST_ASSERT_NOT_NULL(s.format_t1);
        TEST_ASSERT_TRUE(strlen(s.format_t1) > 0);
    }
}

/* ========== Format template spot checks ========== */

static void test_gregorian_format_t0(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_STRING("{date}", s.format_t0);
}

static void test_gregorian_format_t1(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_STRING("{weekday} {date}", s.format_t1);
}

static void test_tzolkin_format_t0(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_STRING("{kin}", s.format_t0);
}

static void test_buddhist_format_t0(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_STRING("BE", s.format_t0);
}

static void test_hebrew_format_t2(void)
{
    dh_system_spec_t s = dh_system_spec(TS_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(s.format_t2);
    TEST_ASSERT_TRUE(strlen(s.format_t2) > 0);
}

int main(void)
{
    UNITY_BEGIN();

    /* counts */
    RUN_TEST(test_system_count_returns_20);
    RUN_TEST(test_position_count_returns_8);

    /* individual system specs */
    RUN_TEST(test_spec_gregorian);
    RUN_TEST(test_spec_hebrew);
    RUN_TEST(test_spec_islamic);
    RUN_TEST(test_spec_buddhist);
    RUN_TEST(test_spec_hindu);
    RUN_TEST(test_spec_kabbalah);
    RUN_TEST(test_spec_iching);
    RUN_TEST(test_spec_tzolkin);
    RUN_TEST(test_spec_astrology);
    RUN_TEST(test_spec_human_design);
    RUN_TEST(test_spec_chinese);
    RUN_TEST(test_spec_haab);
    RUN_TEST(test_spec_geological);
    RUN_TEST(test_spec_cosmic);

    /* all 20 valid */
    RUN_TEST(test_all_20_systems_have_valid_names);

    /* invalid specs */
    RUN_TEST(test_spec_invalid_20);
    RUN_TEST(test_spec_invalid_negative);

    /* key lookup */
    RUN_TEST(test_key_H_is_hebrew);
    RUN_TEST(test_key_I_is_islamic);
    RUN_TEST(test_key_Q_is_kabbalah);
    RUN_TEST(test_key_T_is_tzolkin);
    RUN_TEST(test_key_X_is_iching);
    RUN_TEST(test_key_Z_not_found);
    RUN_TEST(test_key_nul_not_found);

    /* position queries */
    RUN_TEST(test_left_panel_has_multiple_systems);
    RUN_TEST(test_top_right_has_systems);
    RUN_TEST(test_systems_at_position_null_out);
    RUN_TEST(test_systems_at_position_invalid);
    RUN_TEST(test_systems_at_position_max_limit);
    RUN_TEST(test_top_left_is_empty);

    /* position names */
    RUN_TEST(test_position_name_top_left);
    RUN_TEST(test_position_name_top_center);
    RUN_TEST(test_position_name_top_right);
    RUN_TEST(test_position_name_bottom_left);
    RUN_TEST(test_position_name_bottom_center);
    RUN_TEST(test_position_name_bottom_right);
    RUN_TEST(test_position_name_left_panel);
    RUN_TEST(test_position_name_right_panel);
    RUN_TEST(test_position_name_invalid);

    /* uniqueness & completeness */
    RUN_TEST(test_no_duplicate_toggle_keys);
    RUN_TEST(test_all_systems_have_format_t0);
    RUN_TEST(test_all_systems_have_format_t1);

    /* format template spot checks */
    RUN_TEST(test_gregorian_format_t0);
    RUN_TEST(test_gregorian_format_t1);
    RUN_TEST(test_tzolkin_format_t0);
    RUN_TEST(test_buddhist_format_t0);
    RUN_TEST(test_hebrew_format_t2);

    return UNITY_END();
}
