/* test_coptic_i18n.c — Tests for locale-aware Coptic interpretation. */

#include "unity.h"
#include "systems/coptic/coptic_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    coptic_interp_t r = cci_interpret_locale(1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Tho", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thout"));
}

static void test_locale_en_matches_base(void)
{
    coptic_interp_t en = cci_interpret_locale(1, 1, 1, I18N_LOCALE_EN);
    coptic_interp_t base = cci_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    coptic_interp_t r = cci_interpret_locale(1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thout"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    coptic_interp_t fr = cci_interpret_locale(1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Thout"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    coptic_interp_t r = cci_interpret_locale(0, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 1; m <= 13; m++) {
        coptic_interp_t r = cci_interpret_locale(m, 1, 0, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Feast path ---- */

static void test_locale_es_feast(void)
{
    coptic_interp_t r = cci_interpret_locale(1, 1, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nayrouz"));
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
    RUN_TEST(test_locale_es_feast);

    return UNITY_END();
}
