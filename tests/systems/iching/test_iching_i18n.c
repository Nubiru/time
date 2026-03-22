/* test_iching_i18n.c — Tests for locale-aware I Ching interpretation. */

#include "unity.h"
#include "systems/iching/iching_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void test_locale_en_creative(void)
{
    iching_interp_t r = ii_interpret_locale(1, "Heaven", "Heaven", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Creative"));
}

static void test_locale_en_same_as_base(void)
{
    iching_interp_t en = ii_interpret_locale(23, "Mountain", "Earth", I18N_LOCALE_EN);
    iching_interp_t base = ii_interpret(23, "Mountain", "Earth");
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

static void test_locale_es_creative(void)
{
    iching_interp_t r = ii_interpret_locale(1, "Cielo", "Cielo", I18N_LOCALE_ES);
    /* Spanish name for hexagram 1 */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Creativ"));
}

static void test_locale_es_receptive(void)
{
    iching_interp_t r = ii_interpret_locale(2, "Tierra", "Tierra", I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Receptiv"));
}

static void test_locale_fallback_to_en(void)
{
    iching_interp_t r = ii_interpret_locale(1, "Heaven", "Heaven", I18N_LOCALE_DE);
    TEST_ASSERT_EQUAL_STRING("1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Creative"));
}

static void test_locale_invalid_king_wen(void)
{
    iching_interp_t r = ii_interpret_locale(0, "?", "?", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

static void test_locale_es_all_hexagrams(void)
{
    for (int i = 1; i <= 64; i++) {
        iching_interp_t r = ii_interpret_locale(i, "?", "?", I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_locale_en_creative);
    RUN_TEST(test_locale_en_same_as_base);
    RUN_TEST(test_locale_es_creative);
    RUN_TEST(test_locale_es_receptive);
    RUN_TEST(test_locale_fallback_to_en);
    RUN_TEST(test_locale_invalid_king_wen);
    RUN_TEST(test_locale_es_all_hexagrams);
    return UNITY_END();
}
