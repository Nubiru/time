/* test_chakra.c — Chakra knowledge system tests
 * TDD RED phase: tests written before implementation.
 * Source: Traditional Hindu/yogic 7-chakra system, Solfeggio frequencies. */

#include "../../unity/unity.h"
#include "../../../src/systems/chakra/chakra.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants ===== */

void test_chakra_count_constant(void)
{
    TEST_ASSERT_EQUAL_INT(7, CHAKRA_COUNT);
}

void test_chakra_count_function(void)
{
    TEST_ASSERT_EQUAL_INT(7, chakra_count());
}

void test_element_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, CHAKRA_ELEM_COUNT);
}

/* ===== Enum ordering ===== */

void test_enum_muladhara_is_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, CHAKRA_MULADHARA);
}

void test_enum_svadhisthana_is_one(void)
{
    TEST_ASSERT_EQUAL_INT(1, CHAKRA_SVADHISTHANA);
}

void test_enum_manipura_is_two(void)
{
    TEST_ASSERT_EQUAL_INT(2, CHAKRA_MANIPURA);
}

void test_enum_anahata_is_three(void)
{
    TEST_ASSERT_EQUAL_INT(3, CHAKRA_ANAHATA);
}

void test_enum_vishuddha_is_four(void)
{
    TEST_ASSERT_EQUAL_INT(4, CHAKRA_VISHUDDHA);
}

void test_enum_ajna_is_five(void)
{
    TEST_ASSERT_EQUAL_INT(5, CHAKRA_AJNA);
}

void test_enum_sahasrara_is_six(void)
{
    TEST_ASSERT_EQUAL_INT(6, CHAKRA_SAHASRARA);
}

/* ===== Sanskrit names ===== */

void test_sanskrit_muladhara(void)
{
    TEST_ASSERT_EQUAL_STRING("Muladhara", chakra_sanskrit_name(CHAKRA_MULADHARA));
}

void test_sanskrit_svadhisthana(void)
{
    TEST_ASSERT_EQUAL_STRING("Svadhisthana", chakra_sanskrit_name(CHAKRA_SVADHISTHANA));
}

void test_sanskrit_manipura(void)
{
    TEST_ASSERT_EQUAL_STRING("Manipura", chakra_sanskrit_name(CHAKRA_MANIPURA));
}

void test_sanskrit_anahata(void)
{
    TEST_ASSERT_EQUAL_STRING("Anahata", chakra_sanskrit_name(CHAKRA_ANAHATA));
}

void test_sanskrit_vishuddha(void)
{
    TEST_ASSERT_EQUAL_STRING("Vishuddha", chakra_sanskrit_name(CHAKRA_VISHUDDHA));
}

void test_sanskrit_ajna(void)
{
    TEST_ASSERT_EQUAL_STRING("Ajna", chakra_sanskrit_name(CHAKRA_AJNA));
}

void test_sanskrit_sahasrara(void)
{
    TEST_ASSERT_EQUAL_STRING("Sahasrara", chakra_sanskrit_name(CHAKRA_SAHASRARA));
}

void test_sanskrit_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_sanskrit_name((chakra_t)99));
}

/* ===== English names ===== */

void test_english_root(void)
{
    TEST_ASSERT_EQUAL_STRING("Root", chakra_english_name(CHAKRA_MULADHARA));
}

void test_english_sacral(void)
{
    TEST_ASSERT_EQUAL_STRING("Sacral", chakra_english_name(CHAKRA_SVADHISTHANA));
}

void test_english_solar_plexus(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Plexus", chakra_english_name(CHAKRA_MANIPURA));
}

void test_english_heart(void)
{
    TEST_ASSERT_EQUAL_STRING("Heart", chakra_english_name(CHAKRA_ANAHATA));
}

void test_english_throat(void)
{
    TEST_ASSERT_EQUAL_STRING("Throat", chakra_english_name(CHAKRA_VISHUDDHA));
}

void test_english_third_eye(void)
{
    TEST_ASSERT_EQUAL_STRING("Third Eye", chakra_english_name(CHAKRA_AJNA));
}

void test_english_crown(void)
{
    TEST_ASSERT_EQUAL_STRING("Crown", chakra_english_name(CHAKRA_SAHASRARA));
}

void test_english_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_english_name((chakra_t)-1));
}

/* ===== Locations ===== */

void test_location_root(void)
{
    TEST_ASSERT_EQUAL_STRING("Base of spine", chakra_location(CHAKRA_MULADHARA));
}

void test_location_sacral(void)
{
    TEST_ASSERT_EQUAL_STRING("Lower abdomen", chakra_location(CHAKRA_SVADHISTHANA));
}

void test_location_solar_plexus(void)
{
    TEST_ASSERT_EQUAL_STRING("Upper abdomen", chakra_location(CHAKRA_MANIPURA));
}

void test_location_heart(void)
{
    TEST_ASSERT_EQUAL_STRING("Center of chest", chakra_location(CHAKRA_ANAHATA));
}

void test_location_throat(void)
{
    TEST_ASSERT_EQUAL_STRING("Throat", chakra_location(CHAKRA_VISHUDDHA));
}

void test_location_third_eye(void)
{
    TEST_ASSERT_EQUAL_STRING("Between eyebrows", chakra_location(CHAKRA_AJNA));
}

void test_location_crown(void)
{
    TEST_ASSERT_EQUAL_STRING("Top of head", chakra_location(CHAKRA_SAHASRARA));
}

void test_location_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_location((chakra_t)10));
}

/* ===== Bija mantras ===== */

void test_bija_lam(void)
{
    TEST_ASSERT_EQUAL_STRING("LAM", chakra_bija(CHAKRA_MULADHARA));
}

void test_bija_vam(void)
{
    TEST_ASSERT_EQUAL_STRING("VAM", chakra_bija(CHAKRA_SVADHISTHANA));
}

void test_bija_ram(void)
{
    TEST_ASSERT_EQUAL_STRING("RAM", chakra_bija(CHAKRA_MANIPURA));
}

void test_bija_yam(void)
{
    TEST_ASSERT_EQUAL_STRING("YAM", chakra_bija(CHAKRA_ANAHATA));
}

void test_bija_ham(void)
{
    TEST_ASSERT_EQUAL_STRING("HAM", chakra_bija(CHAKRA_VISHUDDHA));
}

void test_bija_om(void)
{
    TEST_ASSERT_EQUAL_STRING("OM", chakra_bija(CHAKRA_AJNA));
}

void test_bija_aum(void)
{
    TEST_ASSERT_EQUAL_STRING("AUM", chakra_bija(CHAKRA_SAHASRARA));
}

void test_bija_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_bija((chakra_t)7));
}

/* ===== Elements ===== */

void test_element_root_earth(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_EARTH, chakra_element(CHAKRA_MULADHARA));
}

void test_element_sacral_water(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_WATER, chakra_element(CHAKRA_SVADHISTHANA));
}

void test_element_solar_plexus_fire(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_FIRE, chakra_element(CHAKRA_MANIPURA));
}

void test_element_heart_air(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_AIR, chakra_element(CHAKRA_ANAHATA));
}

void test_element_throat_ether(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_ETHER, chakra_element(CHAKRA_VISHUDDHA));
}

void test_element_third_eye_light(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_LIGHT, chakra_element(CHAKRA_AJNA));
}

void test_element_crown_thought(void)
{
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_THOUGHT, chakra_element(CHAKRA_SAHASRARA));
}

/* ===== Element names ===== */

void test_element_name_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", chakra_element_name(CHAKRA_ELEM_EARTH));
}

void test_element_name_water(void)
{
    TEST_ASSERT_EQUAL_STRING("Water", chakra_element_name(CHAKRA_ELEM_WATER));
}

void test_element_name_fire(void)
{
    TEST_ASSERT_EQUAL_STRING("Fire", chakra_element_name(CHAKRA_ELEM_FIRE));
}

void test_element_name_air(void)
{
    TEST_ASSERT_EQUAL_STRING("Air", chakra_element_name(CHAKRA_ELEM_AIR));
}

void test_element_name_ether(void)
{
    TEST_ASSERT_EQUAL_STRING("Ether", chakra_element_name(CHAKRA_ELEM_ETHER));
}

void test_element_name_light(void)
{
    TEST_ASSERT_EQUAL_STRING("Light", chakra_element_name(CHAKRA_ELEM_LIGHT));
}

void test_element_name_thought(void)
{
    TEST_ASSERT_EQUAL_STRING("Thought", chakra_element_name(CHAKRA_ELEM_THOUGHT));
}

void test_element_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_element_name(CHAKRA_ELEM_COUNT));
}

void test_element_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_element_name((chakra_element_t)-1));
}

/* ===== Colors ===== */

void test_color_root_red(void)
{
    chakra_color_t c = chakra_color(CHAKRA_MULADHARA);
    TEST_ASSERT_TRUE(fabsf(c.r - 1.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 0.0f) < 0.001f);
}

void test_color_sacral_orange(void)
{
    chakra_color_t c = chakra_color(CHAKRA_SVADHISTHANA);
    TEST_ASSERT_TRUE(fabsf(c.r - 1.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 0.5f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 0.0f) < 0.001f);
}

void test_color_solar_plexus_yellow(void)
{
    chakra_color_t c = chakra_color(CHAKRA_MANIPURA);
    TEST_ASSERT_TRUE(fabsf(c.r - 1.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 1.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 0.0f) < 0.001f);
}

void test_color_heart_green(void)
{
    chakra_color_t c = chakra_color(CHAKRA_ANAHATA);
    TEST_ASSERT_TRUE(fabsf(c.r - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 0.8f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 0.0f) < 0.001f);
}

void test_color_throat_blue(void)
{
    chakra_color_t c = chakra_color(CHAKRA_VISHUDDHA);
    TEST_ASSERT_TRUE(fabsf(c.r - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 0.5f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 1.0f) < 0.001f);
}

void test_color_third_eye_indigo(void)
{
    chakra_color_t c = chakra_color(CHAKRA_AJNA);
    TEST_ASSERT_TRUE(fabsf(c.r - 0.3f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 0.5f) < 0.001f);
}

void test_color_crown_violet(void)
{
    chakra_color_t c = chakra_color(CHAKRA_SAHASRARA);
    TEST_ASSERT_TRUE(fabsf(c.r - 0.6f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.g - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(c.b - 0.8f) < 0.001f);
}

/* ===== Frequencies ===== */

void test_freq_muladhara(void)
{
    double f = chakra_frequency(CHAKRA_MULADHARA);
    TEST_ASSERT_TRUE(f > 395.0 && f < 397.0);
}

void test_freq_svadhisthana(void)
{
    double f = chakra_frequency(CHAKRA_SVADHISTHANA);
    TEST_ASSERT_TRUE(f > 416.0 && f < 418.0);
}

void test_freq_manipura(void)
{
    double f = chakra_frequency(CHAKRA_MANIPURA);
    TEST_ASSERT_TRUE(f > 527.0 && f < 529.0);
}

void test_freq_anahata(void)
{
    double f = chakra_frequency(CHAKRA_ANAHATA);
    TEST_ASSERT_TRUE(f > 638.0 && f < 640.0);
}

void test_freq_vishuddha(void)
{
    double f = chakra_frequency(CHAKRA_VISHUDDHA);
    TEST_ASSERT_TRUE(f > 740.0 && f < 742.0);
}

void test_freq_ajna(void)
{
    double f = chakra_frequency(CHAKRA_AJNA);
    TEST_ASSERT_TRUE(f > 851.0 && f < 853.0);
}

void test_freq_sahasrara(void)
{
    double f = chakra_frequency(CHAKRA_SAHASRARA);
    TEST_ASSERT_TRUE(f > 962.0 && f < 964.0);
}

void test_freq_ascending_order(void)
{
    /* Solfeggio frequencies ascend root to crown */
    for (int i = 0; i < CHAKRA_COUNT - 1; i++) {
        double f1 = chakra_frequency((chakra_t)i);
        double f2 = chakra_frequency((chakra_t)(i + 1));
        TEST_ASSERT_TRUE(f2 > f1);
    }
}

/* ===== Petals ===== */

void test_petals_muladhara(void)
{
    TEST_ASSERT_EQUAL_INT(4, chakra_petals(CHAKRA_MULADHARA));
}

void test_petals_svadhisthana(void)
{
    TEST_ASSERT_EQUAL_INT(6, chakra_petals(CHAKRA_SVADHISTHANA));
}

void test_petals_manipura(void)
{
    TEST_ASSERT_EQUAL_INT(10, chakra_petals(CHAKRA_MANIPURA));
}

void test_petals_anahata(void)
{
    TEST_ASSERT_EQUAL_INT(12, chakra_petals(CHAKRA_ANAHATA));
}

void test_petals_vishuddha(void)
{
    TEST_ASSERT_EQUAL_INT(16, chakra_petals(CHAKRA_VISHUDDHA));
}

void test_petals_ajna(void)
{
    TEST_ASSERT_EQUAL_INT(2, chakra_petals(CHAKRA_AJNA));
}

void test_petals_sahasrara(void)
{
    TEST_ASSERT_EQUAL_INT(1000, chakra_petals(CHAKRA_SAHASRARA));
}

void test_petals_total(void)
{
    /* Total petals across all 7 chakras: 4+6+10+12+16+2+1000 = 1050 */
    int total = 0;
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        total += chakra_petals((chakra_t)i);
    }
    TEST_ASSERT_EQUAL_INT(1050, total);
}

/* ===== Planets ===== */

void test_planet_muladhara(void)
{
    TEST_ASSERT_EQUAL_STRING("Saturn", chakra_planet(CHAKRA_MULADHARA));
}

void test_planet_svadhisthana(void)
{
    TEST_ASSERT_EQUAL_STRING("Jupiter", chakra_planet(CHAKRA_SVADHISTHANA));
}

void test_planet_manipura(void)
{
    TEST_ASSERT_EQUAL_STRING("Mars", chakra_planet(CHAKRA_MANIPURA));
}

void test_planet_anahata(void)
{
    TEST_ASSERT_EQUAL_STRING("Venus", chakra_planet(CHAKRA_ANAHATA));
}

void test_planet_vishuddha(void)
{
    TEST_ASSERT_EQUAL_STRING("Mercury", chakra_planet(CHAKRA_VISHUDDHA));
}

void test_planet_ajna(void)
{
    TEST_ASSERT_EQUAL_STRING("Moon", chakra_planet(CHAKRA_AJNA));
}

void test_planet_sahasrara(void)
{
    TEST_ASSERT_EQUAL_STRING("Sun", chakra_planet(CHAKRA_SAHASRARA));
}

void test_planet_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", chakra_planet((chakra_t)20));
}

/* ===== chakra_info struct ===== */

void test_info_muladhara_id(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_INT(CHAKRA_MULADHARA, info.id);
}

void test_info_muladhara_sanskrit(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_STRING("Muladhara", info.sanskrit_name);
}

void test_info_muladhara_english(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_STRING("Root", info.english_name);
}

void test_info_muladhara_location(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_STRING("Base of spine", info.location);
}

void test_info_muladhara_bija(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_STRING("LAM", info.bija_mantra);
}

void test_info_muladhara_element(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_EARTH, info.element);
}

void test_info_muladhara_color(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_TRUE(fabsf(info.color.r - 1.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(info.color.g - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(info.color.b - 0.0f) < 0.001f);
}

void test_info_muladhara_frequency(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_TRUE(info.frequency_hz > 395.0 && info.frequency_hz < 397.0);
}

void test_info_muladhara_petals(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_INT(4, info.petal_count);
}

void test_info_muladhara_planet(void)
{
    chakra_info_t info = chakra_info(CHAKRA_MULADHARA);
    TEST_ASSERT_EQUAL_STRING("Saturn", info.planet);
}

void test_info_sahasrara_complete(void)
{
    chakra_info_t info = chakra_info(CHAKRA_SAHASRARA);
    TEST_ASSERT_EQUAL_INT(CHAKRA_SAHASRARA, info.id);
    TEST_ASSERT_EQUAL_STRING("Sahasrara", info.sanskrit_name);
    TEST_ASSERT_EQUAL_STRING("Crown", info.english_name);
    TEST_ASSERT_EQUAL_STRING("Top of head", info.location);
    TEST_ASSERT_EQUAL_STRING("AUM", info.bija_mantra);
    TEST_ASSERT_EQUAL_INT(CHAKRA_ELEM_THOUGHT, info.element);
    TEST_ASSERT_TRUE(fabsf(info.color.r - 0.6f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(info.color.g - 0.0f) < 0.001f);
    TEST_ASSERT_TRUE(fabsf(info.color.b - 0.8f) < 0.001f);
    TEST_ASSERT_TRUE(info.frequency_hz > 962.0 && info.frequency_hz < 964.0);
    TEST_ASSERT_EQUAL_INT(1000, info.petal_count);
    TEST_ASSERT_EQUAL_STRING("Sun", info.planet);
}

/* ===== chakra_from_frequency ===== */

void test_from_freq_exact_396(void)
{
    int idx = chakra_from_frequency(396.0, 1.0);
    TEST_ASSERT_EQUAL_INT(CHAKRA_MULADHARA, idx);
}

void test_from_freq_exact_528(void)
{
    int idx = chakra_from_frequency(528.0, 1.0);
    TEST_ASSERT_EQUAL_INT(CHAKRA_MANIPURA, idx);
}

void test_from_freq_exact_963(void)
{
    int idx = chakra_from_frequency(963.0, 1.0);
    TEST_ASSERT_EQUAL_INT(CHAKRA_SAHASRARA, idx);
}

void test_from_freq_within_tolerance(void)
{
    int idx = chakra_from_frequency(530.0, 5.0);
    TEST_ASSERT_EQUAL_INT(CHAKRA_MANIPURA, idx);
}

void test_from_freq_out_of_tolerance(void)
{
    int idx = chakra_from_frequency(500.0, 1.0);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

void test_from_freq_no_match(void)
{
    int idx = chakra_from_frequency(100.0, 10.0);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

void test_from_freq_boundary_tolerance(void)
{
    /* 639 Hz is Anahata. Test with freq at exact edge of tolerance */
    int idx = chakra_from_frequency(649.0, 10.0);
    TEST_ASSERT_EQUAL_INT(CHAKRA_ANAHATA, idx);
}

void test_from_freq_all_solfeggio(void)
{
    /* Test that all 7 solfeggio frequencies map correctly */
    static const double freqs[] = {396, 417, 528, 639, 741, 852, 963};
    for (int i = 0; i < 7; i++) {
        int idx = chakra_from_frequency(freqs[i], 0.5);
        TEST_ASSERT_EQUAL_INT(i, idx);
    }
}

/* ===== chakra_from_color ===== */

void test_from_color_exact_red(void)
{
    chakra_t c = chakra_from_color(1.0f, 0.0f, 0.0f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_MULADHARA, c);
}

void test_from_color_exact_green(void)
{
    chakra_t c = chakra_from_color(0.0f, 0.8f, 0.0f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_ANAHATA, c);
}

void test_from_color_exact_violet(void)
{
    chakra_t c = chakra_from_color(0.6f, 0.0f, 0.8f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_SAHASRARA, c);
}

void test_from_color_near_yellow(void)
{
    /* Slightly off yellow should still match Manipura */
    chakra_t c = chakra_from_color(0.95f, 0.95f, 0.05f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_MANIPURA, c);
}

void test_from_color_near_orange(void)
{
    /* Near-orange should match Svadhisthana */
    chakra_t c = chakra_from_color(0.95f, 0.45f, 0.05f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_SVADHISTHANA, c);
}

void test_from_color_near_blue(void)
{
    /* Near-blue should match Vishuddha */
    chakra_t c = chakra_from_color(0.05f, 0.45f, 0.95f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_VISHUDDHA, c);
}

void test_from_color_near_indigo(void)
{
    /* Near-indigo should match Ajna */
    chakra_t c = chakra_from_color(0.28f, 0.02f, 0.48f);
    TEST_ASSERT_EQUAL_INT(CHAKRA_AJNA, c);
}

/* ===== Completeness ===== */

void test_all_chakras_have_sanskrit_names(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        const char *name = chakra_sanskrit_name((chakra_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_all_chakras_have_english_names(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        const char *name = chakra_english_name((chakra_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_all_chakras_have_locations(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        const char *loc = chakra_location((chakra_t)i);
        TEST_ASSERT_NOT_NULL(loc);
        TEST_ASSERT_TRUE(strlen(loc) > 0);
    }
}

void test_all_chakras_have_bija(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        const char *bija = chakra_bija((chakra_t)i);
        TEST_ASSERT_NOT_NULL(bija);
        TEST_ASSERT_TRUE(strlen(bija) > 0);
    }
}

void test_all_chakras_have_planets(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        const char *planet = chakra_planet((chakra_t)i);
        TEST_ASSERT_NOT_NULL(planet);
        TEST_ASSERT_TRUE(strlen(planet) > 0);
    }
}

void test_all_chakras_have_positive_frequency(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        double f = chakra_frequency((chakra_t)i);
        TEST_ASSERT_TRUE(f > 0.0);
    }
}

void test_all_chakras_have_positive_petals(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        int p = chakra_petals((chakra_t)i);
        TEST_ASSERT_TRUE(p > 0);
    }
}

void test_all_info_ids_match(void)
{
    for (int i = 0; i < CHAKRA_COUNT; i++) {
        chakra_info_t info = chakra_info((chakra_t)i);
        TEST_ASSERT_EQUAL_INT(i, info.id);
    }
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Constants (3) */
    RUN_TEST(test_chakra_count_constant);
    RUN_TEST(test_chakra_count_function);
    RUN_TEST(test_element_count);

    /* Enum ordering (7) */
    RUN_TEST(test_enum_muladhara_is_zero);
    RUN_TEST(test_enum_svadhisthana_is_one);
    RUN_TEST(test_enum_manipura_is_two);
    RUN_TEST(test_enum_anahata_is_three);
    RUN_TEST(test_enum_vishuddha_is_four);
    RUN_TEST(test_enum_ajna_is_five);
    RUN_TEST(test_enum_sahasrara_is_six);

    /* Sanskrit names (8) */
    RUN_TEST(test_sanskrit_muladhara);
    RUN_TEST(test_sanskrit_svadhisthana);
    RUN_TEST(test_sanskrit_manipura);
    RUN_TEST(test_sanskrit_anahata);
    RUN_TEST(test_sanskrit_vishuddha);
    RUN_TEST(test_sanskrit_ajna);
    RUN_TEST(test_sanskrit_sahasrara);
    RUN_TEST(test_sanskrit_invalid);

    /* English names (8) */
    RUN_TEST(test_english_root);
    RUN_TEST(test_english_sacral);
    RUN_TEST(test_english_solar_plexus);
    RUN_TEST(test_english_heart);
    RUN_TEST(test_english_throat);
    RUN_TEST(test_english_third_eye);
    RUN_TEST(test_english_crown);
    RUN_TEST(test_english_invalid);

    /* Locations (8) */
    RUN_TEST(test_location_root);
    RUN_TEST(test_location_sacral);
    RUN_TEST(test_location_solar_plexus);
    RUN_TEST(test_location_heart);
    RUN_TEST(test_location_throat);
    RUN_TEST(test_location_third_eye);
    RUN_TEST(test_location_crown);
    RUN_TEST(test_location_invalid);

    /* Bija mantras (8) */
    RUN_TEST(test_bija_lam);
    RUN_TEST(test_bija_vam);
    RUN_TEST(test_bija_ram);
    RUN_TEST(test_bija_yam);
    RUN_TEST(test_bija_ham);
    RUN_TEST(test_bija_om);
    RUN_TEST(test_bija_aum);
    RUN_TEST(test_bija_invalid);

    /* Elements (7) */
    RUN_TEST(test_element_root_earth);
    RUN_TEST(test_element_sacral_water);
    RUN_TEST(test_element_solar_plexus_fire);
    RUN_TEST(test_element_heart_air);
    RUN_TEST(test_element_throat_ether);
    RUN_TEST(test_element_third_eye_light);
    RUN_TEST(test_element_crown_thought);

    /* Element names (9) */
    RUN_TEST(test_element_name_earth);
    RUN_TEST(test_element_name_water);
    RUN_TEST(test_element_name_fire);
    RUN_TEST(test_element_name_air);
    RUN_TEST(test_element_name_ether);
    RUN_TEST(test_element_name_light);
    RUN_TEST(test_element_name_thought);
    RUN_TEST(test_element_name_invalid);
    RUN_TEST(test_element_name_negative);

    /* Colors (7) */
    RUN_TEST(test_color_root_red);
    RUN_TEST(test_color_sacral_orange);
    RUN_TEST(test_color_solar_plexus_yellow);
    RUN_TEST(test_color_heart_green);
    RUN_TEST(test_color_throat_blue);
    RUN_TEST(test_color_third_eye_indigo);
    RUN_TEST(test_color_crown_violet);

    /* Frequencies (8) */
    RUN_TEST(test_freq_muladhara);
    RUN_TEST(test_freq_svadhisthana);
    RUN_TEST(test_freq_manipura);
    RUN_TEST(test_freq_anahata);
    RUN_TEST(test_freq_vishuddha);
    RUN_TEST(test_freq_ajna);
    RUN_TEST(test_freq_sahasrara);
    RUN_TEST(test_freq_ascending_order);

    /* Petals (8) */
    RUN_TEST(test_petals_muladhara);
    RUN_TEST(test_petals_svadhisthana);
    RUN_TEST(test_petals_manipura);
    RUN_TEST(test_petals_anahata);
    RUN_TEST(test_petals_vishuddha);
    RUN_TEST(test_petals_ajna);
    RUN_TEST(test_petals_sahasrara);
    RUN_TEST(test_petals_total);

    /* Planets (8) */
    RUN_TEST(test_planet_muladhara);
    RUN_TEST(test_planet_svadhisthana);
    RUN_TEST(test_planet_manipura);
    RUN_TEST(test_planet_anahata);
    RUN_TEST(test_planet_vishuddha);
    RUN_TEST(test_planet_ajna);
    RUN_TEST(test_planet_sahasrara);
    RUN_TEST(test_planet_invalid);

    /* chakra_info struct (11) */
    RUN_TEST(test_info_muladhara_id);
    RUN_TEST(test_info_muladhara_sanskrit);
    RUN_TEST(test_info_muladhara_english);
    RUN_TEST(test_info_muladhara_location);
    RUN_TEST(test_info_muladhara_bija);
    RUN_TEST(test_info_muladhara_element);
    RUN_TEST(test_info_muladhara_color);
    RUN_TEST(test_info_muladhara_frequency);
    RUN_TEST(test_info_muladhara_petals);
    RUN_TEST(test_info_muladhara_planet);
    RUN_TEST(test_info_sahasrara_complete);

    /* chakra_from_frequency (8) */
    RUN_TEST(test_from_freq_exact_396);
    RUN_TEST(test_from_freq_exact_528);
    RUN_TEST(test_from_freq_exact_963);
    RUN_TEST(test_from_freq_within_tolerance);
    RUN_TEST(test_from_freq_out_of_tolerance);
    RUN_TEST(test_from_freq_no_match);
    RUN_TEST(test_from_freq_boundary_tolerance);
    RUN_TEST(test_from_freq_all_solfeggio);

    /* chakra_from_color (7) */
    RUN_TEST(test_from_color_exact_red);
    RUN_TEST(test_from_color_exact_green);
    RUN_TEST(test_from_color_exact_violet);
    RUN_TEST(test_from_color_near_yellow);
    RUN_TEST(test_from_color_near_orange);
    RUN_TEST(test_from_color_near_blue);
    RUN_TEST(test_from_color_near_indigo);

    /* Completeness (8) */
    RUN_TEST(test_all_chakras_have_sanskrit_names);
    RUN_TEST(test_all_chakras_have_english_names);
    RUN_TEST(test_all_chakras_have_locations);
    RUN_TEST(test_all_chakras_have_bija);
    RUN_TEST(test_all_chakras_have_planets);
    RUN_TEST(test_all_chakras_have_positive_frequency);
    RUN_TEST(test_all_chakras_have_positive_petals);
    RUN_TEST(test_all_info_ids_match);

    return UNITY_END();
}
