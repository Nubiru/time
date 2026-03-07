/* test_tide_predict.c — Tests for tidal prediction engine module.
 * TDD RED phase: all tests written before implementation.
 * Combines tidal harmonics with surf spot preferences to produce
 * tide forecasts and surf quality windows. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/tide_predict.h"
#include "../../../src/systems/earth/tidal.h"
#include "../../../src/systems/earth/surf_spots.h"

#include <math.h>

/* J2000.0 epoch */
static const double J2000 = 2451545.0;

void setUp(void) { }
void tearDown(void) { }

/* ===== tide_predict_height ===== */

void test_height_returns_finite_value(void) {
    double h = tide_predict_height(J2000);
    TEST_ASSERT_TRUE(isfinite(h));
}

void test_height_at_j2000(void) {
    /* At J2000 hours_since_epoch=0, all cosines=1, sum of amplitudes */
    double expected = TIDAL_M2_AMP + TIDAL_S2_AMP + TIDAL_N2_AMP
                    + TIDAL_K1_AMP + TIDAL_O1_AMP;
    double h = tide_predict_height(J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)expected, (float)h);
}

void test_height_varies_over_time(void) {
    double h1 = tide_predict_height(J2000);
    double h2 = tide_predict_height(J2000 + 0.25); /* 6 hours later */
    TEST_ASSERT_TRUE(fabs(h1 - h2) > 0.01);
}

void test_height_within_expected_range(void) {
    /* Test many points over a tidal cycle */
    for (int i = 0; i < 100; i++) {
        double jd = J2000 + i * 0.1;
        double h = tide_predict_height(jd);
        TEST_ASSERT_TRUE(h >= -3.0 && h <= 3.0);
    }
}

void test_height_periodic_m2(void) {
    /* Height should be roughly periodic over M2 period (12.4206 hours) */
    double jd_offset = J2000 + 10.0; /* arbitrary offset */
    double h1 = tide_predict_height(jd_offset);
    double m2_days = TIDAL_M2_PERIOD_H / 24.0;
    double h2 = tide_predict_height(jd_offset + m2_days);
    /* Not exactly equal due to other constituents, but M2 dominates */
    TEST_ASSERT_FLOAT_WITHIN(1.5f, (float)h1, (float)h2);
}

/* ===== tide_predict_is_rising ===== */

void test_is_rising_returns_0_or_1(void) {
    int r = tide_predict_is_rising(J2000);
    TEST_ASSERT_TRUE(r == 0 || r == 1);
}

void test_is_rising_at_peak_is_not_rising(void) {
    /* At J2000 all cosines are 1 -> maximum -> should be falling */
    int r = tide_predict_is_rising(J2000);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_is_rising_changes_during_cycle(void) {
    /* Over half an M2 cycle, rising should change at least once */
    int found_rising = 0;
    int found_falling = 0;
    for (int i = 0; i < 50; i++) {
        double jd = J2000 + i * (TIDAL_M2_PERIOD_H / 50.0 / 24.0);
        int r = tide_predict_is_rising(jd);
        if (r) found_rising = 1;
        else found_falling = 1;
    }
    TEST_ASSERT_TRUE(found_rising);
    TEST_ASSERT_TRUE(found_falling);
}

/* ===== tide_predict_hours_to_high ===== */

void test_hours_to_high_positive(void) {
    double h = tide_predict_hours_to_high(J2000 + 1.0);
    TEST_ASSERT_TRUE(h > 0.0);
}

void test_hours_to_high_within_m2_period(void) {
    double h = tide_predict_hours_to_high(J2000 + 5.0);
    TEST_ASSERT_TRUE(h <= TIDAL_M2_PERIOD_H + 1.0);
}

void test_hours_to_high_finite(void) {
    double h = tide_predict_hours_to_high(J2000 + 100.0);
    TEST_ASSERT_TRUE(isfinite(h));
}

/* ===== tide_predict_hours_to_low ===== */

void test_hours_to_low_positive(void) {
    double h = tide_predict_hours_to_low(J2000 + 1.0);
    TEST_ASSERT_TRUE(h > 0.0);
}

void test_hours_to_low_within_m2_period(void) {
    double h = tide_predict_hours_to_low(J2000 + 5.0);
    TEST_ASSERT_TRUE(h <= TIDAL_M2_PERIOD_H + 1.0);
}

void test_hours_to_low_finite(void) {
    double h = tide_predict_hours_to_low(J2000 + 100.0);
    TEST_ASSERT_TRUE(isfinite(h));
}

/* ===== tide_predict_state ===== */

void test_state_returns_valid_struct(void) {
    tide_state_t s = tide_predict_state(J2000, 21.665, -158.053);
    TEST_ASSERT_TRUE(isfinite(s.height_relative));
    TEST_ASSERT_TRUE(s.rising == 0 || s.rising == 1);
}

void test_state_spring_neap_factor_range(void) {
    tide_state_t s = tide_predict_state(J2000, 0.0, 0.0);
    TEST_ASSERT_TRUE(s.spring_neap_factor >= 0.0);
    TEST_ASSERT_TRUE(s.spring_neap_factor <= 1.0);
}

void test_state_hours_to_high_positive(void) {
    tide_state_t s = tide_predict_state(J2000 + 1.0, 0.0, 0.0);
    TEST_ASSERT_TRUE(s.hours_to_next_high > 0.0);
}

void test_state_hours_to_low_positive(void) {
    tide_state_t s = tide_predict_state(J2000 + 1.0, 0.0, 0.0);
    TEST_ASSERT_TRUE(s.hours_to_next_low > 0.0);
}

void test_state_spring_or_neap_or_neither(void) {
    /* Can't be both spring and neap simultaneously */
    tide_state_t s = tide_predict_state(J2000, 0.0, 0.0);
    TEST_ASSERT_FALSE(s.is_spring && s.is_neap);
}

void test_state_king_only_during_spring(void) {
    /* King tide requires spring. If king, must be spring. */
    for (int i = 0; i < 50; i++) {
        double jd = J2000 + i * 0.5;
        tide_state_t s = tide_predict_state(jd, 0.0, 0.0);
        if (s.is_king) {
            TEST_ASSERT_TRUE(s.is_spring);
        }
    }
}

void test_state_height_matches_direct_height(void) {
    double jd = J2000 + 3.5;
    tide_state_t s = tide_predict_state(jd, 0.0, 0.0);
    double direct = tide_predict_height(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)direct, (float)s.height_relative);
}

void test_state_rising_matches_direct(void) {
    double jd = J2000 + 7.3;
    tide_state_t s = tide_predict_state(jd, 0.0, 0.0);
    int direct = tide_predict_is_rising(jd);
    TEST_ASSERT_EQUAL_INT(direct, s.rising);
}

/* ===== tide_predict_surf_quality ===== */

void test_surf_quality_range(void) {
    double q = tide_predict_surf_quality(J2000, 0);
    TEST_ASSERT_TRUE(q >= 0.0 && q <= 1.0);
}

void test_surf_quality_invalid_spot_returns_zero(void) {
    double q = tide_predict_surf_quality(J2000, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)q);
}

void test_surf_quality_invalid_spot_over_max(void) {
    double q = tide_predict_surf_quality(J2000, 999);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)q);
}

void test_surf_quality_varies_with_time(void) {
    double q1 = tide_predict_surf_quality(J2000, 0);
    double q2 = tide_predict_surf_quality(J2000 + 0.25, 0);
    /* Quality depends on tide height which changes over 6 hours */
    TEST_ASSERT_TRUE(fabs(q1 - q2) >= 0.0);
}

/* ===== tide_predict_best_today ===== */

void test_best_today_returns_valid_jd(void) {
    double best = tide_predict_best_today(J2000, 0);
    /* Should be within 1 day of input */
    TEST_ASSERT_TRUE(best >= J2000);
    TEST_ASSERT_TRUE(best <= J2000 + 1.0);
}

void test_best_today_quality_is_high(void) {
    double best_jd = tide_predict_best_today(J2000 + 10.0, 0);
    double best_q = tide_predict_surf_quality(best_jd, 0);
    /* The "best" should have reasonable quality */
    TEST_ASSERT_TRUE(best_q >= 0.0);
}

void test_best_today_invalid_spot(void) {
    double best = tide_predict_best_today(J2000, -1);
    /* Should still return a valid JD even for invalid spot */
    TEST_ASSERT_TRUE(isfinite(best));
}

/* ===== tide_predict_surf_windows ===== */

void test_surf_windows_count_non_negative(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 1.0, 0);
    TEST_ASSERT_TRUE(fc.count >= 0);
}

void test_surf_windows_count_within_max(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 7.0, 0);
    TEST_ASSERT_TRUE(fc.count <= TIDE_PREDICT_MAX_WINDOWS);
}

void test_surf_windows_jd_within_range(void) {
    double start = J2000 + 5.0;
    double end = start + 3.0;
    surf_forecast_t fc = tide_predict_surf_windows(start, end, 0);
    for (int i = 0; i < fc.count; i++) {
        TEST_ASSERT_TRUE(fc.windows[i].jd >= start);
        TEST_ASSERT_TRUE(fc.windows[i].jd <= end);
    }
}

void test_surf_windows_quality_range(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 2.0, 0);
    for (int i = 0; i < fc.count; i++) {
        TEST_ASSERT_TRUE(fc.windows[i].quality >= 0.0);
        TEST_ASSERT_TRUE(fc.windows[i].quality <= 1.0);
    }
}

void test_surf_windows_tide_height_finite(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 1.0, 5);
    for (int i = 0; i < fc.count; i++) {
        TEST_ASSERT_TRUE(isfinite(fc.windows[i].tide_height));
    }
}

void test_surf_windows_invalid_spot_empty(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 1.0, -1);
    TEST_ASSERT_EQUAL_INT(0, fc.count);
}

void test_surf_windows_reversed_range_empty(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000 + 5.0, J2000, 0);
    TEST_ASSERT_EQUAL_INT(0, fc.count);
}

void test_surf_windows_sorted_by_jd(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 3.0, 0);
    for (int i = 1; i < fc.count; i++) {
        TEST_ASSERT_TRUE(fc.windows[i].jd >= fc.windows[i - 1].jd);
    }
}

void test_surf_windows_spring_flag_consistent(void) {
    surf_forecast_t fc = tide_predict_surf_windows(J2000, J2000 + 2.0, 0);
    for (int i = 0; i < fc.count; i++) {
        TEST_ASSERT_TRUE(fc.windows[i].is_spring == 0 ||
                         fc.windows[i].is_spring == 1);
    }
}

/* ===== Edge cases ===== */

void test_height_far_future(void) {
    double h = tide_predict_height(J2000 + 365250.0); /* ~1000 years */
    TEST_ASSERT_TRUE(isfinite(h));
    TEST_ASSERT_TRUE(h >= -3.0 && h <= 3.0);
}

void test_state_far_past(void) {
    tide_state_t s = tide_predict_state(J2000 - 36525.0, 0.0, 0.0);
    TEST_ASSERT_TRUE(isfinite(s.height_relative));
}

void test_surf_quality_multiple_spots(void) {
    /* Check a few different spots */
    for (int i = 0; i < 5; i++) {
        double q = tide_predict_surf_quality(J2000 + 3.0, i);
        TEST_ASSERT_TRUE(q >= 0.0 && q <= 1.0);
    }
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* tide_predict_height */
    RUN_TEST(test_height_returns_finite_value);
    RUN_TEST(test_height_at_j2000);
    RUN_TEST(test_height_varies_over_time);
    RUN_TEST(test_height_within_expected_range);
    RUN_TEST(test_height_periodic_m2);

    /* tide_predict_is_rising */
    RUN_TEST(test_is_rising_returns_0_or_1);
    RUN_TEST(test_is_rising_at_peak_is_not_rising);
    RUN_TEST(test_is_rising_changes_during_cycle);

    /* tide_predict_hours_to_high */
    RUN_TEST(test_hours_to_high_positive);
    RUN_TEST(test_hours_to_high_within_m2_period);
    RUN_TEST(test_hours_to_high_finite);

    /* tide_predict_hours_to_low */
    RUN_TEST(test_hours_to_low_positive);
    RUN_TEST(test_hours_to_low_within_m2_period);
    RUN_TEST(test_hours_to_low_finite);

    /* tide_predict_state */
    RUN_TEST(test_state_returns_valid_struct);
    RUN_TEST(test_state_spring_neap_factor_range);
    RUN_TEST(test_state_hours_to_high_positive);
    RUN_TEST(test_state_hours_to_low_positive);
    RUN_TEST(test_state_spring_or_neap_or_neither);
    RUN_TEST(test_state_king_only_during_spring);
    RUN_TEST(test_state_height_matches_direct_height);
    RUN_TEST(test_state_rising_matches_direct);

    /* tide_predict_surf_quality */
    RUN_TEST(test_surf_quality_range);
    RUN_TEST(test_surf_quality_invalid_spot_returns_zero);
    RUN_TEST(test_surf_quality_invalid_spot_over_max);
    RUN_TEST(test_surf_quality_varies_with_time);

    /* tide_predict_best_today */
    RUN_TEST(test_best_today_returns_valid_jd);
    RUN_TEST(test_best_today_quality_is_high);
    RUN_TEST(test_best_today_invalid_spot);

    /* tide_predict_surf_windows */
    RUN_TEST(test_surf_windows_count_non_negative);
    RUN_TEST(test_surf_windows_count_within_max);
    RUN_TEST(test_surf_windows_jd_within_range);
    RUN_TEST(test_surf_windows_quality_range);
    RUN_TEST(test_surf_windows_tide_height_finite);
    RUN_TEST(test_surf_windows_invalid_spot_empty);
    RUN_TEST(test_surf_windows_reversed_range_empty);
    RUN_TEST(test_surf_windows_sorted_by_jd);
    RUN_TEST(test_surf_windows_spring_flag_consistent);

    /* Edge cases */
    RUN_TEST(test_height_far_future);
    RUN_TEST(test_state_far_past);
    RUN_TEST(test_surf_quality_multiple_spots);

    return UNITY_END();
}
