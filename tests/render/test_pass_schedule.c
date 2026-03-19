#include "../unity/unity.h"
#include "../../src/render/pass_schedule.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Space View: stars enabled --- */

static void test_ps_space_view_has_stars(void) {
    pass_schedule_t s = ps_space_view(0); /* LOD_ULTRA */
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_STARS].enabled);
}

/* --- Space View: planets enabled --- */

static void test_ps_space_view_has_planets(void) {
    pass_schedule_t s = ps_space_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_PLANET].enabled);
}

/* --- Space View: post always enabled --- */

static void test_ps_space_view_has_post(void) {
    pass_schedule_t s = ps_space_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_POST].enabled);
}

/* --- Space View: earth pass enabled (globe visible from space) --- */

static void test_ps_space_view_has_earth(void) {
    pass_schedule_t s = ps_space_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_EARTH].enabled);
}

/* --- Earth View: zodiac ring disabled (can't see from ground) --- */

static void test_ps_earth_view_no_zodiac(void) {
    pass_schedule_t s = ps_earth_view(0);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_ZODIAC].enabled);
}

/* --- Earth View: stars visible from ground --- */

static void test_ps_earth_view_has_stars(void) {
    pass_schedule_t s = ps_earth_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_STARS].enabled);
}

/* --- Earth View: milkyway visible from ground --- */

static void test_ps_earth_view_has_milkyway(void) {
    pass_schedule_t s = ps_earth_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_MILKYWAY].enabled);
}

/* --- Earth View: text/labels always on --- */

static void test_ps_earth_view_has_text(void) {
    pass_schedule_t s = ps_earth_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_TEXT].enabled);
}

/* --- Galaxy View: planets too small --- */

static void test_ps_galaxy_view_no_planets(void) {
    pass_schedule_t s = ps_galaxy_view(0);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_PLANET].enabled);
}

/* --- Galaxy View: milkyway is the main feature --- */

static void test_ps_galaxy_view_has_milkyway(void) {
    pass_schedule_t s = ps_galaxy_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_MILKYWAY].enabled);
}

/* --- LOD_MINIMAL fewer passes than LOD_ULTRA --- */

static void test_ps_lod_minimal_fewer_passes(void) {
    pass_schedule_t ultra = ps_space_view(0);   /* LOD_ULTRA */
    pass_schedule_t minimal = ps_space_view(4); /* LOD_MINIMAL */
    TEST_ASSERT_TRUE(minimal.active_count < ultra.active_count);
}

/* --- LOD_MINIMAL disables diffraction --- */

static void test_ps_lod_minimal_no_diffraction(void) {
    pass_schedule_t s = ps_space_view(4); /* LOD_MINIMAL */
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_DIFFRACTION].enabled);
}

/* --- ps_for_view(VIEW_SPACE) matches ps_space_view() --- */

static void test_ps_for_view_space(void) {
    pass_schedule_t a = ps_space_view(0);
    pass_schedule_t b = ps_for_view(0, 0); /* VIEW_SPACE=0, LOD_ULTRA=0 */
    TEST_ASSERT_EQUAL_INT(a.active_count, b.active_count);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.passes[i].enabled, b.passes[i].enabled);
        TEST_ASSERT_EQUAL_INT(a.passes[i].draw_order, b.passes[i].draw_order);
    }
}

/* --- ps_for_view(VIEW_EARTH) matches ps_earth_view() --- */

static void test_ps_for_view_earth(void) {
    pass_schedule_t a = ps_earth_view(0);
    pass_schedule_t b = ps_for_view(1, 0); /* VIEW_EARTH=1, LOD_ULTRA=0 */
    TEST_ASSERT_EQUAL_INT(a.active_count, b.active_count);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.passes[i].enabled, b.passes[i].enabled);
        TEST_ASSERT_EQUAL_INT(a.passes[i].draw_order, b.passes[i].draw_order);
    }
}

/* --- Focus astrology: zodiac opacity 1.0, cards 1.0 --- */

static void test_ps_apply_focus_astrology(void) {
    pass_schedule_t s = ps_space_view(0);
    pass_schedule_t f = ps_apply_focus(s, 1); /* FOCUS_MODE_ASTROLOGY=1 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, f.passes[PASS_ZODIAC].opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, f.passes[PASS_CARD].opacity_scale);
}

/* --- Focus overview: no opacity changes --- */

static void test_ps_apply_focus_overview(void) {
    pass_schedule_t s = ps_space_view(0);
    pass_schedule_t f = ps_apply_focus(s, 0); /* FOCUS_MODE_OVERVIEW=0 */
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, s.passes[i].opacity_scale,
                                 f.passes[i].opacity_scale);
    }
}

/* --- All pass names non-NULL --- */

static void test_ps_pass_name_valid(void) {
    for (int i = 0; i < PASS_COUNT; i++) {
        const char *name = ps_pass_name((pass_id_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

/* --- Post-processing has highest draw_order (drawn last) --- */

static void test_ps_draw_order_post_last(void) {
    pass_schedule_t s = ps_space_view(0);
    int post_order = s.passes[PASS_POST].draw_order;
    for (int i = 0; i < PASS_COUNT; i++) {
        if (i != PASS_POST) {
            TEST_ASSERT_TRUE(s.passes[i].draw_order <= post_order);
        }
    }
}

/* --- active_count matches actual enabled count --- */

static void test_ps_active_count_matches(void) {
    pass_schedule_t s = ps_space_view(0);
    int counted = 0;
    for (int i = 0; i < PASS_COUNT; i++) {
        if (s.passes[i].enabled) {
            counted++;
        }
    }
    TEST_ASSERT_EQUAL_INT(counted, s.active_count);
}

/* --- ps_is_enabled helper --- */

static void test_ps_is_enabled_works(void) {
    pass_schedule_t s = ps_space_view(0);
    TEST_ASSERT_EQUAL_INT(1, ps_is_enabled(&s, PASS_STARS));
}

/* --- ps_active_count helper --- */

static void test_ps_active_count_helper(void) {
    pass_schedule_t s = ps_space_view(0);
    TEST_ASSERT_EQUAL_INT(s.active_count, ps_active_count(&s));
}

/* --- Earth View disables orbit trail --- */

static void test_ps_earth_view_no_orbit_trail(void) {
    pass_schedule_t s = ps_earth_view(0);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_ORBIT_TRAIL].enabled);
}

/* --- Earth View disables earth globe (ground-level, not from space) --- */

static void test_ps_earth_view_no_earth_globe(void) {
    pass_schedule_t s = ps_earth_view(0);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_EARTH].enabled);
}

/* --- Galaxy View has text --- */

static void test_ps_galaxy_view_has_text(void) {
    pass_schedule_t s = ps_galaxy_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_TEXT].enabled);
}

/* --- Galaxy View has post --- */

static void test_ps_galaxy_view_has_post(void) {
    pass_schedule_t s = ps_galaxy_view(0);
    TEST_ASSERT_EQUAL_INT(1, s.passes[PASS_POST].enabled);
}

/* --- Focus KIN: cards opacity 1.0, 3D dimmed --- */

static void test_ps_apply_focus_kin(void) {
    pass_schedule_t s = ps_space_view(0);
    pass_schedule_t f = ps_apply_focus(s, 2); /* FOCUS_MODE_KIN=2 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, f.passes[PASS_CARD].opacity_scale);
    TEST_ASSERT_TRUE(f.passes[PASS_PLANET].opacity_scale < 0.5f);
}

/* --- Focus ICHING: hexagram opacity 1.0 --- */

static void test_ps_apply_focus_iching(void) {
    pass_schedule_t s = ps_space_view(0);
    pass_schedule_t f = ps_apply_focus(s, 3); /* FOCUS_MODE_ICHING=3 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, f.passes[PASS_HEXAGRAM].opacity_scale);
}

/* --- Focus HD: bodygraph opacity 1.0 --- */

static void test_ps_apply_focus_hd(void) {
    pass_schedule_t s = ps_space_view(0);
    pass_schedule_t f = ps_apply_focus(s, 5); /* FOCUS_MODE_HD=5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, f.passes[PASS_BODYGRAPH].opacity_scale);
}

/* --- LOD_MEDIUM disables orbit_trail and deep_sky --- */

static void test_ps_lod_medium_disables(void) {
    pass_schedule_t s = ps_space_view(2); /* LOD_MEDIUM */
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_ORBIT_TRAIL].enabled);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_DEEP_SKY].enabled);
}

/* --- LOD_LOW disables diffraction, constellation, bodygraph, hexagram, tree --- */

static void test_ps_lod_low_disables(void) {
    pass_schedule_t s = ps_space_view(3); /* LOD_LOW */
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_DIFFRACTION].enabled);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_CONSTELLATION].enabled);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_BODYGRAPH].enabled);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_HEXAGRAM].enabled);
    TEST_ASSERT_EQUAL_INT(0, s.passes[PASS_TREE].enabled);
}

/* --- ps_pass_name out of range returns "Unknown" --- */

static void test_ps_pass_name_out_of_range(void) {
    const char *name = ps_pass_name((pass_id_t)99);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

/* --- ps_for_view with unknown view falls back to space --- */

static void test_ps_for_view_unknown(void) {
    pass_schedule_t a = ps_space_view(0);
    pass_schedule_t b = ps_for_view(99, 0); /* unknown view -> space fallback */
    TEST_ASSERT_EQUAL_INT(a.active_count, b.active_count);
}

/* --- ps_for_view galaxy matches ps_galaxy_view --- */

static void test_ps_for_view_galaxy(void) {
    pass_schedule_t a = ps_galaxy_view(0);
    pass_schedule_t b = ps_for_view(2, 0); /* VIEW_GALAXY=2 */
    TEST_ASSERT_EQUAL_INT(a.active_count, b.active_count);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.passes[i].enabled, b.passes[i].enabled);
    }
}

/* --- Draw order: milkyway before stars before planets --- */

static void test_ps_draw_order_sequence(void) {
    pass_schedule_t s = ps_space_view(0);
    TEST_ASSERT_TRUE(s.passes[PASS_MILKYWAY].draw_order <
                     s.passes[PASS_STARS].draw_order);
    TEST_ASSERT_TRUE(s.passes[PASS_STARS].draw_order <
                     s.passes[PASS_PLANET].draw_order);
}

/* --- All passes have non-negative draw_order --- */

static void test_ps_draw_order_non_negative(void) {
    pass_schedule_t s = ps_space_view(0);
    for (int i = 0; i < PASS_COUNT; i++) {
        TEST_ASSERT_TRUE(s.passes[i].draw_order >= 0);
    }
}

/* --- Default opacity_scale is 1.0 for enabled passes --- */

static void test_ps_default_opacity_one(void) {
    pass_schedule_t s = ps_space_view(0);
    for (int i = 0; i < PASS_COUNT; i++) {
        if (s.passes[i].enabled) {
            TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.passes[i].opacity_scale);
        }
    }
}

/* --- ps_is_enabled with disabled pass returns 0 --- */

static void test_ps_is_enabled_disabled_pass(void) {
    pass_schedule_t s = ps_earth_view(0);
    /* zodiac is disabled in earth view */
    TEST_ASSERT_EQUAL_INT(0, ps_is_enabled(&s, PASS_ZODIAC));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ps_space_view_has_stars);
    RUN_TEST(test_ps_space_view_has_planets);
    RUN_TEST(test_ps_space_view_has_post);
    RUN_TEST(test_ps_space_view_has_earth);
    RUN_TEST(test_ps_earth_view_no_zodiac);
    RUN_TEST(test_ps_earth_view_has_stars);
    RUN_TEST(test_ps_earth_view_has_milkyway);
    RUN_TEST(test_ps_earth_view_has_text);
    RUN_TEST(test_ps_galaxy_view_no_planets);
    RUN_TEST(test_ps_galaxy_view_has_milkyway);
    RUN_TEST(test_ps_lod_minimal_fewer_passes);
    RUN_TEST(test_ps_lod_minimal_no_diffraction);
    RUN_TEST(test_ps_for_view_space);
    RUN_TEST(test_ps_for_view_earth);
    RUN_TEST(test_ps_apply_focus_astrology);
    RUN_TEST(test_ps_apply_focus_overview);
    RUN_TEST(test_ps_pass_name_valid);
    RUN_TEST(test_ps_draw_order_post_last);
    RUN_TEST(test_ps_active_count_matches);
    RUN_TEST(test_ps_is_enabled_works);
    RUN_TEST(test_ps_active_count_helper);
    RUN_TEST(test_ps_earth_view_no_orbit_trail);
    RUN_TEST(test_ps_earth_view_no_earth_globe);
    RUN_TEST(test_ps_galaxy_view_has_text);
    RUN_TEST(test_ps_galaxy_view_has_post);
    RUN_TEST(test_ps_apply_focus_kin);
    RUN_TEST(test_ps_apply_focus_iching);
    RUN_TEST(test_ps_apply_focus_hd);
    RUN_TEST(test_ps_lod_medium_disables);
    RUN_TEST(test_ps_lod_low_disables);
    RUN_TEST(test_ps_pass_name_out_of_range);
    RUN_TEST(test_ps_for_view_unknown);
    RUN_TEST(test_ps_for_view_galaxy);
    RUN_TEST(test_ps_draw_order_sequence);
    RUN_TEST(test_ps_draw_order_non_negative);
    RUN_TEST(test_ps_default_opacity_one);
    RUN_TEST(test_ps_is_enabled_disabled_pass);
    return UNITY_END();
}
