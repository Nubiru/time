/* test_astronomy_i18n.c — Tests for locale-aware astronomy interpretation. */

#include "unity.h"
#include "systems/astronomy/astronomy_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English ---- */

static void test_locale_en_mercury(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(0, 0, -1,
                                                     I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Mer", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mercury"));
}

static void test_locale_en_retrograde(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(0, 1, -1,
                                                     I18N_LOCALE_EN);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "retrograde"));
}

static void test_locale_en_with_moon(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(3, 0, 4,
                                                     I18N_LOCALE_EN);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Full Moon"));
}

/* ---- Spanish ---- */

static void test_locale_es_mercurio(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(0, 0, -1,
                                                     I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Mer", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mercurio"));
}

static void test_locale_es_marte(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(3, 0, -1,
                                                     I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Mar", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Marte"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Guerrero"));
}

static void test_locale_es_jupiter(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(4, 0, -1,
                                                     I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "piter"));
}

static void test_locale_es_retrograde(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(5, 1, -1,
                                                     I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "grado"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "GRADO"));
}

static void test_locale_es_luna_llena(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(1, 0, 4,
                                                     I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Luna Llena"));
}

static void test_locale_es_new_moon(void)
{
    astronomy_interp_t r = ai_astro_interpret_locale(2, 0, 0,
                                                     I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Luna Nueva"));
}

/* ---- All planets produce output ---- */

static void test_locale_es_all_planets(void)
{
    for (int p = 0; p < 8; p++) {
        astronomy_interp_t r = ai_astro_interpret_locale(p, 0, -1,
                                                         I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_all_phases(void)
{
    for (int ph = 0; ph < 8; ph++) {
        astronomy_interp_t r = ai_astro_interpret_locale(0, 0, ph,
                                                         I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.detail) > 10);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_mercury);
    RUN_TEST(test_locale_en_retrograde);
    RUN_TEST(test_locale_en_with_moon);

    /* Spanish */
    RUN_TEST(test_locale_es_mercurio);
    RUN_TEST(test_locale_es_marte);
    RUN_TEST(test_locale_es_jupiter);
    RUN_TEST(test_locale_es_retrograde);
    RUN_TEST(test_locale_es_luna_llena);
    RUN_TEST(test_locale_es_new_moon);

    /* Coverage */
    RUN_TEST(test_locale_es_all_planets);
    RUN_TEST(test_locale_es_all_phases);

    return UNITY_END();
}
