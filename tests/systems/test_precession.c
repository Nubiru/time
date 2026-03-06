#include "../unity/unity.h"
#include "../../src/systems/astronomy/precession.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Epoch reference: 1-3 */

void test_centuries_at_j2000(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10f, 0.0f, (float)prec_centuries_from_j2000(PREC_J2000_JD));
}

void test_centuries_one_century(void)
{
    double jd_j2100 = PREC_J2000_JD + 36525.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.0f, (float)prec_centuries_from_j2000(jd_j2100));
}

void test_accumulated_at_j2000(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10f, 0.0f, (float)prec_accumulated(PREC_J2000_JD));
}

/* Accumulated precession: 4-6 */

void test_accumulated_at_j2100(void)
{
    double jd_j2100 = PREC_J2000_JD + 36525.0;
    double acc = prec_accumulated(jd_j2100);
    /* 5028.796195"/century + small corrections -> ~1.3969 degrees */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.3969f, (float)acc);
}

void test_accumulated_at_j1900(void)
{
    double jd_j1900 = PREC_J2000_JD - 36525.0;
    double acc = prec_accumulated(jd_j1900);
    /* Approximately -1.3969 degrees (symmetric with small quadratic correction) */
    TEST_ASSERT_FLOAT_WITHIN(0.02f, -1.3969f, (float)acc);
}

void test_accumulated_monotonic(void)
{
    double a1 = prec_accumulated(PREC_J2000_JD);
    double a2 = prec_accumulated(PREC_J2000_JD + 36525.0);
    double a3 = prec_accumulated(PREC_J2000_JD + 2.0 * 36525.0);
    TEST_ASSERT_TRUE(a2 > a1);
    TEST_ASSERT_TRUE(a3 > a2);
}

/* Rate: 7-8 */

void test_rate_at_j2000(void)
{
    double rate = prec_rate(PREC_J2000_JD);
    /* 5028.796195 / 100 = 50.28796195 arcsec/year */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 50.288f, (float)rate);
}

void test_rate_varies(void)
{
    double r1 = prec_rate(PREC_J2000_JD);
    double r2 = prec_rate(PREC_J2000_JD + 36525.0);
    /* Rate changes slightly due to quadratic term */
    TEST_ASSERT_TRUE(fabs(r1 - r2) > 0.001);
}

/* Ayanamsa: 9-11 */

void test_ayanamsa_at_j2000(void)
{
    double ayan = prec_ayanamsa_lahiri(PREC_J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 23.853f, (float)ayan);
}

void test_ayanamsa_increases(void)
{
    double a1 = prec_ayanamsa_lahiri(PREC_J2000_JD);
    double a2 = prec_ayanamsa_lahiri(PREC_J2000_JD + 365.25 * 100);
    TEST_ASSERT_TRUE(a2 > a1);
}

void test_ayanamsa_j2100_greater(void)
{
    double a_j2000 = prec_ayanamsa_lahiri(PREC_J2000_JD);
    double a_j2100 = prec_ayanamsa_lahiri(PREC_J2000_JD + 36525.0);
    TEST_ASSERT_TRUE(a_j2100 > a_j2000);
    /* Should be ~23.853 + 1.397 = ~25.25 */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 25.25f, (float)a_j2100);
}

/* Tropical-sidereal conversion: 12-14 */

void test_tropical_to_sidereal_at_j2000(void)
{
    double sid = prec_tropical_to_sidereal(30.0, PREC_J2000_JD);
    /* 30 - 23.853 = ~6.147 */
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 6.15f, (float)sid);
}

void test_round_trip(void)
{
    double jd = PREC_J2000_JD + 10000.0;
    double tropical = 123.456;
    double sid = prec_tropical_to_sidereal(tropical, jd);
    double back = prec_sidereal_to_tropical(sid, jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)tropical, (float)back);
}

void test_tropical_zero_to_sidereal(void)
{
    double sid = prec_tropical_to_sidereal(0.0, PREC_J2000_JD);
    /* 0 - 23.853 = -23.853 -> normalized to ~336.147 */
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 336.15f, (float)sid);
}

/* Period: 15 */

void test_cycle_period(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 25772.0f, (float)prec_cycle_period());
}

/* Astrological ages: 16-17 */

void test_current_age_pisces(void)
{
    /* JD ~2460000 (around 2023): ayanamsa ~24.2, 24.2/30 = 0 -> Pisces */
    TEST_ASSERT_EQUAL_INT(0, prec_current_age(2460000.0));
}

void test_all_age_names(void)
{
    for (int i = 0; i < 12; i++) {
        const char *name = prec_age_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
    TEST_ASSERT_EQUAL_STRING("?", prec_age_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", prec_age_name(12));
}

/* Normalization: 18-19 */

void test_sidereal_in_range(void)
{
    /* Large tropical longitude -> sidereal should still be 0-360 */
    double sid = prec_tropical_to_sidereal(359.0, PREC_J2000_JD);
    TEST_ASSERT_TRUE(sid >= 0.0);
    TEST_ASSERT_TRUE(sid < 360.0);

    sid = prec_tropical_to_sidereal(0.0, PREC_J2000_JD);
    TEST_ASSERT_TRUE(sid >= 0.0);
    TEST_ASSERT_TRUE(sid < 360.0);
}

void test_large_negative_jd(void)
{
    /* Very distant past — should not crash */
    double acc = prec_accumulated(0.0);
    /* JD 0 is ~4713 BC, T ~ -67 centuries */
    TEST_ASSERT_TRUE(acc < 0.0); /* far in past = negative precession */
    TEST_ASSERT_TRUE(isfinite(acc));
}

/* Purity: 20 */

void test_purity(void)
{
    double a = prec_accumulated(2460000.0);
    double b = prec_accumulated(2460000.0);
    TEST_ASSERT_EQUAL_FLOAT((float)a, (float)b);

    double r1 = prec_ayanamsa_lahiri(2460000.0);
    double r2 = prec_ayanamsa_lahiri(2460000.0);
    TEST_ASSERT_EQUAL_FLOAT((float)r1, (float)r2);
}

/* Extra tests */

void test_age_name_specific(void)
{
    TEST_ASSERT_EQUAL_STRING("Pisces", prec_age_name(0));
    TEST_ASSERT_EQUAL_STRING("Aquarius", prec_age_name(1));
    TEST_ASSERT_EQUAL_STRING("Aries", prec_age_name(11));
}

void test_rate_positive(void)
{
    /* Precession rate should always be positive for reasonable epochs */
    TEST_ASSERT_TRUE(prec_rate(PREC_J2000_JD) > 0.0);
    TEST_ASSERT_TRUE(prec_rate(PREC_J2000_JD + 36525.0) > 0.0);
    TEST_ASSERT_TRUE(prec_rate(PREC_J2000_JD - 36525.0) > 0.0);
}

void test_sidereal_tropical_difference(void)
{
    /* Sidereal longitude should be less than tropical (ayanamsa subtracted) */
    double jd = PREC_J2000_JD;
    double tropical = 100.0;
    double sidereal = prec_tropical_to_sidereal(tropical, jd);
    double diff = tropical - sidereal;
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 23.85f, (float)diff);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_centuries_at_j2000);
    RUN_TEST(test_centuries_one_century);
    RUN_TEST(test_accumulated_at_j2000);
    RUN_TEST(test_accumulated_at_j2100);
    RUN_TEST(test_accumulated_at_j1900);
    RUN_TEST(test_accumulated_monotonic);
    RUN_TEST(test_rate_at_j2000);
    RUN_TEST(test_rate_varies);
    RUN_TEST(test_ayanamsa_at_j2000);
    RUN_TEST(test_ayanamsa_increases);
    RUN_TEST(test_ayanamsa_j2100_greater);
    RUN_TEST(test_tropical_to_sidereal_at_j2000);
    RUN_TEST(test_round_trip);
    RUN_TEST(test_tropical_zero_to_sidereal);
    RUN_TEST(test_cycle_period);
    RUN_TEST(test_current_age_pisces);
    RUN_TEST(test_all_age_names);
    RUN_TEST(test_sidereal_in_range);
    RUN_TEST(test_large_negative_jd);
    RUN_TEST(test_purity);
    RUN_TEST(test_age_name_specific);
    RUN_TEST(test_rate_positive);
    RUN_TEST(test_sidereal_tropical_difference);
    return UNITY_END();
}
