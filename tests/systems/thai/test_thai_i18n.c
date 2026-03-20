/* test_thai_i18n.c — Tests for locale-aware Thai interpretation. */

#include "unity.h"
#include "systems/thai/thai_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    thai_interp_t r = tti_interpret_locale(2569, 1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Mok", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mokarakhom"));
}

static void test_locale_en_matches_base(void)
{
    thai_interp_t en = tti_interpret_locale(2569, 4, 13, 1, I18N_LOCALE_EN);
    thai_interp_t base = tti_interpret(2569, 4, 13, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    thai_interp_t r = tti_interpret_locale(2569, 4, 13, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Songkran stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Songkran"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    thai_interp_t fr = tti_interpret_locale(2569, 1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "2569"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    thai_interp_t r = tti_interpret_locale(2569, 0, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 1; m <= 12; m++) {
        thai_interp_t r = tti_interpret_locale(2569, m, 1, 0,
                                               I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Festival path ---- */

static void test_locale_es_festival(void)
{
    thai_interp_t r = tti_interpret_locale(2569, 4, 13, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Songkran"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_locale_en_first_entry);
    RUN_TEST(test_locale_en_matches_base);
    RUN_TEST(test_locale_es_produces_output);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);
    RUN_TEST(test_locale_es_festival);

    return UNITY_END();
}
