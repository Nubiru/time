/* test_phase_space.c — Phase Space Mapper tests
 * TDD RED phase: tests written before implementation.
 * Tests phase position computation, N-dimensional phase points,
 * toroidal/euclidean distance, nearest return search, and
 * pre-defined cycle registry. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/phase_space.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== phase_position ===== */

void test_phase_position_zero_at_epoch(void)
{
    /* At offset, position should be 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_position(100.0, 10.0, 100.0));
}

void test_phase_position_half_cycle(void)
{
    /* Halfway through a cycle */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, phase_position(5.0, 10.0, 0.0));
}

void test_phase_position_quarter_cycle(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.25, phase_position(2.5, 10.0, 0.0));
}

void test_phase_position_full_cycle_wraps(void)
{
    /* Exactly one full cycle wraps back to 0 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_position(10.0, 10.0, 0.0));
}

void test_phase_position_multi_cycle(void)
{
    /* 2.5 cycles in */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, phase_position(25.0, 10.0, 0.0));
}

void test_phase_position_with_offset(void)
{
    /* (13 - 3) mod 10 = 0 => 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_position(13.0, 10.0, 3.0));
}

void test_phase_position_negative_jd(void)
{
    /* fmod(-5,10)+10 = 5 => 5/10 = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, phase_position(-5.0, 10.0, 0.0));
}

void test_phase_position_always_in_range(void)
{
    /* Position must always be in [0, 1) for various inputs */
    double test_jds[] = {-10000.0, -1.0, 0.0, 0.001, 1.0, 100.0, 999999.0};
    int n = (int)(sizeof(test_jds) / sizeof(test_jds[0]));
    for (int i = 0; i < n; i++) {
        double pos = phase_position(test_jds[i], 29.530589, 0.0);
        TEST_ASSERT_TRUE(pos >= 0.0);
        TEST_ASSERT_TRUE(pos < 1.0);
    }
}

void test_phase_position_tzolkin_at_gmt(void)
{
    /* At JD 584283 (GMT correlation), Tzolkin phase should be 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, phase_position(584283.0, 260.0, 63.0));
}

void test_phase_position_large_jd(void)
{
    /* Large JD, ensure no floating-point catastrophe */
    double pos = phase_position(2451545.0, 365.24219, 0.0);
    TEST_ASSERT_TRUE(pos >= 0.0);
    TEST_ASSERT_TRUE(pos < 1.0);
}

/* ===== phase_toroidal_1d ===== */

void test_toroidal_1d_same_point(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_toroidal_1d(0.5, 0.5));
}

void test_toroidal_1d_small_gap(void)
{
    /* 0.2 apart */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.2, phase_toroidal_1d(0.3, 0.5));
}

void test_toroidal_1d_wrapping(void)
{
    /* 0.0 and 0.9 are 0.1 apart on torus, not 0.9 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.1, phase_toroidal_1d(0.0, 0.9));
}

void test_toroidal_1d_wrapping_symmetric(void)
{
    /* Symmetry: d(a,b) = d(b,a) */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.1, phase_toroidal_1d(0.9, 0.0));
}

void test_toroidal_1d_opposite(void)
{
    /* Diametrically opposite: 0.0 and 0.5 are 0.5 apart */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, phase_toroidal_1d(0.0, 0.5));
}

void test_toroidal_1d_max_distance(void)
{
    /* Maximum toroidal distance is 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, phase_toroidal_1d(0.25, 0.75));
}

void test_toroidal_1d_near_boundary(void)
{
    /* 0.95 and 0.05: should be 0.1 apart */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.1, phase_toroidal_1d(0.95, 0.05));
}

/* ===== phase_point_zero ===== */

void test_point_zero_dims(void)
{
    phase_point_t p = phase_point_zero(3);
    TEST_ASSERT_EQUAL_INT(3, p.dims);
}

void test_point_zero_all_zero(void)
{
    phase_point_t p = phase_point_zero(4);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, p.coords[i]);
    }
}

/* ===== phase_point_from_array ===== */

void test_point_from_array(void)
{
    double coords[] = {0.1, 0.2, 0.3};
    phase_point_t p = phase_point_from_array(coords, 3);
    TEST_ASSERT_EQUAL_INT(3, p.dims);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.1, p.coords[0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.2, p.coords[1]);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.3, p.coords[2]);
}

void test_point_from_array_clamps_dims(void)
{
    /* Requesting more than PHASE_MAX_DIMS clamps */
    double coords[20];
    for (int i = 0; i < 20; i++) coords[i] = 0.0;
    phase_point_t p = phase_point_from_array(coords, 20);
    TEST_ASSERT_EQUAL_INT(PHASE_MAX_DIMS, p.dims);
}

/* ===== phase_point_at ===== */

void test_point_at_single_cycle(void)
{
    phase_cycle_t cycles[1] = {{"test", 10.0, 0.0}};
    phase_point_t p = phase_point_at(5.0, cycles, 1);
    TEST_ASSERT_EQUAL_INT(1, p.dims);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, p.coords[0]);
}

void test_point_at_two_cycles(void)
{
    phase_cycle_t cycles[2] = {
        {"fast", 10.0, 0.0},
        {"slow", 20.0, 0.0}
    };
    phase_point_t p = phase_point_at(5.0, cycles, 2);
    TEST_ASSERT_EQUAL_INT(2, p.dims);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, p.coords[0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.25, p.coords[1]);
}

void test_point_at_clamps_count(void)
{
    /* More than PHASE_MAX_DIMS clamps */
    phase_cycle_t cycles[20];
    for (int i = 0; i < 20; i++) {
        cycles[i].name = "x";
        cycles[i].period_days = 10.0;
        cycles[i].epoch_offset = 0.0;
    }
    phase_point_t p = phase_point_at(5.0, cycles, 20);
    TEST_ASSERT_EQUAL_INT(PHASE_MAX_DIMS, p.dims);
}

/* ===== phase_euclidean ===== */

void test_euclidean_same_point(void)
{
    double c[] = {0.5, 0.5};
    phase_point_t a = phase_point_from_array(c, 2);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_euclidean(a, a));
}

void test_euclidean_unit_1d(void)
{
    double ca[] = {0.0};
    double cb[] = {1.0};
    phase_point_t a = phase_point_from_array(ca, 1);
    phase_point_t b = phase_point_from_array(cb, 1);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 1.0, phase_euclidean(a, b));
}

void test_euclidean_2d(void)
{
    double ca[] = {0.0, 0.0};
    double cb[] = {0.3, 0.4};
    phase_point_t a = phase_point_from_array(ca, 2);
    phase_point_t b = phase_point_from_array(cb, 2);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.5, phase_euclidean(a, b));
}

/* ===== phase_toroidal ===== */

void test_toroidal_same_point(void)
{
    double c[] = {0.5, 0.5};
    phase_point_t a = phase_point_from_array(c, 2);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_toroidal(a, a));
}

void test_toroidal_wrapping_2d(void)
{
    /* (0.0, 0.0) vs (0.9, 0.9): toroidal = sqrt(0.1^2 + 0.1^2) */
    double ca[] = {0.0, 0.0};
    double cb[] = {0.9, 0.9};
    phase_point_t a = phase_point_from_array(ca, 2);
    phase_point_t b = phase_point_from_array(cb, 2);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, sqrt(0.1 * 0.1 + 0.1 * 0.1),
                             phase_toroidal(a, b));
}

void test_toroidal_vs_euclidean_no_wrap(void)
{
    /* When no wrapping needed, toroidal == euclidean */
    double ca[] = {0.2, 0.3};
    double cb[] = {0.4, 0.5};
    phase_point_t a = phase_point_from_array(ca, 2);
    phase_point_t b = phase_point_from_array(cb, 2);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, phase_euclidean(a, b),
                             phase_toroidal(a, b));
}

/* ===== phase_distance ===== */

void test_distance_returns_all_fields(void)
{
    double ca[] = {0.0, 0.0};
    double cb[] = {0.9, 0.3};
    phase_point_t a = phase_point_from_array(ca, 2);
    phase_point_t b = phase_point_from_array(cb, 2);
    phase_distance_t d = phase_distance(a, b);

    /* euclidean: sqrt(0.9^2 + 0.3^2) */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, sqrt(0.81 + 0.09), d.euclidean);

    /* toroidal: 0.9 wraps to 0.1, 0.3 stays 0.3 => sqrt(0.01+0.09) */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, sqrt(0.01 + 0.09), d.toroidal);

    /* max_component: dim 1 has 0.3 toroidal, dim 0 has 0.1 toroidal => max is 0.3 */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.3, d.max_component);
    TEST_ASSERT_EQUAL_INT(1, d.furthest_dim);
}

void test_distance_identical_points(void)
{
    double c[] = {0.25, 0.75, 0.5};
    phase_point_t a = phase_point_from_array(c, 3);
    phase_distance_t d = phase_distance(a, a);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, d.euclidean);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, d.toroidal);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, d.max_component);
}

/* ===== phase_known_cycle ===== */

void test_known_cycle_tzolkin(void)
{
    phase_cycle_t c = phase_known_cycle("tzolkin");
    TEST_ASSERT_FLOAT_WITHIN(0.01, 260.0, c.period_days);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 63.0, c.epoch_offset);
}

void test_known_cycle_lunar(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 29.530589,
                             phase_known_cycle("lunar").period_days);
}

void test_known_cycle_zodiac(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 365.24219,
                             phase_known_cycle("zodiac").period_days);
}

void test_known_cycle_metonic(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 6939.688,
                             phase_known_cycle("metonic").period_days);
}

void test_known_cycle_saros(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 6585.321,
                             phase_known_cycle("saros").period_days);
}

void test_known_cycle_week(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 7.0,
                             phase_known_cycle("week").period_days);
}

void test_known_cycle_unknown_returns_zero_period(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0,
                             phase_known_cycle("nonexistent").period_days);
}

void test_known_cycle_count_at_least_9(void)
{
    TEST_ASSERT_TRUE(phase_known_cycle_count() >= 9);
}

void test_known_cycle_get_valid_index(void)
{
    int n = phase_known_cycle_count();
    for (int i = 0; i < n; i++) {
        phase_cycle_t c = phase_known_cycle_get(i);
        TEST_ASSERT_TRUE(c.period_days > 0.0);
        TEST_ASSERT_NOT_NULL(c.name);
    }
}

void test_known_cycle_get_invalid_index(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0,
                             phase_known_cycle_get(-1).period_days);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0,
                             phase_known_cycle_get(999).period_days);
}

/* ===== phase_nearest_return ===== */

void test_nearest_return_finds_exact_repeat(void)
{
    /* Simple 10-day cycle: target at phase 0.0, starting from JD 0.5.
     * Next exact return to phase 0.0 is at JD 10. */
    phase_cycle_t cycles[1] = {{"simple", 10.0, 0.0}};
    double tc[] = {0.0};
    phase_point_t target = phase_point_from_array(tc, 1);

    phase_alignment_t r = phase_nearest_return(
        0.5, target, cycles, 1, 0.5, 100.0, 0.01);

    TEST_ASSERT_FLOAT_WITHIN(1.0, 10.0, r.jd);
    TEST_ASSERT_TRUE(r.is_exact);
}

void test_nearest_return_two_cycles(void)
{
    /* Two cycles: 10 and 20 days. Both at phase 0.0.
     * LCM alignment at JD 20 (both complete). */
    phase_cycle_t cycles[2] = {
        {"fast", 10.0, 0.0},
        {"slow", 20.0, 0.0}
    };
    double tc[] = {0.0, 0.0};
    phase_point_t target = phase_point_from_array(tc, 2);

    phase_alignment_t r = phase_nearest_return(
        0.5, target, cycles, 2, 0.5, 100.0, 0.02);

    TEST_ASSERT_FLOAT_WITHIN(1.0, 20.0, r.jd);
    TEST_ASSERT_TRUE(r.is_exact);
}

void test_nearest_return_respects_max_days(void)
{
    /* If max_days is too short, we should still return best found */
    phase_cycle_t cycles[1] = {{"simple", 100.0, 0.0}};
    double tc[] = {0.0};
    phase_point_t target = phase_point_from_array(tc, 1);

    phase_alignment_t r = phase_nearest_return(
        1.0, target, cycles, 1, 1.0, 50.0, 0.001);

    /* Can't reach full cycle (100 days) within 50 days */
    TEST_ASSERT_FALSE(r.is_exact);
}

void test_nearest_return_non_zero_target(void)
{
    /* Looking for phase 0.5 in a 10-day cycle from JD 0.5.
     * Phase 0.5 occurs at JD 5. */
    phase_cycle_t cycles[1] = {{"simple", 10.0, 0.0}};
    double tc[] = {0.5};
    phase_point_t target = phase_point_from_array(tc, 1);

    phase_alignment_t r = phase_nearest_return(
        0.5, target, cycles, 1, 0.25, 100.0, 0.02);

    TEST_ASSERT_FLOAT_WITHIN(1.0, 5.0, r.jd);
    TEST_ASSERT_TRUE(r.is_exact);
}

/* ===== phase_nearest_past ===== */

void test_nearest_past_finds_previous(void)
{
    /* 10-day cycle, at JD 25. Last phase 0.0 was at JD 20. */
    phase_cycle_t cycles[1] = {{"simple", 10.0, 0.0}};
    double tc[] = {0.0};
    phase_point_t target = phase_point_from_array(tc, 1);

    phase_alignment_t r = phase_nearest_past(
        25.0, target, cycles, 1, 0.5, 100.0, 0.02);

    TEST_ASSERT_FLOAT_WITHIN(1.0, 20.0, r.jd);
    TEST_ASSERT_TRUE(r.is_exact);
}

void test_nearest_past_two_cycles(void)
{
    /* Two cycles: 10 and 15 days. Both at phase 0.0.
     * LCM = 30. At JD 35, last joint alignment was JD 30. */
    phase_cycle_t cycles[2] = {
        {"fast", 10.0, 0.0},
        {"slow", 15.0, 0.0}
    };
    double tc[] = {0.0, 0.0};
    phase_point_t target = phase_point_from_array(tc, 2);

    phase_alignment_t r = phase_nearest_past(
        35.0, target, cycles, 2, 0.5, 100.0, 0.02);

    TEST_ASSERT_FLOAT_WITHIN(1.0, 30.0, r.jd);
    TEST_ASSERT_TRUE(r.is_exact);
}

void test_nearest_past_respects_max_days(void)
{
    /* Can't look back far enough */
    phase_cycle_t cycles[1] = {{"simple", 100.0, 0.0}};
    double tc[] = {0.0};
    phase_point_t target = phase_point_from_array(tc, 1);

    phase_alignment_t r = phase_nearest_past(
        50.0, target, cycles, 1, 1.0, 30.0, 0.001);

    /* Can't find full cycle origin within 30-day lookback from JD 50 */
    TEST_ASSERT_FALSE(r.is_exact);
}

/* ===== Edge cases and integration ===== */

void test_phase_position_very_small_period(void)
{
    /* Very small period should still work */
    double pos = phase_position(1.0, 0.001, 0.0);
    TEST_ASSERT_TRUE(pos >= 0.0);
    TEST_ASSERT_TRUE(pos < 1.0);
}

void test_toroidal_1d_both_at_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.0, phase_toroidal_1d(0.0, 0.0));
}

void test_euclidean_3d(void)
{
    double ca[] = {0.0, 0.0, 0.0};
    double cb[] = {0.1, 0.2, 0.2};
    phase_point_t a = phase_point_from_array(ca, 3);
    phase_point_t b = phase_point_from_array(cb, 3);
    /* sqrt(0.01 + 0.04 + 0.04) = sqrt(0.09) = 0.3 */
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 0.3, phase_euclidean(a, b));
}

void test_point_at_with_offset(void)
{
    /* Cycle with offset: period 100, offset 25, JD 125 => phase 0.0 */
    phase_cycle_t cycles[1] = {{"offset_test", 100.0, 25.0}};
    phase_point_t p = phase_point_at(125.0, cycles, 1);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, p.coords[0]);
}

void test_known_cycle_sexagenary(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1, 21914.55,
                             phase_known_cycle("sexagenary").period_days);
}

void test_known_cycle_hijri(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 10631.016,
                             phase_known_cycle("hijri").period_days);
}

void test_known_cycle_sabbatical(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2556.75,
                             phase_known_cycle("sabbatical").period_days);
}

void test_distance_symmetry(void)
{
    double ca[] = {0.1, 0.9};
    double cb[] = {0.8, 0.2};
    phase_point_t a = phase_point_from_array(ca, 2);
    phase_point_t b = phase_point_from_array(cb, 2);
    phase_distance_t d1 = phase_distance(a, b);
    phase_distance_t d2 = phase_distance(b, a);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, d1.toroidal, d2.toroidal);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, d1.euclidean, d2.euclidean);
}

void test_toroidal_always_leq_euclidean(void)
{
    /* Toroidal distance should never exceed Euclidean */
    double ca[] = {0.1, 0.9, 0.3};
    double cb[] = {0.8, 0.2, 0.7};
    phase_point_t a = phase_point_from_array(ca, 3);
    phase_point_t b = phase_point_from_array(cb, 3);
    phase_distance_t d = phase_distance(a, b);
    TEST_ASSERT_TRUE(d.toroidal <= d.euclidean + 1e-12);
}

void test_nearest_return_distance_decreases(void)
{
    /* The result's distance should be a local minimum */
    phase_cycle_t cycles[1] = {{"simple", 7.0, 0.0}};
    double tc[] = {0.0};
    phase_point_t target = phase_point_from_array(tc, 1);

    phase_alignment_t r = phase_nearest_return(
        0.5, target, cycles, 1, 0.25, 50.0, 0.05);

    TEST_ASSERT_FLOAT_WITHIN(1.0, 7.0, r.jd);
    TEST_ASSERT_TRUE(r.distance < 0.05);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* phase_position (10 tests) */
    RUN_TEST(test_phase_position_zero_at_epoch);
    RUN_TEST(test_phase_position_half_cycle);
    RUN_TEST(test_phase_position_quarter_cycle);
    RUN_TEST(test_phase_position_full_cycle_wraps);
    RUN_TEST(test_phase_position_multi_cycle);
    RUN_TEST(test_phase_position_with_offset);
    RUN_TEST(test_phase_position_negative_jd);
    RUN_TEST(test_phase_position_always_in_range);
    RUN_TEST(test_phase_position_tzolkin_at_gmt);
    RUN_TEST(test_phase_position_large_jd);

    /* phase_toroidal_1d (7 tests) */
    RUN_TEST(test_toroidal_1d_same_point);
    RUN_TEST(test_toroidal_1d_small_gap);
    RUN_TEST(test_toroidal_1d_wrapping);
    RUN_TEST(test_toroidal_1d_wrapping_symmetric);
    RUN_TEST(test_toroidal_1d_opposite);
    RUN_TEST(test_toroidal_1d_max_distance);
    RUN_TEST(test_toroidal_1d_near_boundary);

    /* phase_point helpers (4 tests) */
    RUN_TEST(test_point_zero_dims);
    RUN_TEST(test_point_zero_all_zero);
    RUN_TEST(test_point_from_array);
    RUN_TEST(test_point_from_array_clamps_dims);

    /* phase_point_at (3 tests) */
    RUN_TEST(test_point_at_single_cycle);
    RUN_TEST(test_point_at_two_cycles);
    RUN_TEST(test_point_at_clamps_count);

    /* phase_euclidean (3 tests) */
    RUN_TEST(test_euclidean_same_point);
    RUN_TEST(test_euclidean_unit_1d);
    RUN_TEST(test_euclidean_2d);

    /* phase_toroidal (3 tests) */
    RUN_TEST(test_toroidal_same_point);
    RUN_TEST(test_toroidal_wrapping_2d);
    RUN_TEST(test_toroidal_vs_euclidean_no_wrap);

    /* phase_distance (3 tests) */
    RUN_TEST(test_distance_returns_all_fields);
    RUN_TEST(test_distance_identical_points);
    RUN_TEST(test_distance_symmetry);

    /* phase_known_cycle (10 tests) */
    RUN_TEST(test_known_cycle_tzolkin);
    RUN_TEST(test_known_cycle_lunar);
    RUN_TEST(test_known_cycle_zodiac);
    RUN_TEST(test_known_cycle_metonic);
    RUN_TEST(test_known_cycle_saros);
    RUN_TEST(test_known_cycle_week);
    RUN_TEST(test_known_cycle_unknown_returns_zero_period);
    RUN_TEST(test_known_cycle_count_at_least_9);
    RUN_TEST(test_known_cycle_get_valid_index);
    RUN_TEST(test_known_cycle_get_invalid_index);

    /* phase_nearest_return (4 tests) */
    RUN_TEST(test_nearest_return_finds_exact_repeat);
    RUN_TEST(test_nearest_return_two_cycles);
    RUN_TEST(test_nearest_return_respects_max_days);
    RUN_TEST(test_nearest_return_non_zero_target);

    /* phase_nearest_past (3 tests) */
    RUN_TEST(test_nearest_past_finds_previous);
    RUN_TEST(test_nearest_past_two_cycles);
    RUN_TEST(test_nearest_past_respects_max_days);

    /* Edge cases and integration (9 tests) */
    RUN_TEST(test_phase_position_very_small_period);
    RUN_TEST(test_toroidal_1d_both_at_zero);
    RUN_TEST(test_euclidean_3d);
    RUN_TEST(test_point_at_with_offset);
    RUN_TEST(test_known_cycle_sexagenary);
    RUN_TEST(test_known_cycle_hijri);
    RUN_TEST(test_known_cycle_sabbatical);
    RUN_TEST(test_toroidal_always_leq_euclidean);
    RUN_TEST(test_nearest_return_distance_decreases);

    return UNITY_END();
}
