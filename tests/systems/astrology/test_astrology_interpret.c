/* test_astrology_interpret.c — Zodiac sign interpretation data + tiered text
 * TDD RED phase: tests written before implementation.
 * 12 signs: archetype, keywords, shadow, ruler, element, modality.
 * 4 elements, 3 modalities.
 * Composition: glyph (T0), glance (T1), detail (T3). */

#include "../../unity/unity.h"
#include "../../../src/systems/astrology/astrology_interpret.h"
#include "../../../src/systems/astrology/zodiac.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ai_sign_count / ai_element_count / ai_modality_count ===== */

void test_sign_count_returns_12(void)
{
    TEST_ASSERT_EQUAL_INT(12, ai_sign_count());
}

void test_element_count_returns_4(void)
{
    TEST_ASSERT_EQUAL_INT(4, ai_element_count());
}

void test_modality_count_returns_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, ai_modality_count());
}

/* ===== ai_sign_data — specific signs ===== */

void test_sign_data_aries(void)
{
    ai_sign_t s = ai_sign_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.sign);
    TEST_ASSERT_EQUAL_STRING("Aries", s.name);
    TEST_ASSERT_EQUAL_STRING("Fire", s.element);
    TEST_ASSERT_EQUAL_STRING("Cardinal", s.modality);
    TEST_ASSERT_EQUAL_STRING("Mars", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Warrior", s.archetype);
    TEST_ASSERT_EQUAL_STRING("Courage, initiative, independence", s.keywords);
    TEST_ASSERT_EQUAL_STRING("Impatience, aggression, selfishness", s.shadow);
    TEST_ASSERT_EQUAL_STRING(
        "The spark that begins — raw courage to exist", s.brief);
}

void test_sign_data_leo(void)
{
    ai_sign_t s = ai_sign_data(4);
    TEST_ASSERT_EQUAL_INT(4, s.sign);
    TEST_ASSERT_EQUAL_STRING("Leo", s.name);
    TEST_ASSERT_EQUAL_STRING("Fire", s.element);
    TEST_ASSERT_EQUAL_STRING("Fixed", s.modality);
    TEST_ASSERT_EQUAL_STRING("Sun", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The King", s.archetype);
    TEST_ASSERT_EQUAL_STRING("Creativity, generosity, self-expression",
                             s.keywords);
    TEST_ASSERT_EQUAL_STRING("Pride, drama, dominance", s.shadow);
    TEST_ASSERT_EQUAL_STRING(
        "The fire that sustains — the courage to shine", s.brief);
}

void test_sign_data_scorpio(void)
{
    ai_sign_t s = ai_sign_data(7);
    TEST_ASSERT_EQUAL_INT(7, s.sign);
    TEST_ASSERT_EQUAL_STRING("Scorpio", s.name);
    TEST_ASSERT_EQUAL_STRING("Water", s.element);
    TEST_ASSERT_EQUAL_STRING("Fixed", s.modality);
    TEST_ASSERT_EQUAL_STRING("Pluto", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Alchemist", s.archetype);
    TEST_ASSERT_EQUAL_STRING("Transformation, intensity, depth", s.keywords);
    TEST_ASSERT_EQUAL_STRING("Obsession, jealousy, destruction", s.shadow);
    TEST_ASSERT_EQUAL_STRING(
        "The depth that transforms — death as doorway", s.brief);
}

void test_sign_data_pisces(void)
{
    ai_sign_t s = ai_sign_data(11);
    TEST_ASSERT_EQUAL_INT(11, s.sign);
    TEST_ASSERT_EQUAL_STRING("Pisces", s.name);
    TEST_ASSERT_EQUAL_STRING("Water", s.element);
    TEST_ASSERT_EQUAL_STRING("Mutable", s.modality);
    TEST_ASSERT_EQUAL_STRING("Neptune", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Mystic", s.archetype);
    TEST_ASSERT_EQUAL_STRING(
        "Compassion, imagination, transcendence", s.keywords);
    TEST_ASSERT_EQUAL_STRING("Escapism, confusion, martyrdom", s.shadow);
    TEST_ASSERT_EQUAL_STRING(
        "The ocean that dissolves — unity beyond separation", s.brief);
}

void test_sign_data_taurus(void)
{
    ai_sign_t s = ai_sign_data(1);
    TEST_ASSERT_EQUAL_STRING("Taurus", s.name);
    TEST_ASSERT_EQUAL_STRING("Earth", s.element);
    TEST_ASSERT_EQUAL_STRING("Fixed", s.modality);
    TEST_ASSERT_EQUAL_STRING("Venus", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Builder", s.archetype);
}

void test_sign_data_gemini(void)
{
    ai_sign_t s = ai_sign_data(2);
    TEST_ASSERT_EQUAL_STRING("Gemini", s.name);
    TEST_ASSERT_EQUAL_STRING("Air", s.element);
    TEST_ASSERT_EQUAL_STRING("Mutable", s.modality);
    TEST_ASSERT_EQUAL_STRING("Mercury", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Messenger", s.archetype);
}

void test_sign_data_cancer(void)
{
    ai_sign_t s = ai_sign_data(3);
    TEST_ASSERT_EQUAL_STRING("Cancer", s.name);
    TEST_ASSERT_EQUAL_STRING("Water", s.element);
    TEST_ASSERT_EQUAL_STRING("Cardinal", s.modality);
    TEST_ASSERT_EQUAL_STRING("Moon", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Mother", s.archetype);
}

void test_sign_data_virgo(void)
{
    ai_sign_t s = ai_sign_data(5);
    TEST_ASSERT_EQUAL_STRING("Virgo", s.name);
    TEST_ASSERT_EQUAL_STRING("Earth", s.element);
    TEST_ASSERT_EQUAL_STRING("Mutable", s.modality);
    TEST_ASSERT_EQUAL_STRING("Mercury", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Healer", s.archetype);
}

void test_sign_data_libra(void)
{
    ai_sign_t s = ai_sign_data(6);
    TEST_ASSERT_EQUAL_STRING("Libra", s.name);
    TEST_ASSERT_EQUAL_STRING("Air", s.element);
    TEST_ASSERT_EQUAL_STRING("Cardinal", s.modality);
    TEST_ASSERT_EQUAL_STRING("Venus", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Diplomat", s.archetype);
}

void test_sign_data_sagittarius(void)
{
    ai_sign_t s = ai_sign_data(8);
    TEST_ASSERT_EQUAL_STRING("Sagittarius", s.name);
    TEST_ASSERT_EQUAL_STRING("Fire", s.element);
    TEST_ASSERT_EQUAL_STRING("Mutable", s.modality);
    TEST_ASSERT_EQUAL_STRING("Jupiter", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Explorer", s.archetype);
}

void test_sign_data_capricorn(void)
{
    ai_sign_t s = ai_sign_data(9);
    TEST_ASSERT_EQUAL_STRING("Capricorn", s.name);
    TEST_ASSERT_EQUAL_STRING("Earth", s.element);
    TEST_ASSERT_EQUAL_STRING("Cardinal", s.modality);
    TEST_ASSERT_EQUAL_STRING("Saturn", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Elder", s.archetype);
}

void test_sign_data_aquarius(void)
{
    ai_sign_t s = ai_sign_data(10);
    TEST_ASSERT_EQUAL_STRING("Aquarius", s.name);
    TEST_ASSERT_EQUAL_STRING("Air", s.element);
    TEST_ASSERT_EQUAL_STRING("Fixed", s.modality);
    TEST_ASSERT_EQUAL_STRING("Uranus", s.ruler);
    TEST_ASSERT_EQUAL_STRING("The Visionary", s.archetype);
}

/* ===== ai_sign_data — loop all 12, verify non-empty fields ===== */

void test_all_signs_have_non_empty_fields(void)
{
    for (int i = 0; i < 12; i++) {
        ai_sign_t s = ai_sign_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.sign);
        TEST_ASSERT_TRUE(strlen(s.name) > 0);
        TEST_ASSERT_TRUE(strlen(s.element) > 0);
        TEST_ASSERT_TRUE(strlen(s.modality) > 0);
        TEST_ASSERT_TRUE(strlen(s.ruler) > 0);
        TEST_ASSERT_TRUE(strlen(s.archetype) > 0);
        TEST_ASSERT_TRUE(strlen(s.keywords) > 0);
        TEST_ASSERT_TRUE(strlen(s.shadow) > 0);
        TEST_ASSERT_TRUE(strlen(s.brief) > 0);
    }
}

/* ===== ai_sign_data — invalid inputs ===== */

void test_sign_data_invalid_negative(void)
{
    ai_sign_t s = ai_sign_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
    TEST_ASSERT_EQUAL_STRING("?", s.element);
    TEST_ASSERT_EQUAL_STRING("?", s.modality);
    TEST_ASSERT_EQUAL_STRING("?", s.ruler);
    TEST_ASSERT_EQUAL_STRING("?", s.archetype);
    TEST_ASSERT_EQUAL_STRING("?", s.keywords);
    TEST_ASSERT_EQUAL_STRING("?", s.shadow);
    TEST_ASSERT_EQUAL_STRING("?", s.brief);
}

void test_sign_data_invalid_12(void)
{
    ai_sign_t s = ai_sign_data(12);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

void test_sign_data_invalid_large(void)
{
    ai_sign_t s = ai_sign_data(999);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* ===== ai_element_data — all 4 elements ===== */

void test_element_fire(void)
{
    ai_element_t e = ai_element_data(0);
    TEST_ASSERT_EQUAL_STRING("Fire", e.name);
    TEST_ASSERT_EQUAL_STRING("Will, spirit, creative force", e.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Through action, inspiration, and enthusiasm", e.expression);
}

void test_element_earth(void)
{
    ai_element_t e = ai_element_data(1);
    TEST_ASSERT_EQUAL_STRING("Earth", e.name);
    TEST_ASSERT_EQUAL_STRING("Form, matter, practical wisdom", e.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Through building, sustaining, and manifesting", e.expression);
}

void test_element_air(void)
{
    ai_element_t e = ai_element_data(2);
    TEST_ASSERT_EQUAL_STRING("Air", e.name);
    TEST_ASSERT_EQUAL_STRING("Mind, communication, relationship", e.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Through thinking, connecting, and exchanging", e.expression);
}

void test_element_water(void)
{
    ai_element_t e = ai_element_data(3);
    TEST_ASSERT_EQUAL_STRING("Water", e.name);
    TEST_ASSERT_EQUAL_STRING(
        "Feeling, intuition, the unconscious", e.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Through emotion, empathy, and transformation", e.expression);
}

void test_element_all_have_non_empty_fields(void)
{
    for (int i = 0; i < 4; i++) {
        ai_element_t e = ai_element_data(i);
        TEST_ASSERT_TRUE(strlen(e.name) > 0);
        TEST_ASSERT_TRUE(strlen(e.quality) > 0);
        TEST_ASSERT_TRUE(strlen(e.expression) > 0);
    }
}

/* ===== ai_element_data — invalid ===== */

void test_element_invalid_4(void)
{
    ai_element_t e = ai_element_data(4);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
    TEST_ASSERT_EQUAL_STRING("?", e.quality);
    TEST_ASSERT_EQUAL_STRING("?", e.expression);
}

void test_element_invalid_negative(void)
{
    ai_element_t e = ai_element_data(-1);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
}

/* ===== ai_modality_data — all 3 modalities ===== */

void test_modality_cardinal(void)
{
    ai_modality_t m = ai_modality_data(0);
    TEST_ASSERT_EQUAL_STRING("Cardinal", m.name);
    TEST_ASSERT_EQUAL_STRING(
        "Initiation, leadership, new beginnings", m.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Begins the season — the spark that starts", m.role);
}

void test_modality_fixed(void)
{
    ai_modality_t m = ai_modality_data(1);
    TEST_ASSERT_EQUAL_STRING("Fixed", m.name);
    TEST_ASSERT_EQUAL_STRING(
        "Sustaining, determination, depth", m.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Holds the season — the strength that endures", m.role);
}

void test_modality_mutable(void)
{
    ai_modality_t m = ai_modality_data(2);
    TEST_ASSERT_EQUAL_STRING("Mutable", m.name);
    TEST_ASSERT_EQUAL_STRING(
        "Adaptation, flexibility, transition", m.quality);
    TEST_ASSERT_EQUAL_STRING(
        "Ends the season — the wisdom that releases", m.role);
}

void test_modality_all_have_non_empty_fields(void)
{
    for (int i = 0; i < 3; i++) {
        ai_modality_t m = ai_modality_data(i);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
        TEST_ASSERT_TRUE(strlen(m.quality) > 0);
        TEST_ASSERT_TRUE(strlen(m.role) > 0);
    }
}

/* ===== ai_modality_data — invalid ===== */

void test_modality_invalid_3(void)
{
    ai_modality_t m = ai_modality_data(3);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
    TEST_ASSERT_EQUAL_STRING("?", m.quality);
    TEST_ASSERT_EQUAL_STRING("?", m.role);
}

void test_modality_invalid_negative(void)
{
    ai_modality_t m = ai_modality_data(-1);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ===== ai_interpret — glyph ===== */

void test_interpret_glyph_aries(void)
{
    astrology_interp_t r = ai_interpret(0, 3, 10);
    TEST_ASSERT_EQUAL_STRING("Ari", r.glyph);
}

void test_interpret_glyph_capricorn(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_EQUAL_STRING("Cap", r.glyph);
}

void test_interpret_glyph_all_signs(void)
{
    const char *abbrs[] = {
        "Ari", "Tau", "Gem", "Can", "Leo", "Vir",
        "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"
    };
    for (int i = 0; i < 12; i++) {
        astrology_interp_t r = ai_interpret(i, 0, -1);
        TEST_ASSERT_EQUAL_STRING(abbrs[i], r.glyph);
    }
}

/* ===== ai_interpret — glance ===== */

void test_interpret_glance_sun_cap_moon_cancer(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    /* "Sun in Capricorn · Moon in Cancer" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sun in Capricorn"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Moon in Cancer"));
}

void test_interpret_glance_with_ascendant(void)
{
    astrology_interp_t r = ai_interpret(9, 3, 10);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sun in Capricorn"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Moon in Cancer"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Aquarius Rising"));
}

void test_interpret_glance_without_ascendant(void)
{
    astrology_interp_t r = ai_interpret(0, 4, -1);
    TEST_ASSERT_NULL(strstr(r.glance, "Rising"));
}

void test_interpret_glance_middle_dot_separator(void)
{
    /* Verify the middle dot separator is present */
    astrology_interp_t r = ai_interpret(0, 4, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "\xC2\xB7"));
}

/* ===== ai_interpret — detail ===== */

void test_interpret_detail_contains_archetype(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "The Elder"));
}

void test_interpret_detail_contains_shadow(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Coldness, rigidity, ruthlessness"));
}

void test_interpret_detail_contains_element(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Earth"));
}

void test_interpret_detail_contains_modality(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Cardinal"));
}

void test_interpret_detail_contains_moon_info(void)
{
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Moon in Cancer"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "The Mother"));
}

void test_interpret_detail_contains_ascendant_info(void)
{
    astrology_interp_t r = ai_interpret(0, 4, 7);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Scorpio"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "The Alchemist"));
}

void test_interpret_detail_no_ascendant_no_rising_text(void)
{
    astrology_interp_t r = ai_interpret(0, 4, -1);
    /* Should not have Ascendant/Rising in detail when asc=-1 */
    TEST_ASSERT_NULL(strstr(r.detail, "Ascendant"));
}

void test_interpret_detail_element_quality(void)
{
    /* Capricorn = Earth. Element quality should appear. */
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(
        strstr(r.detail, "Form, matter, practical wisdom"));
}

void test_interpret_detail_modality_role(void)
{
    /* Capricorn = Cardinal. Modality role should appear. */
    astrology_interp_t r = ai_interpret(9, 3, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail,
        "Begins the season"));
}

/* ===== ai_interpret — invalid inputs ===== */

void test_interpret_invalid_sun(void)
{
    astrology_interp_t r = ai_interpret(-1, 3, 10);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_interpret_invalid_sun_high(void)
{
    astrology_interp_t r = ai_interpret(12, 3, 10);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_interpret_invalid_moon(void)
{
    astrology_interp_t r = ai_interpret(0, -1, 10);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_interpret_invalid_moon_high(void)
{
    astrology_interp_t r = ai_interpret(0, 12, 10);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ===== Aries element is Fire, modality Cardinal ===== */

void test_aries_element_fire_modality_cardinal(void)
{
    ai_sign_t s = ai_sign_data(0);
    TEST_ASSERT_EQUAL_STRING("Fire", s.element);
    TEST_ASSERT_EQUAL_STRING("Cardinal", s.modality);
}

/* ===== Pisces element is Water, modality Mutable ===== */

void test_pisces_element_water_modality_mutable(void)
{
    ai_sign_t s = ai_sign_data(11);
    TEST_ASSERT_EQUAL_STRING("Water", s.element);
    TEST_ASSERT_EQUAL_STRING("Mutable", s.modality);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Count functions */
    RUN_TEST(test_sign_count_returns_12);
    RUN_TEST(test_element_count_returns_4);
    RUN_TEST(test_modality_count_returns_3);

    /* Sign data — specific signs */
    RUN_TEST(test_sign_data_aries);
    RUN_TEST(test_sign_data_leo);
    RUN_TEST(test_sign_data_scorpio);
    RUN_TEST(test_sign_data_pisces);
    RUN_TEST(test_sign_data_taurus);
    RUN_TEST(test_sign_data_gemini);
    RUN_TEST(test_sign_data_cancer);
    RUN_TEST(test_sign_data_virgo);
    RUN_TEST(test_sign_data_libra);
    RUN_TEST(test_sign_data_sagittarius);
    RUN_TEST(test_sign_data_capricorn);
    RUN_TEST(test_sign_data_aquarius);

    /* Sign data — loop all 12 */
    RUN_TEST(test_all_signs_have_non_empty_fields);

    /* Sign data — invalid */
    RUN_TEST(test_sign_data_invalid_negative);
    RUN_TEST(test_sign_data_invalid_12);
    RUN_TEST(test_sign_data_invalid_large);

    /* Element data */
    RUN_TEST(test_element_fire);
    RUN_TEST(test_element_earth);
    RUN_TEST(test_element_air);
    RUN_TEST(test_element_water);
    RUN_TEST(test_element_all_have_non_empty_fields);
    RUN_TEST(test_element_invalid_4);
    RUN_TEST(test_element_invalid_negative);

    /* Modality data */
    RUN_TEST(test_modality_cardinal);
    RUN_TEST(test_modality_fixed);
    RUN_TEST(test_modality_mutable);
    RUN_TEST(test_modality_all_have_non_empty_fields);
    RUN_TEST(test_modality_invalid_3);
    RUN_TEST(test_modality_invalid_negative);

    /* Interpret — glyph */
    RUN_TEST(test_interpret_glyph_aries);
    RUN_TEST(test_interpret_glyph_capricorn);
    RUN_TEST(test_interpret_glyph_all_signs);

    /* Interpret — glance */
    RUN_TEST(test_interpret_glance_sun_cap_moon_cancer);
    RUN_TEST(test_interpret_glance_with_ascendant);
    RUN_TEST(test_interpret_glance_without_ascendant);
    RUN_TEST(test_interpret_glance_middle_dot_separator);

    /* Interpret — detail */
    RUN_TEST(test_interpret_detail_contains_archetype);
    RUN_TEST(test_interpret_detail_contains_shadow);
    RUN_TEST(test_interpret_detail_contains_element);
    RUN_TEST(test_interpret_detail_contains_modality);
    RUN_TEST(test_interpret_detail_contains_moon_info);
    RUN_TEST(test_interpret_detail_contains_ascendant_info);
    RUN_TEST(test_interpret_detail_no_ascendant_no_rising_text);
    RUN_TEST(test_interpret_detail_element_quality);
    RUN_TEST(test_interpret_detail_modality_role);

    /* Interpret — invalid */
    RUN_TEST(test_interpret_invalid_sun);
    RUN_TEST(test_interpret_invalid_sun_high);
    RUN_TEST(test_interpret_invalid_moon);
    RUN_TEST(test_interpret_invalid_moon_high);

    /* Cross-checks */
    RUN_TEST(test_aries_element_fire_modality_cardinal);
    RUN_TEST(test_pisces_element_water_modality_mutable);

    return UNITY_END();
}
