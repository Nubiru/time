/* test_lao_i18n.c — Tests for locale-aware Lao interpretation. */

#include "unity.h"
#include "systems/lao/lao_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    lao_interp_t r = lai_interpret_locale(2569, 1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Man", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mangkon"));
}

static void test_locale_en_matches_base(void)
{
    lao_interp_t en = lai_interpret_locale(2569, 4, 14, 1, I18N_LOCALE_EN);
    lao_interp_t base = lai_interpret(2569, 4, 14, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    lao_interp_t r = lai_interpret_locale(2569, 4, 14, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Festival name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pi Mai"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    lao_interp_t fr = lai_interpret_locale(2569, 1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "2569"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    lao_interp_t r = lai_interpret_locale(2569, 0, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 1; m <= 12; m++) {
        lao_interp_t r = lai_interpret_locale(2569, m, 1, 0,
                                              I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Festival path ---- */

static void test_locale_es_festival(void)
{
    lao_interp_t r = lai_interpret_locale(2569, 4, 14, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pi Mai"));
}

/* ---- All festivals ---- */

static void test_locale_all_festivals(void)
{
    for (int f = 1; f <= 3; f++) {
        lao_interp_t r = lai_interpret_locale(2569, 4, 14, f,
                                              I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
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
    RUN_TEST(test_locale_all_festivals);

    return UNITY_END();
}
