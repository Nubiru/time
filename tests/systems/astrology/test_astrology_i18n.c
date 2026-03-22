/* test_astrology_i18n.c — Tests for locale-aware astrology interpretation. */

#include "unity.h"
#include "systems/astrology/astrology_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English (same as ai_interpret) ---- */

static void test_locale_en_aries_sun(void)
{
    astrology_interp_t r = ai_interpret_locale(0, 3, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Ari", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Cancer"));
}

static void test_locale_en_with_ascendant(void)
{
    astrology_interp_t r = ai_interpret_locale(4, 7, 10, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Leo", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Leo"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Scorpio"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Aquarius"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rising"));
}

static void test_locale_en_has_same_content(void)
{
    astrology_interp_t en = ai_interpret_locale(0, 1, -1, I18N_LOCALE_EN);
    astrology_interp_t base = ai_interpret(0, 1, -1);
    /* EN locale fast path delegates to ai_interpret — identical output */
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish ---- */

static void test_locale_es_aries(void)
{
    astrology_interp_t r = ai_interpret_locale(0, 3, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Ari", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Guerrero"));
}

static void test_locale_es_tauro(void)
{
    astrology_interp_t r = ai_interpret_locale(1, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Tau", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tauro"));
}

static void test_locale_es_geminis(void)
{
    astrology_interp_t r = ai_interpret_locale(2, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Gem", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "minis"));
}

static void test_locale_es_cancer(void)
{
    astrology_interp_t r = ai_interpret_locale(3, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Can", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "ncer"));
}

static void test_locale_es_escorpio(void)
{
    astrology_interp_t r = ai_interpret_locale(7, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Sco", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Escorpio"));
}

static void test_locale_es_sagitario(void)
{
    astrology_interp_t r = ai_interpret_locale(8, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Sag", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sagitario"));
}

static void test_locale_es_capricornio(void)
{
    astrology_interp_t r = ai_interpret_locale(9, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Cap", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Capricornio"));
}

static void test_locale_es_acuario(void)
{
    astrology_interp_t r = ai_interpret_locale(10, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Aqu", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Acuario"));
}

static void test_locale_es_piscis(void)
{
    astrology_interp_t r = ai_interpret_locale(11, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Pis", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Piscis"));
}

static void test_locale_es_with_ascendant(void)
{
    astrology_interp_t r = ai_interpret_locale(4, 7, 1, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Leo"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Escorpio"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tauro"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ascendente"));
}

static void test_locale_es_element_translated(void)
{
    astrology_interp_t r = ai_interpret_locale(0, 1, -1, I18N_LOCALE_ES);
    /* Aries element in Spanish is "Fuego" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Fuego"));
}

static void test_locale_es_shadow_translated(void)
{
    astrology_interp_t r = ai_interpret_locale(1, 0, -1, I18N_LOCALE_ES);
    /* Taurus shadow in Spanish: "Terquedad" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Terquedad"));
}

/* ---- Fallback ---- */

static void test_locale_fallback_to_en(void)
{
    astrology_interp_t ko = ai_interpret_locale(0, 1, -1, I18N_LOCALE_KO);
    /* KO has no content table — falls back to EN */
    TEST_ASSERT_EQUAL_STRING("Ari", ko.glyph);
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "Taurus"));
}

static void test_locale_invalid_signs(void)
{
    astrology_interp_t r = ai_interpret_locale(-1, 0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_EQUAL_STRING("?", r.glance);
    TEST_ASSERT_EQUAL_STRING("?", r.detail);
}

/* ---- All 12 signs produce output ---- */

static void test_locale_es_all_signs(void)
{
    for (int s = 0; s < 12; s++) {
        astrology_interp_t r = ai_interpret_locale(s, (s + 1) % 12, -1,
                                                    I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_all_signs_with_asc(void)
{
    for (int s = 0; s < 12; s++) {
        astrology_interp_t r = ai_interpret_locale(s, (s + 3) % 12,
                                                    (s + 6) % 12,
                                                    I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 10);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_aries_sun);
    RUN_TEST(test_locale_en_with_ascendant);
    RUN_TEST(test_locale_en_has_same_content);

    /* Spanish */
    RUN_TEST(test_locale_es_aries);
    RUN_TEST(test_locale_es_tauro);
    RUN_TEST(test_locale_es_geminis);
    RUN_TEST(test_locale_es_cancer);
    RUN_TEST(test_locale_es_escorpio);
    RUN_TEST(test_locale_es_sagitario);
    RUN_TEST(test_locale_es_capricornio);
    RUN_TEST(test_locale_es_acuario);
    RUN_TEST(test_locale_es_piscis);
    RUN_TEST(test_locale_es_with_ascendant);
    RUN_TEST(test_locale_es_element_translated);
    RUN_TEST(test_locale_es_shadow_translated);

    /* Fallback */
    RUN_TEST(test_locale_fallback_to_en);
    RUN_TEST(test_locale_invalid_signs);

    /* Coverage */
    RUN_TEST(test_locale_es_all_signs);
    RUN_TEST(test_locale_es_all_signs_with_asc);

    return UNITY_END();
}
