#include "../unity/unity.h"
#include "../../src/ui/card_flight.h"

void setUp(void) {}
void tearDown(void) {}

/* --- cf_create tests --- */

void test_cf_create_at_depth(void)
{
    card_flight_t cf = cf_create(3.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.5f, cf_depth(cf));
    TEST_ASSERT_EQUAL_INT(0, cf_active(cf));
    TEST_ASSERT_EQUAL_INT(3, cf_current_layer(cf));
}

/* --- cf_fly_deeper tests --- */

void test_cf_fly_deeper_from_cosmic(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    TEST_ASSERT_EQUAL_INT(1, cf_active(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_AGE, cf_target(cf));
    TEST_ASSERT_EQUAL_INT(1, cf_target_layer(cf));
}

void test_cf_fly_deeper_from_today_no_change(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_TODAY);
    cf = cf_fly_deeper(cf);
    TEST_ASSERT_EQUAL_INT(0, cf_active(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_TODAY, cf_target(cf));
}

/* --- cf_fly_shallower tests --- */

void test_cf_fly_shallower_from_today(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_TODAY);
    cf = cf_fly_shallower(cf);
    TEST_ASSERT_EQUAL_INT(1, cf_active(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_CYCLE, cf_target(cf));
    TEST_ASSERT_EQUAL_INT(4, cf_target_layer(cf));
}

void test_cf_fly_shallower_from_cosmic_no_change(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_shallower(cf);
    TEST_ASSERT_EQUAL_INT(0, cf_active(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_COSMIC, cf_target(cf));
}

/* --- cf_fly_to_layer tests --- */

void test_cf_fly_to_layer_valid(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_to_layer(cf, 3);
    TEST_ASSERT_EQUAL_INT(1, cf_active(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_YEAR, cf_target(cf));
    TEST_ASSERT_EQUAL_INT(3, cf_target_layer(cf));
}

void test_cf_fly_to_layer_clamped(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);

    /* Above range clamps to 5 */
    cf = cf_fly_to_layer(cf, 99);
    TEST_ASSERT_EQUAL_INT(5, cf_target_layer(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_TODAY, cf_target(cf));

    /* Below range clamps to 0 */
    card_flight_t cf2 = cf_create(CF_DEPTH_TODAY);
    cf2 = cf_fly_to_layer(cf2, -5);
    TEST_ASSERT_EQUAL_INT(0, cf2.target_layer);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_COSMIC, cf_target(cf2));
}

/* --- cf_fly_to_depth tests --- */

void test_cf_fly_to_depth_snaps_to_layer(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_to_depth(cf, 3.7f);
    /* 3.7 is nearest to layer 3 (depth 3.5) */
    TEST_ASSERT_EQUAL_INT(3, cf_target_layer(cf));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_YEAR, cf_target(cf));
}

/* --- cf_tick / animation tests --- */

void test_cf_tick_animates_depth(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    float initial_depth = cf_depth(cf);
    /* Tick a small step */
    cf = cf_tick(cf, 0.016f);
    /* Depth should have moved toward target */
    TEST_ASSERT_TRUE(cf_depth(cf) > initial_depth);
}

void test_cf_tick_crossfade_ramps(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cf_crossfade(cf));
    cf = cf_tick(cf, 0.1f);
    TEST_ASSERT_TRUE(cf_crossfade(cf) > 0.0f);
}

void test_cf_settles_at_target(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    /* Run many ticks to let spring settle */
    for (int i = 0; i < 300; i++) {
        cf = cf_tick(cf, 0.016f);
    }
    TEST_ASSERT_FLOAT_WITHIN(0.05f, CF_DEPTH_AGE, cf_depth(cf));
}

void test_cf_crossfade_reaches_one(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    /* Run enough ticks for crossfade to complete */
    for (int i = 0; i < 300; i++) {
        cf = cf_tick(cf, 0.016f);
    }
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, cf_crossfade(cf));
}

void test_cf_deactivates_when_settled(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    TEST_ASSERT_EQUAL_INT(1, cf_active(cf));
    /* Run many ticks to settle fully */
    for (int i = 0; i < 600; i++) {
        cf = cf_tick(cf, 0.016f);
    }
    TEST_ASSERT_EQUAL_INT(0, cf_active(cf));
}

/* --- cf_cancel tests --- */

void test_cf_cancel_snaps_to_nearest(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_to_layer(cf, 5);
    /* Tick a few frames to move partway */
    for (int i = 0; i < 20; i++) {
        cf = cf_tick(cf, 0.016f);
    }
    float mid_depth = cf_depth(cf);
    cf = cf_cancel(cf);
    /* Target should now be nearest layer to mid_depth */
    int nearest = cf_nearest_layer(mid_depth);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, cf_layer_depth(nearest), cf_target(cf));
}

/* --- accessor tests --- */

void test_cf_depth_returns_current(void)
{
    card_flight_t cf = cf_create(2.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.5f, cf_depth(cf));
}

void test_cf_target_returns_target(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_to_layer(cf, 4);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, CF_DEPTH_CYCLE, cf_target(cf));
}

void test_cf_active_during_flight(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    cf = cf_fly_deeper(cf);
    TEST_ASSERT_EQUAL_INT(1, cf_active(cf));
}

void test_cf_not_active_idle(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_YEAR);
    TEST_ASSERT_EQUAL_INT(0, cf_active(cf));
}

void test_cf_current_layer_updates(void)
{
    card_flight_t cf = cf_create(CF_DEPTH_COSMIC);
    TEST_ASSERT_EQUAL_INT(0, cf_current_layer(cf));
    cf = cf_fly_deeper(cf);
    /* After many ticks, current layer should be 1 */
    for (int i = 0; i < 300; i++) {
        cf = cf_tick(cf, 0.016f);
    }
    TEST_ASSERT_EQUAL_INT(1, cf_current_layer(cf));
}

/* --- layer utility tests --- */

void test_cf_layer_depth_all_six(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, cf_layer_depth(0));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, cf_layer_depth(1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.5f, cf_layer_depth(2));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.5f, cf_layer_depth(3));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 4.5f, cf_layer_depth(4));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.5f, cf_layer_depth(5));
}

void test_cf_nearest_layer_exact(void)
{
    TEST_ASSERT_EQUAL_INT(0, cf_nearest_layer(0.5f));
    TEST_ASSERT_EQUAL_INT(1, cf_nearest_layer(1.5f));
    TEST_ASSERT_EQUAL_INT(2, cf_nearest_layer(2.5f));
    TEST_ASSERT_EQUAL_INT(3, cf_nearest_layer(3.5f));
    TEST_ASSERT_EQUAL_INT(4, cf_nearest_layer(4.5f));
    TEST_ASSERT_EQUAL_INT(5, cf_nearest_layer(5.5f));
}

void test_cf_nearest_layer_midpoint(void)
{
    /* 2.0 is equidistant between 1.5 and 2.5. Either 1 or 2 is fine,
     * but values clearly closer should be definitive. */
    TEST_ASSERT_EQUAL_INT(0, cf_nearest_layer(0.3f));
    TEST_ASSERT_EQUAL_INT(0, cf_nearest_layer(0.8f));
    TEST_ASSERT_EQUAL_INT(3, cf_nearest_layer(3.6f));
    TEST_ASSERT_EQUAL_INT(5, cf_nearest_layer(5.9f));
    TEST_ASSERT_EQUAL_INT(5, cf_nearest_layer(100.0f));
    TEST_ASSERT_EQUAL_INT(0, cf_nearest_layer(-5.0f));
}

void test_cf_layer_count(void)
{
    TEST_ASSERT_EQUAL_INT(6, cf_layer_count());
}

/* --- edge case: layer_depth out of range returns 0.5 --- */

void test_cf_layer_depth_out_of_range(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, cf_layer_depth(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, cf_layer_depth(6));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, cf_layer_depth(100));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cf_create_at_depth);
    RUN_TEST(test_cf_fly_deeper_from_cosmic);
    RUN_TEST(test_cf_fly_deeper_from_today_no_change);
    RUN_TEST(test_cf_fly_shallower_from_today);
    RUN_TEST(test_cf_fly_shallower_from_cosmic_no_change);
    RUN_TEST(test_cf_fly_to_layer_valid);
    RUN_TEST(test_cf_fly_to_layer_clamped);
    RUN_TEST(test_cf_fly_to_depth_snaps_to_layer);
    RUN_TEST(test_cf_tick_animates_depth);
    RUN_TEST(test_cf_tick_crossfade_ramps);
    RUN_TEST(test_cf_settles_at_target);
    RUN_TEST(test_cf_crossfade_reaches_one);
    RUN_TEST(test_cf_deactivates_when_settled);
    RUN_TEST(test_cf_cancel_snaps_to_nearest);
    RUN_TEST(test_cf_depth_returns_current);
    RUN_TEST(test_cf_target_returns_target);
    RUN_TEST(test_cf_active_during_flight);
    RUN_TEST(test_cf_not_active_idle);
    RUN_TEST(test_cf_current_layer_updates);
    RUN_TEST(test_cf_layer_depth_all_six);
    RUN_TEST(test_cf_nearest_layer_exact);
    RUN_TEST(test_cf_nearest_layer_midpoint);
    RUN_TEST(test_cf_layer_count);
    RUN_TEST(test_cf_layer_depth_out_of_range);
    return UNITY_END();
}
