/* test_prayer_times.c — Islamic prayer times from solar position
 * TDD: tests written first, then implementation.
 * Algorithm: hour angle formula from solar declination + location */

#include "../../unity/unity.h"
#include "../../../src/systems/islamic/prayer_times.h"
#include "../../../src/math/julian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Method name tests ===== */

void test_method_name_mwl(void)
{
    TEST_ASSERT_EQUAL_STRING("Muslim World League", prayer_method_name(PRAYER_METHOD_MWL));
}

void test_method_name_isna(void)
{
    TEST_ASSERT_EQUAL_STRING("ISNA", prayer_method_name(PRAYER_METHOD_ISNA));
}

void test_method_name_egypt(void)
{
    TEST_ASSERT_EQUAL_STRING("Egyptian General Authority", prayer_method_name(PRAYER_METHOD_EGYPT));
}

void test_method_name_makkah(void)
{
    TEST_ASSERT_EQUAL_STRING("Umm al-Qura", prayer_method_name(PRAYER_METHOD_MAKKAH));
}

void test_method_name_karachi(void)
{
    TEST_ASSERT_EQUAL_STRING("University of Islamic Sciences, Karachi", prayer_method_name(PRAYER_METHOD_KARACHI));
}

void test_method_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", prayer_method_name(PRAYER_METHOD_COUNT));
    TEST_ASSERT_EQUAL_STRING("Unknown", prayer_method_name((prayer_method_t)99));
}

/* ===== Fajr angle tests ===== */

void test_fajr_angle_mwl(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 18.0f, (float)prayer_method_fajr_angle(PRAYER_METHOD_MWL));
}

void test_fajr_angle_isna(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 15.0f, (float)prayer_method_fajr_angle(PRAYER_METHOD_ISNA));
}

void test_fajr_angle_egypt(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 19.5f, (float)prayer_method_fajr_angle(PRAYER_METHOD_EGYPT));
}

void test_fajr_angle_makkah(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 18.5f, (float)prayer_method_fajr_angle(PRAYER_METHOD_MAKKAH));
}

void test_fajr_angle_karachi(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 18.0f, (float)prayer_method_fajr_angle(PRAYER_METHOD_KARACHI));
}

/* ===== Isha angle tests ===== */

void test_isha_angle_mwl(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 17.0f, (float)prayer_method_isha_angle(PRAYER_METHOD_MWL));
}

void test_isha_angle_isna(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 15.0f, (float)prayer_method_isha_angle(PRAYER_METHOD_ISNA));
}

void test_isha_angle_makkah(void)
{
    /* Makkah method uses fixed 90 minutes, no angle */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)prayer_method_isha_angle(PRAYER_METHOD_MAKKAH));
}

void test_isha_angle_egypt(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 17.5f, (float)prayer_method_isha_angle(PRAYER_METHOD_EGYPT));
}

void test_isha_angle_karachi(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 18.0f, (float)prayer_method_isha_angle(PRAYER_METHOD_KARACHI));
}

/* ===== Compute: Mecca, MWL, vernal equinox 2025 ===== */

/* JD for 2025-03-21 noon UT (near vernal equinox) */
static double mecca_jd(void)
{
    return gregorian_to_jd(2025, 3, 21.5);
}

/* Mecca: 21.4225N, 39.8262E */
#define MECCA_LAT 21.4225
#define MECCA_LON 39.8262

void test_mecca_mwl_ordering(void)
{
    /* Fundamental invariant: fajr < sunrise < dhuhr < asr < maghrib < isha */
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    TEST_ASSERT_TRUE(p.fajr < p.sunrise);
    TEST_ASSERT_TRUE(p.sunrise < p.dhuhr);
    TEST_ASSERT_TRUE(p.dhuhr < p.asr);
    TEST_ASSERT_TRUE(p.asr < p.maghrib);
    TEST_ASSERT_TRUE(p.maghrib < p.isha);
}

void test_mecca_dhuhr_near_solar_noon(void)
{
    /* Dhuhr should be very close to solar noon (within a few minutes) */
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    /* Dhuhr = solar noon + 65 seconds. Check it's within 20 minutes of crude noon estimate. */
    double noon_approx = floor(mecca_jd() - 0.5) + 0.5 - MECCA_LON / 360.0;
    /* 20 minutes tolerance in JD */
    TEST_ASSERT_FLOAT_WITHIN(20.0f / 1440.0f, (float)noon_approx, (float)p.dhuhr);
}

void test_mecca_fajr_before_sunrise(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    /* Fajr should be roughly 1-2 hours before sunrise at low latitudes */
    double diff_hours = (p.sunrise - p.fajr) * 24.0;
    TEST_ASSERT_TRUE(diff_hours > 0.5);
    TEST_ASSERT_TRUE(diff_hours < 3.0);
}

void test_mecca_maghrib_near_sunset(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    /* Daylight should be 10-16 hours at equinox near Mecca */
    double day_length_hours = (p.maghrib - p.sunrise) * 24.0;
    TEST_ASSERT_TRUE(day_length_hours > 10.0);
    TEST_ASSERT_TRUE(day_length_hours < 16.0);
}

void test_mecca_isha_after_maghrib(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    /* Isha should be roughly 1-2 hours after Maghrib at equinox, low lat */
    double diff_hours = (p.isha - p.maghrib) * 24.0;
    TEST_ASSERT_TRUE(diff_hours > 0.5);
    TEST_ASSERT_TRUE(diff_hours < 3.0);
}

/* ===== London, ISNA ===== */

#define LONDON_LAT 51.5
#define LONDON_LON 0.0

void test_london_isna_ordering(void)
{
    double jd = gregorian_to_jd(2025, 6, 15.5); /* Summer */
    prayer_times_t p = prayer_times_compute(jd, LONDON_LAT, LONDON_LON,
                                             PRAYER_METHOD_ISNA, ASR_SHAFII);
    TEST_ASSERT_TRUE(p.fajr < p.sunrise);
    TEST_ASSERT_TRUE(p.sunrise < p.dhuhr);
    TEST_ASSERT_TRUE(p.dhuhr < p.asr);
    TEST_ASSERT_TRUE(p.asr < p.maghrib);
    TEST_ASSERT_TRUE(p.maghrib < p.isha);
}

/* ===== Cairo, Egyptian method ===== */

#define CAIRO_LAT 30.0
#define CAIRO_LON 31.2

void test_cairo_egypt_ordering(void)
{
    double jd = gregorian_to_jd(2025, 1, 15.5); /* Winter */
    prayer_times_t p = prayer_times_compute(jd, CAIRO_LAT, CAIRO_LON,
                                             PRAYER_METHOD_EGYPT, ASR_SHAFII);
    TEST_ASSERT_TRUE(p.fajr < p.sunrise);
    TEST_ASSERT_TRUE(p.sunrise < p.dhuhr);
    TEST_ASSERT_TRUE(p.dhuhr < p.asr);
    TEST_ASSERT_TRUE(p.asr < p.maghrib);
    TEST_ASSERT_TRUE(p.maghrib < p.isha);
}

/* ===== Hanafi Asr is later than Shafi'i Asr ===== */

void test_hanafi_asr_later_than_shafii(void)
{
    double jd = mecca_jd();
    prayer_times_t shafii = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                                  PRAYER_METHOD_MWL, ASR_SHAFII);
    prayer_times_t hanafi = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                                  PRAYER_METHOD_MWL, ASR_HANAFI);
    TEST_ASSERT_TRUE(hanafi.asr > shafii.asr);
}

/* ===== Makkah method Isha = Maghrib + 90 minutes ===== */

void test_makkah_isha_90_minutes(void)
{
    double jd = mecca_jd();
    prayer_times_t p = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MAKKAH, ASR_SHAFII);
    double diff_min = (p.isha - p.maghrib) * 1440.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 90.0f, (float)diff_min);
}

/* ===== All times are valid JDs (not NaN, not 0) ===== */

void test_all_times_valid(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    TEST_ASSERT_FALSE(isnan(p.fajr));
    TEST_ASSERT_FALSE(isnan(p.sunrise));
    TEST_ASSERT_FALSE(isnan(p.dhuhr));
    TEST_ASSERT_FALSE(isnan(p.asr));
    TEST_ASSERT_FALSE(isnan(p.maghrib));
    TEST_ASSERT_FALSE(isnan(p.isha));

    TEST_ASSERT_TRUE(p.fajr > 0.0);
    TEST_ASSERT_TRUE(p.sunrise > 0.0);
    TEST_ASSERT_TRUE(p.dhuhr > 0.0);
    TEST_ASSERT_TRUE(p.asr > 0.0);
    TEST_ASSERT_TRUE(p.maghrib > 0.0);
    TEST_ASSERT_TRUE(p.isha > 0.0);
}

/* ===== prayer_next tests ===== */

void test_next_before_fajr(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double before_fajr = p.fajr - 1.0 / 24.0;
    next_prayer_t np = prayer_next(p, before_fajr);
    TEST_ASSERT_EQUAL_STRING("Fajr", np.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)p.fajr, (float)np.jd);
}

void test_next_between_dhuhr_and_asr(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double mid = (p.dhuhr + p.asr) / 2.0;
    next_prayer_t np = prayer_next(p, mid);
    TEST_ASSERT_EQUAL_STRING("Asr", np.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)p.asr, (float)np.jd);
}

void test_next_after_isha(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double after_isha = p.isha + 1.0 / 24.0;
    next_prayer_t np = prayer_next(p, after_isha);
    /* After Isha, next prayer is Fajr (next day indication) */
    TEST_ASSERT_EQUAL_STRING("Fajr", np.name);
    /* JD should be 0 (no next-day info in single-day struct) */
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)np.jd);
}

void test_next_between_sunrise_and_dhuhr(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double mid = (p.sunrise + p.dhuhr) / 2.0;
    next_prayer_t np = prayer_next(p, mid);
    TEST_ASSERT_EQUAL_STRING("Dhuhr", np.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)p.dhuhr, (float)np.jd);
}

void test_next_between_fajr_and_sunrise(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double mid = (p.fajr + p.sunrise) / 2.0;
    next_prayer_t np = prayer_next(p, mid);
    TEST_ASSERT_EQUAL_STRING("Sunrise", np.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)p.sunrise, (float)np.jd);
}

void test_next_between_asr_and_maghrib(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double mid = (p.asr + p.maghrib) / 2.0;
    next_prayer_t np = prayer_next(p, mid);
    TEST_ASSERT_EQUAL_STRING("Maghrib", np.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)p.maghrib, (float)np.jd);
}

void test_next_between_maghrib_and_isha(void)
{
    prayer_times_t p = prayer_times_compute(mecca_jd(), MECCA_LAT, MECCA_LON,
                                             PRAYER_METHOD_MWL, ASR_SHAFII);
    double mid = (p.maghrib + p.isha) / 2.0;
    next_prayer_t np = prayer_next(p, mid);
    TEST_ASSERT_EQUAL_STRING("Isha", np.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)p.isha, (float)np.jd);
}

/* ===== Purity test ===== */

void test_purity_same_inputs_same_outputs(void)
{
    double jd = mecca_jd();
    prayer_times_t p1 = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                              PRAYER_METHOD_MWL, ASR_SHAFII);
    prayer_times_t p2 = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                              PRAYER_METHOD_MWL, ASR_SHAFII);
    TEST_ASSERT_FLOAT_WITHIN(0.0f, (float)p1.fajr, (float)p2.fajr);
    TEST_ASSERT_FLOAT_WITHIN(0.0f, (float)p1.sunrise, (float)p2.sunrise);
    TEST_ASSERT_FLOAT_WITHIN(0.0f, (float)p1.dhuhr, (float)p2.dhuhr);
    TEST_ASSERT_FLOAT_WITHIN(0.0f, (float)p1.asr, (float)p2.asr);
    TEST_ASSERT_FLOAT_WITHIN(0.0f, (float)p1.maghrib, (float)p2.maghrib);
    TEST_ASSERT_FLOAT_WITHIN(0.0f, (float)p1.isha, (float)p2.isha);
}

/* ===== Different methods produce different Fajr times ===== */

void test_different_methods_different_fajr(void)
{
    double jd = mecca_jd();
    prayer_times_t mwl = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                               PRAYER_METHOD_MWL, ASR_SHAFII);
    prayer_times_t isna = prayer_times_compute(jd, MECCA_LAT, MECCA_LON,
                                                PRAYER_METHOD_ISNA, ASR_SHAFII);
    /* MWL Fajr=18, ISNA Fajr=15. Larger angle => earlier Fajr */
    TEST_ASSERT_TRUE(mwl.fajr < isna.fajr);
}

/* ===== Additional coverage: Karachi method ordering ===== */

void test_karachi_ordering(void)
{
    double jd = gregorian_to_jd(2025, 9, 1.5);
    prayer_times_t p = prayer_times_compute(jd, 24.8607, 67.0011,
                                             PRAYER_METHOD_KARACHI, ASR_HANAFI);
    TEST_ASSERT_TRUE(p.fajr < p.sunrise);
    TEST_ASSERT_TRUE(p.sunrise < p.dhuhr);
    TEST_ASSERT_TRUE(p.dhuhr < p.asr);
    TEST_ASSERT_TRUE(p.asr < p.maghrib);
    TEST_ASSERT_TRUE(p.maghrib < p.isha);
}

/* ===== Invalid method returns 0.0 for angles ===== */

void test_fajr_angle_invalid(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)prayer_method_fajr_angle(PRAYER_METHOD_COUNT));
}

void test_isha_angle_invalid(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)prayer_method_isha_angle(PRAYER_METHOD_COUNT));
}

int main(void)
{
    UNITY_BEGIN();

    /* Method names (5 + 1 invalid = 6 tests) */
    RUN_TEST(test_method_name_mwl);
    RUN_TEST(test_method_name_isna);
    RUN_TEST(test_method_name_egypt);
    RUN_TEST(test_method_name_makkah);
    RUN_TEST(test_method_name_karachi);
    RUN_TEST(test_method_name_invalid);

    /* Fajr angles (5 + 1 invalid = 6 tests) */
    RUN_TEST(test_fajr_angle_mwl);
    RUN_TEST(test_fajr_angle_isna);
    RUN_TEST(test_fajr_angle_egypt);
    RUN_TEST(test_fajr_angle_makkah);
    RUN_TEST(test_fajr_angle_karachi);
    RUN_TEST(test_fajr_angle_invalid);

    /* Isha angles (5 + 1 invalid = 6 tests) */
    RUN_TEST(test_isha_angle_mwl);
    RUN_TEST(test_isha_angle_isna);
    RUN_TEST(test_isha_angle_makkah);
    RUN_TEST(test_isha_angle_egypt);
    RUN_TEST(test_isha_angle_karachi);
    RUN_TEST(test_isha_angle_invalid);

    /* Compute — Mecca (5 tests) */
    RUN_TEST(test_mecca_mwl_ordering);
    RUN_TEST(test_mecca_dhuhr_near_solar_noon);
    RUN_TEST(test_mecca_fajr_before_sunrise);
    RUN_TEST(test_mecca_maghrib_near_sunset);
    RUN_TEST(test_mecca_isha_after_maghrib);

    /* Compute — London ISNA (1 test) */
    RUN_TEST(test_london_isna_ordering);

    /* Compute — Cairo Egypt (1 test) */
    RUN_TEST(test_cairo_egypt_ordering);

    /* Asr jurisprudence (1 test) */
    RUN_TEST(test_hanafi_asr_later_than_shafii);

    /* Makkah method isha (1 test) */
    RUN_TEST(test_makkah_isha_90_minutes);

    /* Valid JDs (1 test) */
    RUN_TEST(test_all_times_valid);

    /* prayer_next (7 tests) */
    RUN_TEST(test_next_before_fajr);
    RUN_TEST(test_next_between_fajr_and_sunrise);
    RUN_TEST(test_next_between_sunrise_and_dhuhr);
    RUN_TEST(test_next_between_dhuhr_and_asr);
    RUN_TEST(test_next_between_asr_and_maghrib);
    RUN_TEST(test_next_between_maghrib_and_isha);
    RUN_TEST(test_next_after_isha);

    /* Purity (1 test) */
    RUN_TEST(test_purity_same_inputs_same_outputs);

    /* Different methods (1 test) */
    RUN_TEST(test_different_methods_different_fajr);

    /* Karachi method ordering (1 test) */
    RUN_TEST(test_karachi_ordering);

    return UNITY_END();
}
