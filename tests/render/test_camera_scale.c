#include "../unity/unity.h"
#include "../../src/render/camera_scale.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- scale_get --- */

static void test_scale_get_solar(void) {
    scale_level_t s = scale_get(SCALE_SOLAR);
    TEST_ASSERT_EQUAL_INT(SCALE_SOLAR, s.id);
    TEST_ASSERT_NOT_NULL(s.name);
    TEST_ASSERT_TRUE(s.log_zoom_min < s.log_zoom_max);
}

static void test_scale_get_personal(void) {
    scale_level_t s = scale_get(SCALE_PERSONAL);
    TEST_ASSERT_EQUAL_INT(SCALE_PERSONAL, s.id);
}

static void test_scale_get_universe(void) {
    scale_level_t s = scale_get(SCALE_UNIVERSE);
    TEST_ASSERT_EQUAL_INT(SCALE_UNIVERSE, s.id);
}

static void test_scale_get_invalid(void) {
    scale_level_t s = scale_get(99);
    TEST_ASSERT_EQUAL_INT(SCALE_SOLAR, s.id); /* default */
}

/* --- log_to_distance / distance_to_log --- */

static void test_log_to_distance_zero(void) {
    float d = scale_log_to_distance(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, d);
}

static void test_log_to_distance_positive(void) {
    float d = scale_log_to_distance(3.401f); /* ~30 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 30.0f, d);
}

static void test_distance_to_log_roundtrip(void) {
    float orig = 25.0f;
    float log_z = scale_distance_to_log(orig);
    float back = scale_log_to_distance(log_z);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, orig, back);
}

static void test_distance_to_log_zero(void) {
    float log_z = scale_distance_to_log(0.0f);
    TEST_ASSERT_EQUAL_FLOAT(scale_log_zoom_min(), log_z);
}

/* --- scale_from_log_zoom --- */

static void test_from_log_zoom_solar(void) {
    scale_id_t id = scale_from_log_zoom(3.0f);
    TEST_ASSERT_EQUAL_INT(SCALE_SOLAR, id);
}

static void test_from_log_zoom_personal(void) {
    scale_id_t id = scale_from_log_zoom(-3.0f);
    TEST_ASSERT_EQUAL_INT(SCALE_PERSONAL, id);
}

static void test_from_log_zoom_universe(void) {
    scale_id_t id = scale_from_log_zoom(10.0f);
    TEST_ASSERT_EQUAL_INT(SCALE_UNIVERSE, id);
}

static void test_from_log_zoom_very_small(void) {
    scale_id_t id = scale_from_log_zoom(-100.0f);
    TEST_ASSERT_EQUAL_INT(SCALE_PERSONAL, id);
}

/* --- scale_name --- */

static void test_name_solar(void) {
    const char *n = scale_name(SCALE_SOLAR);
    TEST_ASSERT_EQUAL_STRING("Solar System", n);
}

static void test_name_personal(void) {
    const char *n = scale_name(SCALE_PERSONAL);
    TEST_ASSERT_EQUAL_STRING("Personal Chart", n);
}

static void test_name_invalid(void) {
    const char *n = scale_name(99);
    TEST_ASSERT_EQUAL_STRING("Unknown", n);
}

/* --- center log_zoom --- */

static void test_center_solar(void) {
    float c = scale_center_log_zoom(SCALE_SOLAR);
    scale_level_t s = scale_get(SCALE_SOLAR);
    TEST_ASSERT_TRUE(c >= s.log_zoom_min);
    TEST_ASSERT_TRUE(c <= s.log_zoom_max);
}

/* --- min/max --- */

static void test_min_less_than_max(void) {
    TEST_ASSERT_TRUE(scale_log_zoom_min() < scale_log_zoom_max());
}

static void test_min_is_personal(void) {
    float min = scale_log_zoom_min();
    scale_level_t p = scale_get(SCALE_PERSONAL);
    TEST_ASSERT_EQUAL_FLOAT(p.log_zoom_min, min);
}

static void test_max_is_universe(void) {
    float max = scale_log_zoom_max();
    scale_level_t u = scale_get(SCALE_UNIVERSE);
    TEST_ASSERT_EQUAL_FLOAT(u.log_zoom_max, max);
}

/* --- levels are contiguous --- */

static void test_levels_contiguous(void) {
    for (int i = 0; i < SCALE_COUNT - 1; i++) {
        scale_level_t a = scale_get(i);
        scale_level_t b = scale_get(i + 1);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, a.log_zoom_max, b.log_zoom_min);
    }
}

/* --- scale transitions --- */

static void test_transition_create(void) {
    scale_transition_t t = scale_transition_create(3.4f, SCALE_PERSONAL, 1.2f);
    TEST_ASSERT_EQUAL_FLOAT(3.4f, t.from_log_zoom);
    TEST_ASSERT_EQUAL_FLOAT(scale_center_log_zoom(SCALE_PERSONAL), t.to_log_zoom);
    TEST_ASSERT_EQUAL_FLOAT(1.2f, t.duration);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, t.elapsed);
    TEST_ASSERT_EQUAL_INT(1, t.active);
}

static void test_transition_starts_at_from(void) {
    scale_transition_t t = scale_transition_create(3.4f, SCALE_UNIVERSE, 1.0f);
    float v = scale_transition_value(t);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.4f, v);
}

static void test_transition_ends_at_to(void) {
    scale_transition_t t = scale_transition_create(3.4f, SCALE_PERSONAL, 1.0f);
    t = scale_transition_tick(t, 1.0f);
    float v = scale_transition_value(t);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, scale_center_log_zoom(SCALE_PERSONAL), v);
}

static void test_transition_midpoint(void) {
    float from = 0.0f;
    float to = scale_center_log_zoom(SCALE_UNIVERSE);
    scale_transition_t t = scale_transition_create(from, SCALE_UNIVERSE, 2.0f);
    t = scale_transition_tick(t, 1.0f); /* halfway */
    float v = scale_transition_value(t);
    /* ease_in_out_cubic(0.5) = 0.5, so midpoint should be halfway */
    float expected = from + 0.5f * (to - from);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected, v);
}

static void test_transition_tick_accumulates(void) {
    scale_transition_t t = scale_transition_create(0.0f, SCALE_SOLAR, 2.0f);
    t = scale_transition_tick(t, 0.5f);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, t.elapsed);
    TEST_ASSERT_EQUAL_INT(1, t.active);
    t = scale_transition_tick(t, 0.5f);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, t.elapsed);
    TEST_ASSERT_EQUAL_INT(1, t.active);
}

static void test_transition_done_when_complete(void) {
    scale_transition_t t = scale_transition_create(0.0f, SCALE_SOLAR, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, scale_transition_done(t));
    t = scale_transition_tick(t, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, scale_transition_done(t));
}

static void test_transition_overshoot_clamped(void) {
    scale_transition_t t = scale_transition_create(0.0f, SCALE_SOLAR, 1.0f);
    t = scale_transition_tick(t, 5.0f); /* way past duration */
    TEST_ASSERT_EQUAL_INT(1, scale_transition_done(t));
    float v = scale_transition_value(t);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, scale_center_log_zoom(SCALE_SOLAR), v);
}

static void test_transition_inactive_no_change(void) {
    scale_transition_t t = scale_transition_create(0.0f, SCALE_SOLAR, 1.0f);
    t = scale_transition_tick(t, 1.0f); /* completes */
    float v1 = scale_transition_value(t);
    t = scale_transition_tick(t, 1.0f); /* extra tick — should be no-op */
    float v2 = scale_transition_value(t);
    TEST_ASSERT_EQUAL_FLOAT(v1, v2);
}

static void test_transition_easing_not_linear(void) {
    /* At 25% time, ease_in_out_cubic should be < 0.25 (slow start) */
    scale_transition_t t = scale_transition_create(0.0f, SCALE_UNIVERSE, 4.0f);
    t = scale_transition_tick(t, 1.0f); /* 25% through */
    float v = scale_transition_value(t);
    float linear_quarter = 0.25f * scale_center_log_zoom(SCALE_UNIVERSE);
    TEST_ASSERT_TRUE(v < linear_quarter);
}

static void test_transition_zero_duration(void) {
    /* Near-zero duration should still work (clamped to 0.001) */
    scale_transition_t t = scale_transition_create(0.0f, SCALE_SOLAR, 0.0f);
    TEST_ASSERT_TRUE(t.duration > 0.0f);
    t = scale_transition_tick(t, 0.01f);
    TEST_ASSERT_EQUAL_INT(1, scale_transition_done(t));
}

/* --- purity --- */

static void test_purity(void) {
    scale_level_t a1 = scale_get(SCALE_SOLAR);
    scale_level_t a2 = scale_get(SCALE_SOLAR);
    TEST_ASSERT_EQUAL_FLOAT(a1.log_zoom_min, a2.log_zoom_min);
    TEST_ASSERT_EQUAL_FLOAT(a1.log_zoom_max, a2.log_zoom_max);

    float d1 = scale_log_to_distance(3.0f);
    float d2 = scale_log_to_distance(3.0f);
    TEST_ASSERT_EQUAL_FLOAT(d1, d2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_scale_get_solar);
    RUN_TEST(test_scale_get_personal);
    RUN_TEST(test_scale_get_universe);
    RUN_TEST(test_scale_get_invalid);
    RUN_TEST(test_log_to_distance_zero);
    RUN_TEST(test_log_to_distance_positive);
    RUN_TEST(test_distance_to_log_roundtrip);
    RUN_TEST(test_distance_to_log_zero);
    RUN_TEST(test_from_log_zoom_solar);
    RUN_TEST(test_from_log_zoom_personal);
    RUN_TEST(test_from_log_zoom_universe);
    RUN_TEST(test_from_log_zoom_very_small);
    RUN_TEST(test_name_solar);
    RUN_TEST(test_name_personal);
    RUN_TEST(test_name_invalid);
    RUN_TEST(test_center_solar);
    RUN_TEST(test_min_less_than_max);
    RUN_TEST(test_min_is_personal);
    RUN_TEST(test_max_is_universe);
    RUN_TEST(test_levels_contiguous);
    RUN_TEST(test_transition_create);
    RUN_TEST(test_transition_starts_at_from);
    RUN_TEST(test_transition_ends_at_to);
    RUN_TEST(test_transition_midpoint);
    RUN_TEST(test_transition_tick_accumulates);
    RUN_TEST(test_transition_done_when_complete);
    RUN_TEST(test_transition_overshoot_clamped);
    RUN_TEST(test_transition_inactive_no_change);
    RUN_TEST(test_transition_easing_not_linear);
    RUN_TEST(test_transition_zero_duration);
    RUN_TEST(test_purity);
    return UNITY_END();
}
