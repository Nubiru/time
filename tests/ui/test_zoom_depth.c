#include "../unity/unity.h"
#include "../../src/ui/zoom_depth.h"
#include <math.h>

static const float EPS = 0.05f;

void setUp(void) {}
void tearDown(void) {}

/* ---- Creation ---- */

void test_create_initial_tier(void)
{
    /* At solar system scale (default), expect a mid-level tier */
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    /* Should have a valid tier */
    TEST_ASSERT_TRUE(zd.tier >= DEPTH_TIER_GLYPH);
    TEST_ASSERT_TRUE(zd.tier <= DEPTH_TIER_BOARD);
}

void test_create_no_system_ring_today(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, zd.ring);
}

void test_create_with_system(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, TS_SYS_ASTROLOGY);
    /* Ring should match what depth_ring_for_system returns */
    depth_ring_t expected = depth_ring_for_system(TS_SYS_ASTROLOGY);
    TEST_ASSERT_EQUAL_INT(expected, zd.ring);
}

void test_create_not_transitioning(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    TEST_ASSERT_EQUAL_INT(0, zoom_depth_transitioning(zd));
}

/* ---- Update ---- */

void test_update_same_zoom_no_change(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    depth_tier_t initial = zoom_depth_tier(zd);

    zd = zoom_depth_update(zd, log_zoom, 1.0f / 60.0f);
    TEST_ASSERT_EQUAL_INT(initial, zoom_depth_tier(zd));
}

void test_update_zoom_in_increases_tier(void)
{
    /* Start at universe scale (far out, low tier) */
    float log_zoom_far = scale_center_log_zoom(SCALE_UNIVERSE);
    zoom_depth_t zd = zoom_depth_create(log_zoom_far, -1);
    depth_tier_t far_tier = zoom_depth_tier(zd);

    /* Jump to personal scale (close in) and let spring settle */
    float log_zoom_near = scale_center_log_zoom(SCALE_PERSONAL);
    for (int i = 0; i < 600; i++) {
        zd = zoom_depth_update(zd, log_zoom_near, 1.0f / 60.0f);
    }

    depth_tier_t near_tier = zoom_depth_tier(zd);
    /* Closer zoom should give equal or higher tier (more detail) */
    TEST_ASSERT_TRUE(near_tier >= far_tier);
}

void test_update_triggers_transition(void)
{
    float log_zoom_far = scale_center_log_zoom(SCALE_UNIVERSE);
    zoom_depth_t zd = zoom_depth_create(log_zoom_far, -1);

    /* Jump to a very different zoom level */
    float log_zoom_near = scale_center_log_zoom(SCALE_PERSONAL);
    zd = zoom_depth_update(zd, log_zoom_near, 1.0f / 60.0f);

    /* If tiers differ, should be transitioning */
    depth_tier_t target = depth_tier_for_scale((int)SCALE_PERSONAL, -1, -1);
    depth_tier_t source = depth_tier_for_scale((int)SCALE_UNIVERSE, -1, -1);
    if (target != source) {
        TEST_ASSERT_EQUAL_INT(1, zoom_depth_transitioning(zd));
    }
}

void test_update_settles_after_time(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_UNIVERSE);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);

    float log_zoom2 = scale_center_log_zoom(SCALE_PERSONAL);
    for (int i = 0; i < 600; i++) {
        zd = zoom_depth_update(zd, log_zoom2, 1.0f / 60.0f);
    }

    TEST_ASSERT_EQUAL_INT(0, zoom_depth_transitioning(zd));
}

void test_update_zero_dt_ignored(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    float blend_before = zoom_depth_blend(zd);

    zd = zoom_depth_update(zd, scale_center_log_zoom(SCALE_PERSONAL), 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPS, blend_before, zoom_depth_blend(zd));
}

/* ---- Blend ---- */

void test_blend_at_creation_equals_tier(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    float blend = zoom_depth_blend(zd);
    TEST_ASSERT_FLOAT_WITHIN(EPS, (float)zoom_depth_tier(zd), blend);
}

void test_blend_clamped_low(void)
{
    /* Even with extreme values, blend should not go below 0 */
    zoom_depth_t zd = zoom_depth_create(scale_log_zoom_max(), -1);
    float blend = zoom_depth_blend(zd);
    TEST_ASSERT_TRUE(blend >= 0.0f);
}

void test_blend_clamped_high(void)
{
    zoom_depth_t zd = zoom_depth_create(scale_log_zoom_min(), -1);
    float blend = zoom_depth_blend(zd);
    TEST_ASSERT_TRUE(blend <= (float)(DEPTH_TIER_COUNT - 1));
}

/* ---- Focus ---- */

void test_focus_changes_ring(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, zoom_depth_ring(zd));

    zd = zoom_depth_focus(zd, TS_SYS_GEOLOGICAL);
    depth_ring_t expected = depth_ring_for_system(TS_SYS_GEOLOGICAL);
    TEST_ASSERT_EQUAL_INT(expected, zoom_depth_ring(zd));
}

void test_focus_invalid_system_defaults_today(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, TS_SYS_ASTROLOGY);
    zd = zoom_depth_focus(zd, -1);
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, zoom_depth_ring(zd));
}

void test_focus_preserves_blend(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    float blend_before = zoom_depth_blend(zd);

    zd = zoom_depth_focus(zd, TS_SYS_TZOLKIN);
    /* Blend should be same (or transitioning toward new tier) */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, blend_before, zoom_depth_blend(zd));
}

/* ---- Scale tracking ---- */

void test_scale_tracked(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_GALAXY);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);
    TEST_ASSERT_EQUAL_INT(SCALE_GALAXY, zd.scale);
}

void test_scale_updates_on_zoom(void)
{
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    zoom_depth_t zd = zoom_depth_create(log_zoom, -1);

    float new_zoom = scale_center_log_zoom(SCALE_EARTH);
    zd = zoom_depth_update(zd, new_zoom, 1.0f / 60.0f);
    TEST_ASSERT_EQUAL_INT(SCALE_EARTH, zd.scale);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_initial_tier);
    RUN_TEST(test_create_no_system_ring_today);
    RUN_TEST(test_create_with_system);
    RUN_TEST(test_create_not_transitioning);
    RUN_TEST(test_update_same_zoom_no_change);
    RUN_TEST(test_update_zoom_in_increases_tier);
    RUN_TEST(test_update_triggers_transition);
    RUN_TEST(test_update_settles_after_time);
    RUN_TEST(test_update_zero_dt_ignored);
    RUN_TEST(test_blend_at_creation_equals_tier);
    RUN_TEST(test_blend_clamped_low);
    RUN_TEST(test_blend_clamped_high);
    RUN_TEST(test_focus_changes_ring);
    RUN_TEST(test_focus_invalid_system_defaults_today);
    RUN_TEST(test_focus_preserves_blend);
    RUN_TEST(test_scale_tracked);
    RUN_TEST(test_scale_updates_on_zoom);
    return UNITY_END();
}
