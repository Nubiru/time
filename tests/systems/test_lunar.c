#include "../unity/unity.h"
#include "../../src/systems/astronomy/lunar.h"
#include "../../src/math/julian.h"
#include <math.h>
#include <string.h>

#define EPSILON 1e-4f

void setUp(void) { }
void tearDown(void) { }

/* --- Known date tests --- */

void test_reference_new_moon_2000_jan_06(void) {
    /* 2000-01-06 18:14 UT = JD 2451550.26 — reference New Moon */
    lunar_info_t info = lunar_phase(2451550.26);
    TEST_ASSERT_EQUAL_INT(MOON_NEW, info.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.0f, (float)info.phase_fraction);
}

void test_first_quarter_2000_jan_14(void) {
    /* ~8 days after reference new moon -> ~0.25 phase fraction */
    double jd = 2451550.26 + 7.38; /* ~first quarter */
    lunar_info_t info = lunar_phase(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.25f, (float)info.phase_fraction);
}

void test_full_moon_2000_jan_21(void) {
    /* ~14.77 days after reference -> ~0.5 phase fraction */
    double jd = 2451550.26 + 14.77;
    lunar_info_t info = lunar_phase(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.5f, (float)info.phase_fraction);
    TEST_ASSERT_EQUAL_INT(MOON_FULL, info.phase);
}

void test_new_moon_2024_apr_08(void) {
    /* 2024-04-08 solar eclipse — known New Moon.
     * Phase fraction wraps near 0/1 boundary; check distance to 0 mod 1. */
    double jd = gregorian_to_jd(2024, 4, 8.75);
    lunar_info_t info = lunar_phase(jd);
    double dist = info.phase_fraction;
    if (dist > 0.5) dist = 1.0 - dist;
    TEST_ASSERT_FLOAT_WITHIN(0.06f, 0.0f, (float)dist);
}

void test_full_moon_2024_mar_25(void) {
    /* 2024-03-25 lunar eclipse — known Full Moon */
    double jd = gregorian_to_jd(2024, 3, 25.3);
    lunar_info_t info = lunar_phase(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.06f, 0.5f, (float)info.phase_fraction);
}

/* --- Illumination tests --- */

void test_new_moon_illumination_zero(void) {
    lunar_info_t info = lunar_phase(2451550.26);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.0f, (float)info.illumination);
}

void test_full_moon_illumination_one(void) {
    double jd = 2451550.26 + 14.77;
    lunar_info_t info = lunar_phase(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, (float)info.illumination);
}

void test_quarter_moon_illumination_half(void) {
    double jd = 2451550.26 + 7.38;
    lunar_info_t info = lunar_phase(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.5f, (float)info.illumination);
}

/* --- Phase fraction tests --- */

void test_phase_fraction_in_range(void) {
    double dates[] = {2451550.26, 2460000.5, 2440000.5, 2470000.5};
    for (int i = 0; i < 4; i++) {
        lunar_info_t info = lunar_phase(dates[i]);
        TEST_ASSERT_TRUE(info.phase_fraction >= 0.0);
        TEST_ASSERT_TRUE(info.phase_fraction < 1.0);
    }
}

void test_phase_fraction_increases_daily(void) {
    double jd = 2451550.26 + 2.0; /* start a bit after new moon */
    double prev = lunar_phase(jd).phase_fraction;
    for (int i = 1; i <= 5; i++) {
        double frac = lunar_phase(jd + (double)i).phase_fraction;
        TEST_ASSERT_TRUE(frac > prev);
        prev = frac;
    }
}

void test_phase_returns_after_synodic_month(void) {
    double jd = 2460000.5;
    double f1 = lunar_phase(jd).phase_fraction;
    double f2 = lunar_phase(jd + 29.53058868).phase_fraction;
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)f1, (float)f2);
}

/* --- Longitude tests --- */

void test_moon_longitude_in_range(void) {
    double dates[] = {2451550.26, 2460000.5, 2440000.5};
    for (int i = 0; i < 3; i++) {
        lunar_info_t info = lunar_phase(dates[i]);
        TEST_ASSERT_TRUE(info.moon_longitude >= 0.0);
        TEST_ASSERT_TRUE(info.moon_longitude < 360.0);
    }
}

void test_moon_moves_about_13_deg_per_day(void) {
    double jd = 2460000.5;
    double lon1 = lunar_phase(jd).moon_longitude;
    double lon2 = lunar_phase(jd + 1.0).moon_longitude;
    double diff = lon2 - lon1;
    if (diff < 0.0) diff += 360.0;
    /* Moon moves ~12-15 deg/day */
    TEST_ASSERT_TRUE(diff > 10.0 && diff < 16.0);
}

/* --- Phase name tests --- */

void test_phase_name_new(void) {
    TEST_ASSERT_EQUAL_STRING("New Moon", lunar_phase_name(MOON_NEW));
}

void test_phase_name_full(void) {
    TEST_ASSERT_EQUAL_STRING("Full Moon", lunar_phase_name(MOON_FULL));
}

void test_all_phase_symbols_non_null(void) {
    for (int i = 0; i < 8; i++) {
        const char *sym = lunar_phase_symbol((moon_phase_t)i);
        TEST_ASSERT_NOT_NULL(sym);
        TEST_ASSERT_TRUE(strlen(sym) > 0);
    }
}

/* --- Next event tests --- */

void test_next_new_moon_in_future(void) {
    double jd = 2460000.5;
    double next = lunar_next_new_moon(jd);
    TEST_ASSERT_TRUE(next > jd);
}

void test_next_full_moon_in_future(void) {
    double jd = 2460000.5;
    double next = lunar_next_full_moon(jd);
    TEST_ASSERT_TRUE(next > jd);
}

void test_next_new_moon_within_30_days(void) {
    double jd = 2460000.5;
    double next = lunar_next_new_moon(jd);
    TEST_ASSERT_TRUE((next - jd) <= 30.0);
}

/* --- Zodiac sign test --- */

void test_lunar_zodiac_sign_in_range(void) {
    double dates[] = {2451550.26, 2460000.5, 2440000.5};
    for (int i = 0; i < 3; i++) {
        int sign = lunar_zodiac_sign(dates[i]);
        TEST_ASSERT_TRUE(sign >= 0 && sign <= 11);
    }
}

/* --- Purity test --- */

void test_purity_same_input_same_output(void) {
    lunar_info_t a = lunar_phase(2460000.5);
    lunar_info_t b = lunar_phase(2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, (float)a.phase_fraction, (float)b.phase_fraction);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, (float)a.illumination, (float)b.illumination);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, (float)a.moon_longitude, (float)b.moon_longitude);
    TEST_ASSERT_EQUAL_INT(a.phase, b.phase);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Known dates */
    RUN_TEST(test_reference_new_moon_2000_jan_06);
    RUN_TEST(test_first_quarter_2000_jan_14);
    RUN_TEST(test_full_moon_2000_jan_21);
    RUN_TEST(test_new_moon_2024_apr_08);
    RUN_TEST(test_full_moon_2024_mar_25);

    /* Illumination */
    RUN_TEST(test_new_moon_illumination_zero);
    RUN_TEST(test_full_moon_illumination_one);
    RUN_TEST(test_quarter_moon_illumination_half);

    /* Phase fraction */
    RUN_TEST(test_phase_fraction_in_range);
    RUN_TEST(test_phase_fraction_increases_daily);
    RUN_TEST(test_phase_returns_after_synodic_month);

    /* Longitude */
    RUN_TEST(test_moon_longitude_in_range);
    RUN_TEST(test_moon_moves_about_13_deg_per_day);

    /* Names */
    RUN_TEST(test_phase_name_new);
    RUN_TEST(test_phase_name_full);
    RUN_TEST(test_all_phase_symbols_non_null);

    /* Next events */
    RUN_TEST(test_next_new_moon_in_future);
    RUN_TEST(test_next_full_moon_in_future);
    RUN_TEST(test_next_new_moon_within_30_days);

    /* Zodiac */
    RUN_TEST(test_lunar_zodiac_sign_in_range);

    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);

    return UNITY_END();
}
