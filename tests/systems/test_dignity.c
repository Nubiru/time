#include "../unity/unity.h"
#include "../../src/systems/astrology/dignity.h"

void setUp(void) { }
void tearDown(void) { }

/* ===== Domicile Tests ===== */

void test_mars_in_aries_domicile(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MARS, 0);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, r.dignity);
    TEST_ASSERT_EQUAL_INT(5, r.score);
}

void test_venus_in_taurus_domicile(void) {
    dignity_result_t r = planet_dignity(DIGNITY_VENUS, 1);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, r.dignity);
    TEST_ASSERT_EQUAL_INT(5, r.score);
}

void test_moon_in_cancer_domicile(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MOON, 3);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, r.dignity);
    TEST_ASSERT_EQUAL_INT(5, r.score);
}

void test_sun_in_leo_domicile(void) {
    dignity_result_t r = planet_dignity(DIGNITY_SUN, 4);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, r.dignity);
    TEST_ASSERT_EQUAL_INT(5, r.score);
}

void test_mercury_in_gemini_domicile(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MERCURY, 2);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, r.dignity);
    TEST_ASSERT_EQUAL_INT(5, r.score);
}

void test_jupiter_in_pisces_domicile(void) {
    dignity_result_t r = planet_dignity(DIGNITY_JUPITER, 11);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, r.dignity);
    TEST_ASSERT_EQUAL_INT(5, r.score);
}

/* ===== Exaltation Tests ===== */

void test_sun_in_aries_exaltation(void) {
    dignity_result_t r = planet_dignity(DIGNITY_SUN, 0);
    TEST_ASSERT_EQUAL_INT(DIGNITY_EXALTATION, r.dignity);
    TEST_ASSERT_EQUAL_INT(4, r.score);
}

void test_moon_in_taurus_exaltation(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MOON, 1);
    TEST_ASSERT_EQUAL_INT(DIGNITY_EXALTATION, r.dignity);
    TEST_ASSERT_EQUAL_INT(4, r.score);
}

void test_saturn_in_libra_exaltation(void) {
    dignity_result_t r = planet_dignity(DIGNITY_SATURN, 6);
    TEST_ASSERT_EQUAL_INT(DIGNITY_EXALTATION, r.dignity);
    TEST_ASSERT_EQUAL_INT(4, r.score);
}

/* ===== Detriment Tests ===== */

void test_mars_in_libra_detriment(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MARS, 6);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DETRIMENT, r.dignity);
    TEST_ASSERT_EQUAL_INT(-5, r.score);
}

void test_sun_in_aquarius_detriment(void) {
    dignity_result_t r = planet_dignity(DIGNITY_SUN, 10);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DETRIMENT, r.dignity);
    TEST_ASSERT_EQUAL_INT(-5, r.score);
}

/* ===== Fall Tests ===== */

void test_sun_in_libra_fall(void) {
    dignity_result_t r = planet_dignity(DIGNITY_SUN, 6);
    /* Sun exalted in Aries(0), fall in opposite = Libra(6).
     * But Saturn is exalted in Libra, and Venus rules Libra.
     * Sun is neither ruler nor exalted in Libra, check detriment:
     * Libra opposite = Aries, ruler of Aries = Mars, not Sun.
     * Check fall: Libra opposite = Aries, exaltation of Aries = Sun. YES — Sun in fall. */
    TEST_ASSERT_EQUAL_INT(DIGNITY_FALL, r.dignity);
    TEST_ASSERT_EQUAL_INT(-4, r.score);
}

void test_moon_in_scorpio_fall(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MOON, 7);
    /* Moon exalted in Taurus(1), fall in opposite = Scorpio(7). */
    TEST_ASSERT_EQUAL_INT(DIGNITY_FALL, r.dignity);
    TEST_ASSERT_EQUAL_INT(-4, r.score);
}

/* ===== Peregrine Tests ===== */

void test_mars_in_gemini_peregrine(void) {
    dignity_result_t r = planet_dignity(DIGNITY_MARS, 2);
    TEST_ASSERT_EQUAL_INT(DIGNITY_PEREGRINE, r.dignity);
    TEST_ASSERT_EQUAL_INT(0, r.score);
}

void test_earth_always_peregrine(void) {
    for (int sign = 0; sign < 12; sign++) {
        dignity_result_t r = planet_dignity(DIGNITY_EARTH, sign);
        TEST_ASSERT_EQUAL_INT(DIGNITY_PEREGRINE, r.dignity);
        TEST_ASSERT_EQUAL_INT(0, r.score);
    }
}

/* ===== Lookup Tests ===== */

void test_sign_ruler_aries(void) {
    TEST_ASSERT_EQUAL_INT(3, sign_ruler(0));  /* Mars */
}

void test_sign_ruler_leo(void) {
    TEST_ASSERT_EQUAL_INT(8, sign_ruler(4));  /* Sun */
}

void test_sign_exaltation_ruler_gemini_none(void) {
    TEST_ASSERT_EQUAL_INT(-1, sign_exaltation_ruler(2));
}

void test_sign_exaltation_ruler_aries(void) {
    TEST_ASSERT_EQUAL_INT(8, sign_exaltation_ruler(0));  /* Sun */
}

void test_planet_domicile_sign_mercury(void) {
    TEST_ASSERT_EQUAL_INT(2, planet_domicile_sign(0));  /* Gemini */
}

void test_planet_domicile_sign_earth_none(void) {
    TEST_ASSERT_EQUAL_INT(-1, planet_domicile_sign(2));
}

/* ===== Name Tests ===== */

void test_dignity_name_domicile(void) {
    TEST_ASSERT_EQUAL_STRING("Domicile", dignity_name(DIGNITY_DOMICILE));
}

void test_dignity_name_fall(void) {
    TEST_ASSERT_EQUAL_STRING("Fall", dignity_name(DIGNITY_FALL));
}

void test_dignity_name_all(void) {
    TEST_ASSERT_EQUAL_STRING("Exaltation", dignity_name(DIGNITY_EXALTATION));
    TEST_ASSERT_EQUAL_STRING("Peregrine", dignity_name(DIGNITY_PEREGRINE));
    TEST_ASSERT_EQUAL_STRING("Detriment", dignity_name(DIGNITY_DETRIMENT));
}

/* ===== Batch Test ===== */

void test_dignity_score_total_all_domicile(void) {
    /* Place each planet in its domicile sign */
    int signs[10] = {
        2,  /* Mercury in Gemini */
        1,  /* Venus in Taurus */
        0,  /* Earth in Aries (peregrine = 0) */
        0,  /* Mars in Aries */
        8,  /* Jupiter in Sagittarius */
        9,  /* Saturn in Capricorn */
        0,  /* Uranus in Aries (peregrine = 0) */
        0,  /* Neptune in Aries (peregrine = 0) */
        4,  /* Sun in Leo */
        3   /* Moon in Cancer */
    };
    int total = dignity_score_total(signs);
    /* 7 planets in domicile * 5 + 3 peregrine * 0 = 35 */
    TEST_ASSERT_EQUAL_INT(35, total);
}

/* ===== Purity Test ===== */

void test_purity_same_input_same_output(void) {
    dignity_result_t r1 = planet_dignity(DIGNITY_MARS, 0);
    dignity_result_t r2 = planet_dignity(DIGNITY_MARS, 0);
    TEST_ASSERT_EQUAL_INT(r1.dignity, r2.dignity);
    TEST_ASSERT_EQUAL_INT(r1.score, r2.score);
    TEST_ASSERT_EQUAL_INT(r1.planet, r2.planet);
    TEST_ASSERT_EQUAL_INT(r1.sign, r2.sign);
}

int main(void) {
    UNITY_BEGIN();
    /* Domicile */
    RUN_TEST(test_mars_in_aries_domicile);
    RUN_TEST(test_venus_in_taurus_domicile);
    RUN_TEST(test_moon_in_cancer_domicile);
    RUN_TEST(test_sun_in_leo_domicile);
    RUN_TEST(test_mercury_in_gemini_domicile);
    RUN_TEST(test_jupiter_in_pisces_domicile);
    /* Exaltation */
    RUN_TEST(test_sun_in_aries_exaltation);
    RUN_TEST(test_moon_in_taurus_exaltation);
    RUN_TEST(test_saturn_in_libra_exaltation);
    /* Detriment */
    RUN_TEST(test_mars_in_libra_detriment);
    RUN_TEST(test_sun_in_aquarius_detriment);
    /* Fall */
    RUN_TEST(test_sun_in_libra_fall);
    RUN_TEST(test_moon_in_scorpio_fall);
    /* Peregrine */
    RUN_TEST(test_mars_in_gemini_peregrine);
    RUN_TEST(test_earth_always_peregrine);
    /* Lookup */
    RUN_TEST(test_sign_ruler_aries);
    RUN_TEST(test_sign_ruler_leo);
    RUN_TEST(test_sign_exaltation_ruler_gemini_none);
    RUN_TEST(test_sign_exaltation_ruler_aries);
    RUN_TEST(test_planet_domicile_sign_mercury);
    RUN_TEST(test_planet_domicile_sign_earth_none);
    /* Names */
    RUN_TEST(test_dignity_name_domicile);
    RUN_TEST(test_dignity_name_fall);
    RUN_TEST(test_dignity_name_all);
    /* Batch */
    RUN_TEST(test_dignity_score_total_all_domicile);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
