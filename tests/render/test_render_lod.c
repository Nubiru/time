#include "../unity/unity.h"
#include "../../src/render/render_lod.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- lod_init --- */

static void test_lod_init(void) {
    lod_state_t s = lod_init(60.0f);
    TEST_ASSERT_EQUAL_INT(LOD_ULTRA, s.current_tier);
    TEST_ASSERT_EQUAL_INT(0, s.sample_count);
    TEST_ASSERT_EQUAL_INT(0, s.write_index);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 16.67f, s.target_frame_ms);
}

/* --- lod_record_frame fast --- */

static void test_lod_record_frame_fast(void) {
    lod_state_t s = lod_init(60.0f);
    for (int i = 0; i < LOD_FRAME_HISTORY; i++) {
        s = lod_record_frame(s, 8.0f); /* 8ms = 125fps */
    }
    TEST_ASSERT_EQUAL_INT(LOD_ULTRA, s.current_tier);
    TEST_ASSERT_EQUAL_INT(LOD_FRAME_HISTORY, s.sample_count);
}

/* --- lod_record_frame slow --- */

static void test_lod_record_frame_slow(void) {
    lod_state_t s = lod_init(60.0f);
    for (int i = 0; i < LOD_FRAME_HISTORY; i++) {
        s = lod_record_frame(s, 30.0f); /* 30ms = 33fps */
    }
    /* With 30ms frames vs 16.67ms target, should drop quality */
    TEST_ASSERT_TRUE(s.current_tier >= LOD_HIGH);
}

/* --- lod_avg_frame_time empty --- */

static void test_lod_avg_frame_time_empty(void) {
    lod_state_t s = lod_init(60.0f);
    float avg = lod_avg_frame_time(&s);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, avg);
}

/* --- lod_avg_frame_time basic --- */

static void test_lod_avg_frame_time_basic(void) {
    lod_state_t s = lod_init(60.0f);
    s = lod_record_frame(s, 10.0f);
    s = lod_record_frame(s, 20.0f);
    s = lod_record_frame(s, 30.0f);
    float avg = lod_avg_frame_time(&s);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 20.0f, avg);
}

/* --- lod_params_ultra --- */

static void test_lod_params_ultra(void) {
    lod_params_t p = lod_params_for_tier(LOD_ULTRA);
    TEST_ASSERT_EQUAL_INT(5000, p.star_count_max);
    TEST_ASSERT_EQUAL_INT(8, p.ring_segments);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.particle_density);
    TEST_ASSERT_EQUAL_INT(1, p.enable_post_process);
}

/* --- lod_params_minimal --- */

static void test_lod_params_minimal(void) {
    lod_params_t p = lod_params_for_tier(LOD_MINIMAL);
    TEST_ASSERT_EQUAL_INT(500, p.star_count_max);
    TEST_ASSERT_EQUAL_INT(2, p.ring_segments);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, p.particle_density);
    TEST_ASSERT_EQUAL_INT(0, p.enable_post_process);
}

/* --- lod_params_monotonic --- */

static void test_lod_params_monotonic(void) {
    for (int tier = 0; tier < LOD_TIER_COUNT - 1; tier++) {
        lod_params_t lo = lod_params_for_tier((lod_tier_t)tier);
        lod_params_t hi = lod_params_for_tier((lod_tier_t)(tier + 1));
        TEST_ASSERT_TRUE(lo.star_count_max >= hi.star_count_max);
        TEST_ASSERT_TRUE(lo.ring_segments >= hi.ring_segments);
        TEST_ASSERT_TRUE(lo.particle_density >= hi.particle_density);
        TEST_ASSERT_TRUE(lo.text_max_glyphs >= hi.text_max_glyphs);
    }
}

/* --- lod_from_distance close --- */

static void test_lod_from_distance_close(void) {
    lod_tier_t t = lod_from_distance(0.5f, 1.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(LOD_ULTRA, t);
}

/* --- lod_from_distance far --- */

static void test_lod_from_distance_far(void) {
    lod_tier_t t = lod_from_distance(200.0f, 1.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(LOD_MINIMAL, t);
}

/* --- lod_from_distance mid --- */

static void test_lod_from_distance_mid(void) {
    lod_tier_t t = lod_from_distance(50.0f, 1.0f, 100.0f);
    TEST_ASSERT_TRUE(t >= LOD_ULTRA);
    TEST_ASSERT_TRUE(t <= LOD_MINIMAL);
    /* Midpoint should give an intermediate tier */
    TEST_ASSERT_TRUE(t >= LOD_HIGH);
    TEST_ASSERT_TRUE(t <= LOD_LOW);
}

/* --- lod_combine takes worse --- */

static void test_lod_combine_takes_worse(void) {
    TEST_ASSERT_EQUAL_INT(LOD_MEDIUM, lod_combine(LOD_ULTRA, LOD_MEDIUM));
    TEST_ASSERT_EQUAL_INT(LOD_MINIMAL, lod_combine(LOD_MINIMAL, LOD_HIGH));
    TEST_ASSERT_EQUAL_INT(LOD_LOW, lod_combine(LOD_LOW, LOD_LOW));
    TEST_ASSERT_EQUAL_INT(LOD_HIGH, lod_combine(LOD_ULTRA, LOD_HIGH));
}

/* --- lod_tier_name valid --- */

static void test_lod_tier_name_valid(void) {
    for (int i = 0; i < LOD_TIER_COUNT; i++) {
        const char *name = lod_tier_name((lod_tier_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

/* --- lod_pass_budget sums to target --- */

static void test_lod_pass_budget_sums_to_target(void) {
    int total_passes = 10;
    float sum = 0.0f;
    for (int i = 0; i < total_passes; i++) {
        sum += lod_pass_budget_ms(LOD_ULTRA, total_passes);
    }
    /* Sum should be close to target frame time (16.67ms) */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 16.67f, sum);
}

/* --- lod_record recovers --- */

static void test_lod_record_recovers(void) {
    lod_state_t s = lod_init(60.0f);

    /* Drive quality down with slow frames */
    for (int i = 0; i < LOD_FRAME_HISTORY; i++) {
        s = lod_record_frame(s, 30.0f);
    }
    lod_tier_t degraded = s.current_tier;
    TEST_ASSERT_TRUE(degraded > LOD_ULTRA);

    /* Now send fast frames to recover */
    for (int i = 0; i < LOD_FRAME_HISTORY * 2; i++) {
        s = lod_record_frame(s, 5.0f);
    }
    TEST_ASSERT_TRUE(s.current_tier < degraded);
}

/* --- lod_current_params matches tier --- */

static void test_lod_current_params(void) {
    lod_state_t s = lod_init(60.0f);
    lod_params_t p = lod_current_params(&s);
    lod_params_t ultra = lod_params_for_tier(LOD_ULTRA);
    TEST_ASSERT_EQUAL_INT(ultra.star_count_max, p.star_count_max);
    TEST_ASSERT_EQUAL_INT(ultra.ring_segments, p.ring_segments);
}

/* --- lod_current_tier --- */

static void test_lod_current_tier(void) {
    lod_state_t s = lod_init(60.0f);
    TEST_ASSERT_EQUAL_INT(LOD_ULTRA, lod_current_tier(&s));
}

/* --- ring buffer wraps correctly --- */

static void test_lod_ring_buffer_wrap(void) {
    lod_state_t s = lod_init(60.0f);
    /* Fill buffer well past capacity */
    for (int i = 0; i < LOD_FRAME_HISTORY * 3; i++) {
        s = lod_record_frame(s, 10.0f);
    }
    /* sample_count caps at LOD_FRAME_HISTORY */
    TEST_ASSERT_EQUAL_INT(LOD_FRAME_HISTORY, s.sample_count);
    float avg = lod_avg_frame_time(&s);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, avg);
}

/* --- lod_from_distance edge cases --- */

static void test_lod_from_distance_at_boundaries(void) {
    /* Exactly at close_distance */
    lod_tier_t t1 = lod_from_distance(1.0f, 1.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(LOD_ULTRA, t1);

    /* Exactly at far_distance */
    lod_tier_t t2 = lod_from_distance(100.0f, 1.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(LOD_MINIMAL, t2);
}

/* --- lod_pass_budget lower tier gives more time --- */

static void test_lod_pass_budget_scales_with_tier(void) {
    /* Lower quality tiers should have a more relaxed (larger) budget per pass */
    float budget_ultra = lod_pass_budget_ms(LOD_ULTRA, 10);
    float budget_low = lod_pass_budget_ms(LOD_LOW, 10);
    TEST_ASSERT_TRUE(budget_low >= budget_ultra);
}

/* --- lod_tier_name returns distinct strings --- */

static void test_lod_tier_name_distinct(void) {
    for (int i = 0; i < LOD_TIER_COUNT; i++) {
        for (int j = i + 1; j < LOD_TIER_COUNT; j++) {
            const char *a = lod_tier_name((lod_tier_t)i);
            const char *b = lod_tier_name((lod_tier_t)j);
            TEST_ASSERT_FALSE(a == b); /* different pointers */
        }
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_lod_init);
    RUN_TEST(test_lod_record_frame_fast);
    RUN_TEST(test_lod_record_frame_slow);
    RUN_TEST(test_lod_avg_frame_time_empty);
    RUN_TEST(test_lod_avg_frame_time_basic);
    RUN_TEST(test_lod_params_ultra);
    RUN_TEST(test_lod_params_minimal);
    RUN_TEST(test_lod_params_monotonic);
    RUN_TEST(test_lod_from_distance_close);
    RUN_TEST(test_lod_from_distance_far);
    RUN_TEST(test_lod_from_distance_mid);
    RUN_TEST(test_lod_combine_takes_worse);
    RUN_TEST(test_lod_tier_name_valid);
    RUN_TEST(test_lod_pass_budget_sums_to_target);
    RUN_TEST(test_lod_record_recovers);
    RUN_TEST(test_lod_current_params);
    RUN_TEST(test_lod_current_tier);
    RUN_TEST(test_lod_ring_buffer_wrap);
    RUN_TEST(test_lod_from_distance_at_boundaries);
    RUN_TEST(test_lod_pass_budget_scales_with_tier);
    RUN_TEST(test_lod_tier_name_distinct);
    return UNITY_END();
}
