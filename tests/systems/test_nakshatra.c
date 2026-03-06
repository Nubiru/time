#include "../unity/unity.h"
#include "../../src/systems/hindu/nakshatra.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== 1. Count returns 27 ===== */

void test_nakshatra_count(void)
{
    TEST_ASSERT_EQUAL_INT(27, nakshatra_count());
}

/* ===== 2. First Nakshatra: Ashwini ===== */

void test_nakshatra_get_ashwini(void)
{
    nakshatra_t n = nakshatra_get(0);
    TEST_ASSERT_EQUAL_INT(0, n.id);
    TEST_ASSERT_EQUAL_STRING("Ashwini", n.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)n.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 13.333f, (float)n.end_deg);
    TEST_ASSERT_EQUAL_INT(0, n.ruling_planet); /* Ketu */
    TEST_ASSERT_EQUAL_STRING("Deva", n.quality);
}

/* ===== 3. Last Nakshatra: Revati ===== */

void test_nakshatra_get_revati(void)
{
    nakshatra_t n = nakshatra_get(26);
    TEST_ASSERT_EQUAL_INT(26, n.id);
    TEST_ASSERT_EQUAL_STRING("Revati", n.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 346.667f, (float)n.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 360.0f, (float)n.end_deg);
    TEST_ASSERT_EQUAL_INT(8, n.ruling_planet); /* Mercury */
    TEST_ASSERT_EQUAL_STRING("Deva", n.quality);
}

/* ===== 4. Boundary: invalid index returns id = -1 ===== */

void test_nakshatra_get_invalid_negative(void)
{
    nakshatra_t n = nakshatra_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, n.id);
}

void test_nakshatra_get_invalid_high(void)
{
    nakshatra_t n = nakshatra_get(27);
    TEST_ASSERT_EQUAL_INT(-1, n.id);
}

/* ===== 5. Nakshatra name lookup ===== */

void test_nakshatra_name_rohini(void)
{
    const char *name = nakshatra_name(3);
    TEST_ASSERT_EQUAL_STRING("Rohini", name);
}

void test_nakshatra_name_invalid(void)
{
    const char *name = nakshatra_name(99);
    TEST_ASSERT_NULL(name);
}

/* ===== 6. Longitude -> Nakshatra (basic) ===== */

void test_nakshatra_from_longitude_zero(void)
{
    nakshatra_t n = nakshatra_from_longitude(0.0);
    TEST_ASSERT_EQUAL_INT(0, n.id);
    TEST_ASSERT_EQUAL_STRING("Ashwini", n.name);
}

void test_nakshatra_from_longitude_mid_rohini(void)
{
    /* 46 degrees is in Rohini (40-53.333) */
    nakshatra_t n = nakshatra_from_longitude(46.0);
    TEST_ASSERT_EQUAL_INT(3, n.id);
    TEST_ASSERT_EQUAL_STRING("Rohini", n.name);
}

void test_nakshatra_from_longitude_end_revati(void)
{
    /* 359.99 degrees is in Revati (346.667-360) */
    nakshatra_t n = nakshatra_from_longitude(359.99);
    TEST_ASSERT_EQUAL_INT(26, n.id);
}

/* ===== 7. Longitude wrapping ===== */

void test_nakshatra_from_longitude_wrap_negative(void)
{
    /* -10 degrees should wrap to 350 -> Revati */
    nakshatra_t n = nakshatra_from_longitude(-10.0);
    TEST_ASSERT_EQUAL_INT(26, n.id);
}

void test_nakshatra_from_longitude_wrap_over_360(void)
{
    /* 370 degrees should wrap to 10 -> Ashwini */
    nakshatra_t n = nakshatra_from_longitude(370.0);
    TEST_ASSERT_EQUAL_INT(0, n.id);
}

/* ===== 8. Ruler cycle repeats 3x ===== */

void test_ruler_cycle_first_nine(void)
{
    /* First cycle: 0=Ketu,1=Venus,2=Sun,3=Moon,4=Mars,5=Rahu,6=Jupiter,7=Saturn,8=Mercury */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_get(0).ruling_planet); /* Ketu */
    TEST_ASSERT_EQUAL_INT(1, nakshatra_get(1).ruling_planet); /* Venus */
    TEST_ASSERT_EQUAL_INT(2, nakshatra_get(2).ruling_planet); /* Sun */
    TEST_ASSERT_EQUAL_INT(3, nakshatra_get(3).ruling_planet); /* Moon */
    TEST_ASSERT_EQUAL_INT(4, nakshatra_get(4).ruling_planet); /* Mars */
    TEST_ASSERT_EQUAL_INT(5, nakshatra_get(5).ruling_planet); /* Rahu */
    TEST_ASSERT_EQUAL_INT(6, nakshatra_get(6).ruling_planet); /* Jupiter */
    TEST_ASSERT_EQUAL_INT(7, nakshatra_get(7).ruling_planet); /* Saturn */
    TEST_ASSERT_EQUAL_INT(8, nakshatra_get(8).ruling_planet); /* Mercury */
}

void test_ruler_cycle_second_repeat(void)
{
    /* Second cycle: indices 9-17 repeat the same pattern */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_get(9).ruling_planet);  /* Ketu */
    TEST_ASSERT_EQUAL_INT(1, nakshatra_get(10).ruling_planet); /* Venus */
    TEST_ASSERT_EQUAL_INT(8, nakshatra_get(17).ruling_planet); /* Mercury */
}

void test_ruler_cycle_third_repeat(void)
{
    /* Third cycle: indices 18-26 repeat the same pattern */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_get(18).ruling_planet); /* Ketu */
    TEST_ASSERT_EQUAL_INT(8, nakshatra_get(26).ruling_planet); /* Mercury */
}

/* ===== 9. Ruler name lookup ===== */

void test_ruler_name_ketu(void)
{
    TEST_ASSERT_EQUAL_STRING("Ketu", nakshatra_ruler_name(0));
}

void test_ruler_name_mercury(void)
{
    TEST_ASSERT_EQUAL_STRING("Mercury", nakshatra_ruler_name(8));
}

void test_ruler_name_invalid(void)
{
    TEST_ASSERT_NULL(nakshatra_ruler_name(9));
    TEST_ASSERT_NULL(nakshatra_ruler_name(-1));
}

/* ===== 10. Yogatara star catalog cross-reference ===== */

void test_yogatara_aldebaran_rohini(void)
{
    /* Rohini yogatara = Aldebaran = catalog index 13 */
    nakshatra_t n = nakshatra_get(3);
    TEST_ASSERT_EQUAL_INT(13, n.yogatara_catalog_index);
}

void test_yogatara_regulus_magha(void)
{
    /* Magha yogatara = Regulus = catalog index 20 */
    nakshatra_t n = nakshatra_get(9);
    TEST_ASSERT_EQUAL_INT(20, n.yogatara_catalog_index);
}

void test_yogatara_spica_chitra(void)
{
    /* Chitra yogatara = Spica = catalog index 15 */
    nakshatra_t n = nakshatra_get(13);
    TEST_ASSERT_EQUAL_INT(15, n.yogatara_catalog_index);
}

void test_yogatara_arcturus_swati(void)
{
    /* Swati yogatara = Arcturus = catalog index 3 */
    nakshatra_t n = nakshatra_get(14);
    TEST_ASSERT_EQUAL_INT(3, n.yogatara_catalog_index);
}

void test_yogatara_antares_jyeshtha(void)
{
    /* Jyeshtha yogatara = Antares = catalog index 14 */
    nakshatra_t n = nakshatra_get(17);
    TEST_ASSERT_EQUAL_INT(14, n.yogatara_catalog_index);
}

void test_yogatara_altair_shravana(void)
{
    /* Shravana yogatara = Altair = catalog index 11 */
    nakshatra_t n = nakshatra_get(21);
    TEST_ASSERT_EQUAL_INT(11, n.yogatara_catalog_index);
}

void test_yogatara_not_in_catalog(void)
{
    /* Ashwini yogatara = beta Ari, not in 195-star catalog */
    nakshatra_t n = nakshatra_get(0);
    TEST_ASSERT_EQUAL_INT(-1, n.yogatara_catalog_index);
}

/* ===== 11. Pada from longitude ===== */

void test_pada_from_longitude_first(void)
{
    /* 1.0 deg -> Ashwini pada 1 */
    nakshatra_pada_t p = nakshatra_pada_from_longitude(1.0);
    TEST_ASSERT_EQUAL_INT(0, p.nakshatra_id);
    TEST_ASSERT_EQUAL_INT(1, p.pada_number);
    TEST_ASSERT_EQUAL_INT(0, p.navamsa_sign); /* Aries */
}

void test_pada_from_longitude_second(void)
{
    /* 4.0 deg -> Ashwini pada 2 (3.333-6.667) */
    nakshatra_pada_t p = nakshatra_pada_from_longitude(4.0);
    TEST_ASSERT_EQUAL_INT(0, p.nakshatra_id);
    TEST_ASSERT_EQUAL_INT(2, p.pada_number);
    TEST_ASSERT_EQUAL_INT(1, p.navamsa_sign); /* Taurus */
}

void test_pada_from_longitude_last(void)
{
    /* 359.0 deg -> Revati pada 4 (356.667-360) */
    nakshatra_pada_t p = nakshatra_pada_from_longitude(359.0);
    TEST_ASSERT_EQUAL_INT(26, p.nakshatra_id);
    TEST_ASSERT_EQUAL_INT(4, p.pada_number);
    /* Pada global index = 26*4 + 3 = 107, navamsa = 107 % 12 = 11 (Pisces) */
    TEST_ASSERT_EQUAL_INT(11, p.navamsa_sign);
}

/* ===== 12. Pada get by id ===== */

void test_pada_get_ashwini_1(void)
{
    nakshatra_pada_t p = nakshatra_pada_get(0, 1);
    TEST_ASSERT_EQUAL_INT(0, p.nakshatra_id);
    TEST_ASSERT_EQUAL_INT(1, p.pada_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.333f, (float)p.end_deg);
    TEST_ASSERT_EQUAL_INT(0, p.navamsa_sign); /* Aries */
}

void test_pada_get_ashwini_4(void)
{
    nakshatra_pada_t p = nakshatra_pada_get(0, 4);
    TEST_ASSERT_EQUAL_INT(0, p.nakshatra_id);
    TEST_ASSERT_EQUAL_INT(4, p.pada_number);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, (float)p.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 13.333f, (float)p.end_deg);
    TEST_ASSERT_EQUAL_INT(3, p.navamsa_sign); /* Cancer */
}

void test_pada_get_invalid(void)
{
    nakshatra_pada_t p = nakshatra_pada_get(0, 5);
    TEST_ASSERT_EQUAL_INT(-1, p.nakshatra_id);
}

/* ===== 13. Navamsa from longitude ===== */

void test_navamsa_zero(void)
{
    /* 0 deg -> pada global 0 -> navamsa 0 (Aries) */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_navamsa(0.0));
}

void test_navamsa_mid_cycle(void)
{
    /* 120 deg -> Magha pada 1. Global index = 9*4+0 = 36. 36%12 = 0 (Aries) */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_navamsa(120.0));
}

void test_navamsa_end_of_zodiac(void)
{
    /* 359 deg -> Revati pada 4. Global index = 107. 107%12 = 11 (Pisces) */
    TEST_ASSERT_EQUAL_INT(11, nakshatra_navamsa(359.0));
}

/* ===== 14. Lahiri ayanamsa ===== */

void test_lahiri_ayanamsa_j2000(void)
{
    /* At J2000.0 (JD 2451545.0): ayanamsa = 23.85 + 0 = 23.85 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 23.85f,
                             (float)nakshatra_lahiri_ayanamsa(2451545.0));
}

void test_lahiri_ayanamsa_j2025(void)
{
    /* At J2025.0 (JD 2451545.0 + 25*365.25 = 2460676.25):
     * ayanamsa = 23.85 + 0.01396 * 25 = 23.85 + 0.349 = 24.199 */
    double jd_2025 = 2451545.0 + 25.0 * 365.25;
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 24.20f,
                             (float)nakshatra_lahiri_ayanamsa(jd_2025));
}

/* ===== 15. Tropical to sidereal conversion ===== */

void test_tropical_to_sidereal_basic(void)
{
    /* tropical 30.0 with ayanamsa 24.0 -> sidereal 6.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 6.0f,
                             (float)nakshatra_tropical_to_sidereal(30.0, 24.0));
}

void test_tropical_to_sidereal_wrap(void)
{
    /* tropical 10.0 with ayanamsa 24.0 -> sidereal -14 -> wraps to 346.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 346.0f,
                             (float)nakshatra_tropical_to_sidereal(10.0, 24.0));
}

/* ===== 16. Quality assignments ===== */

void test_quality_deva(void)
{
    /* Ashwini (0) = Deva */
    TEST_ASSERT_EQUAL_STRING("Deva", nakshatra_get(0).quality);
    /* Pushya (7) = Deva */
    TEST_ASSERT_EQUAL_STRING("Deva", nakshatra_get(7).quality);
    /* Shravana (21) = Deva */
    TEST_ASSERT_EQUAL_STRING("Deva", nakshatra_get(21).quality);
}

void test_quality_manushya(void)
{
    /* Bharani (1) = Manushya */
    TEST_ASSERT_EQUAL_STRING("Manushya", nakshatra_get(1).quality);
    /* Rohini (3) = Manushya */
    TEST_ASSERT_EQUAL_STRING("Manushya", nakshatra_get(3).quality);
    /* Uttara Ashadha (20) = Manushya */
    TEST_ASSERT_EQUAL_STRING("Manushya", nakshatra_get(20).quality);
}

void test_quality_rakshasa(void)
{
    /* Krittika (2) = Rakshasa */
    TEST_ASSERT_EQUAL_STRING("Rakshasa", nakshatra_get(2).quality);
    /* Ashlesha (8) = Rakshasa */
    TEST_ASSERT_EQUAL_STRING("Rakshasa", nakshatra_get(8).quality);
    /* Mula (18) = Rakshasa */
    TEST_ASSERT_EQUAL_STRING("Rakshasa", nakshatra_get(18).quality);
}

/* ===== 17. Deity and symbol populated ===== */

void test_deity_populated(void)
{
    nakshatra_t n = nakshatra_get(0);
    TEST_ASSERT_NOT_NULL(n.deity);
    TEST_ASSERT_EQUAL_STRING("Ashwini Kumaras", n.deity);
}

void test_symbol_populated(void)
{
    nakshatra_t n = nakshatra_get(0);
    TEST_ASSERT_NOT_NULL(n.symbol);
    TEST_ASSERT_EQUAL_STRING("Horse head", n.symbol);
}

/* ===== 18. Meaning populated ===== */

void test_meaning_populated(void)
{
    nakshatra_t n = nakshatra_get(0);
    TEST_ASSERT_NOT_NULL(n.meaning);
    /* Ashwini = "Horse riders" or similar */
    TEST_ASSERT_TRUE(strlen(n.meaning) > 0);
}

/* ===== 19. Boundaries continuous (no gaps) ===== */

void test_boundaries_continuous(void)
{
    /* Each nakshatra end_deg should equal next start_deg */
    for (int i = 0; i < 26; i++) {
        nakshatra_t curr = nakshatra_get(i);
        nakshatra_t next = nakshatra_get(i + 1);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)curr.end_deg,
                                 (float)next.start_deg);
    }
    /* Last one ends at 360 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 360.0f,
                             (float)nakshatra_get(26).end_deg);
}

/* ===== 20. Each Nakshatra spans exactly 13.333... degrees ===== */

void test_span_width(void)
{
    float expected_span = 360.0f / 27.0f;
    for (int i = 0; i < 27; i++) {
        nakshatra_t n = nakshatra_get(i);
        float span = (float)(n.end_deg - n.start_deg);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_span, span);
    }
}

/* ===== 21. 108 padas total, each spans 3.333... degrees ===== */

void test_pada_span_width(void)
{
    float expected = 360.0f / 108.0f;
    for (int nak = 0; nak < 27; nak++) {
        for (int p = 1; p <= 4; p++) {
            nakshatra_pada_t pad = nakshatra_pada_get(nak, p);
            float span = (float)(pad.end_deg - pad.start_deg);
            TEST_ASSERT_FLOAT_WITHIN(0.01f, expected, span);
        }
    }
}

/* ===== 22. Navamsa cycle: 108 padas cycle through 12 signs 9 times ===== */

void test_navamsa_full_cycle(void)
{
    /* First pada of each Nakshatra at 0,9,18 should map to Aries (0) */
    /* Global index 0 -> 0%12 = 0 (Aries) */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_pada_get(0, 1).navamsa_sign);
    /* Global index 36 -> 36%12 = 0 (Aries) */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_pada_get(9, 1).navamsa_sign);
    /* Global index 72 -> 72%12 = 0 (Aries) */
    TEST_ASSERT_EQUAL_INT(0, nakshatra_pada_get(18, 1).navamsa_sign);
}

/* ===== 23. Mid-zodiac nakshatra: Chitra ===== */

void test_nakshatra_chitra(void)
{
    nakshatra_t n = nakshatra_get(13);
    TEST_ASSERT_EQUAL_STRING("Chitra", n.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 173.333f, (float)n.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 186.667f, (float)n.end_deg);
    TEST_ASSERT_EQUAL_INT(4, n.ruling_planet); /* Mars */
    TEST_ASSERT_EQUAL_STRING("Vishvakarma", n.deity);
    TEST_ASSERT_EQUAL_STRING("Rakshasa", n.quality);
}

/* ===== 24. Longitude at exact boundary ===== */

void test_longitude_at_boundary(void)
{
    /* At exactly 13.333... degrees: should be Bharani (1) not Ashwini (0)
     * because 13.333 is the end of Ashwini / start of Bharani.
     * The start_deg is inclusive. */
    double boundary = 360.0 / 27.0;
    nakshatra_t n = nakshatra_from_longitude(boundary);
    TEST_ASSERT_EQUAL_INT(1, n.id);
}

/* ===== 25. Betelgeuse and Pollux yogataras ===== */

void test_yogatara_betelgeuse_ardra(void)
{
    nakshatra_t n = nakshatra_get(5);
    TEST_ASSERT_EQUAL_INT(9, n.yogatara_catalog_index); /* Betelgeuse */
}

void test_yogatara_pollux_punarvasu(void)
{
    nakshatra_t n = nakshatra_get(6);
    TEST_ASSERT_EQUAL_INT(16, n.yogatara_catalog_index); /* Pollux */
}

/* ===== 26. All 27 have non-null names ===== */

void test_all_names_non_null(void)
{
    for (int i = 0; i < 27; i++) {
        TEST_ASSERT_NOT_NULL(nakshatra_name(i));
    }
}

/* ===== Main runner ===== */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_nakshatra_count);
    RUN_TEST(test_nakshatra_get_ashwini);
    RUN_TEST(test_nakshatra_get_revati);
    RUN_TEST(test_nakshatra_get_invalid_negative);
    RUN_TEST(test_nakshatra_get_invalid_high);
    RUN_TEST(test_nakshatra_name_rohini);
    RUN_TEST(test_nakshatra_name_invalid);
    RUN_TEST(test_nakshatra_from_longitude_zero);
    RUN_TEST(test_nakshatra_from_longitude_mid_rohini);
    RUN_TEST(test_nakshatra_from_longitude_end_revati);
    RUN_TEST(test_nakshatra_from_longitude_wrap_negative);
    RUN_TEST(test_nakshatra_from_longitude_wrap_over_360);
    RUN_TEST(test_ruler_cycle_first_nine);
    RUN_TEST(test_ruler_cycle_second_repeat);
    RUN_TEST(test_ruler_cycle_third_repeat);
    RUN_TEST(test_ruler_name_ketu);
    RUN_TEST(test_ruler_name_mercury);
    RUN_TEST(test_ruler_name_invalid);
    RUN_TEST(test_yogatara_aldebaran_rohini);
    RUN_TEST(test_yogatara_regulus_magha);
    RUN_TEST(test_yogatara_spica_chitra);
    RUN_TEST(test_yogatara_arcturus_swati);
    RUN_TEST(test_yogatara_antares_jyeshtha);
    RUN_TEST(test_yogatara_altair_shravana);
    RUN_TEST(test_yogatara_not_in_catalog);
    RUN_TEST(test_pada_from_longitude_first);
    RUN_TEST(test_pada_from_longitude_second);
    RUN_TEST(test_pada_from_longitude_last);
    RUN_TEST(test_pada_get_ashwini_1);
    RUN_TEST(test_pada_get_ashwini_4);
    RUN_TEST(test_pada_get_invalid);
    RUN_TEST(test_navamsa_zero);
    RUN_TEST(test_navamsa_mid_cycle);
    RUN_TEST(test_navamsa_end_of_zodiac);
    RUN_TEST(test_lahiri_ayanamsa_j2000);
    RUN_TEST(test_lahiri_ayanamsa_j2025);
    RUN_TEST(test_tropical_to_sidereal_basic);
    RUN_TEST(test_tropical_to_sidereal_wrap);
    RUN_TEST(test_quality_deva);
    RUN_TEST(test_quality_manushya);
    RUN_TEST(test_quality_rakshasa);
    RUN_TEST(test_deity_populated);
    RUN_TEST(test_symbol_populated);
    RUN_TEST(test_meaning_populated);
    RUN_TEST(test_boundaries_continuous);
    RUN_TEST(test_span_width);
    RUN_TEST(test_pada_span_width);
    RUN_TEST(test_navamsa_full_cycle);
    RUN_TEST(test_nakshatra_chitra);
    RUN_TEST(test_longitude_at_boundary);
    RUN_TEST(test_yogatara_betelgeuse_ardra);
    RUN_TEST(test_yogatara_pollux_punarvasu);
    RUN_TEST(test_all_names_non_null);

    return UNITY_END();
}
