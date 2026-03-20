/* test_hindu_i18n.c — Tests for locale-aware Hindu interpretation. */

#include "unity.h"
#include "systems/hindu/hindu_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void test_locale_en_first_tithi(void)
{
    hindu_interp_t r = hndi_interpret_locale(1, NULL, NULL, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("S1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Shukla Pratipada"));
}

static void test_locale_en_same_as_base(void)
{
    hindu_interp_t en = hndi_interpret_locale(15, NULL, NULL, I18N_LOCALE_EN);
    hindu_interp_t base = hndi_interpret(15, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

static void test_locale_es_first_tithi(void)
{
    hindu_interp_t r = hndi_interpret_locale(1, NULL, NULL, I18N_LOCALE_ES);
    /* Sanskrit tithi names stay in all languages */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Shukla Pratipada"));
}

static void test_locale_es_purnima(void)
{
    hindu_interp_t r = hndi_interpret_locale(15, NULL, NULL, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Purnima"));
}

static void test_locale_es_with_nakshatra(void)
{
    hindu_interp_t r = hndi_interpret_locale(1, "Ashwini", NULL, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Nakshatra"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ashwini"));
}

static void test_locale_fallback_to_en(void)
{
    hindu_interp_t r = hndi_interpret_locale(1, NULL, NULL, I18N_LOCALE_FR);
    TEST_ASSERT_EQUAL_STRING("S1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Shukla Pratipada"));
}

static void test_locale_invalid_tithi(void)
{
    hindu_interp_t r = hndi_interpret_locale(0, NULL, NULL, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

static void test_locale_es_all_tithis(void)
{
    for (int i = 1; i <= 30; i++) {
        hindu_interp_t r = hndi_interpret_locale(i, NULL, NULL, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_locale_en_first_tithi);
    RUN_TEST(test_locale_en_same_as_base);
    RUN_TEST(test_locale_es_first_tithi);
    RUN_TEST(test_locale_es_purnima);
    RUN_TEST(test_locale_es_with_nakshatra);
    RUN_TEST(test_locale_fallback_to_en);
    RUN_TEST(test_locale_invalid_tithi);
    RUN_TEST(test_locale_es_all_tithis);
    return UNITY_END();
}
