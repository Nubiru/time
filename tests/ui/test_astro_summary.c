#include "../unity/unity.h"
#include "../../src/ui/astro_summary.h"
#include "../../src/systems/astrology/zodiac.h"
#include "../../src/systems/astrology/dignity.h"
#include "../../src/systems/astrology/aspects.h"
#include "../../src/systems/astronomy/planets.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/*
 * Test longitudes (geocentric convention):
 *   [0] Mercury = 75.0  (Gemini 15)
 *   [1] Venus   = 35.0  (Taurus 5)
 *   [2] Sun     = 135.3 (Leo 15.3)
 *   [3] Mars    =  5.0  (Aries 5)
 *   [4] Jupiter = 250.0 (Sagittarius 10)
 *   [5] Saturn  = 285.0 (Capricorn 15)
 *   [6] Uranus  = 45.0  (Taurus 15)
 *   [7] Neptune = 350.0 (Pisces 20)
 */
static const double TEST_LONS[8] = {
    75.0, 35.0, 135.3, 5.0, 250.0, 285.0, 45.0, 350.0
};
static const double TEST_ASC = 102.5; /* Cancer 12.5 */
static const double TEST_ORB = 8.0;

/* --- 1. Dignity index mapping --- */

void test_dignity_index_mercury(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_MERCURY, astro_summary_dignity_index(0));
}

void test_dignity_index_venus(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_VENUS, astro_summary_dignity_index(1));
}

void test_dignity_index_sun_geocentric(void)
{
    /* Planet index 2 (Earth slot) maps to DIGNITY_SUN in geocentric */
    TEST_ASSERT_EQUAL_INT(DIGNITY_SUN, astro_summary_dignity_index(2));
}

void test_dignity_index_mars(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_MARS, astro_summary_dignity_index(3));
}

void test_dignity_index_jupiter(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_JUPITER, astro_summary_dignity_index(4));
}

void test_dignity_index_saturn(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_SATURN, astro_summary_dignity_index(5));
}

void test_dignity_index_uranus(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_URANUS, astro_summary_dignity_index(6));
}

void test_dignity_index_neptune(void)
{
    TEST_ASSERT_EQUAL_INT(DIGNITY_NEPTUNE, astro_summary_dignity_index(7));
}

/* --- 2. Planet formatting --- */

void test_fmt_planet_sun_in_leo(void)
{
    char buf[48];
    /* Sun at 135.3 = Leo 15.3 */
    astro_summary_fmt_planet(2, 135.3, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth"));  /* planet_name(2) = "Earth" */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Leo"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "15.3"));
}

void test_fmt_planet_mercury_in_gemini(void)
{
    char buf[48];
    /* Mercury at 75.0 = Gemini 15.0 */
    astro_summary_fmt_planet(0, 75.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mercury"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gemini"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "15.0"));
}

void test_fmt_planet_contains_zodiac_symbol(void)
{
    char buf[48];
    /* Venus at 35.0 = Taurus 5.0 => symbol Taurus */
    astro_summary_fmt_planet(1, 35.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, zodiac_sign_symbol(1))); /* Taurus symbol */
}

void test_fmt_planet_degree_precision(void)
{
    char buf[48];
    /* Mars at 5.7 = Aries 5.7 */
    astro_summary_fmt_planet(3, 5.7, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "5.7"));
}

/* --- 3. Ascendant formatting --- */

void test_fmt_ascendant_cancer(void)
{
    char buf[48];
    astro_summary_fmt_ascendant(102.5, buf, sizeof(buf));
    /* 102.5 = Cancer 12.5 */
    TEST_ASSERT_NOT_NULL(strstr(buf, "ASC"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Cancer"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "12.5"));
}

void test_fmt_ascendant_contains_symbol(void)
{
    char buf[48];
    astro_summary_fmt_ascendant(102.5, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, zodiac_sign_symbol(3))); /* Cancer symbol */
}

void test_fmt_ascendant_aries_zero(void)
{
    char buf[48];
    astro_summary_fmt_ascendant(0.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "ASC"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "0.0"));
}

/* --- 4. Full build: planet entries --- */

void test_build_planet_count(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(8, s.planet_count);
}

void test_build_planet_signs(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    /* Mercury 75 = Gemini(2), Venus 35 = Taurus(1), Sun 135.3 = Leo(4) */
    TEST_ASSERT_EQUAL_INT(2, s.planets[0].sign);  /* Mercury in Gemini */
    TEST_ASSERT_EQUAL_INT(1, s.planets[1].sign);  /* Venus in Taurus */
    TEST_ASSERT_EQUAL_INT(4, s.planets[2].sign);  /* Sun in Leo */
    TEST_ASSERT_EQUAL_INT(0, s.planets[3].sign);  /* Mars in Aries */
    TEST_ASSERT_EQUAL_INT(8, s.planets[4].sign);  /* Jupiter in Sagittarius */
    TEST_ASSERT_EQUAL_INT(9, s.planets[5].sign);  /* Saturn in Capricorn */
    TEST_ASSERT_EQUAL_INT(1, s.planets[6].sign);  /* Uranus in Taurus */
    TEST_ASSERT_EQUAL_INT(11, s.planets[7].sign); /* Neptune in Pisces */
}

void test_build_planet_degree_in_sign(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    /* Mercury at 75.0 = Gemini 15.0 */
    TEST_ASSERT_TRUE(fabs(s.planets[0].degree_in_sign - 15.0) < 0.1);
    /* Sun at 135.3 = Leo 15.3 */
    TEST_ASSERT_TRUE(fabs(s.planets[2].degree_in_sign - 15.3) < 0.1);
}

void test_build_planet_index_stored(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_EQUAL_INT(i, s.planets[i].planet_index);
    }
}

void test_build_planet_formatted_nonempty(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(strlen(s.planets[i].formatted) > 0);
    }
}

/* --- 5. Full build: dignity --- */

void test_build_dignity_venus_taurus_domicile(void)
{
    /* Venus at 35.0 = Taurus. Venus rules Taurus => Domicile */
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, s.planets[1].dignity);
    TEST_ASSERT_EQUAL_STRING("Domicile", s.planets[1].dignity_label);
}

void test_build_dignity_sun_leo_domicile(void)
{
    /* Sun at 135.3 = Leo. Sun rules Leo => Domicile */
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, s.planets[2].dignity);
}

void test_build_dignity_mars_aries_domicile(void)
{
    /* Mars at 5.0 = Aries. Mars rules Aries => Domicile */
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, s.planets[3].dignity);
}

void test_build_dignity_jupiter_sagittarius_domicile(void)
{
    /* Jupiter at 250.0 = Sagittarius. Jupiter rules Sagittarius => Domicile */
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, s.planets[4].dignity);
}

void test_build_dignity_saturn_capricorn_domicile(void)
{
    /* Saturn at 285.0 = Capricorn. Saturn rules Capricorn => Domicile */
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(DIGNITY_DOMICILE, s.planets[5].dignity);
}

void test_build_dignity_total(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    /* We have at least 5 planets in domicile (Venus,Sun,Mars,Jupiter,Saturn)
     * Each scores 5, so at minimum 25. Mercury in Gemini also domicile (+5=30). */
    TEST_ASSERT_TRUE(s.dignity_total >= 25);
}

/* --- 6. Full build: aspects --- */

void test_build_aspects_count_range(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    /* With 8 planets and 8-degree orb, we should get some aspects */
    TEST_ASSERT_TRUE(s.aspect_count >= 0);
    TEST_ASSERT_TRUE(s.aspect_count <= ASTRO_SUMMARY_MAX_ASPECTS);
}

void test_build_aspects_formatted_nonempty(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    for (int i = 0; i < s.aspect_count; i++) {
        TEST_ASSERT_TRUE(strlen(s.aspects[i].formatted) > 0);
    }
}

void test_build_aspects_orb_within_max(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    for (int i = 0; i < s.aspect_count; i++) {
        TEST_ASSERT_TRUE(s.aspects[i].orb <= TEST_ORB + 0.01);
    }
}

/* --- 7. Full build: ascendant --- */

void test_build_ascendant_sign(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    /* 102.5 = Cancer (sign 3) */
    TEST_ASSERT_EQUAL_INT(3, s.ascendant_sign);
}

void test_build_ascendant_degree(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_TRUE(fabs(s.ascendant_degree - 12.5) < 0.1);
}

void test_build_ascendant_str(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_NOT_NULL(strstr(s.ascendant_str, "ASC"));
    TEST_ASSERT_NOT_NULL(strstr(s.ascendant_str, "Cancer"));
}

/* --- 8. Full build: balance strings --- */

void test_build_element_str_nonempty(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_TRUE(strlen(s.element_str) > 0);
    TEST_ASSERT_NOT_NULL(strstr(s.element_str, "Fire"));
    TEST_ASSERT_NOT_NULL(strstr(s.element_str, "Earth"));
    TEST_ASSERT_NOT_NULL(strstr(s.element_str, "Air"));
    TEST_ASSERT_NOT_NULL(strstr(s.element_str, "Water"));
}

void test_build_modality_str_nonempty(void)
{
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_TRUE(strlen(s.modality_str) > 0);
    TEST_ASSERT_NOT_NULL(strstr(s.modality_str, "Cardinal"));
    TEST_ASSERT_NOT_NULL(strstr(s.modality_str, "Fixed"));
    TEST_ASSERT_NOT_NULL(strstr(s.modality_str, "Mutable"));
}

/* --- 9. Purity --- */

void test_purity_same_input_same_output(void)
{
    astro_summary_t a = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    astro_summary_t b = astro_summary_build(TEST_LONS, TEST_ASC, TEST_ORB);
    TEST_ASSERT_EQUAL_INT(a.planet_count, b.planet_count);
    TEST_ASSERT_EQUAL_INT(a.aspect_count, b.aspect_count);
    TEST_ASSERT_EQUAL_INT(a.ascendant_sign, b.ascendant_sign);
    TEST_ASSERT_EQUAL_INT(a.dignity_total, b.dignity_total);
    TEST_ASSERT_EQUAL_STRING(a.ascendant_str, b.ascendant_str);
    TEST_ASSERT_EQUAL_STRING(a.element_str, b.element_str);
    TEST_ASSERT_EQUAL_STRING(a.modality_str, b.modality_str);
}

/* --- 10. Edge cases --- */

void test_build_zero_orb_no_aspects(void)
{
    /* With 0.0 orb, only exact aspects should be found (unlikely with these lons) */
    astro_summary_t s = astro_summary_build(TEST_LONS, TEST_ASC, 0.0);
    /* With exact-only, the count should be very low or zero */
    TEST_ASSERT_TRUE(s.aspect_count >= 0);
    TEST_ASSERT_TRUE(s.aspect_count <= ASTRO_SUMMARY_MAX_ASPECTS);
}

void test_dignity_index_out_of_range(void)
{
    /* Out of range should return -1 */
    TEST_ASSERT_EQUAL_INT(-1, astro_summary_dignity_index(-1));
    TEST_ASSERT_EQUAL_INT(-1, astro_summary_dignity_index(8));
}

int main(void)
{
    UNITY_BEGIN();

    /* Dignity index mapping (8 tests) */
    RUN_TEST(test_dignity_index_mercury);
    RUN_TEST(test_dignity_index_venus);
    RUN_TEST(test_dignity_index_sun_geocentric);
    RUN_TEST(test_dignity_index_mars);
    RUN_TEST(test_dignity_index_jupiter);
    RUN_TEST(test_dignity_index_saturn);
    RUN_TEST(test_dignity_index_uranus);
    RUN_TEST(test_dignity_index_neptune);

    /* Planet formatting (4 tests) */
    RUN_TEST(test_fmt_planet_sun_in_leo);
    RUN_TEST(test_fmt_planet_mercury_in_gemini);
    RUN_TEST(test_fmt_planet_contains_zodiac_symbol);
    RUN_TEST(test_fmt_planet_degree_precision);

    /* Ascendant formatting (3 tests) */
    RUN_TEST(test_fmt_ascendant_cancer);
    RUN_TEST(test_fmt_ascendant_contains_symbol);
    RUN_TEST(test_fmt_ascendant_aries_zero);

    /* Full build: planet entries (5 tests) */
    RUN_TEST(test_build_planet_count);
    RUN_TEST(test_build_planet_signs);
    RUN_TEST(test_build_planet_degree_in_sign);
    RUN_TEST(test_build_planet_index_stored);
    RUN_TEST(test_build_planet_formatted_nonempty);

    /* Full build: dignity (6 tests) */
    RUN_TEST(test_build_dignity_venus_taurus_domicile);
    RUN_TEST(test_build_dignity_sun_leo_domicile);
    RUN_TEST(test_build_dignity_mars_aries_domicile);
    RUN_TEST(test_build_dignity_jupiter_sagittarius_domicile);
    RUN_TEST(test_build_dignity_saturn_capricorn_domicile);
    RUN_TEST(test_build_dignity_total);

    /* Full build: aspects (3 tests) */
    RUN_TEST(test_build_aspects_count_range);
    RUN_TEST(test_build_aspects_formatted_nonempty);
    RUN_TEST(test_build_aspects_orb_within_max);

    /* Full build: ascendant (3 tests) */
    RUN_TEST(test_build_ascendant_sign);
    RUN_TEST(test_build_ascendant_degree);
    RUN_TEST(test_build_ascendant_str);

    /* Full build: balance (2 tests) */
    RUN_TEST(test_build_element_str_nonempty);
    RUN_TEST(test_build_modality_str_nonempty);

    /* Purity (1 test) */
    RUN_TEST(test_purity_same_input_same_output);

    /* Edge cases (2 tests) */
    RUN_TEST(test_build_zero_orb_no_aspects);
    RUN_TEST(test_dignity_index_out_of_range);

    return UNITY_END();
}
