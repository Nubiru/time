/* test_chakra_i18n.c — Tests for locale-aware Chakra interpretation. */

#include "unity.h"
#include "systems/chakra/chakra_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_root(void)
{
    chakra_interp_t r = chi_interpret_locale(0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("LAM", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Muladhara"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Root"));
}

static void test_locale_en_matches_base(void)
{
    chakra_interp_t en = chi_interpret_locale(3, I18N_LOCALE_EN);
    chakra_interp_t base = chi_interpret(3);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish ---- */

static void test_locale_es_muladhara_stays(void)
{
    /* Sanskrit names are preserved across locales */
    chakra_interp_t r = chi_interpret_locale(0, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Muladhara"));
}

static void test_locale_es_theme_translated(void)
{
    /* Theme for root chakra in Spanish: "Fundamento" or similar */
    chakra_interp_t r = chi_interpret_locale(0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_all_chakras(void)
{
    for (int c = 0; c < 7; c++) {
        chakra_interp_t r = chi_interpret_locale(c, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_bija_universal(void)
{
    /* Bija mantras are Sanskrit, should be same across locales */
    chakra_interp_t en = chi_interpret_locale(3, I18N_LOCALE_EN);
    chakra_interp_t es = chi_interpret_locale(3, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING(en.glyph, es.glyph);
}

/* ---- Fallback ---- */

static void test_locale_fr_fallback(void)
{
    /* FR falls back to EN content */
    chakra_interp_t fr = chi_interpret_locale(0, I18N_LOCALE_FR);
    TEST_ASSERT_EQUAL_STRING("LAM", fr.glyph);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Muladhara"));
}

/* ---- Invalid ---- */

static void test_locale_invalid_chakra(void)
{
    chakra_interp_t r = chi_interpret_locale(-1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = chi_interpret_locale(7, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_root);
    RUN_TEST(test_locale_en_matches_base);

    /* Spanish */
    RUN_TEST(test_locale_es_muladhara_stays);
    RUN_TEST(test_locale_es_theme_translated);
    RUN_TEST(test_locale_es_all_chakras);
    RUN_TEST(test_locale_es_bija_universal);

    /* Fallback */
    RUN_TEST(test_locale_fr_fallback);

    /* Invalid */
    RUN_TEST(test_locale_invalid_chakra);

    return UNITY_END();
}
