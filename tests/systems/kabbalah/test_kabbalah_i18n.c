/* test_kabbalah_i18n.c — Tests for locale-aware Kabbalah interpretation. */

#include "unity.h"
#include "systems/kabbalah/kabbalah_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void test_locale_en_keter(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah_locale(0, "Neptune", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Ket", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Keter"));
}

static void test_locale_en_same_as_base(void)
{
    kabbalah_interp_t en = ki_interpret_sefirah_locale(5, "Sun", I18N_LOCALE_EN);
    kabbalah_interp_t base = ki_interpret_sefirah(5, "Sun");
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

static void test_locale_es_keter(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah_locale(0, "Neptuno", I18N_LOCALE_ES);
    /* Hebrew name stays in all languages */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Keter"));
}

static void test_locale_es_tiferet(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah_locale(5, "Sol", I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tiferet"));
}

static void test_locale_fallback_to_en(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah_locale(0, "Neptune", I18N_LOCALE_FR);
    TEST_ASSERT_EQUAL_STRING("Ket", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Neptune"));
}

static void test_locale_invalid_index(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah_locale(-1, NULL, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

static void test_locale_es_all_sefirot(void)
{
    for (int i = 0; i < 10; i++) {
        kabbalah_interp_t r = ki_interpret_sefirah_locale(i, "?", I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_locale_en_keter);
    RUN_TEST(test_locale_en_same_as_base);
    RUN_TEST(test_locale_es_keter);
    RUN_TEST(test_locale_es_tiferet);
    RUN_TEST(test_locale_fallback_to_en);
    RUN_TEST(test_locale_invalid_index);
    RUN_TEST(test_locale_es_all_sefirot);
    return UNITY_END();
}
