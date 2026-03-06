/* test_tidal.c — Tests for tidal harmonic constants module.
 * TDD RED phase: all tests written before implementation.
 * Data source: NOAA tidal constituent tables, Doodson (1921). */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/tidal.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Constituent Data ===== */

void test_constituent_count_is_5(void) {
    TEST_ASSERT_EQUAL_INT(5, tidal_constituent_count());
}

void test_constituent_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(TIDAL_CONSTITUENT_COUNT, tidal_constituent_count());
}

void test_constituent_m2_name(void) {
    tidal_constituent_t c = tidal_constituent_get(0);
    TEST_ASSERT_EQUAL_STRING("M2", c.name);
}

void test_constituent_m2_period(void) {
    tidal_constituent_t c = tidal_constituent_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)TIDAL_M2_PERIOD_H,
                             (float)c.period_hours);
}

void test_constituent_m2_amplitude(void) {
    tidal_constituent_t c = tidal_constituent_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)TIDAL_M2_AMP,
                             (float)c.amplitude);
}

void test_constituent_m2_description(void) {
    tidal_constituent_t c = tidal_constituent_get(0);
    TEST_ASSERT_NOT_NULL(c.description);
    TEST_ASSERT_TRUE(strlen(c.description) > 0);
}

void test_constituent_s2(void) {
    tidal_constituent_t c = tidal_constituent_get(1);
    TEST_ASSERT_EQUAL_STRING("S2", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)TIDAL_S2_PERIOD_H,
                             (float)c.period_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)TIDAL_S2_AMP,
                             (float)c.amplitude);
}

void test_constituent_n2(void) {
    tidal_constituent_t c = tidal_constituent_get(2);
    TEST_ASSERT_EQUAL_STRING("N2", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)TIDAL_N2_PERIOD_H,
                             (float)c.period_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)TIDAL_N2_AMP,
                             (float)c.amplitude);
}

void test_constituent_k1(void) {
    tidal_constituent_t c = tidal_constituent_get(3);
    TEST_ASSERT_EQUAL_STRING("K1", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)TIDAL_K1_PERIOD_H,
                             (float)c.period_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)TIDAL_K1_AMP,
                             (float)c.amplitude);
}

void test_constituent_o1(void) {
    tidal_constituent_t c = tidal_constituent_get(4);
    TEST_ASSERT_EQUAL_STRING("O1", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)TIDAL_O1_PERIOD_H,
                             (float)c.period_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)TIDAL_O1_AMP,
                             (float)c.amplitude);
}

void test_constituent_out_of_bounds_negative(void) {
    tidal_constituent_t c = tidal_constituent_get(-1);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)c.period_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)c.amplitude);
}

void test_constituent_out_of_bounds_high(void) {
    tidal_constituent_t c = tidal_constituent_get(5);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)c.period_hours);
}

/* ===== Tidal Force ===== */

void test_force_at_mean_distances(void) {
    /* At mean distances: lunar ~2.17x solar */
    tidal_force_t f = tidal_force(384400.0, 149597870.7);
    double ratio = f.lunar_force / f.solar_force;
    TEST_ASSERT_FLOAT_WITHIN(0.15f, 2.17f, (float)ratio);
}

void test_force_lunar_fraction_at_mean(void) {
    /* Moon's share is ~0.685 at mean distances */
    tidal_force_t f = tidal_force(384400.0, 149597870.7);
    TEST_ASSERT_FLOAT_WITHIN(0.03f, 0.685f, (float)f.lunar_fraction);
}

void test_force_total_is_sum(void) {
    tidal_force_t f = tidal_force(384400.0, 149597870.7);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)(f.lunar_force + f.solar_force),
                             (float)f.total_force);
}

void test_force_closer_moon_stronger(void) {
    /* Perigee Moon should produce stronger lunar force */
    tidal_force_t f_mean = tidal_force(384400.0, 149597870.7);
    tidal_force_t f_close = tidal_force(356500.0, 149597870.7);
    TEST_ASSERT_TRUE(f_close.lunar_force > f_mean.lunar_force);
}

void test_force_farther_moon_weaker(void) {
    /* Apogee Moon should produce weaker lunar force */
    tidal_force_t f_mean = tidal_force(384400.0, 149597870.7);
    tidal_force_t f_far = tidal_force(406700.0, 149597870.7);
    TEST_ASSERT_TRUE(f_far.lunar_force < f_mean.lunar_force);
}

void test_force_scales_inversely_cubed(void) {
    /* At half-distance, force should be 8x */
    tidal_force_t f1 = tidal_force(384400.0, 149597870.7);
    tidal_force_t f2 = tidal_force(192200.0, 149597870.7);
    double ratio = f2.lunar_force / f1.lunar_force;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 8.0f, (float)ratio);
}

void test_force_solar_positive(void) {
    tidal_force_t f = tidal_force(384400.0, 149597870.7);
    TEST_ASSERT_TRUE(f.solar_force > 0.0);
}

void test_force_lunar_positive(void) {
    tidal_force_t f = tidal_force(384400.0, 149597870.7);
    TEST_ASSERT_TRUE(f.lunar_force > 0.0);
}

/* ===== Harmonic Height ===== */

void test_harmonic_height_bounded(void) {
    /* Height should be bounded: max possible is sum of all amplitudes */
    double max_amp = TIDAL_M2_AMP + TIDAL_S2_AMP + TIDAL_N2_AMP
                   + TIDAL_K1_AMP + TIDAL_O1_AMP;
    for (int i = 0; i < 100; i++) {
        double h = tidal_harmonic_height((double)i * 1.0);
        TEST_ASSERT_TRUE(h <= max_amp + 0.001);
        TEST_ASSERT_TRUE(h >= -max_amp - 0.001);
    }
}

void test_harmonic_height_varies(void) {
    /* Not all the same value — interference pattern should vary */
    double h0 = tidal_harmonic_height(0.0);
    double h6 = tidal_harmonic_height(6.0);
    TEST_ASSERT_TRUE(fabs(h0 - h6) > 0.01);
}

void test_harmonic_height_periodic(void) {
    /* M2 period is ~12.42h. After many M2 cycles the heights should
     * roughly repeat (not exactly due to other constituents). */
    double t_cycle = TIDAL_M2_PERIOD_H * 100.0;
    double h0 = tidal_harmonic_height(0.0);
    double h_cycle = tidal_harmonic_height(t_cycle);
    /* After 100 M2 cycles, M2 component is in phase; others shift.
     * Height won't match exactly, but M2 dominates so should be close-ish. */
    TEST_ASSERT_FLOAT_WITHIN(1.5f, (float)h0, (float)h_cycle);
}

void test_harmonic_height_at_zero(void) {
    /* At t=0 with all phases 0: sum of all amplitudes (cos(0)=1) */
    double expected = TIDAL_M2_AMP + TIDAL_S2_AMP + TIDAL_N2_AMP
                    + TIDAL_K1_AMP + TIDAL_O1_AMP;
    double h = tidal_harmonic_height(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)expected, (float)h);
}

/* ===== Spring-Neap Factor ===== */

void test_spring_neap_factor_new_moon(void) {
    /* phase_angle = 0 -> spring -> factor = 1.0 */
    double f = tidal_spring_neap_factor(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, (float)f);
}

void test_spring_neap_factor_full_moon(void) {
    /* phase_angle = 180 -> spring -> factor = 1.0 */
    double f = tidal_spring_neap_factor(180.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, (float)f);
}

void test_spring_neap_factor_first_quarter(void) {
    /* phase_angle = 90 -> neap -> factor = 0.0 */
    double f = tidal_spring_neap_factor(90.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)f);
}

void test_spring_neap_factor_third_quarter(void) {
    /* phase_angle = 270 -> neap -> factor = 0.0 */
    double f = tidal_spring_neap_factor(270.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)f);
}

void test_spring_neap_factor_45_degrees(void) {
    /* cos^2(45) = cos(45deg)^2 = 0.5 */
    double f = tidal_spring_neap_factor(45.0);
    double expected = cos(45.0 * PI / 180.0);
    expected = expected * expected;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected, (float)f);
}

void test_spring_neap_factor_range_0_to_1(void) {
    /* Factor should always be in [0, 1] */
    for (int deg = 0; deg < 360; deg++) {
        double f = tidal_spring_neap_factor((double)deg);
        TEST_ASSERT_TRUE(f >= -0.0001);
        TEST_ASSERT_TRUE(f <= 1.0001);
    }
}

void test_spring_neap_factor_negative_angle(void) {
    /* -90 should normalize to 270 -> neap -> factor = 0.0 */
    double f = tidal_spring_neap_factor(-90.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)f);
}

void test_spring_neap_factor_large_angle(void) {
    /* 720 should normalize to 0 -> spring -> factor = 1.0 */
    double f = tidal_spring_neap_factor(720.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, (float)f);
}

/* ===== Spring/Neap Detection ===== */

void test_is_spring_at_0(void) {
    TEST_ASSERT_EQUAL_INT(1, tidal_is_spring(0.0));
}

void test_is_spring_at_180(void) {
    TEST_ASSERT_EQUAL_INT(1, tidal_is_spring(180.0));
}

void test_is_spring_at_25(void) {
    /* Within 30 of 0 */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_spring(25.0));
}

void test_is_spring_at_190(void) {
    /* Within 30 of 180 */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_spring(190.0));
}

void test_is_spring_at_345(void) {
    /* 345 is 15 from 360/0 -> spring */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_spring(345.0));
}

void test_is_not_spring_at_90(void) {
    TEST_ASSERT_EQUAL_INT(0, tidal_is_spring(90.0));
}

void test_is_not_spring_at_270(void) {
    TEST_ASSERT_EQUAL_INT(0, tidal_is_spring(270.0));
}

void test_is_neap_at_90(void) {
    TEST_ASSERT_EQUAL_INT(1, tidal_is_neap(90.0));
}

void test_is_neap_at_270(void) {
    TEST_ASSERT_EQUAL_INT(1, tidal_is_neap(270.0));
}

void test_is_neap_at_100(void) {
    /* Within 30 of 90 */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_neap(100.0));
}

void test_is_neap_at_260(void) {
    /* Within 30 of 270 */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_neap(260.0));
}

void test_is_not_neap_at_0(void) {
    TEST_ASSERT_EQUAL_INT(0, tidal_is_neap(0.0));
}

void test_is_not_neap_at_180(void) {
    TEST_ASSERT_EQUAL_INT(0, tidal_is_neap(180.0));
}

/* ===== King Tide ===== */

void test_is_king_spring_plus_perigee(void) {
    /* Spring (phase=0) + close Moon (355000 km < 356500) */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_king(355000.0, 0.0));
}

void test_is_not_king_spring_but_far(void) {
    /* Spring (phase=0) but Moon at mean distance */
    TEST_ASSERT_EQUAL_INT(0, tidal_is_king(384400.0, 0.0));
}

void test_is_not_king_perigee_but_neap(void) {
    /* Close Moon but neap tide (phase=90) */
    TEST_ASSERT_EQUAL_INT(0, tidal_is_king(355000.0, 90.0));
}

void test_is_king_full_moon_perigee(void) {
    /* Spring (phase=180, full moon) + perigee */
    TEST_ASSERT_EQUAL_INT(1, tidal_is_king(354000.0, 180.0));
}

/* ===== Tidal State ===== */

void test_state_spring_neap_matches_factor(void) {
    double phase = 45.0;
    tidal_state_t s = tidal_state(0.0, 384400.0, 149597870.7, phase);
    double expected = tidal_spring_neap_factor(phase);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)expected, (float)s.spring_neap);
}

void test_state_height_matches_harmonic(void) {
    double hours = 123.456;
    tidal_state_t s = tidal_state(hours, 384400.0, 149597870.7, 45.0);
    double expected = tidal_harmonic_height(hours);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)expected, (float)s.height);
}

void test_state_spring_flag(void) {
    tidal_state_t s = tidal_state(0.0, 384400.0, 149597870.7, 5.0);
    TEST_ASSERT_EQUAL_INT(1, s.is_spring);
    TEST_ASSERT_EQUAL_INT(0, s.is_neap);
}

void test_state_neap_flag(void) {
    tidal_state_t s = tidal_state(0.0, 384400.0, 149597870.7, 90.0);
    TEST_ASSERT_EQUAL_INT(0, s.is_spring);
    TEST_ASSERT_EQUAL_INT(1, s.is_neap);
}

void test_state_king_flag(void) {
    tidal_state_t s = tidal_state(0.0, 354000.0, 149597870.7, 0.0);
    TEST_ASSERT_EQUAL_INT(1, s.is_king);
    TEST_ASSERT_EQUAL_INT(1, s.is_spring);
}

void test_state_neither_spring_nor_neap(void) {
    /* phase=45 is neither spring (within 30 of 0/180) nor neap
     * (within 30 of 90/270) */
    tidal_state_t s = tidal_state(0.0, 384400.0, 149597870.7, 45.0);
    TEST_ASSERT_EQUAL_INT(0, s.is_spring);
    TEST_ASSERT_EQUAL_INT(0, s.is_neap);
    TEST_ASSERT_EQUAL_INT(0, s.is_king);
}

/* ===== Hours to Next Spring/Neap ===== */

void test_hours_to_next_spring_from_0(void) {
    /* Already at spring (0). Next spring at 180. */
    /* Moon advances ~0.508 deg/hour. 180 / 0.508 ~ 354.3 hours */
    double h = tidal_hours_to_next_spring(0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 354.3f, (float)h);
}

void test_hours_to_next_spring_from_90(void) {
    /* At 90, next spring is at 180. 90 / 0.508 ~ 177.2 hours */
    double h = tidal_hours_to_next_spring(90.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 177.2f, (float)h);
}

void test_hours_to_next_spring_from_270(void) {
    /* At 270, next spring is at 360/0. 90 / 0.508 ~ 177.2 hours */
    double h = tidal_hours_to_next_spring(270.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 177.2f, (float)h);
}

void test_hours_to_next_neap_from_0(void) {
    /* At 0, next neap is at 90. 90 / 0.508 ~ 177.2 hours */
    double h = tidal_hours_to_next_neap(0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 177.2f, (float)h);
}

void test_hours_to_next_neap_from_180(void) {
    /* At 180, next neap is at 270. 90 / 0.508 ~ 177.2 hours */
    double h = tidal_hours_to_next_neap(180.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 177.2f, (float)h);
}

void test_hours_to_next_neap_from_90(void) {
    /* Already at neap (90). Next neap at 270. 180 / 0.508 ~ 354.3 hours */
    double h = tidal_hours_to_next_neap(90.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 354.3f, (float)h);
}

void test_hours_to_next_spring_positive(void) {
    /* Hours should always be positive */
    for (int deg = 0; deg < 360; deg += 10) {
        double h = tidal_hours_to_next_spring((double)deg);
        TEST_ASSERT_TRUE(h > 0.0);
    }
}

void test_hours_to_next_neap_positive(void) {
    for (int deg = 0; deg < 360; deg += 10) {
        double h = tidal_hours_to_next_neap((double)deg);
        TEST_ASSERT_TRUE(h > 0.0);
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Constituent data */
    RUN_TEST(test_constituent_count_is_5);
    RUN_TEST(test_constituent_count_matches_define);
    RUN_TEST(test_constituent_m2_name);
    RUN_TEST(test_constituent_m2_period);
    RUN_TEST(test_constituent_m2_amplitude);
    RUN_TEST(test_constituent_m2_description);
    RUN_TEST(test_constituent_s2);
    RUN_TEST(test_constituent_n2);
    RUN_TEST(test_constituent_k1);
    RUN_TEST(test_constituent_o1);
    RUN_TEST(test_constituent_out_of_bounds_negative);
    RUN_TEST(test_constituent_out_of_bounds_high);

    /* Tidal force */
    RUN_TEST(test_force_at_mean_distances);
    RUN_TEST(test_force_lunar_fraction_at_mean);
    RUN_TEST(test_force_total_is_sum);
    RUN_TEST(test_force_closer_moon_stronger);
    RUN_TEST(test_force_farther_moon_weaker);
    RUN_TEST(test_force_scales_inversely_cubed);
    RUN_TEST(test_force_solar_positive);
    RUN_TEST(test_force_lunar_positive);

    /* Harmonic height */
    RUN_TEST(test_harmonic_height_bounded);
    RUN_TEST(test_harmonic_height_varies);
    RUN_TEST(test_harmonic_height_periodic);
    RUN_TEST(test_harmonic_height_at_zero);

    /* Spring-neap factor */
    RUN_TEST(test_spring_neap_factor_new_moon);
    RUN_TEST(test_spring_neap_factor_full_moon);
    RUN_TEST(test_spring_neap_factor_first_quarter);
    RUN_TEST(test_spring_neap_factor_third_quarter);
    RUN_TEST(test_spring_neap_factor_45_degrees);
    RUN_TEST(test_spring_neap_factor_range_0_to_1);
    RUN_TEST(test_spring_neap_factor_negative_angle);
    RUN_TEST(test_spring_neap_factor_large_angle);

    /* Spring/neap detection */
    RUN_TEST(test_is_spring_at_0);
    RUN_TEST(test_is_spring_at_180);
    RUN_TEST(test_is_spring_at_25);
    RUN_TEST(test_is_spring_at_190);
    RUN_TEST(test_is_spring_at_345);
    RUN_TEST(test_is_not_spring_at_90);
    RUN_TEST(test_is_not_spring_at_270);
    RUN_TEST(test_is_neap_at_90);
    RUN_TEST(test_is_neap_at_270);
    RUN_TEST(test_is_neap_at_100);
    RUN_TEST(test_is_neap_at_260);
    RUN_TEST(test_is_not_neap_at_0);
    RUN_TEST(test_is_not_neap_at_180);

    /* King tide */
    RUN_TEST(test_is_king_spring_plus_perigee);
    RUN_TEST(test_is_not_king_spring_but_far);
    RUN_TEST(test_is_not_king_perigee_but_neap);
    RUN_TEST(test_is_king_full_moon_perigee);

    /* Tidal state */
    RUN_TEST(test_state_spring_neap_matches_factor);
    RUN_TEST(test_state_height_matches_harmonic);
    RUN_TEST(test_state_spring_flag);
    RUN_TEST(test_state_neap_flag);
    RUN_TEST(test_state_king_flag);
    RUN_TEST(test_state_neither_spring_nor_neap);

    /* Hours to next spring/neap */
    RUN_TEST(test_hours_to_next_spring_from_0);
    RUN_TEST(test_hours_to_next_spring_from_90);
    RUN_TEST(test_hours_to_next_spring_from_270);
    RUN_TEST(test_hours_to_next_neap_from_0);
    RUN_TEST(test_hours_to_next_neap_from_180);
    RUN_TEST(test_hours_to_next_neap_from_90);
    RUN_TEST(test_hours_to_next_spring_positive);
    RUN_TEST(test_hours_to_next_neap_positive);

    return UNITY_END();
}
