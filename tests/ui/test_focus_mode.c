#include "../unity/unity.h"
#include "../../src/ui/focus_mode.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Init: 1-3 --- */

void test_init_overview(void)
{
    focus_modes_t fm = focus_mode_init();
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
}

void test_init_no_card_visible(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_OVERVIEW);
    TEST_ASSERT_EQUAL_INT(0, p.card_mask);
}

void test_preset_count(void)
{
    TEST_ASSERT_EQUAL_INT(6, FOCUS_MODE_COUNT);
}

/* --- Preset data: 4-9 --- */

void test_astrology_preset(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_ASTROLOGY);
    TEST_ASSERT_EQUAL_STRING("Astrology", p.name);
    TEST_ASSERT_EQUAL_INT('A', p.key);
    TEST_ASSERT_TRUE(p.card_mask & (1 << CARD_ASTROLOGY));
    TEST_ASSERT_TRUE(p.zoom_target > 0.0f);
}

void test_kin_preset(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_KIN);
    TEST_ASSERT_EQUAL_STRING("Kin", p.name);
    TEST_ASSERT_EQUAL_INT('K', p.key);
    TEST_ASSERT_TRUE(p.card_mask & (1 << CARD_TZOLKIN));
}

void test_iching_preset(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_ICHING);
    TEST_ASSERT_EQUAL_STRING("I Ching", p.name);
    TEST_ASSERT_EQUAL_INT('I', p.key);
    TEST_ASSERT_TRUE(p.card_mask & (1 << CARD_ICHING));
}

void test_chinese_preset(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_CHINESE);
    TEST_ASSERT_EQUAL_STRING("Chinese", p.name);
    TEST_ASSERT_EQUAL_INT('C', p.key);
    TEST_ASSERT_TRUE(p.card_mask & (1 << CARD_CHINESE));
}

void test_hd_preset(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_HD);
    TEST_ASSERT_EQUAL_STRING("Human Design", p.name);
    TEST_ASSERT_EQUAL_INT('D', p.key);
    TEST_ASSERT_TRUE(p.card_mask & (1 << CARD_HD));
}

void test_overview_preset(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_OVERVIEW);
    TEST_ASSERT_EQUAL_STRING("Overview", p.name);
    TEST_ASSERT_EQUAL_INT(27, p.key); /* Esc */
    TEST_ASSERT_EQUAL_INT(0, p.card_mask);
}

/* --- Mode switching: 10-16 --- */

void test_switch_to_astrology(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_ASTROLOGY);
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_ASTROLOGY, focus_mode_active(fm));
}

void test_switch_to_overview_via_esc(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_KIN);
    fm = focus_mode_switch(fm, FOCUS_MODE_OVERVIEW);
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
}

void test_switch_same_mode_returns_overview(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_ICHING);
    fm = focus_mode_switch(fm, FOCUS_MODE_ICHING); /* toggle off */
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
}

void test_switch_from_key(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_from_key(fm, 'A');
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_ASTROLOGY, focus_mode_active(fm));
}

void test_esc_returns_overview(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_HD);
    fm = focus_mode_from_key(fm, 27); /* Esc */
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
}

void test_unknown_key_no_change(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_from_key(fm, 'X'); /* not a focus key */
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
}

void test_switch_invalid_mode_no_change(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, -1);
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
    fm = focus_mode_switch(fm, 99);
    TEST_ASSERT_EQUAL_INT(FOCUS_MODE_OVERVIEW, focus_mode_active(fm));
}

/* --- Layer config per mode: 17-20 --- */

void test_astrology_highlights_zodiac(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_ASTROLOGY);
    /* Zodiac ring and aspects should be highlighted (high opacity) */
    TEST_ASSERT_TRUE(p.layer_opacities[LAYER_ZODIAC_RING] > 0.8f);
    TEST_ASSERT_TRUE(p.layer_opacities[LAYER_PLANETS] > 0.5f);
}

void test_kin_dims_3d(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_KIN);
    /* Stars and orbits should be dimmed */
    TEST_ASSERT_TRUE(p.layer_opacities[LAYER_STARS] < 0.5f);
    TEST_ASSERT_TRUE(p.layer_opacities[LAYER_ORBITS] < 0.5f);
}

void test_overview_all_layers_full(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, FOCUS_MODE_OVERVIEW);
    for (int i = 0; i < LAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, p.layer_opacities[i]);
    }
}

void test_preset_invalid_returns_overview(void)
{
    focus_modes_t fm = focus_mode_init();
    focus_preset_t p = focus_mode_preset(fm, -1);
    TEST_ASSERT_EQUAL_STRING("Overview", p.name);
    p = focus_mode_preset(fm, 99);
    TEST_ASSERT_EQUAL_STRING("Overview", p.name);
}

/* --- Transition state: 21-24 --- */

void test_is_transitioning_after_switch(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_ASTROLOGY);
    TEST_ASSERT_TRUE(focus_mode_is_transitioning(fm));
}

void test_transition_completes(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_ICHING);
    fm = focus_mode_tick(fm, 5.0f); /* well past any transition */
    TEST_ASSERT_FALSE(focus_mode_is_transitioning(fm));
}

void test_transition_progress(void)
{
    focus_modes_t fm = focus_mode_init();
    fm = focus_mode_switch(fm, FOCUS_MODE_KIN);
    float p = focus_mode_transition_progress(fm);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, p);
    fm = focus_mode_tick(fm, 5.0f);
    p = focus_mode_transition_progress(fm);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, p);
}

void test_no_transition_in_overview(void)
{
    focus_modes_t fm = focus_mode_init();
    TEST_ASSERT_FALSE(focus_mode_is_transitioning(fm));
}

/* --- Mode name: 25-26 --- */

void test_mode_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Overview", focus_mode_name(FOCUS_MODE_OVERVIEW));
    TEST_ASSERT_EQUAL_STRING("Astrology", focus_mode_name(FOCUS_MODE_ASTROLOGY));
    TEST_ASSERT_EQUAL_STRING("Kin", focus_mode_name(FOCUS_MODE_KIN));
}

void test_mode_name_invalid(void)
{
    const char *name = focus_mode_name(-1);
    TEST_ASSERT_EQUAL_STRING("Overview", name);
}

/* --- Purity: 27 --- */

void test_purity(void)
{
    focus_modes_t a = focus_mode_init();
    focus_modes_t b = focus_mode_init();
    TEST_ASSERT_EQUAL_INT(focus_mode_active(a), focus_mode_active(b));
    focus_preset_t pa = focus_mode_preset(a, FOCUS_MODE_ASTROLOGY);
    focus_preset_t pb = focus_mode_preset(b, FOCUS_MODE_ASTROLOGY);
    TEST_ASSERT_EQUAL_STRING(pa.name, pb.name);
    TEST_ASSERT_EQUAL_INT(pa.key, pb.key);
}

int main(void)
{
    UNITY_BEGIN();
    /* Init */
    RUN_TEST(test_init_overview);
    RUN_TEST(test_init_no_card_visible);
    RUN_TEST(test_preset_count);
    /* Preset data */
    RUN_TEST(test_astrology_preset);
    RUN_TEST(test_kin_preset);
    RUN_TEST(test_iching_preset);
    RUN_TEST(test_chinese_preset);
    RUN_TEST(test_hd_preset);
    RUN_TEST(test_overview_preset);
    /* Mode switching */
    RUN_TEST(test_switch_to_astrology);
    RUN_TEST(test_switch_to_overview_via_esc);
    RUN_TEST(test_switch_same_mode_returns_overview);
    RUN_TEST(test_switch_from_key);
    RUN_TEST(test_esc_returns_overview);
    RUN_TEST(test_unknown_key_no_change);
    RUN_TEST(test_switch_invalid_mode_no_change);
    /* Layer config */
    RUN_TEST(test_astrology_highlights_zodiac);
    RUN_TEST(test_kin_dims_3d);
    RUN_TEST(test_overview_all_layers_full);
    RUN_TEST(test_preset_invalid_returns_overview);
    /* Transition */
    RUN_TEST(test_is_transitioning_after_switch);
    RUN_TEST(test_transition_completes);
    RUN_TEST(test_transition_progress);
    RUN_TEST(test_no_transition_in_overview);
    /* Names */
    RUN_TEST(test_mode_name);
    RUN_TEST(test_mode_name_invalid);
    /* Purity */
    RUN_TEST(test_purity);
    return UNITY_END();
}
